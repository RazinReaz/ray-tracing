
#pragma once

#include "vector3f.h"
#include "ray.h"
#include <GL/glut.h>
#include <cassert>
#include "rtutils.h"

typedef struct face_
{
    // all faces are parallel to either xy, yz or zx plane
    vector3f normal;
    vector3f mid_point;
    double size;
    face_() {}

    face_(vector3f normal, vector3f mid_point, double size)
    {
        this->normal = normal;
        this->mid_point = mid_point;
        this->size = size;
        assert(size > 0);
        // assert(normal.length() == 1); // omitted to reduce sqrt ops
        assert((normal.x == 0 && normal.y == 0) || (normal.y == 0 && normal.z == 0) || (normal.z == 0 && normal.x == 0));
    }

    // assignment operator
    face_ &operator=(const face_ &f)
    {
        this->normal = f.normal;
        this->mid_point = f.mid_point;
        this->size = f.size;
        return *this;
    }

    inline bool within(const double &q, const double &min, const double &max) const
    {
        return (q >= min && q <= max);
    }

    bool is_hit_by_ray(ray &r, double &distance) const 
    {
        distance = (mid_point - r.origin).dot(normal) / r.direction.dot(normal);
        if (distance < 0)
            return false;
        vector3f point = r.origin + r.direction * distance;
        return contains_point(point);
    }

    bool contains_point(const vector3f &point) const
    {
        if (normal.x != 0)
        {
            // face parallel to yz plane
            return within(point.y, mid_point.y - size / 2, mid_point.y + size / 2) && within(point.z, mid_point.z - size / 2, mid_point.z + size / 2) && approx_equals(point.x, mid_point.x);
        }
        else if (normal.y != 0)
        {
            // face parallel to zx plane
            return within(point.x, mid_point.x - size / 2, mid_point.x + size / 2) && within(point.z, mid_point.z - size / 2, mid_point.z + size / 2) && approx_equals(point.y, mid_point.y);
        }
        else
        {
            // face parallel to xy plane
            return within(point.x, mid_point.x - size / 2, mid_point.x + size / 2) && within(point.y, mid_point.y - size / 2, mid_point.y + size / 2) && approx_equals(point.z, mid_point.z);
        }
    }

    void show()
    {
        glBegin(GL_QUADS);
        if (normal.x != 0)
        {
            glVertex3f(mid_point.x, mid_point.y - size / 2, mid_point.z - size / 2);
            glVertex3f(mid_point.x, mid_point.y + size / 2, mid_point.z - size / 2);
            glVertex3f(mid_point.x, mid_point.y + size / 2, mid_point.z + size / 2);
            glVertex3f(mid_point.x, mid_point.y - size / 2, mid_point.z + size / 2);
        }
        else if (normal.y != 0)
        {
            glVertex3f(mid_point.x - size / 2, mid_point.y, mid_point.z - size / 2);
            glVertex3f(mid_point.x + size / 2, mid_point.y, mid_point.z - size / 2);
            glVertex3f(mid_point.x + size / 2, mid_point.y, mid_point.z + size / 2);
            glVertex3f(mid_point.x - size / 2, mid_point.y, mid_point.z + size / 2);
        }
        else
        {
            glVertex3f(mid_point.x - size / 2, mid_point.y - size / 2, mid_point.z);
            glVertex3f(mid_point.x + size / 2, mid_point.y - size / 2, mid_point.z);
            glVertex3f(mid_point.x + size / 2, mid_point.y + size / 2, mid_point.z);
            glVertex3f(mid_point.x - size / 2, mid_point.y + size / 2, mid_point.z);
        }
        glEnd();
    }

    ~face_()
    {
    }
} face_;
