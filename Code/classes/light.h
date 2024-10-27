#pragma once

#include <vector>
#include "shape3d.h"

class light
{
public:
    vector3f position;
    double falloff, offset = 0.1;

    light(vector3f position, double falloff)
    {
        this->position = position;
        this->falloff = falloff;
    }
    virtual bool is_visible_from(vector3f point, vector3f &normal, std::vector<shape3d *> shape) = 0;
    virtual void show() = 0;
    virtual void print() = 0;
};
