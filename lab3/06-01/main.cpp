#include "rational.hpp"

#include <iostream>
#include <cassert>

namespace {

void run_basic_tests() {
    Rational x = 1;
    Rational y(2, 1);
    
    assert(Rational(1, 2) == Rational(1, 2));
    assert(equal(static_cast<double>(Rational(1, 2)), 0.5));
    assert(equal(static_cast<double>(Rational(3, 4)), 0.75));
    
    // Test +=
    {
        Rational a(1, 2);
        a += Rational(1, 3);
        assert(a == Rational(5, 6));
    }
    
    // Test -=
    {
        Rational a(1, 2);
        a -= Rational(1, 3);
        assert(a == Rational(1, 6));
    }
    
    // Test *=
    {
        Rational a(1, 2);
        a *= Rational(2, 3);
        assert(a == Rational(1, 3));
    }
    
    // Test /=
    {
        Rational a(1, 2);
        a /= Rational(2, 3);
        assert(a == Rational(3, 4));
    }
    
    // Test increment/decrement
    {
        Rational a(1, 2);
        assert(a++ == Rational(1, 2));
        assert(a == Rational(3, 2));
        assert(++a == Rational(5, 2));
        assert(a == Rational(5, 2));
    }
    
    // Test comparison
    {
        Rational a(1, 2);
        Rational b(1, 3);
        assert(a > b);
        assert(b < a);
        assert(a != b);
        assert(a == Rational(1, 2));
    }
    
    std::cout << "Basic tests passed!\n";
}

void demo_features() {
    Rational a(1, 3);
    Rational b(2, 5);
    
    std::cout << "a = " << a << "\n";
    std::cout << "b = " << b << "\n";
    
    std::cout << "a + b = " << a + b << "\n";
    std::cout << "a - b = " << a - b << "\n";
    std::cout << "a * b = " << a * b << "\n";
    std::cout << "a / b = " << a / b << "\n";
    
    std::cout << "a + 1 = " << a + 1 << "\n";
    std::cout << "2 * b = " << 2 * b << "\n";
    
    Rational c(1, 2);
    std::cout << "c = " << c << "\n";
    std::cout << "c++ = " << c++ << " (now c = " << c << ")\n";
    std::cout << "++c = " << ++c << " (now c = " << c << ")\n";
    
    std::cout << "a as double: " << static_cast<double>(a) << "\n";
    std::cout << "b as double: " << static_cast<double>(b) << "\n";
    
    std::cout << "a < b: " << (a < b ? "true" : "false") << "\n";
    std::cout << "a == b: " << (a == b ? "true" : "false") << "\n";
}

}

int main() {
    run_basic_tests();
    demo_features();

    return 0;
}
