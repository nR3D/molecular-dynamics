#include "LinkedCells.h"

#include <cmath>
#include <tbb/parallel_for.h>

void LinkedCells::distribute(std::vector<Particle>& particles, Vec3s num_cells, Vec3d min_bounding_box,
                             Vec3d max_bounding_box) {
    _particles = particles;
    _num_cells = num_cells;
    _min_bounding_box = min_bounding_box;
    _max_bounding_box = max_bounding_box;
    Vec3d domain_size = _max_bounding_box - _min_bounding_box;
    _size_cell = domain_size / Vec3d(num_cells);
    _head.clear();
    _cell.clear();
    _head.resize(num_cells.getX() * num_cells.getY() * num_cells.getZ() + 1);
    _cell.resize(_particles.size());


    for (int i = 0; i < particles.size(); ++i) {
        auto& particle = particles.at(i);
        auto particle_pos = particle.getPosition();

        // Periodic position
        tbb::parallel_for(
                tbb::blocked_range<std::size_t>(0, 3),
                [&](const tbb::blocked_range<std::size_t> &r) {
                    for (std::size_t d = r.begin(); d != r.end(); ++d) {
                        if(particle_pos.getCoord(d) < min_bounding_box.getCoord(d)) {
                            auto periodic_pos = std::fmod(std::abs(particle_pos.getCoord(d) - min_bounding_box.getCoord(d)),
                                                          domain_size.getCoord(d));
                            particle_pos.setCoord(d, max_bounding_box.getCoord(d) - periodic_pos);
                        }
                        else if (particle_pos.getCoord(d) > max_bounding_box.getCoord(d)) {
                            auto periodic_pos = std::fmod(std::abs(particle_pos.getCoord(d) - min_bounding_box.getCoord(d)),
                                                          domain_size.getCoord(d));
                            particle_pos.setCoord(d, min_bounding_box.getCoord(d) + periodic_pos);
                        }
                    }
                }
        );
        particle.setPosition(particle_pos);

        auto head_index = flattenIndex((particle_pos - min_bounding_box)/_size_cell);
        if(_head.at(head_index).has_value()) {
            auto tail_cell = _head.at(head_index).value();
            // Find index of last particle stored inside cell
            while(_cell.at(tail_cell).has_value())
                tail_cell = _cell.at(tail_cell).value();
            _cell.at(tail_cell).emplace(i);
        } else {
            _head.at(head_index).emplace(i);
        }

        // Particles associated to the same cell will have the same color
        Vec3d color(static_cast<double>(head_index));
        particle.setColor(color);
    }
}

std::size_t LinkedCells::flattenIndex(Vec3d index) const {
    return std::floor(index.getX()) + _num_cells.getX() * std::floor(index.getY()) + _num_cells.getX() * _num_cells.getY() * std::floor(index.getZ());
}

std::vector<Particle> LinkedCells::getNearbyParticlesCell(Vec3s cell_index, int radius) const {
    std::vector<Particle> nearby_particles;
    for(auto z = -radius; z <= radius; ++z)
        for(auto y = -radius; y <= radius; ++y)
            for(auto x = -radius; x <= radius; ++x) {
                if(x == 0 && y == 0 && z == 0)
                    continue;
                Vec3i cell_displacement = {x,y,z};
                auto periodic_nearby_index = cell_index;
                Vec3i periodic_pos = {0,0,0};
                tbb::parallel_for(
                        tbb::blocked_range<std::size_t>(0, 3),
                        [&](const tbb::blocked_range<std::size_t> &r) {
                            for (std::size_t d = r.begin(); d != r.end(); ++d) {
                                int coord = static_cast<int>(cell_index.getCoord(d)) + cell_displacement.getCoord(d);
                                auto num_cell = static_cast<int>(_num_cells.getCoord(d));
                                std::size_t periodic_coord;
                                if (coord < 0)
                                    periodic_coord = (num_cell - (-coord % num_cell)) % num_cell;
                                else
                                    periodic_coord = coord % num_cell;
                                periodic_nearby_index.setCoord(d, periodic_coord);
                                if (coord < 0)
                                    periodic_pos.setCoord(d, coord);
                                else if (coord >= num_cell)
                                    periodic_pos.setCoord(d, coord - static_cast<int>(_num_cells.getCoord(d)) + 1);
                            }
                        }
                );

                auto particles_nearby_cell = getParticlesCell(periodic_nearby_index);
                for(auto i : particles_nearby_cell) {
                    // Insert copy of neighbor cell, possibly changing its periodic position
                    auto p = _particles.at(i);
                    p.setPosition(p.getPosition() + Vec3d(periodic_pos) * (_max_bounding_box - _min_bounding_box));
                    nearby_particles.push_back(p);
                }
            }
    return nearby_particles;
}

std::vector<std::size_t> LinkedCells::getParticlesCell(Vec3s cell_index) const {
    auto flatten_index = flattenIndex(Vec3d(cell_index));
    std::vector<std::size_t> particles;
    if(_head.at(flatten_index).has_value()) {
        auto tail_cell = _head.at(flatten_index).value();
        particles.push_back(tail_cell);
        while(_cell.at(tail_cell).has_value()) {
            tail_cell = _cell.at(tail_cell).value();
            particles.push_back(tail_cell);
        }
    }
    return particles;
}
