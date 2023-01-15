#ifndef PARTICLE_FLUID_MATH_H
#define PARTICLE_FLUID_MATH_H


#include <random>

namespace math {
    static std::random_device rd;
    static std::mt19937 mt(rd());
    static std::uniform_real_distribution<double> dist(0.0, 1.0);

    double random_number(double min, double max);
};


#endif //PARTICLE_FLUID_MATH_H
