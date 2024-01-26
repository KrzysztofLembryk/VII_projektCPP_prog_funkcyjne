#ifndef SURFACES_H
#define SURFACES_H

#include "real.h"
#include <iostream>
#include <functional>
#include <cmath>

constexpr Real STEP_HEIGHT = 1;
constexpr Real SEMI_FULL_ANGLE = 180;
constexpr Real PLAIN_CONSTANT = 0;
constexpr Real INIT_ARG_VAL = 1;

constexpr Real RET_VAL_TRUE = 1;
constexpr Real RET_VAL_FALSE = 0;
constexpr Real RET_VAL_WRONG = 0;

constexpr Real S_BOUND = 0;
constexpr Real RECT_A_BOUND = 0;
constexpr Real RECT_B_BOUND = 0;
constexpr Real ELLIPSE_A_BOUND = 0;
constexpr Real ELLIPSE_B_BOUND = 0;

constexpr int EVEN = 0;
constexpr int ODD = 1;

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

inline std::ostream &operator<<(std::ostream &out, const Point &p)
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
 * Generates plain f(x, y) = PLAIN_CONSTANT;
*/
inline Surface plain()
{
    // We want to stress that returned value is of type REAL.
    return []([[maybe_unused]] const Point &p) -> Real {return PLAIN_CONSTANT;};
}

/**
 * Generates slope f(x, y) = x;
*/
inline Surface slope()
{
    return [](const Point &p) -> Real {return p.x;};
}

/**
 * Generates step plain along OX, with step width = s (default = 1)
 * and step height = 1. 
 * If s <= 0 : f(x, y) = 0
 * For 0 <= x < s we have f(x, y) = 1,
 * for -s <= x < 0 : f(x, y) = -1
 * Main formula for getting step height for given x: floor(x / s) * step_height
 * Since new step starts when x >= s, so we have 
 * first step: s <= x < 2s
 * second step: 2s <= x < 3s ... So we se that when taking floor(x / s)
 * we will get k that satisfies k * s <= x, and k is the number of step. 
*/
inline Surface steps(const Real s = INIT_ARG_VAL)
{
    const Real step_height = STEP_HEIGHT;

    return [s, step_height] (const Point &p) -> Real {

        return (s <= S_BOUND) ? RET_VAL_WRONG : 
        ((p.x >= 0) ? Real(std::floor(p.x / s)* step_height) : 
        Real(std::floor(p.x / s) * step_height));
    };
}

/**
 * We want to create checkers board with tile of width = s.
 * When 0 <= x < s we have f(x, y) = 1, if 0 <= y < s
 * and f(x, y) = 0, if s <= y < 2 * s . Basically we calculate x'= floor(x/s) 
 * and y' = floor(y/s) and check whether (x' + y') % 2 == 0 since only when
 * we have the same parity of x and y f is equal to 1.
*/
inline Surface checker(const Real s = INIT_ARG_VAL)
{
    // Edge case with x and y negative
    return [=] (const Point &p) -> Real {

        const int x_parity = s <= S_BOUND ? RET_VAL_WRONG : std::floor(p.x / s);
        const int y_parity = s <= S_BOUND ? RET_VAL_WRONG : std::floor(p.y / s);

        return (s <= S_BOUND) ? RET_VAL_WRONG : 
            ((x_parity + y_parity) % 2 == EVEN ? RET_VAL_TRUE : RET_VAL_FALSE);
    }; 
}

inline Surface sqr()
{
    return [] (const Point &p) -> Real {
        return Real(p.x * p.x);
    };
}

inline Surface sin_wave()
{
    return [] (const Point &p) -> Real {
        return Real(std::sin(p.x));
    };
}

inline Surface cos_wave()
{
    return [] (const Point &p) -> Real {
        return Real(std::cos(p.x));
    };
}

