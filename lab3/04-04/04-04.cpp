#include <gtest/gtest.h>
#include <cassert>
#include <cmath>
#include <compare>
#include <istream>
#include <numeric>
#include <ostream>
#include <sstream>
#include <type_traits>

template <typename T>
class Rational {
    static_assert(std::is_integral_v<T>, "Rational requires integral type");
    
public:
    Rational(T num = 0, T den = 1) : m_num(num), m_den(den) { reduce(); }

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

    Rational const operator++(int) {
        auto x = *this;
        *this += Rational(1, 1);
        return x;
    }

    Rational const operator--(int) {
        auto x = *this;
        *this -= Rational(1, 1);
        return x;
    }

    Rational& operator++() {
        *this += Rational(1, 1);
        return *this;
    }

    Rational& operator--() {
        *this -= Rational(1, 1);
        return *this;
    }

    friend Rational operator+(Rational lhs, Rational const& rhs) { return lhs += rhs; }
    friend Rational operator-(Rational lhs, Rational const& rhs) { return lhs -= rhs; }
    friend Rational operator*(Rational lhs, Rational const& rhs) { return lhs *= rhs; }
    friend Rational operator/(Rational lhs, Rational const& rhs) { return lhs /= rhs; }

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
