#pragma once 

#include <iostream>
#include <vector>
#include "vector3f.h"
#include "color.h"


#define EPSILON 0.00001

inline bool approx_equals(double a, double b)
{
    return fabs(a - b) < EPSILON;
}

inline double gamma_correction(double linear_component)
{
    return linear_component < 0.0031308 ? 12.92 * linear_component : 1.055 * pow(linear_component, 1 / 2.4) - 0.055;
    // if (linear_component > 0) 
    //     return std::sqrt(linear_component);
    // return 0;
}

inline color gamma_correction(color c)
{
    return {gamma_correction(c.r), gamma_correction(c.g), gamma_correction(c.b)};
}

