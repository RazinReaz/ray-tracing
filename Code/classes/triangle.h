#pragma once

#include "vector3f.h"
#include "ray.h"
#include <GL/glut.h>

typedef struct triangle_
{
    vector3f a, b, c;
    vector3f normal;

    triangle_(vector3f a, vector3f b, vector3f c)
    {
        this->a = a;
        this->b = b;
        this->c = c;
        this->normal = (b - a).cross(c - a).normalize();
    }

    bool contains_point(const vector3f &point) const
    {
        vector3f ab = b - a;
        vector3f ac = c - a;
        vector3f ap = point - a;
        float dot00 = ac.dot(ac);
        float dot01 = ac.dot(ab);
        float dot02 = ac.dot(ap);
        float dot11 = ab.dot(ab);
        float dot12 = ab.dot(ap);
        float denom = dot00 * dot11 - dot01 * dot01;
        if (denom == 0.0f)
            return false; // Avoid division by zero
        float invDenom = 1.0f / denom;

        float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
        float v = (dot00 * dot12 - dot01 * dot02) * invDenom;
        return (u >= 0) && (v >= 0) && (u + v <= 1);
    }

    bool is_hit_by_ray(ray &r, double &distance)
    {
        double D = -a.dot(normal);
        distance = -(r.origin.dot(normal) + D) / r.direction.dot(normal);
        if (distance < 0)
            return false;
        vector3f point = r.origin + r.direction * distance;
        return contains_point(point);
    }

    void show()
    {
        glBegin(GL_TRIANGLES);
        glVertex3f(a.x, a.y, a.z);
        glVertex3f(b.x, b.y, b.z);
        glVertex3f(c.x, c.y, c.z);
        glEnd();
    }

    void print()
    {
        std::cout << "normal : " << normal << std::endl;
    }
} triangle_;
