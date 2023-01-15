#include "ParticleSimulation.h"

ParticleSimulation::ParticleSimulation(const std::string &filepath) : savefile(filepath) {
    Logger::instance().open(filepath);
}

ParticleSimulation::~ParticleSimulation() {
    Logger::instance().close();
}

void ParticleSimulation::addParticlesToVTKGrid(const std::vector<Particle> &particles,
                                               vtkSmartPointer<vtkUnstructuredGrid> &dataSet) const {
    auto pts = vtkSmartPointer<vtkPoints>::New();
    auto velocity = vtkSmartPointer<vtkDoubleArray>::New();
    velocity->SetNumberOfComponents(3);
    velocity->SetName("Velocity");
    auto color = vtkSmartPointer<vtkDoubleArray>::New();
    color->SetNumberOfComponents(3);
    color->SetName("Color");

    // Loop through particles
    Vec3d vObj;
    for (const auto& particle : particles) {
        /*if(particle.isFixed())
            continue;*/

        // Position
        auto pos = particle.getPosition();
        pts->InsertNextPoint(pos.getX(), pos.getY(), pos.getZ());

        // Velocity
        auto vel = particle.getVelocity();
        velocity->InsertNextTuple3(vel.getX(), vel.getY(), vel.getZ());

        // Color
        auto col = particle.getColor();
        if(particle.isFixed())
            col = {200,200,200};
        color->InsertNextTuple3(col.getX(), col.getY(), col.getZ());
    }

    // Set the points
    dataSet->SetPoints(pts);
    // Add points to data set
    dataSet->GetPointData()->AddArray(velocity);
    dataSet->GetPointData()->AddArray(color);
}
