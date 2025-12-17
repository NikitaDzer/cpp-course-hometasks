#include <cassert>
#include <cstdlib>
#include <utility>
#include <numeric>
#include <gtest/gtest.h>

namespace gcd {

int remainder(int a, int b) {
    if (int r = a % b; r >= 0) {
        return r;
    } else {
        return r + std::abs(b);
    }
}

int gcd_iterative(int a, int b) {
    int r = remainder(a, b);
    while (r != 0) {
        a = std::exchange(b, r);
        r = remainder(a, b);
    }
    return std::abs(b);
}

int gcd_recursive(int a, int b) {
    int r = remainder(a, b);
    return r != 0 ? gcd_recursive(b, r) : std::abs(b);
}

}  // namespace gcd

///////////////////////

namespace lcm {

int lcm_iterative(int a, int b) {
    if (a == 0 || b == 0) {
        return 0;
    }
    auto a_abs = std::abs(a);
    auto b_abs = std::abs(b);

    return (a_abs / gcd::gcd_iterative(a_abs, b_abs)) * b_abs;
}

int lcm_recursive(int a, int b) {
    if (a == 0 || b == 0) {
        return 0;
    }
    auto a_abs = std::abs(a);
    auto b_abs = std::abs(b);

    return (a_abs / gcd::gcd_recursive(a_abs, b_abs)) * b_abs;
}

}  // namespace lcm

/////////////////////////

TEST(GCDTest, PositiveNumbers) {
    std::pair<int, int> test_cases[] = {{48, 18},   {56, 42}, {17, 13},  {100, 75},
                                        {1, 1},     {1, 100}, {100, 1},  {123456, 7890},
                                        {999, 111}, {81, 27}, {255, 85}, {144, 89}};

    for (const auto& [a, b] : test_cases) {
        int std_result = std::gcd(a, b);
        EXPECT_EQ(gcd::gcd_iterative(a, b), std_result);
        EXPECT_EQ(gcd::gcd_recursive(a, b), std_result);
    }
}

TEST(GCDTest, NegativeNumbers) {
    std::pair<int, int> test_cases[] = {{-48, 18},  {56, -42}, {-100, -75}, {-17, 13}, {17, -13},
                                        {-17, -13}, {-1, 1},   {1, -1},     {-1, -1}};

    for (const auto& [a, b] : test_cases) {
        int std_result = std::gcd(a, b);
        EXPECT_EQ(gcd::gcd_iterative(a, b), std_result);
        EXPECT_EQ(gcd::gcd_recursive(a, b), std_result);
    }
}

TEST(GCDTest, EqualNumbers) {
    std::pair<int, int> test_cases[] = {{15, 15}, {1, 1},       {100, 100}, {-15, -15},
                                        {-1, -1}, {-100, -100}};

    for (const auto& [a, b] : test_cases) {
        int std_result = std::gcd(a, b);
        EXPECT_EQ(gcd::gcd_iterative(a, b), std_result);
        EXPECT_EQ(gcd::gcd_recursive(a, b), std_result);
    }
}

TEST(GCDTest, LargeNumbers) {
    std::pair<int, int> test_cases[] = {{2147483647, 2147483646},
                                        {1000000000, 999999999},
                                        {123456789, 987654321},
                                        {999999999, 888888888}};

    for (const auto& [a, b] : test_cases) {
        int std_result = std::gcd(a, b);
        EXPECT_EQ(gcd::gcd_iterative(a, b), std_result);
        EXPECT_EQ(gcd::gcd_recursive(a, b), std_result);
    }
}

////////////////

TEST(LCMTest, PositiveNumbers) {
    std::pair<int, int> test_cases[] = {{48, 18},   {56, 42}, {17, 13},  {100, 75},
                                        {1, 1},     {1, 100}, {100, 1},  {123456, 7890},
                                        {999, 111}, {81, 27}, {255, 85}, {144, 89}};

    for (const auto& [a, b] : test_cases) {
        int std_result = std::lcm(a, b);
        EXPECT_EQ(lcm::lcm_iterative(a, b), std_result);
        EXPECT_EQ(lcm::lcm_recursive(a, b), std_result);
    }
}

TEST(LCMTest, NegativeNumbers) {
    std::pair<int, int> test_cases[] = {{-48, 18},  {56, -42}, {-100, -75}, {-17, 13}, {17, -13},
                                        {-17, -13}, {-1, 1},   {1, -1},     {-1, -1}};

    for (const auto& [a, b] : test_cases) {
        int std_result = std::lcm(a, b);
        EXPECT_EQ(lcm::lcm_iterative(a, b), std_result);
        EXPECT_EQ(lcm::lcm_recursive(a, b), std_result);
    }
}

TEST(LCMTest, EqualNumbers) {
    std::pair<int, int> test_cases[] = {{15, 15}, {1, 1},       {100, 100}, {-15, -15},
                                        {-1, -1}, {-100, -100}};

    for (const auto& [a, b] : test_cases) {
        int std_result = std::lcm(a, b);
        EXPECT_EQ(lcm::lcm_iterative(a, b), std_result);
        EXPECT_EQ(lcm::lcm_recursive(a, b), std_result);
    }
}
