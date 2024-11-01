#pragma once

#include "vector3f.h"
#include <memory>
#include <GL/glut.h>

class material;

typedef struct Hit_info
{
    bool hit;
    double distance;
    vector3f normal;
    vector3f point;
    std::shared_ptr<material> mat;

} Hit_info;

class ray
{
public:
    vector3f origin;
    vector3f direction;
    Hit_info hit_info;

    ray(vector3f origin, vector3f direction)
    {
        this->origin = origin;
        this->direction = direction.normalize();

        this->hit_info.distance = 1000000.0;
        this->hit_info.hit = false;
        this->hit_info.normal = vector3f(0, 0, 0);
        this->hit_info.point = vector3f(0, 0, 0);
        this->hit_info.mat = NULL;
    }

    ray reflect(const vector3f &reflection_point, const vector3f &normal) const;
    void set_hit(double hit_distance, const vector3f& normal, std::shared_ptr<material> mat, const double offset = 0.01);
    void reset_hit();
    void show();
    // assignment operator
    ray &operator=(const ray &r);
};

ray ray::reflect(const vector3f &reflection_point, const vector3f &normal) const
{
    vector3f reflected_direction = this->direction - normal * 2.0 * this->direction.dot(normal);
    return ray(reflection_point, reflected_direction);
};

void ray::set_hit(double hit_distance, const vector3f& normal, std::shared_ptr<material> mat, const double offset)
{
    if (hit_distance < 0)
        return;
    if (this->hit_info.distance <= hit_distance)
        return;

    this->hit_info.hit = true;
    this->hit_info.distance = hit_distance;
    this->hit_info.normal = normal;
    this->hit_info.mat = mat;
    this->hit_info.point = this->origin + this->direction * hit_distance + normal * offset;
    //! RAZIN: NORMAL in hitinfo will always be pointing towards the ray origin
    if (this->direction.dot(normal) > 0)
    {
        this->hit_info.normal = normal * -1;
    }
    return;
};

void ray::reset_hit()
{
    this->hit_info.distance = 1000000.0;
    this->hit_info.hit = false;
    this->hit_info.normal = vector3f(0, 0, 0);
    this->hit_info.point = vector3f(0, 0, 0);
    this->hit_info.mat = NULL;
};

void ray::show()
{
    glPushMatrix();
    glBegin(GL_LINES);
    glColor3f(1, 1, 1);
    glLineWidth(10);
    glVertex3f(origin.x, origin.y, origin.z);
    glVertex3f(origin.x + direction.x, origin.y + direction.y, origin.z + direction.z);
    glEnd();
    glPopMatrix();
};

ray &ray::operator=(const ray &r)
{
    this->origin = r.origin;
    this->direction = r.direction;
    this->hit_info = r.hit_info;
    return *this;
};