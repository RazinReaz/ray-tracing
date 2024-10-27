#pragma once
#include <cstdlib>
#include "vector3f.h"

inline double random_double()
{
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max)
{
    return min + (max - min) * random_double();
}

inline vector3f random_unit_vector3f()
{
    //! optimizable
    while (true) {
        vector3f v = {random_double(-1, 1), random_double(-1, 1), random_double(-1, 1)};
        if (v.length_squared() >= 1) continue;
        return v.normalize();
    }
}