#include <iostream>
#include <chrono>

#include "src/methods/MolecularDynamics.h"
#include "src/utils/math.h"

using namespace oneapi;

int main() {

    ParticleSimulation* sim = new MolecularDynamics(4, 2, "/tmp/file_sim");

    double v0 = std::sqrt(2.0);
    Vec3i grid_size = {0,10,10};
    for(auto i = -grid_size.getX(); i <= grid_size.getX(); ++i)
        for(auto j = -grid_size.getY(); j <= grid_size.getY(); ++j)
            for(auto k = -grid_size.getZ(); k <= grid_size.getZ(); ++k) {
                Vec3d rand_vec = {0, math::random_number(-1,1), math::random_number(-1,1)};
                Particle p {{4.0*i,
                             4.0*j + (k % 2 == 0 ? 2.0 : 0),
                             4.0*k}, rand_vec.unit() * v0, {0,0,0}, 1};
                sim->spawn(p);
            }

    auto start = std::chrono::steady_clock::now();
    for(auto i = 0; i < 2000; ++i) {
        sim->step(0.005);
        if(i%10 == 0)
            sim->print(i/10);
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";

    return 0;
}
