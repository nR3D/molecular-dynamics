#ifndef PARTICLE_FLUID_PARTICLE_H
#define PARTICLE_FLUID_PARTICLE_H


#include "Vec3.h"

class Particle {
public:
    explicit Particle(Vec3d position = {0,0,0}, Vec3d velocity = {0,0,0}, Vec3d forces = {0,0,0}, double mass = 1);

    explicit Particle(Vec3d position = {0,0,0}, double mass = 1);

    double getMass() const;

    void setMass(double mass);

    const Vec3d &getPosition() const;

    void setPosition(const Vec3d &position);

    const Vec3d &getVelocity() const;

    void setVelocity(const Vec3d &velocity);

    const Vec3d &getForces() const;

    void setForces(const Vec3d &forces);

    void setFixed() {
        fixed = true;
    }

    bool isFixed() const {
        return fixed;
    }

    const Vec3d &getColor() const;

    void setColor(const Vec3d &color);

private:
    double _mass;
    bool fixed = false;
    Vec3d _position, _velocity, _forces;

    Vec3d _color;
};


#endif //PARTICLE_FLUID_PARTICLE_H
