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

/**
 * Generates step plain along OX, with step width = s (default = 1)
 * and step height = 1. 
 * If s <= 0 : f(x, y) = 0
 * For 0 <= x < s we have f(x, y) = 0,
 * for -s <= x < 0 : f(x, y) = -1
 * Main formula for getting step height for given x: floor(x / s) * step_height
 * Since new step starts when x >= s, so we have 
 * first step: s <= x < 2s
 * second step: 2s <= x < 3s ... So we se that when taking floor(x / s)
 * we will get k that satisfies k * s <= x, and k is the number of step. 
*/
Surface steps(const Real s = 1)
{
    const Real step_height = STEP_HEIGHT;

    return [s, step_height] (const Point &p) -> Real {
        
        return (s <= 0) ? Real(0) : 
        ((p.x >= 0) ? Real(std::floor(p.x / s)* step_height) : 
        Real(std::floor(((-1) * p.x) / s) * (-1) * step_height - 1));
    };
}

/**
 * 
*/
Surface checkers(const Real s = 1)
{
    // Edge case with x and y negative
    return [s] (const Point &p) -> Real {

        // If x < 0 we add s to is since we want to change its parity
        // since if we didnt we could get -s < x1 <= 0 and 0 <= x2 < s
        // with values of f both equal to i.e. 1.
        const int x_parity = (s <= 0) ? 0 : (p.x < 0) ? 
            ((int)(std::floor((-p.x + s) / s)) % 2) : 
            ((int)(std::floor(p.x / s)) % 2);
            
        const int y_parity = (s <= 0) ? 0 : (p.y < 0) ? 
            ((int)(std::floor((-p.y) / s)) % 2) : 
            ((int)(std::floor(p.y / s)) % 2);

        return (s <= 0) ? Real(0) : 
                                ((x_parity == y_parity) ? Real(1) : Real(0));
    };
    

}

#endif