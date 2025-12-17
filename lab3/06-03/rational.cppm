export module rational;

import std;

export namespace math {

class Rational {
public:
    Rational(int num, int den = 1) : m_num(num), m_den(den) {
        reduce();
    }
    
    explicit operator double() const { return 1.0 * m_num / m_den; }
    
    Rational& operator+=(Rational const& other);
    Rational& operator-=(Rational const& other);
    Rational& operator*=(Rational const& other);
    Rational& operator/=(Rational const& other);
    
    Rational const operator++(int);
    Rational const operator--(int);
    
    Rational& operator++();
    Rational& operator--();
    
    friend Rational operator+(Rational lhs, Rational const& rhs) { return lhs += rhs; }
    friend Rational operator-(Rational lhs, Rational const& rhs) { return lhs -= rhs; }
    friend Rational operator*(Rational lhs, Rational const& rhs) { return lhs *= rhs; }
    friend Rational operator/(Rational lhs, Rational const& rhs) { return lhs /= rhs; }
    
    friend std::strong_ordering operator<=>(Rational const& lhs, Rational const& rhs);
    friend bool operator==(Rational const& lhs, Rational const& rhs);
    
    friend std::istream& operator>>(std::istream& stream, Rational& rational);
    friend std::ostream& operator<<(std::ostream& stream, Rational const& rational);
    
private:
    void reduce();
    int m_num = 0, m_den = 1;
};

inline bool equal(double x, double y, double epsilon = 1e-6) { 
    return std::abs(x - y) < epsilon;
}

}
