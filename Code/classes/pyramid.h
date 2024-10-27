#pragma once

#include <cassert>
#include <gl/glut.h>
#include <cmath>
#include <iostream>
#include <vector>

#include "vector3f.h"
#include "interfaces.h"
#include "color.h"


class pyramid : public shape3d {
    public:

    vector3f base_center;
    double width, height;
    face_ face;
    std::vector<triangle_*> triangles;

    pyramid(vector3f base_center, double width, double height, std::shared_ptr<material> mat) : shape3d(mat) {
        this->base_center = base_center;
        this->width = width;
        this->height = height;

        vector3f base_points[4] = {
            base_center + vector3f(width / 2, 0, width / 2),
            base_center + vector3f(width / 2, 0, -width / 2),
            base_center + vector3f(-width / 2, 0, -width / 2),
            base_center + vector3f(-width / 2, 0, width / 2)
        };
        vector3f apex = base_center + vector3f(0, height, 0);
        this->triangles.push_back(new triangle_(base_points[0], base_points[1], apex));
        this->triangles.push_back(new triangle_(base_points[1], base_points[2], apex));
        this->triangles.push_back(new triangle_(base_points[2], base_points[3], apex));
        this->triangles.push_back(new triangle_(base_points[3], base_points[0], apex));
        this->face = face_(vector3f(0, -1, 0), base_center, width);
    }

    vector3f normal_at(vector3f &point) override {
        for (auto triangle : triangles) {
            if (triangle->contains_point(point)) {
                return triangle->normal;
            }
        }
        return face.normal;

    }

    color get_color_at(vector3f &point) override {
        return clr;
    }

    void calculate_hit_distance(ray &r) {
        double distance = -1;
        for (auto triangle : triangles) {
            if (triangle->is_hit_by_ray(r, distance)) {
                r.set_hit(distance, triangle->normal, this->mat);
            }
        }
        if (face.is_hit_by_ray(r, distance)) {
            r.set_hit(distance, face.normal, this->mat);
        }
        return;
    }

    void show() override {
        glColor3f(clr.r, clr.g, clr.b);
        for (auto triangle : triangles) {
            triangle->show();
        }
        
        glBegin(GL_QUADS);
        glVertex3f(face.mid_point.x - face.size / 2, face.mid_point.y, face.mid_point.z - face.size / 2);
        glVertex3f(face.mid_point.x + face.size / 2, face.mid_point.y, face.mid_point.z - face.size / 2);
        glVertex3f(face.mid_point.x + face.size / 2, face.mid_point.y, face.mid_point.z + face.size / 2);
        glEnd();
    }

    void print() override {
        for (auto triangle : triangles) {
            triangle->print();
        }
        std::cout << "face with normal " << face.normal << " and mid point " << face.mid_point << std::endl;
    }
};
