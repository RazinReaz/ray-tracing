#pragma once

#include <gl/glut.h>
#include <fstream>
#include <iostream>
#include <memory>
#include "color.h"
#include "shape3d.h"


class plane : public shape3d
{
public:
    plane(std::shared_ptr<material> mat) : shape3d(mat) {}

    void calculate_hit_distance(ray &r) const override
    {
        double hit_distance = -1.0 * r.origin.z / r.direction.z;
        if (hit_distance < 0)
            return;
        r.set_hit(hit_distance, vector3f(0, 0, 1), this->mat);
    }

    vector3f normal_at(const vector3f &point) const override
    {
        return vector3f(0, 0, 1);
    }

    color get_color_at(const vector3f &point) const override
    {
        return this->mat->get_color();
    }

    void show() override {
        glBegin(GL_QUADS);
        glColor3f(this->mat->get_color().r, this->mat->get_color().g, this->mat->get_color().b);
        glVertex3f(-1000, -1000, 0);
        glVertex3f(1000, -1000, 0);
        glVertex3f(1000, 1000, 0);
        glVertex3f(-1000, 1000, 0);
        glEnd();
    }

    void print() override {
        std::cout << "Plane: " << std::endl;
        std::cout << "Material: " << this->mat->type() << std::endl;
    }
};



class checkerboard : public shape3d
{
    const int i_limit = 100, j_limit = 100;
public:
    int size;
    checkerboard(int size, std::shared_ptr<material> mat) : shape3d(mat){
        this->size = size;
    }

    void calculate_hit_distance(ray &r) const override
    {
        double hit_distance = -1.0 * r.origin.z / r.direction.z;
        if (hit_distance < 0)
            return;
        r.set_hit(hit_distance, vector3f(0, 0, 1), this->mat);
    }

    vector3f normal_at(const vector3f &point) const override
    {
        return vector3f(0, 0, 1);
    }

    color get_color_at(const vector3f &point) const override
    {
        int i = (int) (point.y + i_limit * this->size) / this->size; // don't optimize this
        int j = (int) (point.x + j_limit * this->size) / this->size; // don't optimize this
        if ((i + j) % 2 == 0)
            return color(1, 1, 1);
        else
            return color(0, 0, 0);
    }

    void show() override {
        for (int i = -i_limit; i < i_limit; i++) {
            for (int j = -j_limit; j < j_limit; j++) {
                if ((i + j) % 2 == 0) 
                    glColor3f(1, 1, 1);
                else 
                    glColor3f(0, 0, 0);
                glBegin(GL_QUADS);
                glVertex3f(j * this->size, i * this->size, 0);
                glVertex3f((j + 1) * this->size, i * this->size, 0);
                glVertex3f((j + 1) * this->size, (i + 1) * this->size, 0);
                glVertex3f(j * this->size, (i + 1) * this->size, 0);
                glEnd();
            }
        }
    }

    void print() override {
        std::cout << "Checkerboard: " << std::endl;
        std::cout << "Size: " << this->size << std::endl;
        std::cout << "Material: " << this->mat->type() << std::endl;
    }
};