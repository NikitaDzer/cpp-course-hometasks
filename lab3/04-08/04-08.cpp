#include <gtest/gtest.h>
#include <cstddef>
#include <string>
#include <type_traits>
#include <utility>

template <typename... Ts>
class Tuple {};

template <typename T, typename... Ts>
class Tuple<T, Ts...> {
public:
    constexpr Tuple(T&& x, Ts&&... xs)
        : m_head(std::forward<T>(x)),
          m_tail(std::forward<Ts>(xs)...)
    {}

    template <std::size_t I>
    constexpr auto get() const {
        if constexpr (I > 0) {
            return m_tail.template get<I - 1>();
        } else {
            return m_head;
        }
    }

    constexpr std::size_t size() const {
        return 1 + sizeof...(Ts);
    }

private:
    T m_head;
    Tuple<Ts...> m_tail;
};

template <>
class Tuple<> {
public:
    constexpr Tuple() = default;
    
    constexpr std::size_t size() const {
        return 0;
    }
};

static_assert(std::is_same_v<decltype(Tuple<int, double>(1, 2.0)), Tuple<int, double>>);

constexpr Tuple<int> single_tuple(42);
static_assert(single_tuple.size() == 1);
static_assert(single_tuple.get<0>() == 42);

constexpr Tuple<int, double> double_tuple(10, 3.14);
static_assert(double_tuple.size() == 2);
static_assert(double_tuple.get<0>() == 10);
static_assert(3.1 < double_tuple.get<1>() && double_tuple.get<1>() < 3.2);

constexpr Tuple<int, double, char> triple_tuple(1, 2.0, 'A');
static_assert(triple_tuple.size() == 3);
static_assert(triple_tuple.get<0>() == 1);
static_assert(1.9 < triple_tuple.get<1>() && triple_tuple.get<1>() < 2.1);
static_assert(triple_tuple.get<2>() == 'A');

constexpr Tuple<int, int, int, int> quad_tuple(1, 2, 3, 4);
static_assert(quad_tuple.size() == 4);
static_assert(quad_tuple.get<0>() == 1);
static_assert(quad_tuple.get<1>() == 2);
static_assert(quad_tuple.get<2>() == 3);
static_assert(quad_tuple.get<3>() == 4);