/**
 * We calculate the distance of point p from (0, 0) and if floor(dist / s)
 * is even our function returns 1 (so our dist is dist = 2k * s + r), meaning
 * that we create a ring on which our function is 1, then another ring where is 
 * 0 etc. But we have edge cases i.e. when dist is 1, f should return 1, but 
 * with above approach we would return 0, since 1 is not even, so we need to add
 * another case in if statement 1) checking if floor(dist / s) is odd but rest 
 * of division of dist / s is equal 0, then we return 1.
*/
inline Surface rings(const Real s = INIT_ARG_VAL)
{
    auto calc_dist_from_zero = [] (const Point &p) -> Real {
        return std::sqrt(p.x * p.x + p.y * p.y);
    };

    return [=] (const Point &p) -> Real {

        const Real dist = calc_dist_from_zero(p);
        const int dists_in_s = static_cast<int>(std::floor(dist / s));
        
        return (s <= S_BOUND) ? RET_VAL_WRONG : (dist <= s) ? RET_VAL_TRUE : 
            (dists_in_s % 2 == EVEN && fmod(dist, s) != 0) || 
            (dists_in_s % 2 == ODD && fmod(dist, s) == 0)  ? RET_VAL_TRUE : 
                RET_VAL_FALSE;
    };
}

inline Surface ellipse(const Real a = INIT_ARG_VAL, const Real b = INIT_ARG_VAL)
{   
    auto check_if_in_ellipse = [=] (const Point &p) -> bool {
        return ((p.x * p.x ) / (a * a) + (p.y * p.y) / (b * b)) <= 1;
    };

    return [=] (const Point &p) -> Real {
        return (a <= ELLIPSE_A_BOUND || b <= ELLIPSE_B_BOUND) ? RET_VAL_WRONG : (check_if_in_ellipse(p) ? RET_VAL_TRUE : RET_VAL_FALSE);
    };
}

/**
 * Rectangle has sides equal to 2a and 2b. The center of rectangle is in (0, 0).
*/
inline Surface rectangle(const Real a = INIT_ARG_VAL, 
    const Real b = INIT_ARG_VAL)
{
    auto check_if_inside = [=] (const Point &p) -> bool {
        return (p.x <= a && p.x >= -a && 
                p.y <= b && p.y >= -b) ? true : false;
    };

    return [=] (const Point &p) -> Real {
        return (a <= RECT_A_BOUND || b <= RECT_B_BOUND) ? RET_VAL_WRONG : (check_if_inside(p) ? RET_VAL_TRUE : RET_VAL_FALSE);
    };
}

inline Real calc_first_greater_s_positive(const Real curr_s, const Real s, 
    const Real x)
{
    return curr_s < x ? calc_first_greater_s_positive(curr_s + s, s, x) : curr_s;
}

inline Real calc_first_greater_s_negative(const Real curr_s, const Real s, 
    const Real x)
{
    return curr_s > x ? calc_first_greater_s_negative(curr_s - s, s, x) : curr_s; 
}

/**
 * If x >= 0 we calculate first S = k*s that S >= x, we need this since 
 * inequalities that we need to satisfy are 
 * f(x, y) = 1, if 0 < x <= s and f(x, y) = 0, if -s < x <= 0 etc.
 * Thus taking floor(x / s) is not good enough since i.e. 0 / s = 0 and we would
 * assign f = 1 if we did like in previous functions. We need S to check these
 * edge cases.
*/
inline Surface stripes(const Real s)
{
    return [=] (const Point &p) -> Real {

        const Real first_grtr_or_eq_s = s <= S_BOUND ? RET_VAL_WRONG : 
        (p.x < 0 ? calc_first_greater_s_negative(s, s, p.x) :       
                calc_first_greater_s_positive(0, s, p.x));

        const int x_parity = (s <= S_BOUND) ? RET_VAL_WRONG : (p.x < 0) ? 
           (std::floor((-p.x + s) / s)) : (std::floor(p.x / s));

        const int positive_x = p.x > 0 ? (p.x == first_grtr_or_eq_s ? 
        (x_parity + 1) % 2 : x_parity % 2) : -1;
        
        return (s <= S_BOUND) ? RET_VAL_WRONG : (p.x >= 0 ? 
            Real((positive_x + 1) % 2) : Real((x_parity + 1) % 2)); 
    };
}

