#ifndef PARTICLE_FLUID_VEC3_H
#define PARTICLE_FLUID_VEC3_H

#include <array>
#include <cmath>

template<typename T>
class Vec3 {
public:
    Vec3(T x, T y, T z) : _data({x, y, z}) {}

    explicit Vec3(T value) : _data({value, value, value}) {}

    template<typename U>
    explicit Vec3(Vec3<U> vec) {
        _data[0] = vec.getX();
        _data[1] = vec.getY();
        _data[2] = vec.getZ();
    }

    Vec3() {}

    T getX() const {
        return _data.at(0);
    }

    void setX(T x) {
        _data.at(0) = x;
    }

    T getY() const {
        return _data.at(1);
    }

    void setY(T y) {
        _data.at(1) = y;
    }

    T getZ() const {
        return _data.at(2);
    }

    void setZ(T z) {
        _data.at(2) = z;
    }

    T getCoord(std::size_t index) const {
        return _data.at(index);
    }

    void setCoord(std::size_t index, T value) {
        _data.at(index) = value;
    }

    Vec3 operator+(Vec3 vec) const {
        Vec3 res = *this;
        for(int i = 0; i < 3; ++i)
            res._data.at(i) += vec.getCoord(i);
        return res;
    }

    Vec3 operator-(Vec3 vec) const {
        Vec3 res = *this;
        for(int i = 0; i < 3; ++i)
            res._data.at(i) -= vec.getCoord(i);
        return res;
    }

    Vec3 operator*(Vec3 vec) const {
        Vec3 res = *this;
        for(int i = 0; i < 3; ++i)
            res._data.at(i) *= vec.getCoord(i);
        return res;
    }

    Vec3 operator/(Vec3 vec) const {
        Vec3 res = *this;
        for(int i = 0; i < 3; ++i)
            res._data.at(i) /= vec.getCoord(i);
        return res;
    }

    Vec3 operator+(T value) const {
        Vec3 res = *this;
        for(auto& d : res._data)
            d += value;
        return res;
    }

    Vec3 operator-(T value) const {
        Vec3 res = *this;
        for(auto& d : res._data)
            d -= value;
        return res;
    }

    Vec3 operator*(T value) const {
        Vec3 res = *this;
        for(auto& d : res._data)
            d *= value;
        return res;
    }

    Vec3 operator/(T value) const {
        Vec3 res = *this;
        for(auto& d : res._data)
            d /= value;
        return res;
    }

    bool operator==(Vec3 vec) const {
        for (int i = 0; i < 3; ++i)
            if(getCoord(i) != vec.getCoord(i))
                return false;
        return true;
    }

    bool operator!=(Vec3 vec) const {
        return !(*this == vec);
    }

    decltype(auto) begin() {
        return _data.begin();
    }

    decltype(auto) end() {
        return _data.end();
    }

    decltype(auto) cbegin() const {
        return _data.cbegin();
    }

    decltype(auto) cend() const {
        return _data.cend();
    }

private:
    std::array<T, 3> _data;
};

using Vec3s = Vec3<std::size_t>;
using Vec3i = Vec3<int>;


class Vec3d : public Vec3<double> {
public:
    Vec3d(Vec3<double> v) : Vec3d(v.getX(), v.getY(), v.getZ()) {}

    using Vec3<double>::Vec3;

    double dot(const Vec3d &vec) const {
        double sum = 0;
        for(int i = 0; i < 3; ++i)
            sum += getCoord(i) * vec.getCoord(i);
        return sum;
    }

    Vec3d cross(const Vec3d &vec) const {
        return {
                getY() * vec.getZ() - getZ() * vec.getY(),
                getZ() * vec.getX() - getX() * vec.getZ(),
                getX() * vec.getY() - getY() * vec.getX()
        };
    }

    double length() const {
        return std::sqrt(dot(*this));
    }

    double distance(const Vec3d &vec) const {
        double diff_x = getCoord(0) - vec.getCoord(0),
                diff_y = getCoord(1) - vec.getCoord(1),
                diff_z = getCoord(2) - vec.getCoord(2);
        return std::sqrt(diff_x * diff_x + diff_y * diff_y + diff_z * diff_z);
    }

    Vec3d unit() const {
        auto len = length();
        if(len == 0)
            return *this;
        return *this / length();
    }
};

#endif //PARTICLE_FLUID_VEC3_H
