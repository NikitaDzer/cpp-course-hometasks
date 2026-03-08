#include <gtest/gtest.h>
#include <cassert>
#include <cmath>
#include <compare>
#include <istream>
#include <numeric>
#include <ostream>
#include <sstream>
#include <type_traits>
#include <exception>
#include <variant>
#include <optional>
#include <vector>
#include <string>
#include <new>

class Exception : public std::exception {
public:
    explicit Exception(const char* message) : msg(message) {}
    const char* what() const noexcept override { return msg; }
private:
    const char* msg;
};

template <typename Derived>
class addable {
public:
    friend Derived operator+(Derived const& lhs, Derived const& rhs) {
        Derived result(lhs);
        result += rhs;
        return result;
    }
};

template <typename Derived>
class subtractable {
public:
    friend Derived operator-(Derived const& lhs, Derived const& rhs) {
        Derived result(lhs);
        result -= rhs;
        return result;
    }
};

template <typename Derived>
class multipliable {
public:
    friend Derived operator*(Derived const& lhs, Derived const& rhs) {
        Derived result(lhs);
        result *= rhs;
        return result;
    }
};

template <typename Derived>
class dividable {
public:
    friend Derived operator/(Derived const& lhs, Derived const& rhs) {
        Derived result(lhs);
        result /= rhs;
        return result;
    }
};

template <typename Derived>
class incrementable {
public:
    friend Derived& operator++(Derived& self) {
        self += Derived(1, 1);
        return self;
    }
    friend Derived operator++(Derived& self, int) {
        Derived tmp(self);
        ++self;
        return tmp;
    }
};

template <typename Derived>
class decrementable {
public:
    friend Derived& operator--(Derived& self) {
        self -= Derived(1, 1);
        return self;
    }
    friend Derived operator--(Derived& self, int) {
        Derived tmp(self);
        --self;
        return tmp;
    }
};

template <typename T>
class Rational : public addable<Rational<T>>,
                 public subtractable<Rational<T>>,
                 public multipliable<Rational<T>>,
                 public dividable<Rational<T>>,
                 public incrementable<Rational<T>>,
                 public decrementable<Rational<T>> {
    static_assert(std::is_integral_v<T>, "Rational requires integral type");
    
public:
    Rational(T num = 0, T den = 1) : m_num(num), m_den(den) {
        if (den == 0) {
            throw Exception("denominator cannot be zero");
        }
        reduce();
    }

    explicit operator double() const { return static_cast<double>(m_num) / static_cast<double>(m_den); }

    Rational& operator+=(Rational const& other) {
        auto lcm = std::lcm(m_den, other.m_den);
        m_num = m_num * (lcm / m_den) + other.m_num * (lcm / other.m_den);
        m_den = lcm;
        reduce();
        return *this;
    }

    Rational& operator-=(Rational const& other) {
        return *this += Rational(-other.m_num, other.m_den);
    }

    Rational& operator*=(Rational const& other) {
        m_num *= other.m_num;
        m_den *= other.m_den;
        reduce();
        return *this;
    }

    Rational& operator/=(Rational const& other) {
        return *this *= Rational(other.m_den, other.m_num);
    }

    friend std::strong_ordering operator<=>(Rational const& lhs, Rational const& rhs) {
        T left = lhs.m_num * rhs.m_den;
        T right = rhs.m_num * lhs.m_den;
        if (left < right) return std::strong_ordering::less;
        if (left > right) return std::strong_ordering::greater;
        return std::strong_ordering::equal;
    }

    friend bool operator==(Rational const& lhs, Rational const& rhs) {
        return lhs.m_num == rhs.m_num && lhs.m_den == rhs.m_den;
    }

    friend std::istream& operator>>(std::istream& stream, Rational& rational) {
        return (stream >> rational.m_num).ignore() >> rational.m_den;
    }

    friend std::ostream& operator<<(std::ostream& stream, Rational const& rational) {
        return stream << rational.m_num << '/' << rational.m_den;
    }

    T numerator() const { return m_num; }
    T denominator() const { return m_den; }

private:
    void reduce() {
        if (m_den < 0) {
            m_num = -m_num;
            m_den = -m_den;
        }
        auto gcd = std::gcd(m_num, m_den);
        m_num /= gcd;
        m_den /= gcd;
    }

    T m_num;
    T m_den;
};

bool equal(double x, double y, double epsilon = 1e-6) {
    return std::abs(x - y) < epsilon;
}

TEST(RationalTemplate, IntType) {
    Rational<int> x = 1, y(2, 1);

    EXPECT_TRUE(equal(static_cast<double>(x), 1.0));

    EXPECT_EQ((x += y), Rational<int>(3, 1));
    EXPECT_EQ((x -= y), Rational<int>(1, 1));
    EXPECT_EQ((x *= y), Rational<int>(2, 1));
    EXPECT_EQ((x /= y), Rational<int>(1, 1));

    EXPECT_EQ((x++), Rational<int>(1, 1));
    EXPECT_EQ((x--), Rational<int>(2, 1));
    EXPECT_EQ((++y), Rational<int>(3, 1));
    EXPECT_EQ((--y), Rational<int>(2, 1));

    EXPECT_EQ((x + y), Rational<int>(3, 1));
    EXPECT_EQ((x - y), Rational<int>(-1, 1));
    EXPECT_EQ((x * y), Rational<int>(2, 1));
    EXPECT_EQ((x / y), Rational<int>(1, 2));

    EXPECT_EQ((x += 1), Rational<int>(2, 1));
    EXPECT_EQ((x + 1), Rational<int>(3, 1));
    EXPECT_EQ((1 + y), Rational<int>(3, 1));
    EXPECT_EQ((1 + 1), Rational<int>(2, 1));

    EXPECT_FALSE(x < y);
    EXPECT_FALSE(x > y);
    EXPECT_TRUE(x <= y);
    EXPECT_TRUE(x >= y);
    EXPECT_TRUE(x == y);
    EXPECT_FALSE(x != y);

    std::stringstream stream_1("1/2");
    std::stringstream stream_2;
    stream_1 >> x;
    stream_2 << x;
    EXPECT_EQ(stream_2.str(), "1/2");
}

