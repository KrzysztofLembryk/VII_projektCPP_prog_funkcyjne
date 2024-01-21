#include "surfaces.h"
#include <cassert>

using std::cout;

// BASIC CREATING PLAINS TESTS
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

    // s = 0.75
    auto stepping_func_0_75 = steps(0.75);

    assert(stepping_func_0_75(Point(0.5, 65)) == 0);
    assert(stepping_func_0_75(Point(0.75, 65)) == 1);
    assert(stepping_func_0_75(Point(1, 65)) == 1);
    assert(stepping_func_0_75(Point(1.5, 65)) == 2);
    assert(stepping_func_0_75(Point(1.55, 65)) == 2);

}

void TEST_checker()
{
    auto checker_func = checker();

    assert(checker_func(Point(0.5, 0.5)) == 1);
    assert(checker_func(Point(1, 0.5)) == 0);
    assert(checker_func(Point(1, 1)) == 1);
    assert(checker_func(Point(1.5, 1.24)) == 1);
    assert(checker_func(Point(2, 1.5)) == 0);
    
    assert(checker_func(Point(-0.5, 0.5)) == 0);
    assert(checker_func(Point(-0.5, 1)) == 1);


    auto mulled_checker = mul(checker(), 0.5);

    assert(mulled_checker(Point(0.5, 0.5)) == 0.5);
    assert(mulled_checker(Point(1, 0.5)) == 0);
    assert(mulled_checker(Point(1, 1)) == 0.5);
    assert(mulled_checker(Point(1.5, 1.24)) == 0.5);
    assert(mulled_checker(Point(2, 1.5)) == 0);
    
    assert(mulled_checker(Point(-0.5, 0.5)) == 0);
    assert(mulled_checker(Point(-0.5, 1)) == 0.5);
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

void TEST_stripes()
{
    auto stripes_func = stripes(1);

    assert(stripes_func(Point(0, 5)) == 0);
    assert(stripes_func(Point(0.5, 5)) == 1);
    assert(stripes_func(Point(1, 5)) == 1);
    assert(stripes_func(Point(1.2, 5)) == 0);
    assert(stripes_func(Point(2, 5)) == 0);
    assert(stripes_func(Point(2.5, 5)) == 1);
}
// MODIFYING PLAINS TESTS

void TEST_rotate()
{
    auto rotate_func = rotate(slope(), 90);

    assert(rotate_func(Point(0, 0)) == 0);
    //assert(rotate_func(Point(1, 0)) == 0.0);
}

void TEST_flip()
{
    auto flip_func = flip(slope());

    assert(flip_func(Point(1, 1)) == -1);
    assert(flip_func(Point(1.99, 1)) == -1.99);
}

// TEMPLATE FUNC TESTS

void TEST_evaluate()
{
    auto addition = [](Real const & x, Real const & y) {return x + y;};
    auto eval_func1 = evaluate(addition, sqr(),slope());

    assert(eval_func1(Point(1, 5)) == 2);
    assert(eval_func1(Point(2, 5)) == 6);
    assert(eval_func1(Point(1.2, 5)) == 1.44 + 1.2);
}

void TEST_compose()
{
    assert(compose()(4.2) == 4.2);
    assert(compose([](auto x) {return x - 0.5;}, [](auto x) {return x * x;})(3.5) == 9);
}

int main()
{
    // Making plains test:
    TEST_plain();
    TEST_slope();
    TEST_steps();
    TEST_checker();
    TEST_rings();
    TEST_stripes();
    // Modifying plains tests:
    TEST_rotate();
    TEST_flip();

    // Template tests:
    TEST_evaluate();
    TEST_compose();
    return 0;
}