// FUNCTIONS FOR PLAIN MANIPULATION

inline Surface rotate(const Surface &f, const Real deg)
{
    return [=] (const Point &p) -> Real {
        const Real radians = (-1) * deg * (M_PI / SEMI_FULL_ANGLE); // = 180
        const Real new_x = p.x * std::cos(radians) - p.y * std::sin(radians);
        const Real new_y = p.x * std::sin(radians) + p.y * std::cos(radians);
        return f(Point(new_x, new_y));
    };
}

inline Surface translate(const Surface &f, const Point &vec)
{
    return [=] (const Point &p) -> Real {
        const Real new_x = p.x - vec.x;
        const Real new_y = p.y - vec.y;
        return f(Point(new_x, new_y));
    };
}

inline Surface scale(const Surface &f, const Point &scale)
{
    return [=] (const Point &p) -> Real {
        const Real new_x = p.x * (1 / scale.x);
        const Real new_y = p.y * (1 / scale.y);
        return f(Point(new_x, new_y));
    };
}

inline Surface invert(const Surface &f)
{
    return [=] (const Point &p) -> Real {
        return f(Point(p.y, p.x));
    };
}

inline Surface flip(const Surface &f)
{
    return [=] (const Point &p) -> Real {
        return f(Point(-p.x, p.y));
    };
}

inline Surface mul(const Surface &f, const Real c)
{
    return [=] (const Point &p) -> Real {
        return (f(p) * c);
    };
}

inline Surface add(const Surface &f, const Real c)
{
    return [=] (const Point &p) -> Real {
        return f(p) + c;
    };
}

// TEMPLATE FUNCTIONS

/**
 * Args - are functions that we want to invoke with p argument
 * H h - is a function that we want to invoke with all of results of Args 
 * functions invoked with p
 * so we want to calculate h(f1(p), f2(p), ..., fn(p)).
 * Thus in unpack_and_calc_h we take fi calc fi(p), then bind result with h.
*/
template <typename H, typename T, typename... Args>
inline constexpr auto unpack_and_calc_h(const auto &p, H &&h, T &&f, 
    Args &&...args)
{
    if constexpr (sizeof...(args) == 0)
    {
        const auto val = std::invoke(f, p);
        const auto binded_h_func = std::bind_front(h, val);
        return std::invoke(binded_h_func);
    }
    else
    {
        const auto val = std::invoke(f, p);
        const auto binded_h_func = std::bind_front(h, val);

        return unpack_and_calc_h(p, binded_h_func,
             std::forward<Args>(args)...);
    }
}

template<typename T, typename... Args>
inline auto evaluate(T &&h, Args &&...f_args)
{
    // We cannot do i.e. std::forward<T>(h) since we do [=] in lambda expr
    // thus we make a new object - a copy of h, so its no longer &&. 
    return [=] (const auto &p) {
        return unpack_and_calc_h(p, h, f_args...);
    };

}


template <typename F, typename... Args>
inline constexpr auto calc_composition(const auto &val, F &&f, Args &&...args)
{
    if constexpr (sizeof...(args) == 0)
    {
        return std::invoke(f, val);
    }
    else
    {
        const auto new_val = std::invoke(f, val);
        return calc_composition(new_val, std::forward<Args>(args)...);
    }
}


template<typename... Args>
inline auto compose(Args &&...args)
{
    return [=] (const auto &val) {
        return calc_composition(val, args...);
    };
}

inline auto compose()
{
    return [] (const auto &val) {
        return val;
    };
}

#endif