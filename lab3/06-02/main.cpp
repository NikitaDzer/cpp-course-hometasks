#include <cassert>

import std;
import rational;

namespace {

void run_basic_tests() {
    math::Rational x = 1;
    math::Rational y(2, 1);
    
    assert(math::Rational(1, 2) == math::Rational(1, 2));
    assert(math::equal(static_cast<double>(math::Rational(1, 2)), 0.5));
    assert(math::equal(static_cast<double>(math::Rational(3, 4)), 0.75));
    
    // Test +=
    {
        math::Rational a(1, 2);
        a += math::Rational(1, 3);
        assert(a == math::Rational(5, 6));
    }
    
    // Test -=
    {
        math::Rational a(1, 2);
        a -= math::Rational(1, 3);
        assert(a == math::Rational(1, 6));
    }
    
    // Test *=
    {
        math::Rational a(1, 2);
        a *= math::Rational(2, 3);
        assert(a == math::Rational(1, 3));
    }
    
    // Test /=
    {
        math::Rational a(1, 2);
        a /= math::Rational(2, 3);
        assert(a == math::Rational(3, 4));
    }
    
    // Test increment/decrement
    {
        math::Rational a(1, 2);
        assert(a++ == math::Rational(1, 2));
        assert(a == math::Rational(3, 2));
        assert(++a == math::Rational(5, 2));
        assert(a == math::Rational(5, 2));
    }
    
    // Test comparison
    {
        math::Rational a(1, 2);
        math::Rational b(1, 3);
        assert(a > b);
        assert(b < a);
        assert(a != b);
        assert(a == math::Rational(1, 2));
    }
    
    std::cout << "Basic tests passed!\n";
}

void demo_features() {
    math::Rational a(1, 3);
    math::Rational b(2, 5);
    
    std::cout << "a = " << a << "\n";
    std::cout << "b = " << b << "\n";
    
    std::cout << "a + b = " << a + b << "\n";
    std::cout << "a - b = " << a - b << "\n";
    std::cout << "a * b = " << a * b << "\n";
    std::cout << "a / b = " << a / b << "\n";
    
    std::cout << "a + 1 = " << a + 1 << "\n";
    std::cout << "2 * b = " << 2 * b << "\n";
    
    math::Rational c(1, 2);
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
