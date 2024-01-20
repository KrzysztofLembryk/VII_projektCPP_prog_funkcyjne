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

// FUNCTIONS FOR CREATING PLAINS

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

Surface sqr()
{
    return [] (const Point &p) -> Real {
        return p.x * p.x;
    };
}

Surface sin_wave()
{
    return [] (const Point &p) -> Real {
        return std::sin(p.x);
    };
}

Surface cos_wave()
{
    return [] (const Point &p) -> Real {
        return std::cos(p.x);
    };
}

Surface rings(const Real s = 1)
{
    auto calc_dist_from_zero = [] (const Point &p) -> Real {
        return std::sqrt(p.x * p.x + p.y * p.y);
    };

    return [=] (const Point &p) -> Real {
        return (s <= 0) ? 0 : (std::fmod(calc_dist_from_zero(p), s) == 0 ? 1 : 0);
    };
}

Surface ellipse(const Real a = 1, const Real b = 1)
{   
    auto check_if_in_ellipse = [=] (const Point &p) -> bool {
        return ((p.x * p.x ) / a + (p.y * p.y) / b) <= 1;
    };

    return [=] (const Point &p) -> Real {
        return (a <= 0 || b <= 0) ? 0 : (check_if_in_ellipse(p) ? 1 : 0);
    };
}

Surface rectangle(const Real a = 1, const Real b = 1)
{
    auto check_if_inside = [=] (const Point &p) -> bool {
        return (p.x <= a / 2 && p.x >= -a / 2 && 
                p.y <= b / 2 && p.y >= -b / 2) ? 1 : 0;
    };

    return [=] (const Point &p) -> Real {
        return (a <= 0 || b <= 0) ? 0 : (check_if_inside(p) ? 1 : 0);
    };
}

Surface stripes(const Real s)
{
    return [=] (const Point &p) -> Real {

        const int x_parity = (s <= 0) ? 0 : (p.x < 0) ? 
            ((int)(std::floor((-p.x + s) / s)) % 2) : 
            ((int)(std::floor(p.x / s)) % 2);

        return (s <= 0) ? 0 : (x_parity == 0) ? 1 : 0; 
    };
}

// FUNCTIONS FOR PLAIN MANIPULATION

Surface rotate(const Surface &f, const Real deg)
{
    return [=] (const Point &p) -> Real {
        const Real new_x = p.x * std::cos(deg) - p.y * std::sin(deg);
        const Real new_y = p.y * std::cos(deg) + p.x * std::sin(deg);
        return f(Point(new_x, new_y));
    };
}

Surface translate(const Surface &f, const Point &vec)
{
    return [=] (const Point &p) -> Real {
        const Real new_x = p.x + vec.x;
        const Real new_y = p.y + vec.y;
        return f(Point(new_x, new_y));
    };
}

Surface scale(const Surface &f, const Point &scale)
{
    return [=] (const Point &p) -> Real {
        const Real new_x = p.x * scale.x;
        const Real new_y = p.y * scale.y;
        return f(Point(new_x, new_y));
    };
}

Surface invert(const Surface &f)
{
    return [=] (const Point &p) -> Real {
        return f(Point(p.y, p.x));
    };
}

Surface flip(const Surface &f)
{
    return [=] (const Point &p) -> Real {
        return f(Point(-p.x, p.y));
    };
}

Surface mul(const Surface &f, const Real c)
{
    return [=] (const Point &p) -> Real {
        return f(p) * c;
    };
}

Surface add(const Surface &f, const Real c)
{
    return [=] (const Point &p) -> Real {
        return f(p) + c;
    };
}


#endif