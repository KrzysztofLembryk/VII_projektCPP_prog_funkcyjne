#ifndef SURFACES_H
#define SURFACES_H

#include "real.h"
#include <iostream>
#include <functional>

class Point
{
public:
    Point() = delete;
    Point(Real _x, Real _y) : x(_x), y(_y) {}
    ~Point() = default;

    friend std::ostream &operator<<(std::ostream &out, const Point &p);

private:
    const Real x;
    const Real y;
};

std::ostream &operator<<(std::ostream &out, const Point &p)
{
    out << p.x << " " << p.y;
    return out;
}

#endif