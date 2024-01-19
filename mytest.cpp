#include "surfaces.h"
#include <cassert>

using std::cout;

void TEST_plain_func()
{
    auto plain_function = plain();

    assert(plain_function(Point(0, 0)) == 0);
    assert(plain_function(Point(69, 0)) == 0);
    assert(plain_function(Point(-69, 0)) == 0);
}


void TEST_slope_func()
{
    auto slope_function = slope();

    assert(slope_function(Point(2, 22)) == 2);
    assert(slope_function(Point(0, 22)) == 0);
    assert(slope_function(Point(-7, 22)) == -7);
}

void TEST_steps_func()
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


int main()
{
    TEST_plain_func();
    TEST_slope_func();
    TEST_steps_func();
    

    return 0;
}