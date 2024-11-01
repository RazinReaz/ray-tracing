#pragma once
#include <cstdlib>
#include "vector3f.h"

// Xoroshiro/Xoshiro/Xorshift random number generator
uint32_t seed = 0x12345678;
inline uint32_t XOrShift32(uint32_t *state)
{
    uint32_t x = *state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    *state = x;
    return x;
}

static uint32_t PCG_hash(uint32_t &input)
{
    uint32_t state = input * 747796405u + 2891336453u;
    uint32_t word = (( state >> ((state >> 28) + 4u)) ^ state ) * 277803737u;
    return (word >> 22u) ^ word;
}

inline double random_double()
{
    seed = PCG_hash(seed); 
    return seed / (double)UINT32_MAX;
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