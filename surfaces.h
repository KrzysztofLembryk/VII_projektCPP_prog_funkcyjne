#ifndef SURFACES_H
#define SURFACES_H

#include "real.h"
#include <iostream>
#include <functional>
#include <cmath>

constexpr Real STEP_HEIGHT = 1;

class Point
{
public:
    Point() = delete;
    Point(Real _x, Real _y) : x(_x), y(_y) {}
    ~Point() = default;

    friend std::ostream &operator<<(std::ostream &out, const Point &p);

    const Real x;
    const Real y;
};

std::ostream &operator<<(std::ostream &out, const Point &p)
{
    out << p.x << " " << p.y;
    return out;
}

/**
 *  Maybe function should get Point via reference???
*/
using Surface = std::function<Real(Point)>;

/**
 * Generates plain f(x, y) = 0;
*/
Surface plain()
{
    // We want to stress that returned value is of type REAL.
    return []([[maybe_unused]] const Point &p) -> Real {return 0;};
}

/**
 * Generates slope f(x, y) = x;
*/
Surface slope()
{
    return [](const Point &p) -> Real {return p.x;};
}

Surface steps(const Real s = 1)
{
    const Real step_height = STEP_HEIGHT;
    return [s, step_height] (const Point &p) -> Real {
        
        return (s <= 0) ? Real(0) : 
        ((p.x >= 0) ? std::floor(p.x / s)* step_height : 
        (std::floor(((-1) * p.x) / s) * (-1) * step_height - 1));
    };
}


#endif