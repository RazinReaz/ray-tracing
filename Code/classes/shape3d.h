#pragma once

#include <memory>
#include "vector3f.h"
#include "material.h"
#include "ray.h"
#include "color.h"

class shape3d
{
public:
    std::shared_ptr<material> mat;
    color clr;
    shape3d(std::shared_ptr<material> m)
    {
        this->mat = m;
        this->clr = m->get_color();
    }

    virtual void calculate_hit_distance(ray &r) const = 0;
    virtual vector3f normal_at(const vector3f &point) const = 0; // assumes that the point is always on the shape
    virtual color get_color_at(const vector3f &point) const = 0;
    virtual void show() = 0;
    virtual void print() = 0;

    std::shared_ptr<material> get_material() { return this->mat; }
};
