#include "surfaces.h"
#include <cassert>

using std::cout;

void check_zero(auto f) {
    for(int i = -10; i < 10; i++) {
        for(int j = -10; j < 10; j++) {
            assert(f(Point(i / 0.1237, j / 0.1234)) == 0.0);
            assert(f(Point(i, j)) == 0.0);
        }
    }
}

void tests() {
    const int big = int(1e6);

    // Plain
    assert(plain()(Point(0.0, 0.0)) == 0.0);
    assert(plain()(Point(1234567.8, 29872134.43)) == 0.0);

    // Slope
    assert(slope()(Point(0.0, -1.0)) == 0.0);
    assert(slope()(Point(-1.0, -1.0)) == -1.0);
    assert(slope()(Point(-12345, 12345)) == -12345);
    
    // Steps
    assert(steps()(Point(1.9999, 1)) == 1.0);
    assert(steps()(Point(0.9999, -7)) == 0.0);
    assert(steps()(Point(1.0, 213)) == 1.0);
    assert(steps()(Point(-0.0001, 222)) == -1.0);
    assert(steps()(Point(-1.0001, -22222)) == -2.0);
    
    assert(steps(2.0)(Point(1.9999, 1)) == 0.0);
    assert(steps(0.9)(Point(0.9999, -7)) == 1.0);
    assert(steps(0.9)(Point(-0.9, 117)) == -1.0);

    check_zero(steps(0.0));
    check_zero(steps(-0.001));
    check_zero(steps(-1e18));
    
    // Checker
    assert(checker()(Point(0.0, 0.0)) == 1.0);
    assert(checker()(Point(1.0, 0.0)) == 0.0);
    assert(checker()(Point(0.0, 1.0)) == 0.0);
    assert(checker()(Point(1.0, 1.0)) == 1.0);

    assert(checker(0.5)(Point(1.0, 1.0)) == 1.0);

    assert(checker(0.5)(Point(0.0, 0.5)) == 0.0);
    assert(checker(0.5)(Point(0.0, 0.5001)) == 0.0);
    assert(checker(0.5)(Point(0.0, 0.4999)) == 1.0);

    assert(checker(0.5)(Point(0.5, 0.0)) == 0.0);
    assert(checker(0.5)(Point(0.5001, 0.0)) == 0.0);
    assert(checker(0.5)(Point(0.4999, 0.0)) == 1.0);

    assert(checker(0.1)(Point(-0.1, -0.001)) == 1.0);
    assert(checker(0.1)(Point(-0.1, 0.001)) == 0.0);

    check_zero(checker(0.0));
    check_zero(checker(-0.001));
    check_zero(checker(-1e18));

    // Rings
    assert(rings()(Point(1.0, 0.0)) == 1.0);
    assert(rings()(Point(0.0, 0.0)) == 1.0);
    assert(rings()(Point(0.0, 1.00001)) == 0.0);
    assert(rings()(Point(0.0, 1.90001)) == 0.0);
    assert(rings()(Point(0.0, 2.00000)) == 0.0);
    assert(rings()(Point(1.0, 1.0)) == 0.0);
    
    assert(rings(3.0)(Point(1.0, 0.0)) == 1.0);
    assert(rings(3.1)(Point(3.1, 0.0)) == 1.0);
    assert(rings(5.0)(Point(3.0, 4.0)) == 1.0);
    assert(rings(5.0)(Point(3.0000001, 4.0)) == 0.0);
    
    check_zero(rings(0.0));
    check_zero(rings(-0.001));
    check_zero(rings(-1e18));

    // Ellipse
    assert(ellipse()(Point(1.0, 0.0)) == 1.0);
    assert(ellipse()(Point(1.0, 0.00001)) == 0.0);
    
    assert(ellipse(7.0)(Point(7.0, 0.0)) == 1.0);
    assert(ellipse(5.0)(Point(3.0, 4.0 / 5.0)) == 1.0);
    assert(ellipse(5.0)(Point(-3.0, 4.0 / 5.0)) == 1.0);
    assert(ellipse(5.0)(Point(3.0, 4.0 / 5.0 + 0.001)) == 0.0);
    assert(ellipse(2.5)(Point(1.5, 0.4)) == 1.0);
    
    assert(ellipse(5.0, 5.0)(Point(3.0, 4.0)) == 1.0);
    assert(ellipse(5.0, 5.0)(Point(3.0, 4.0 + 0.0001)) == 0.0);

    check_zero(ellipse(0.0));
    check_zero(ellipse(0.0, 1.0));
    check_zero(ellipse(0.0, 0.0));
    check_zero(ellipse(0.0, -1.0));
    check_zero(ellipse(0.1, -1.0));
    check_zero(ellipse(0.1, 0.0));
    check_zero(ellipse(-0.1, 0.0));
    check_zero(ellipse(-0.1, 1.0));
    

    // Rectangle
    assert(rectangle()(Point(0.0, 0.0)) == 1.0);
    assert(rectangle()(Point(1.0, 1.0)) == 1.0);
    assert(rectangle()(Point(-1.0, 1.0)) == 1.0);
    assert(rectangle()(Point(1.0, -1.0)) == 1.0);
    assert(rectangle()(Point(1.0, -1.0001)) == 0.0);
    assert(rectangle()(Point(1.0001, -1.0)) == 0.0);
    assert(rectangle()(Point(1.0001, 1.0)) == 0.0);
    assert(rectangle()(Point(1.0, 1.00001)) == 0.0);
    assert(rectangle()(Point(-1.0, -1.0001)) == 0.0);
    assert(rectangle()(Point(-1.0001, -1.0)) == 0.0);
    
    assert(rectangle(2.0)(Point(2.0, 1.0)) == 1.0);
    assert(rectangle(2.0)(Point(2.0001, 1.0)) == 0.0);
    
    assert(rectangle(2.0, 3.0)(Point(2.0, 3.0)) == 1.0);
    assert(rectangle(2.0, 3.0)(Point(2.0, -3.0)) == 1.0);
    assert(rectangle(2.0, 3.0)(Point(-2.0, 3.0)) == 1.0);
    assert(rectangle(2.0, 3.0)(Point(-2.0, -3.0)) == 1.0);
    

    check_zero(rectangle(0.0));
    check_zero(rectangle(0.0, 1.0));
    check_zero(rectangle(0.0, 0.0));
    check_zero(rectangle(0.0, -1.0));
    check_zero(rectangle(0.1, -1.0));
    check_zero(rectangle(0.1, 0.0));
    check_zero(rectangle(-0.1, 0.0));
    check_zero(rectangle(-0.1, 1.0));

    // Stripes
    
    // if(true) {
    //     // Zakomentuj caly ten blok
    //     std::cout << "To sie nie powinno kompilowac, masz zle!" << std::endl;
    //     std::cout << stripes()(Point(0.0, 0.0)) << std::endl;
    //     assert(false);
    // } 

    assert(stripes(1.0)(Point(0.0, 0.0)) == 0.0);
    assert(stripes(1.0)(Point(0.0, 123213.0)) == 0.0);
    assert(stripes(1.0)(Point(0.0, -123213.0)) == 0.0);

    assert(stripes(1.0)(Point(0.0001, 0.0)) == 1.0);
    assert(stripes(1.0)(Point(0.0001, 123213.0)) == 1.0);
    assert(stripes(1.0)(Point(0.0001, -123213.0)) == 1.0);

    assert(stripes(1.0)(Point(1.0, 0.0)) == 1.0);
    assert(stripes(1.0)(Point(1.0, 123213.0)) == 1.0);
    assert(stripes(1.0)(Point(1.0, -123213.0)) == 1.0);

    assert(stripes(1.0)(Point(1.00001, 0.0)) == 0.0);
    assert(stripes(1.0)(Point(1.000001, 123213.0)) == 0.0);
    assert(stripes(1.0)(Point(1.00001, -123213.0)) == 0.0);

    check_zero(stripes(0.0));
    check_zero(stripes(-1.0));
}   


int main()
{
    tests();
    //steps(1.0)(Point(-1, 117));

    //cout << "floor: " << std::floor(-0.5 / 1) << "\n"; 

    assert(evaluate([](){return 1.5;})(Point(-1.0, 2.0)) == 1.5);
    assert(evaluate([](){return true;})(Point(-1.0, 2.0)) == true);
    assert(evaluate([](){return std::make_pair(false, -0.125);})(Point(-1.0, 2.0)) == std::make_pair(false, -0.125));

    return 0;
}