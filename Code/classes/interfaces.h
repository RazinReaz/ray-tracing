#pragma once

#include <iostream>
#include <cmath>
#include <cassert>
#include <vector>
#include <memory>
#include "rtutils.h"

class shape3d;

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

    ray reflect(const vector3f &reflection_point, vector3f &normal);
    void set_hit(double hit_distance, vector3f normal, std::shared_ptr<material> mat, const double offset = 0.01);
    void reset_hit();
    void show();
    // assignment operator
    ray &operator=(const ray &r);
};

ray ray::reflect(const vector3f &reflection_point, vector3f &normal)
{
    vector3f reflected_direction = this->direction - normal * 2.0 * this->direction.dot(normal);
    return ray(reflection_point, reflected_direction);
};

void ray::set_hit(double hit_distance, vector3f normal, std::shared_ptr<material> mat, const double offset)
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

typedef struct face_
{
    // all faces are parallel to either xy, yz or zx plane
    vector3f normal;
    vector3f mid_point;
    double size;
    face_(){}

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

    inline bool within(const double &q, const double &min, const double &max)
    {
        return (q >= min && q <= max);
    }

    bool is_hit_by_ray(ray &r, double &distance)
    {
        distance = (mid_point - r.origin).dot(normal) / r.direction.dot(normal);
        if (distance < 0)
            return false;
        vector3f point = r.origin + r.direction * distance;
        return contains_point(point);
    }

    bool contains_point(const vector3f &point)
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

    void show() {
        glBegin(GL_QUADS);
        if (normal.x != 0) {
            glVertex3f(mid_point.x, mid_point.y - size / 2, mid_point.z - size / 2);
            glVertex3f(mid_point.x, mid_point.y + size / 2, mid_point.z - size / 2);
            glVertex3f(mid_point.x, mid_point.y + size / 2, mid_point.z + size / 2);
            glVertex3f(mid_point.x, mid_point.y - size / 2, mid_point.z + size / 2);
        } else if (normal.y != 0) {
            glVertex3f(mid_point.x - size / 2, mid_point.y, mid_point.z - size / 2);
            glVertex3f(mid_point.x + size / 2, mid_point.y, mid_point.z - size / 2);
            glVertex3f(mid_point.x + size / 2, mid_point.y, mid_point.z + size / 2);
            glVertex3f(mid_point.x - size / 2, mid_point.y, mid_point.z + size / 2);
        } else {
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


typedef struct triangle_ {
    vector3f a, b, c;
    vector3f normal;

    triangle_(vector3f a, vector3f b, vector3f c) {
        this->a = a;
        this->b = b;
        this->c = c;
        this->normal = (b-a).cross(c-a).normalize();
    }
    
    bool contains_point(vector3f &point) {
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

    bool is_hit_by_ray(ray &r, double &distance) {
        double D = -a.dot(normal);
        distance = -(r.origin.dot(normal) + D) / r.direction.dot(normal);
        if (distance < 0) return false;
        vector3f point = r.origin + r.direction * distance;
        return contains_point(point);
    }

    void show() {
        glBegin(GL_TRIANGLES);
        glVertex3f(a.x, a.y, a.z);
        glVertex3f(b.x, b.y, b.z);
        glVertex3f(c.x, c.y, c.z);
        glEnd();
    }

    void print() {
        std::cout << "normal : " << normal << std::endl;
    }
} triangle_;



class shape3d
{
public:
    
    std::shared_ptr<material> mat;
    color clr;
    shape3d(std::shared_ptr<material> m)
    {
        this->mat = m;
        this->clr = m->albedo;
    }
    // virtual bool is_ray_intersecting(ray& r) = 0;

    virtual void calculate_hit_distance(ray &r) = 0;
    virtual vector3f normal_at(vector3f &point) = 0; // assumes that the point is always on the shape
    virtual color get_color_at(vector3f &point) = 0;
    virtual void show() = 0;
    virtual void print() = 0;
    
    // color get_diffuse_and_specular_color(vector3f point, ray &r, std::vector<light *> lights, std::vector<shape3d *> objects);
    // color get_reflection_color(vector3f point, ray &r, std::vector<light *> lights, std::vector<shape3d *> objects, int recursions);
    std::shared_ptr<material> get_material() { return this->mat; }
};


// color shape3d::get_diffuse_and_specular_color(vector3f point, ray &r, std::vector<light *> lights, std::vector<shape3d *> objects)
// {
//     // assumes that the point is always on the shape
//     double lambert = 0,
//            phong = 0;
//     vector3f N = r.hit_info.normal;

//     for (auto light : lights)
//     {
//         if (!light->is_visible_from(point, N, objects))
//         {
//             continue;
//         }
//         vector3f to_source = light->position - point;
//         double distance = to_source.length();
//         to_source = to_source / distance;
//         double scaling_factor = exp(-1 * distance * distance * light->falloff); //! optimizable? 

//         vector3f R = r.reflect(point, N).direction; //! optimizable?
//         lambert += scaling_factor * std::max(0.0, N.dot(to_source)); // diffuse component
//         phong += pow(R.dot(to_source), shininess) * scaling_factor; // specular component
//     }
    
//     color object_color = get_color_at(point);

//     double diffuse_component = this->diffuse * lambert;
//     double specular_component = this->specular * phong;
    
//     return object_color * (this->ambient + diffuse_component + specular_component);
// };

// color shape3d::get_reflection_color(vector3f point, ray &r, std::vector<light *> lights, std::vector<shape3d *> objects, int recursions)
// {

//     ray current_ray = r;
//     double offset = 0.1, multiplier = 1.0;
//     color reflected = {0, 0, 0};
//     vector3f final_point = point;
//     while (--recursions)
//     {
//         vector3f normal = current_ray.hit_info.normal;
//         current_ray = current_ray.reflect(final_point, normal);
//         current_ray.origin = current_ray.origin + current_ray.direction * offset;
//         for (auto object : objects)
//         {
//             object->calculate_hit_distance(current_ray);
//         }
//         if (!current_ray.hit_info.hit)
//         {
//             reflected += sky_color * multiplier;
//             break;
//         }
//         final_point = current_ray.origin + current_ray.direction * current_ray.hit_info.distance;
//         reflected += current_ray.hit_info.object->get_diffuse_and_specular_color(final_point, current_ray, lights, objects) * multiplier;
//         multiplier *= current_ray.hit_info.object->reflection;
//     }
//     // vector3f final_point = current_ray.origin + current_ray.direction *current_ray.hit_info.distance;
//     // color reflected_ = current_ray.hit_info.object->get_diffuse_and_specular_color(final_point, current_ray, lights, objects );
//     return reflected * this->reflection;
// }

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