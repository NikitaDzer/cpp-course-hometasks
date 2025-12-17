#include <algorithm>
#include <cassert>
#include <cstddef>
#include <numeric>
#include <utility>
#include <vector>
#include <gtest/gtest.h>

namespace quicksort {

template<typename T>
void order(std::vector<T>& vector, std::size_t left, std::size_t right) {
    for (auto i = left + 1; i < right; ++i) {
        for (auto j = i; j > left; --j) {
            if (vector[j - 1] > vector[j]) {
                std::swap(vector[j], vector[j - 1]);
            }
        }
    }
}

template<typename T>
std::size_t partition(std::vector<T>& vector, std::size_t left, std::size_t right) {
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

template<typename T>
void split(std::vector<T>& vector, std::size_t left, std::size_t right) {
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

template<typename T>
void quicksort(std::vector<T>& vector) { 
    if (!vector.empty()) {
        split(vector, 0, vector.size()); 
    }
}

}  // namespace quicksort


TEST(QuickSort, DoubleType) {
    std::vector<double> vector{3.14, 2.71, 1.41, 1.73, 0.0, -1.0, 2.5};
    
    quicksort::quicksort(vector);
    EXPECT_TRUE(std::ranges::is_sorted(vector));
}

TEST(QuickSort, StringType) {
    std::vector<std::string> vector{"banana", "apple", "cherry", "date", "fig"};
    
    quicksort::quicksort(vector);
    EXPECT_TRUE(std::ranges::is_sorted(vector));
}

TEST(QuickSort, CharType) {
    std::vector<char> vector{'z', 'a', 'm', 'b', 'k', 'c'};
    
    quicksort::quicksort(vector);
    EXPECT_TRUE(std::ranges::is_sorted(vector));
}

struct Person {
    std::string name;
    int age;
    
    bool operator>(const Person& other) const {
        return age > other.age;
    }
    
    bool operator<(const Person& other) const {
        return age < other.age;
    }
    
    bool operator==(const Person& other) const {
        return age == other.age && name == other.name;
    }
};

TEST(QuickSort, CustomType) {
    std::vector<Person> vector{
        {"Alice", 25},
        {"Bob", 30},
        {"Charlie", 20},
        {"David", 35}
    };
    
    quicksort::quicksort(vector);
    EXPECT_TRUE(std::ranges::is_sorted(vector, [](const Person& a, const Person& b) {
        return a.age < b.age;
    }));
}
