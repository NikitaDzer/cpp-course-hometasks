#include <gtest/gtest.h>
#include <cassert>
#include <numeric>

template<int N = 0, int D = 1>
struct Ratio {
    constexpr static auto num = N;
    constexpr static auto den = D;
};

template<typename R>
struct NormalizedRatio {
private:
    constexpr static int gcd_value = std::gcd(R::num, R::den);
    
public:
    constexpr static auto num = R::num / gcd_value;
    constexpr static auto den = R::den / gcd_value;
    using type = Ratio<num, den>;
};

template<typename R>
using normalized = typename NormalizedRatio<R>::type;

template<typename R1, typename R2>
struct Sum {
private:
    constexpr static auto num = R1::num * R2::den + R2::num * R1::den;
    constexpr static auto den = R1::den * R2::den;
    
public:
    using type = normalized<Ratio<num, den>>;
};

template<typename R1, typename R2>
using sum = typename Sum<R1, R2>::type;

template<typename R1, typename R2>
struct Mul {
private:
    constexpr static auto num = R1::num * R2::num;
    constexpr static auto den = R1::den * R2::den;
    
public:
    using type = normalized<Ratio<num, den>>;
};

template<typename R1, typename R2>
using mul = typename Mul<R1, R2>::type;

template<typename R1, typename R2>
struct Div {
    static_assert(R2::num != 0, "Division by zero");
    using type = mul<R1, Ratio<R2::den, R2::num>>;
};

template<typename R1, typename R2>
using div_v = typename Div<R1, R2>::type;

template<typename R1, typename R2>
struct Sub {
    using type = sum<R1, Ratio<-R2::num, R2::den>>;
};

template<typename R1, typename R2>
using sub = typename Sub<R1, R2>::type;

template<typename T, typename R = Ratio<1>>
struct Duration {
    T x = T();
};

template<typename T1, typename R1, typename T2, typename R2>
constexpr auto operator+(Duration<T1, R1> const& lhs, Duration<T2, R2> const& rhs) {
    using ratio_t = Ratio<1, sum<R1, R2>::den>;
    
    auto x = (
        lhs.x * ratio_t::den / R1::den * R1::num +
        rhs.x * ratio_t::den / R2::den * R2::num
    );
    
    return Duration<decltype(x), ratio_t>(x);
}

template<typename T1, typename R1, typename T2, typename R2>
constexpr auto operator-(Duration<T1, R1> const& lhs, Duration<T2, R2> const& rhs) {
    return lhs + Duration<T2, Ratio<-R2::num, R2::den>>{rhs.x};
}

static_assert(Ratio<1, 2>::num == 1);
static_assert(Ratio<1, 2>::den == 2);

static_assert(std::is_same_v<normalized<Ratio<2, 4>>, Ratio<1, 2>>);
static_assert(std::is_same_v<normalized<Ratio<3, 9>>, Ratio<1, 3>>);
static_assert(std::is_same_v<normalized<Ratio<4, 2>>, Ratio<2, 1>>);

static_assert(std::is_same_v<sum<Ratio<1, 2>, Ratio<1, 3>>, Ratio<5, 6>>);
static_assert(std::is_same_v<sum<Ratio<1, 4>, Ratio<1, 4>>, Ratio<1, 2>>);
static_assert(std::is_same_v<sum<Ratio<1, 6>, Ratio<1, 3>>, Ratio<1, 2>>);

static_assert(std::is_same_v<mul<Ratio<1, 2>, Ratio<2, 3>>, Ratio<1, 3>>);
static_assert(std::is_same_v<mul<Ratio<3, 4>, Ratio<2, 3>>, Ratio<1, 2>>);
static_assert(std::is_same_v<mul<Ratio<2, 5>, Ratio<5, 2>>, Ratio<1, 1>>);

static_assert(std::is_same_v<div_v<Ratio<1, 2>, Ratio<2, 3>>, Ratio<3, 4>>);
static_assert(std::is_same_v<div_v<Ratio<3, 4>, Ratio<3, 2>>, Ratio<1, 2>>);
static_assert(std::is_same_v<div_v<Ratio<2, 3>, Ratio<4, 5>>, Ratio<5, 6>>);

static_assert(std::is_same_v<sub<Ratio<1, 2>, Ratio<1, 3>>, Ratio<1, 6>>);
static_assert(std::is_same_v<sub<Ratio<3, 4>, Ratio<1, 4>>, Ratio<1, 2>>);
static_assert(std::is_same_v<sub<Ratio<5, 6>, Ratio<1, 3>>, Ratio<1, 2>>);
