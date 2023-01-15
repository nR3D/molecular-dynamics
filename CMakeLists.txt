cmake_minimum_required(VERSION 3.16)
project(particle_fluid)

set(CMAKE_CXX_STANDARD 20)

find_package(TBB REQUIRED)
message (STATUS "TBB_VERSION: ${TBB_VERSION}")

find_package(VTK REQUIRED)
message (STATUS "VTK_VERSION: ${VTK_VERSION}")
include(${VTK_USE_FILE})

add_executable(particle_fluid
        main.cpp
        src/ParticleSimulation.cpp
        src/ParticleSimulation.h
        src/utils/Vec3D.cpp
        src/utils/Vec3D.h
        src/utils/Particle.cpp
        src/utils/Particle.h
        src/methods/MolecularDynamics.cpp
        src/methods/MolecularDynamics.h
        src/utils/Logger.cpp
        src/utils/Logger.hpp
        src/methods/MetropolisMonteCarlo.cpp
        src/methods/MetropolisMonteCarlo.h
        src/utils/math.cpp
        src/utils/math.h
        src/datastructures/LinkedCells.cpp
        src/datastructures/LinkedCells.h
        src/utils/Vec3.cpp
        src/utils/Vec3.h)
target_link_libraries(particle_fluid TBB::tbb)
target_link_libraries(particle_fluid ${VTK_LIBRARIES})
