#include "Particle.h"

Particle::Particle(Vec3d position, Vec3d velocity, Vec3d forces, double mass) :
        _position(position), _velocity(velocity), _forces(forces), _mass(mass) {}

Particle::Particle(Vec3d position, double mass) :
        _position(position), _mass(mass) {}

double Particle::getMass() const {
    return _mass;
}

void Particle::setMass(double mass) {
    _mass = mass;
}

const Vec3d &Particle::getPosition() const {
    return _position;
}

void Particle::setPosition(const Vec3d &position) {
    _position = position;
}

const Vec3d &Particle::getVelocity() const {
    return _velocity;
}

void Particle::setVelocity(const Vec3d &velocity) {
    _velocity = velocity;
}

const Vec3d &Particle::getForces() const {
    return _forces;
}

void Particle::setForces(const Vec3d &forces) {
    _forces = forces;
}

const Vec3d &Particle::getColor() const {
    return _color;
}

void Particle::setColor(const Vec3d &color) {
    _color = color;
}
