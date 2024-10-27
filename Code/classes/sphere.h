#pragma once

#include <cassert>
#include <gl/glut.h>
#include <memory>

#include "vector3f.h"
#include "color.h"
#include "interfaces.h"

bool fast_disregard = false;

class sphere : public shape3d
{
private:
    inline bool is_ray_inside(ray &r)
    {
        return (this->center - r.origin).length() < this->radius;
    }
    inline bool is_ray_outside(ray &r)
    {
        return (this->center - r.origin).length() > this->radius;
    }

public:
    vector3f center;
    double radius;
    sphere(vector3f center, double radius, std::shared_ptr<material> mat) : shape3d(mat)
    {
        this->center = center;
        this->radius = radius;
    }

    void calculate_hit_distance(ray &r)
    {
        double t;
        vector3f hypotenuse = r.origin - this->center;  // vector from sphere center to ray origin
        double h2 = hypotenuse.dot(hypotenuse);
        double r2 = this->radius * this->radius;

        double b = hypotenuse.dot(r.direction);
        double c = h2 - r2; // c > 0 if ray origin is outside the sphere
        if (c > 0 && b > 0) // ray origin is outside the sphere and pointing away from the sphere
            return;
            // this will disregard all spheres that are behind the ray origin (fast disregard)

        double discriminant = b * b - c;
        if (discriminant < 0)
            return;

        double sqrt_discriminant = sqrt(discriminant);
        double t1 = -b + sqrt_discriminant;
        double t2 = -b - sqrt_discriminant;
        
        if (c > 0){
            t = t1 < t2 ? t1 : t2;
        }
        else {
            t = t1 > t2 ? t1 : t2;  // ray origin is inside the sphere or on the sphere

        }      

        vector3f hit_point = r.origin + r.direction * t;
        vector3f normal = normal_at(hit_point);
        r.set_hit(t, normal, this->mat);
        return;
    }

    vector3f normal_at(vector3f &point)
    {
        return (point - this->center)/this->radius; //! RAZIN: changed
    }

    color get_color_at(vector3f &point) override
    {
        return this->clr;
    }

    void show()
    {
        // cout << "showing sphere " << this->center << endl;
        int stacks = 100, slices = 100;
        vector3f vertices[stacks + 1][slices + 1];

        for (int j = 0; j <= stacks; j++)
        {
            double phi = PI / 2 - j * PI / stacks;
            double y = this->center.y + radius * sin(phi);

            for (int i = 0; i <= slices; i++)
            {
                double theta = i * 2 * PI / slices;
                double x = this->center.x + radius * cos(phi) * cos(theta);
                double z = this->center.z + radius * cos(phi) * sin(theta);
                vertices[j][i] = vector3f(x, y, z);
            }
        }

        for (int j = 0; j < stacks; j++)
        {
            glBegin(GL_QUADS);
            for (int i = 0; i < slices; i++)
            {
                glColor3f(this->clr.r, this->clr.g, this->clr.b);
                glVertex3f(vertices[j][i].x, vertices[j][i].y, vertices[j][i].z);
                glVertex3f(vertices[j][i + 1].x, vertices[j][i + 1].y, vertices[j][i + 1].z);
                glVertex3f(vertices[j + 1][i + 1].x, vertices[j + 1][i + 1].y, vertices[j + 1][i + 1].z);
                glVertex3f(vertices[j + 1][i].x, vertices[j + 1][i].y, vertices[j + 1][i].z);
            }
            glEnd();
        }
    }

    void print()
    {
        std::cout << "Sphere: " << std::endl;
        std::cout << "Center: " << this->center << std::endl;
        std::cout << "Radius: " << this->radius << std::endl;
        std::cout << "Material: " << this->mat->type() << std::endl;

    }


    ~sphere()
    {
        std::cerr << "Sphere destructor called" << std::endl;
    }
};