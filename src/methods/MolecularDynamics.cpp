#include <numeric>
#include "MolecularDynamics.h"

MolecularDynamics::MolecularDynamics(double energy_interaction, double length_interaction, const std::string &filepath)
        : ParticleSimulation(filepath), _energy_interaction(energy_interaction), _length_interaction(length_interaction) {}

void MolecularDynamics::step(double dt) {
    // Reset force
    for(auto& p : _particles)
        p.setForces({0,0,0});

    Vec3s num_cells = {1,8,8};
    Vec3d box_sz{5, 42, 42};
    linkedCells.distribute(_particles, num_cells, box_sz * -1, box_sz);

    // Parallelization for 1 neighbor radius in 3d
    /*for(int n = 0; n < 8; ++n) {
        tbb::parallel_for(
            tbb::blocked_range<std::size_t>(0, num_cells.getZ() / 2),
            [&](const tbb::blocked_range<std::size_t> &rz) {
                for (std::size_t z = rz.begin(); z != rz.end(); ++z) {
                    tbb::parallel_for(
                        tbb::blocked_range<std::size_t>(0, num_cells.getY() / 2),
                        [&](const tbb::blocked_range<std::size_t> &ry) {
                            for (std::size_t y = ry.begin(); y != ry.end(); ++y) {
                                tbb::parallel_for(
                                    tbb::blocked_range<std::size_t>(0, num_cells.getX() / 2),
                                    [&](const tbb::blocked_range<std::size_t> &r) {
                                        for (std::size_t x = rx.begin(); x != rx.end(); ++x) {
                                            Vec3s cell_index = {x * 2 + n / 4, y * 2 + n % 2, z * 2 + n%4 / 2};
                                            compute_step_cell(cell_index, dt);
                                        }
                                    }
                                );
                            }
                        }
                    );
                }
            }
        );
    }*/

    // Parallelization for 1 neighbor radius in 2d
    for(int n = 0; n < 4; ++n) {
        tbb::parallel_for(
            tbb::blocked_range<std::size_t>(0, num_cells.getZ() / 2),
            [&](const tbb::blocked_range<std::size_t> &rz) {
                for (std::size_t z = rz.begin(); z != rz.end(); ++z) {
                    tbb::parallel_for(
                        tbb::blocked_range<std::size_t>(0, num_cells.getY() / 2),
                        [&](const tbb::blocked_range<std::size_t> &ry) {
                            for (std::size_t y = ry.begin(); y != ry.end(); ++y) {
                                Vec3s cell_index = {0, y * 2 + n % 2, z * 2 + n / 2};
                                compute_step_cell(cell_index, dt);
                            }
                        }
                    );
                }
            }
        );
    }

    // Sequential computation
    /*for(std::size_t z = 0; z < num_cells.getZ(); ++z)
        for(std::size_t y = 0; y < num_cells.getY(); ++y)
            for(std::size_t x = 0; x < num_cells.getX(); ++x)
                compute_step_cell({x,y,z}, dt);*/

    auto iso = isokinetic_scaling();
    for(auto& p : _particles) {
        p.setVelocity(p.getVelocity() * iso);
    }
}

void MolecularDynamics::compute_step_cell(Vec3s cell_index, double dt) {
    auto cell_particles_indices = linkedCells.getParticlesCell(cell_index);
    auto nearby_particles = linkedCells.getNearbyParticlesCell(cell_index, 1);
    for(int i = 0; i < cell_particles_indices.size(); ++i) {
        auto &particle = _particles.at(cell_particles_indices.at(i));
        auto dt_half = dt / 2;
        auto vel_half = particle.getVelocity() + particle.getForces() * dt_half;
        if(!particle.isFixed())
            particle.setPosition(particle.getPosition() + vel_half * dt);
        for (auto j = i + 1; j < cell_particles_indices.size(); ++j)
            compute_force_interaction(particle, _particles.at(cell_particles_indices.at(j)));
        for(auto nearby_p : nearby_particles)
            compute_force_interaction(particle, nearby_p);
        if(!particle.isFixed())
            particle.setVelocity(vel_half + particle.getForces() * dt_half);
    }
}

void MolecularDynamics::spawn(const Particle &particle) {
    _particles.push_back(particle);
}

void MolecularDynamics::spawn(const std::vector<Particle> &particles) {
    _particles.insert(_particles.end(), particles.begin(), particles.end());
}

void MolecularDynamics::print(std::size_t timestep) const {
    Logger::instance().log(log_level::INFO)<< "Print timestep: " << timestep;

    // Create a writer
    auto writer = vtkSmartPointer<vtkUnstructuredGridWriter>::New();
    // Get the filename with extension
    // Create a pointer to a VTK Unstructured Grid data set
    auto grid = vtkSmartPointer<vtkUnstructuredGrid>::New();
    // Add the particle data to the unstructured grid
    addParticlesToVTKGrid(_particles, grid);
    // Remove unused memory
    grid->Squeeze();
    // Write the data
    auto filename = savefile/"fluid-sim.";
    filename += std::to_string(timestep) + ".vtk";
    writer->SetFileName(filename.c_str());
    writer->SetInputData(grid);
    writer->Write();
}

void MolecularDynamics::compute_force_interaction(Particle &p1, Particle &p2) const {
    auto distance = p1.getPosition().distance(p2.getPosition());
    auto force_applied = Vec3d(p1.getPosition() - p2.getPosition()).unit() * force(distance);
    p1.setForces(p1.getForces() + force_applied);
    p2.setForces(p2.getForces() - force_applied);
}

double MolecularDynamics::force(double distance) const {
    return Lennard_Jones_force(distance);
}

double MolecularDynamics::Lennard_Jones_force(double distance) const {
    auto length_dist = _length_interaction / distance;
    return 24 * _energy_interaction / distance * (2 * std::pow(length_dist, 12) - std::pow(length_dist, 6));
}

double MolecularDynamics::isokinetic_scaling() const {
    return std::sqrt(2.0 / temperature());
}

double MolecularDynamics::temperature() const {
    double sum_velocities = 0;
    std::for_each(_particles.begin(), _particles.end(), [&](const Particle& p){
        if(!p.isFixed()) {
            auto vel = p.getVelocity() * p.getVelocity();
            sum_velocities += std::reduce(vel.cbegin(), vel.cend());
        }
    });
    return sum_velocities / static_cast<double>(_particles.size());
}
