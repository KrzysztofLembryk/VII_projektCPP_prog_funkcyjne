#include "surfaces.h"
#include <cassert>

using std::cout;

void TEST_plain()
{
    auto plain_function = plain();

    assert(plain_function(Point(0, 0)) == 0);
    assert(plain_function(Point(69, 0)) == 0);
    assert(plain_function(Point(-69, 0)) == 0);
}


void TEST_slope()
{
    auto slope_function = slope();

    assert(slope_function(Point(2, 22)) == 2);
    assert(slope_function(Point(0, 22)) == 0);
    assert(slope_function(Point(-7, 22)) == -7);
}

void TEST_steps()
{
    auto stepping_function = steps();
    
    // x >= 0
    assert(stepping_function(Point(0.5, 69)) == 0);
    assert(stepping_function(Point(1, 12)) == 1);
    assert(stepping_function(Point(1.9, 12)) == 1);
    assert(stepping_function(Point(2, 12)) == 2);
    assert(stepping_function(Point(10, 12)) == 10);

    // x < 0
    assert(stepping_function(Point(-0.5, 12)) == -1);
    assert(stepping_function(Point(-1, 112)) == -2);
    assert(stepping_function(Point(-5, 12)) == -6);

}


void TEST_rings()
{   
    // Width of ring is 1;
    auto rings_func = rings(1);

    assert(rings_func(Point(0, 1)) == 1);
    assert(rings_func(Point(0, -1)) == 1);
    assert(rings_func(Point(1, 0)) == 1);
    assert(rings_func(Point(-1, 0)) == 1);
    assert(rings_func(Point(2, 0)) == 1);
    assert(rings_func(Point(std::sqrt(2) / 2, std::sqrt(2) / 2)) == 1);

    assert(rings_func(Point(1, 1)) == 0);
    assert(rings_func(Point(1, 2)) == 0);
    assert(rings_func(Point(0.5, 1.5)) == 0);
}


int main()
{
    TEST_plain();
    TEST_slope();
    TEST_steps();
    TEST_rings();
    return 0;
}