TEST(RationalTemplate, LongType) {
    Rational<long> a(1, 2);
    Rational<long> b(2, 3);
    
    EXPECT_EQ(a + b, Rational<long>(7, 6));
    EXPECT_EQ(a - b, Rational<long>(-1, 6));
    EXPECT_EQ(a * b, Rational<long>(2, 6));
    EXPECT_EQ(a / b, Rational<long>(3, 4));
}

TEST(RationalTemplate, LongLongType) {
    Rational<long long> a(1000000000LL, 1);
    Rational<long long> b(1, 1000000000LL);
    
    EXPECT_EQ(a * b, Rational<long long>(1, 1));
    EXPECT_EQ(a / b, Rational<long long>(1000000000000000000LL, 1));
}

TEST(RationalTemplate, ShortType) {
    Rational<short> a(3, 4);
    Rational<short> b(1, 2);
    
    EXPECT_EQ(a + b, Rational<short>(5, 4));
    EXPECT_EQ(a - b, Rational<short>(1, 4));
    EXPECT_EQ(a * b, Rational<short>(3, 8));
    EXPECT_EQ(a / b, Rational<short>(3, 2));
}

TEST(RationalTemplate, CharType) {
    Rational<signed char> a(1, 2);
    Rational<signed char> b(1, 3);
    
    EXPECT_EQ(a + b, Rational<signed char>(5, 6));
    EXPECT_EQ(a - b, Rational<signed char>(1, 6));
    EXPECT_EQ(a * b, Rational<signed char>(1, 6));
    EXPECT_EQ(a / b, Rational<signed char>(3, 2));
}

TEST(RationalTemplate, Comparison) {
    Rational<int> a(1, 2);
    Rational<int> b(2, 4);
    Rational<int> c(3, 4);
    
    EXPECT_EQ(a, b);
    EXPECT_NE(a, c);
    EXPECT_LT(a, c);
    EXPECT_LE(a, c);
    EXPECT_GT(c, a);
    EXPECT_GE(c, a);
}

TEST(RationalTemplate, DoubleConversion) {
    Rational<int> r1(1, 2);
    Rational<int> r2(3, 4);
    Rational<int> r3(2, 3);
    
    EXPECT_DOUBLE_EQ(0.5, static_cast<double>(r1));
    EXPECT_DOUBLE_EQ(0.75, static_cast<double>(r2));
    EXPECT_NEAR(0.666666, static_cast<double>(r3), 1e-6);
}

int main() {
    std::cerr << "1. Exception from Rational constructor (zero denominator):\n";
    try {
        Rational<int> r(1, 0);
    } catch (const std::exception& e) {
        std::cerr << "Caught std::exception: " << e.what() << '\n';
    } catch (...) {
        std::cerr << "Caught unknown exception\n";
    }

    std::cerr << "\n2. std::bad_alloc (attempt to allocate huge memory block):\n";
    try {
        int* p = new int[1000000000000];
        delete[] p;
    } catch (const std::exception& e) {
        std::cerr << "Caught std::exception: " << e.what() << '\n';
    } catch (...) {
        std::cerr << "Caught unknown exception\n";
    }

    std::cerr << "\n3. std::bad_variant_access (wrong variant access):\n";
    try {
        std::variant<int, double> v = 42;
        double d = std::get<double>(v);
    } catch (const std::exception& e) {
        std::cerr << "Caught std::exception: " << e.what() << '\n';
    } catch (...) {
        std::cerr << "Caught unknown exception\n";
    }

    std::cerr << "\n4. std::bad_optional_access (access empty optional):\n";
    try {
        std::optional<int> opt;
        int val = opt.value();
    } catch (const std::exception& e) {
        std::cerr << "Caught std::exception: " << e.what() << '\n';
    } catch (...) {
        std::cerr << "Caught unknown exception\n";
    }

    std::cerr << "\n5. std::length_error (reserve exceeds max_size):\n";
    try {
        std::string s;
        s.reserve(std::string::npos);
    } catch (const std::exception& e) {
        std::cerr << "Caught std::exception: " << e.what() << '\n';
    } catch (...) {
        std::cerr << "Caught unknown exception\n";
    }

    std::cerr << "\n6. std::out_of_range (vector::at with index out of range):\n";
    try {
        std::vector<int> v = {1, 2, 3};
        int x = v.at(10);
    } catch (const std::exception& e) {
        std::cerr << "Caught std::exception: " << e.what() << '\n';
    } catch (...) {
        std::cerr << "Caught unknown exception\n";
    }

    return 0;
}