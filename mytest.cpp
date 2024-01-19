#include "surfaces.h"
#include <cassert>

using std::cout;


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
    assert(stepping_function(Point(-1, 12)) == -2);
    assert(stepping_function(Point(-5, 12)) == -6);

}


int main()
{
    TEST_steps_func();
    

    return 0;
}