#include <algorithm>
#include <cassert>
#include <cstddef>
#include <numeric>
#include <utility>
#include <vector>
#include <iterator>
#include <gtest/gtest.h>

namespace quicksort {

template<typename RandIt>
void order(RandIt first, RandIt last) {
    if (first == last) return;
    for (auto i = std::next(first); i != last; ++i) {
        for (auto j = i; j != first; --j) {
            auto prev_j = std::prev(j);
            if (*prev_j > *j) {
                std::iter_swap(j, prev_j);
            }
        }
    }
}

template<typename RandIt>
RandIt partition(RandIt first, RandIt last) {
    auto dist = std::distance(first, last);
    auto mid = std::next(first, dist / 2);
    auto middle_val = *mid;

    auto i = first;
    auto j = std::prev(last);

    while (true) {
        while (*i < middle_val) {
            ++i;
        }
        while (*j > middle_val) {
            --j;
        }
        if (i >= j) {
            return j;
        }
        std::iter_swap(i, j);
        ++i;
        --j;
    }
}

template<typename RandIt>
void split(RandIt first, RandIt last) {
    auto dist = std::distance(first, last);
    if (dist > 16) {
        auto mid = std::next(first, dist / 2);
        auto prev_last = std::prev(last);

        if (*mid < *first) {
            std::iter_swap(mid, first);
        }
        if (*prev_last < *first) {
            std::iter_swap(prev_last, first);
        }
        if (*prev_last < *mid) {
            std::iter_swap(prev_last, mid);
        }

        auto p = partition(first, last);
        split(first, std::next(p));
        split(std::next(p), last);

    } else {
        order(first, last);
    }
}

template<typename RandIt>
void quicksort(RandIt first, RandIt last) {
    if (first != last) {
        split(first, last);
    }
}

template<typename Container>
void quicksort(Container& container) {
    quicksort(std::begin(container), std::end(container));
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