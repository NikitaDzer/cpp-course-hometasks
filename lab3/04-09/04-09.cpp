#include <gtest/gtest.h>
#include <type_traits>

namespace my_traits {

template <typename...>
using void_t = void;

template <typename T, typename = void>
struct is_class : std::false_type {};

template <typename T>
struct is_class<T, void_t<int T::*>> : std::true_type {};

template <typename T>
inline constexpr bool is_class_v = is_class<T>::value;

template <typename T>
struct add_const {
    using type = const T;
};

template <typename T>
struct add_const<const T> {
    using type = const T;
};

template <typename T>
using add_const_t = typename add_const<T>::type;

template <typename T>
struct remove_const {
    using type = T;
};

template <typename T>
struct remove_const<const T> {
    using type = T;
};

template <typename T>
using remove_const_t = typename remove_const<T>::type;

template <typename T>
struct remove_reference {
    using type = T;
};

template <typename T>
struct remove_reference<T&> {
    using type = T;
};

template <typename T>
struct remove_reference<T&&> {
    using type = T;
};

template <typename T>
using remove_reference_t = typename remove_reference<T>::type;

template <typename T>
struct remove_cv {
    using type = T;
};

template <typename T>
struct remove_cv<const T> {
    using type = T;
};

template <typename T>
struct remove_cv<volatile T> {
    using type = T;
};

template <typename T>
struct remove_cv<const volatile T> {
    using type = T;
};

template <typename T>
using remove_cv_t = typename remove_cv<T>::type;

template <typename T>
struct remove_cvref {
    using type = remove_cv_t<remove_reference_t<T>>;
};

template <typename T>
using remove_cvref_t = typename remove_cvref<T>::type;

template <typename T>
struct decay {
private:
    using U = remove_reference_t<T>;
    
public:
    using type = std::conditional_t<
        std::is_array_v<U>,
        std::remove_extent_t<U>*,
        std::conditional_t<
            std::is_function_v<U>,
            U*,
            remove_cv_t<U>
        >
    >;
};

template <typename T>
using decay_t = typename decay<T>::type;

template <bool B, typename T, typename F>
struct conditional {
    using type = T;
};

template <typename T, typename F>
struct conditional<false, T, F> {
    using type = F;
};

template <bool B, typename T, typename F>
using conditional_t = typename conditional<B, T, F>::type;

} // namespace my_traits

static_assert(my_traits::is_class_v<int> == false);
static_assert(my_traits::is_class_v<double> == false);
static_assert(my_traits::is_class_v<int*> == false);

struct TestClass {};
static_assert(my_traits::is_class_v<TestClass> == true);

union TestUnion {};
static_assert(my_traits::is_class_v<TestUnion> == true);

enum TestEnum {};
static_assert(my_traits::is_class_v<TestEnum> == false);

static_assert(std::is_same_v<my_traits::add_const_t<int>, const int>);
static_assert(std::is_same_v<my_traits::add_const_t<const int>, const int>);
static_assert(std::is_same_v<my_traits::add_const_t<int*>, int* const>);
static_assert(std::is_same_v<my_traits::add_const_t<const int*>, const int* const>);

static_assert(std::is_same_v<my_traits::remove_const_t<const int>, int>);
static_assert(std::is_same_v<my_traits::remove_const_t<int>, int>);
static_assert(std::is_same_v<my_traits::remove_const_t<const volatile int>, volatile int>);
static_assert(std::is_same_v<my_traits::remove_const_t<int* const>, int*>);

static_assert(std::is_same_v<my_traits::decay_t<int>, int>);
static_assert(std::is_same_v<my_traits::decay_t<const int>, int>);
static_assert(std::is_same_v<my_traits::decay_t<int&>, int>);
static_assert(std::is_same_v<my_traits::decay_t<int&&>, int>);
static_assert(std::is_same_v<my_traits::decay_t<const int&>, int>);

static_assert(std::is_same_v<my_traits::decay_t<int[3]>, int*>);
static_assert(std::is_same_v<my_traits::decay_t<const int[3]>, const int*>);

void test_func();
static_assert(std::is_same_v<my_traits::decay_t<decltype(test_func)>, void(*)()>);
static_assert(std::is_same_v<my_traits::decay_t<void()>, void(*)()>);

static_assert(std::is_same_v<my_traits::conditional_t<true, int, double>, int>);
static_assert(std::is_same_v<my_traits::conditional_t<false, int, double>, double>);
static_assert(std::is_same_v<my_traits::conditional_t<(1 < 2), int, double>, int>);
static_assert(std::is_same_v<my_traits::conditional_t<(1 > 2), int, double>, double>);
