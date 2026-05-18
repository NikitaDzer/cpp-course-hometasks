#include <cmath>
#include <optional>
#include <utility>
#include <variant>

#include <gtest/gtest.h>

using RootVariant = std::variant<double, std::pair<double, double>, std::monostate>;
using OptionalRoots = std::optional<RootVariant>;

OptionalRoots solve(double a, double b, double c) {
    constexpr double epsilon = 1e-9;

    if (std::abs(a) < epsilon) {
        if (std::abs(b) < epsilon) {
            if (std::abs(c) < epsilon) {
                return RootVariant{std::monostate{}};
            }
            return std::nullopt;
        }
        return RootVariant{-c / b};
    }

    double discriminant = b * b - 4.0 * a * c;

    if (std::abs(discriminant) < epsilon) {
        return RootVariant{-b / (2.0 * a)};
    }

    if (discriminant > 0.0) {
        double sqrt_d = std::sqrt(discriminant);
        double x1 = (-b - sqrt_d) / (2.0 * a);
        double x2 = (-b + sqrt_d) / (2.0 * a);
        return RootVariant{std::pair<double, double>{x1, x2}};
    }

    return std::nullopt;
}

class Visitor {
public:
    void operator()(double) const { ++count_one; }
    void operator()(const std::pair<double, double>&) const { ++count_two; }
    void operator()(std::monostate) const { ++count_infinite; }

    mutable int count_one = 0;
    mutable int count_two = 0;
    mutable int count_infinite = 0;
};

TEST(QuadraticEquation, TwoRoots) {
    auto result = solve(1.0, -5.0, 6.0);
    ASSERT_TRUE(result.has_value());
    Visitor v;
    std::visit(v, *result);
    EXPECT_EQ(v.count_two, 1);
    auto roots = std::get<std::pair<double, double>>(*result);
    EXPECT_NEAR(roots.first, 2.0, 1e-9);
    EXPECT_NEAR(roots.second, 3.0, 1e-9);
}

TEST(QuadraticEquation, OneRoot) {
    auto result = solve(1.0, -2.0, 1.0);
    ASSERT_TRUE(result.has_value());
    Visitor v;
    std::visit(v, *result);
    EXPECT_EQ(v.count_one, 1);
    EXPECT_NEAR(std::get<double>(*result), 1.0, 1e-9);
}

TEST(QuadraticEquation, NoRoots) {
    auto result = solve(1.0, 0.0, 1.0);
    EXPECT_FALSE(result.has_value());
}

TEST(QuadraticEquation, InfiniteRoots) {
    auto result = solve(0.0, 0.0, 0.0);
    ASSERT_TRUE(result.has_value());
    Visitor v;
    std::visit(v, *result);
    EXPECT_EQ(v.count_infinite, 1);
}

TEST(QuadraticEquation, Contradiction) {
    auto result = solve(0.0, 0.0, 1.0);
    EXPECT_FALSE(result.has_value());
}

TEST(QuadraticEquation, LinearEquation) {
    auto result = solve(0.0, 2.0, -4.0);
    ASSERT_TRUE(result.has_value());
    EXPECT_NEAR(std::get<double>(*result), 2.0, 1e-9);
}
