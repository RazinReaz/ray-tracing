#pragma once
#include "vector3f.h"

typedef struct color {
    double r, g, b;
    color(double r = 0, double g = 0, double b = 0) {
        this->r = r;
        this->g = g;
        this->b = b;
    }
    color(vector3f v) {
        v = v.normalize();
        this->r = v.x;
        this->g = v.y;
        this->b = v.z;
    }
    double clamp(double x) const;
    color operator+(const color &c) const;
    color operator*(double d) const;
    color operator*(const color &c) const;
    color operator+=(const color &c);
    color operator*=(const color &c);

    friend std::ostream &operator<<(std::ostream &os, const color &c)
    {
        os << "color(" << c.r << ", " << c.g << ", " << c.b << ")";
        return os;
    }
} color;

// enum to define colors
enum colors {
    RED, GREEN, BLUE, YELLOW, PURPLE, CYAN
};

// array of colors
color color_array[] = {
    color(1, 0, 0), // red
    color(0, 1, 0), // green
    color(0, 0, 1), // blue
    color(1, 1, 0), // yellow
    color(1, 0, 1),  // purple
    color(0, 1, 1)  // cyan
};

double color::clamp(double x) const {
    return x > 1 ? 1 : x;
}

color color::operator+(const color &c) const {
    double r = clamp(this->r + c.r);
    double g = clamp(this->g + c.g);
    double b = clamp(this->b + c.b);
    return color(r, g, b);
}

color color::operator+=(const color &c) {
    this->r = clamp(this->r + c.r);
    this->g = clamp(this->g + c.g);
    this->b = clamp(this->b + c.b);
    return *this;
}

color color::operator*(double d) const {
    double r = clamp(this->r * d), g = clamp(this->g * d), b = clamp(this->b * d);
    return color(r, g, b);
}

color color::operator*(const color &c) const {

    double  r = this->r * c.r,
            g = this->g * c.g,
            b = this->b * c.b;
    return color(r, g, b);
}

color color::operator*=(const color &c) {
    this->r = clamp(this->r * c.r);
    this->g = clamp(this->g * c.g);
    this->b = clamp(this->b * c.b);
    return *this;
}