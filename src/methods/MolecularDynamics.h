#ifndef PARTICLE_FLUID_MOLECULARDYNAMICS_H
#define PARTICLE_FLUID_MOLECULARDYNAMICS_H

#include <iostream>

#include <tbb/concurrent_vector.h>
#include <tbb/parallel_for.h>
#include <vtkUnstructuredGridWriter.h>
#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>
#include <vtkUnstructuredGridWriter.h>

#include "../ParticleSimulation.h"
#include "../datastructures/LinkedCells.h"

using namespace oneapi;

class MolecularDynamics : public ParticleSimulation {
public:
    MolecularDynamics(double energy_interaction, double length_interaction,const std::string& filepath);

    void step(double dt) override;

    void spawn(const Particle &particle) override;

    void spawn(const std::vector<Particle> &particles) override;

    void print(std::size_t timestep) const override;

protected:

    void compute_force_interaction(Particle& p1, Particle& p2) const;

    double force(double distance) const;

private:

    double Lennard_Jones_force(double distance) const;

    double isokinetic_scaling() const;

    double temperature() const;

    void compute_step_cell(Vec3s cell_index, double dt);

private:
    double _energy_interaction, _length_interaction;
    std::vector<Particle> _particles;
    LinkedCells linkedCells;

    static tbb::affinity_partitioner ap;
};


#endif //PARTICLE_FLUID_MOLECULARDYNAMICS_H
