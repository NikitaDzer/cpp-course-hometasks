#include <gtest/gtest.h>

namespace {

double max_value(double first) {
    return first;
}

template <typename... Args>
double max_value(double first, Args... args) {
    double rest_max = max_value(args...);
    return first > rest_max ? first : rest_max;
}

double min_value(double first) {
    return first;
}

template <typename... Args>
double min_value(double first, Args... args) {
    double rest_min = min_value(args...);
    return first < rest_min ? first : rest_min;
}

template <typename... Args>
double sum(Args... args) {
    return (args + ...);
}

template <typename... Args>
double average(Args... args) {
    return sum(args...) / static_cast<double>(sizeof...(Args));
}

} // namespace

TEST(VariadicTemplateFunctions, MaxValue) {
    EXPECT_DOUBLE_EQ(5.0, max_value(5.0));
    EXPECT_DOUBLE_EQ(10.0, max_value(1.0, 10.0, 5.0));
    EXPECT_DOUBLE_EQ(-1.0, max_value(-5.0, -1.0, -10.0));
    EXPECT_DOUBLE_EQ(3.14, max_value(1.0, 2.0, 3.14, 3.0, 2.5));
    EXPECT_DOUBLE_EQ(0.0, max_value(-1.5, -2.0, 0.0, -3.0));
}

TEST(VariadicTemplateFunctions, MinValue) {
    EXPECT_DOUBLE_EQ(5.0, min_value(5.0));
    EXPECT_DOUBLE_EQ(1.0, min_value(1.0, 10.0, 5.0));
    EXPECT_DOUBLE_EQ(-10.0, min_value(-5.0, -1.0, -10.0));
    EXPECT_DOUBLE_EQ(1.0, min_value(1.0, 2.0, 3.14, 3.0, 2.5));
    EXPECT_DOUBLE_EQ(-3.0, min_value(-1.5, -2.0, 0.0, -3.0));
}

TEST(VariadicTemplateFunctions, Sum) {
    EXPECT_DOUBLE_EQ(5.0, sum(5.0));
    EXPECT_DOUBLE_EQ(16.0, sum(1.0, 10.0, 5.0));
    EXPECT_DOUBLE_EQ(-16.0, sum(-5.0, -1.0, -10.0));
    EXPECT_DOUBLE_EQ(11.64, sum(1.0, 2.0, 3.14, 3.0, 2.5));
    EXPECT_DOUBLE_EQ(-6.5, sum(-1.5, -2.0, 0.0, -3.0));
}

TEST(VariadicTemplateFunctions, Average) {
    EXPECT_DOUBLE_EQ(5.0, average(5.0));
    EXPECT_DOUBLE_EQ(5.333333333333333, average(1.0, 10.0, 5.0));
    EXPECT_DOUBLE_EQ(-5.333333333333333, average(-5.0, -1.0, -10.0));
    EXPECT_DOUBLE_EQ(2.328, average(1.0, 2.0, 3.14, 3.0, 2.5));
    EXPECT_DOUBLE_EQ(-1.625, average(-1.5, -2.0, 0.0, -3.0));
}

TEST(VariadicTemplateFunctions, AllFunctionsTogether) {
    double a = 1.5;
    double b = 2.5;
    double c = 3.5;
    double d = 4.5;
    
    EXPECT_DOUBLE_EQ(4.5, max_value(a, b, c, d));
    EXPECT_DOUBLE_EQ(1.5, min_value(a, b, c, d));
    EXPECT_DOUBLE_EQ(12.0, sum(a, b, c, d));
    EXPECT_DOUBLE_EQ(3.0, average(a, b, c, d));
}
