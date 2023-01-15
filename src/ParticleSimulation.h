#ifndef PARTICLE_FLUID_PARTICLESIMULATION_H
#define PARTICLE_FLUID_PARTICLESIMULATION_H


#include <vector>
#include <fstream>
#include <filesystem>
#include <vtkPoints.h>
#include <vtkStructuredGrid.h>
#include <vtkSmartPointer.h>
#include <vtkDoubleArray.h>
#include <vtkPointData.h>
#include <vtkUnstructuredGrid.h>
#include "utils/Particle.h"
#include "utils/Logger.hpp"

class ParticleSimulation {
public:
    explicit ParticleSimulation(const std::string& filepath);
    virtual ~ParticleSimulation();

    virtual void step(double dt) = 0;
    virtual void spawn(const Particle &particle) = 0;
    virtual void spawn(const std::vector<Particle> &particles) = 0;
    virtual void print(std::size_t timestep) const = 0;

protected:
    void addParticlesToVTKGrid(const std::vector<Particle>& particles,
                               vtkSmartPointer<vtkUnstructuredGrid>& dataSet) const;

    std::filesystem::path savefile;
};


#endif //PARTICLE_FLUID_PARTICLESIMULATION_H
