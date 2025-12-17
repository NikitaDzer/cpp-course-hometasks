#include <algorithm>
#include <cassert>
#include <cstddef>
#include <numeric>
#include <utility>
#include <vector>
#include <gtest/gtest.h>

namespace quicksort {

void order(std::vector<int>& vector, std::size_t left, std::size_t right) {
    for (auto i = left + 1; i < right; ++i) {
        for (auto j = i; j > left; --j) {
            if (vector[j - 1] > vector[j]) {
                std::swap(vector[j], vector[j - 1]);
            }
        }
    }
}

std::size_t partition(std::vector<int>& vector, std::size_t left, std::size_t right) {
    auto middle_val = vector[std::midpoint(left, right)];

    auto i = left - 1;
    auto j = right;

    while (true) {
        while (vector[++i] < middle_val) {
        }
        while (vector[--j] > middle_val) {
        }
        if (i >= j) {
            return j;
        }
        std::swap(vector[i], vector[j]);
    }
}

void split(std::vector<int>& vector, std::size_t left, std::size_t right) {
    if (right - left > 16) {
        auto middle = std::midpoint(left, right);

        if (vector[middle] < vector[left]) {
            std::swap(vector[middle], vector[left]);
        }
        if (vector[right - 1] < vector[left]) {
            std::swap(vector[right - 1], vector[left]);
        }
        if (vector[right - 1] < vector[middle]) {
            std::swap(vector[right - 1], vector[middle]);
        }

        auto p = partition(vector, left, right);
        split(vector, left, p);
        split(vector, p, right);

    } else {
        order(vector, left, right);
    }
}

void quicksort(std::vector<int>& vector) { split(vector, 0, std::size(vector)); }

}  // namespace quicksort


TEST(QuickSort, EmptySequence) {
    std::vector<int> vector{};

    quicksort::quicksort(vector);
    EXPECT_TRUE(std::ranges::is_sorted(vector));
}

TEST(QuickSort, OneElement) {
    std::vector<int> vector{0x52};

    quicksort::quicksort(vector);
    EXPECT_TRUE(std::ranges::is_sorted(vector));
}

TEST(QuickSort, TwoElementsSorted) {
    std::vector<int> vector{1, 2};

    quicksort::quicksort(vector);
    EXPECT_TRUE(std::ranges::is_sorted(vector));
}

TEST(QuickSort, TwoElementsNotSorted) {
    std::vector<int> vector{2, 1};

    quicksort::quicksort(vector);
    EXPECT_TRUE(std::ranges::is_sorted(vector));
}

TEST(QuickSort, TwoElementsEqual) {
    std::vector<int> vector{1, 1};
    quicksort::quicksort(vector);

    EXPECT_TRUE(std::ranges::is_sorted(vector));
}

TEST(QuickSort, LongUnsortedSequence) {
    std::vector<int> vector{-5, 1, 12, 2, 8, 9, 5, 8, 11, -12, 37, 42, 1, 35};

    quicksort::quicksort(vector);
    EXPECT_TRUE(std::ranges::is_sorted(vector));
}
