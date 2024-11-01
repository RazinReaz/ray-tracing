#pragma once

#include "color.h"
#include "vector3f.h"
#include "random.h"
#include "ray.h"

class material
{
public:
    material() = default;
    virtual ~material() = default;
    virtual ray scatter(ray &r, const vector3f& point, const vector3f& normal) const = 0;
    virtual color get_color() const = 0;
    virtual color get_emission() const = 0;
    virtual std::string type()const = 0;
};

class metal : public material
{
public:
    double fuzziness;
    color albedo;
    metal(const color &a, double f) : albedo(a), fuzziness(f < 1 ? f : 1) {}

    virtual ray scatter(ray &r, const vector3f &point, const vector3f& normal) const override
    {
        const vector3f reflected = r.reflect(point, normal).direction;
        vector3f scatter_direction = reflected + random_unit_vector3f() * fuzziness;
        return ray(point, scatter_direction);
    }

    virtual std::string type() const override
    {
        return "metal";
    }

    virtual color get_color() const override
    {
        return albedo;
    }

    virtual color get_emission() const override
    {
        return {0, 0, 0};
    }
};



class lambertian : public material
{
    color albedo;
public:
    lambertian(const color &a) : albedo(a) {}

    virtual ray scatter(ray &r, const vector3f &point, const vector3f &normal) const override
    {
        vector3f scatter_direction = normal + random_unit_vector3f();
        if (scatter_direction.near_zero())
            scatter_direction = normal;

        return ray(point, scatter_direction);
    }

    virtual std::string type() const override
    {
        return "lambertian";
    }

    virtual color get_color() const override
    {
        return albedo;
    }

    virtual color get_emission() const override
    {
        return {0, 0, 0};
    }
};



class emissive : public material
{
    color emission_color;
    double emission_intensity;

public:
    emissive(const color &e, double i) : emission_color(e), emission_intensity(i) {}
    virtual ray scatter(ray &r, const vector3f &point, const vector3f &normal) const override
    {
        vector3f scatter_direction = normal + random_unit_vector3f();
        if (scatter_direction.near_zero())
            scatter_direction = normal;

        return ray(point, scatter_direction);
    }

    virtual std::string type() const override
    {
        return "emisive";
    }

    virtual color get_color() const override
    {
        return emission_color;
    }

    virtual color get_emission() const override
    {
        return emission_color * emission_intensity;
    }
};