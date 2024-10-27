#pragma once

#include "color.h"
#include "vector3f.h"
#include "random.h"

class ray;

class material {
    public:
    color albedo; //! RAZIN : THIS MIGHT CHANGE
    material() = default;
    material(const color& a) : albedo(a) {}
    virtual ~material() = default;
    virtual ray scatter(ray& r, vector3f point, vector3f normal) = 0;
    virtual std::string type() = 0;
};