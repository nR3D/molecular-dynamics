#ifndef PARTICLE_FLUID_LINKEDCELLS_H
#define PARTICLE_FLUID_LINKEDCELLS_H


#include <vector>
#include <optional>
#include <iostream>
#include "../utils/Particle.h"

class LinkedCells {
public:

    void distribute(std::vector<Particle>& particles, Vec3s num_cells, Vec3d min_bounding_box, Vec3d max_bounding_box);

    std::vector<std::size_t> getParticlesCell(Vec3s cell_index) const;

    std::vector<Particle> getNearbyParticlesCell(Vec3s cell_index, int radius) const;

    const Vec3d &getMinBoundingBox() const {
        return _min_bounding_box;
    }

    const Vec3d &getMaxBoundingBox() const {
        return _max_bounding_box;
    }

    const Vec3s &getNumCells() const {
        return _num_cells;
    }

    const Vec3d &getSizeCell() const {
        return _size_cell;
    }

private:
    std::size_t flattenIndex(Vec3d index) const;

    std::vector<std::optional<std::size_t>> _head, _cell;
    Vec3d _min_bounding_box, _max_bounding_box, _size_cell;
    Vec3s _num_cells;

    std::vector<Particle> _particles;
};


#endif //PARTICLE_FLUID_LINKEDCELLS_H
