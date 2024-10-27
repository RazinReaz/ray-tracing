#pragma once

#include "color.h"
#include "vector3f.h"
#include "random.h"
#include "ray.h"

class material
{
public:
    color albedo; //! RAZIN : THIS MIGHT CHANGE
    material() = default;
    material(const color &a) : albedo(a) {}
    virtual ~material() = default;
    virtual ray scatter(ray &r, vector3f point, vector3f normal) = 0;
    virtual std::string type() = 0;
};

class metal : public material
{
public:
    double fuzziness;

    metal(const color &a, double f) : material(a), fuzziness(f < 1 ? f : 1) {}

    virtual ray scatter(ray &r, vector3f point, vector3f normal) override
    {
        vector3f reflected = r.reflect(point, normal).direction;
        vector3f scatter_direction = reflected + random_unit_vector3f() * fuzziness;
        return ray(point, scatter_direction);
    }

    virtual std::string type() override
    {
        return "metal";
    }
};

class lambertian : public material
{
public:
    lambertian(const color &a) : material(a) {}

    virtual ray scatter(ray &r, vector3f point, vector3f normal) override
    {
        vector3f scatter_direction = normal + random_unit_vector3f();
        if (scatter_direction.near_zero())
            scatter_direction = normal;

        return ray(point, scatter_direction);
    }

    virtual std::string type() override
    {
        return "lambertian";
    }
};