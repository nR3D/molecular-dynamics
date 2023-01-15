#include "math.h"

double math::random_number(double min, double max) {
    return dist(mt) * (max - min) + min;
}
