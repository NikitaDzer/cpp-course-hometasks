#include <algorithm>
#include <cassert>
#include <cstddef>
#include <functional>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace quicksort {

template<typename RandIt, typename Compare = std::less<void>>
void order(RandIt first, RandIt last, Compare comp = Compare{}) {
    if (first == last) return;
    for (auto i = std::next(first); i != last; ++i) {
        for (auto j = i; j != first; --j) {
            auto prev_j = std::prev(j);
            if (comp(*j, *prev_j)) {
                std::iter_swap(j, prev_j);
            }
        }
    }
}

template<typename RandIt, typename Compare = std::less<void>>
RandIt partition(RandIt first, RandIt last, Compare comp = Compare{}) {
    auto dist = std::distance(first, last);
    auto mid = std::next(first, dist / 2);
    auto middle_val = *mid;

    auto i = first;
    auto j = std::prev(last);

    while (true) {
        while (comp(*i, middle_val)) {
            ++i;
        }
        while (comp(middle_val, *j)) {
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

template<typename RandIt, typename Compare = std::less<void>>
void split(RandIt first, RandIt last, Compare comp = Compare{}) {
    auto dist = std::distance(first, last);
    if (dist > 16) {
        auto mid = std::next(first, dist / 2);
        auto prev_last = std::prev(last);

        if (comp(*mid, *first)) {
            std::iter_swap(mid, first);
        }
        if (comp(*prev_last, *first)) {
            std::iter_swap(prev_last, first);
        }
        if (comp(*prev_last, *mid)) {
            std::iter_swap(prev_last, mid);
        }

        auto p = quicksort::partition(first, last, comp);
        split(first, std::next(p), comp);
        split(std::next(p), last, comp);

    } else {
        order(first, last, comp);
    }
}

template<typename RandIt, typename Compare = std::less<void>>
void quicksort(RandIt first, RandIt last, Compare comp = Compare{}) {
    if (first != last) {
        split(first, last, comp);
    }
}

template<typename Container, typename Compare = std::less<void>>
void quicksort(Container& container, Compare comp = Compare{}) {
    quicksort(std::begin(container), std::end(container), comp);
}

}  // namespace quicksort

struct Person {
    std::string name;
    int age;
};

bool compare_by_age(const Person& a, const Person& b) {
    return a.age < b.age;
}

bool compare_by_name(const Person& a, const Person& b) {
    return a.name < b.name;
}

int main() {
    std::cout << "=== QuickSort with Custom Comparators ===" << std::endl;
    
    std::vector<int> vec1 = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    quicksort::quicksort(vec1);
    std::cout << "Default (std::less): ";
    for (int x : vec1) std::cout << x << " ";
    std::cout << std::endl;
    
    std::vector<int> vec2 = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    quicksort::quicksort(vec2, std::greater<>());
    std::cout << "std::greater: ";
    for (int x : vec2) std::cout << x << " ";
    std::cout << std::endl;
    
    std::vector<int> vec3 = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    quicksort::quicksort(vec3, [](int a, int b) { return a > b; });
    std::cout << "Lambda (descending): ";
    for (int x : vec3) std::cout << x << " ";
    std::cout << std::endl;
    
    std::vector<Person> people = {
        {"Alice", 30},
        {"Bob", 25},
        {"Charlie", 35},
        {"Diana", 28}
    };
    
    std::cout << "\nPeople sorted by age (custom function):" << std::endl;
    quicksort::quicksort(people, compare_by_age);
    for (const auto& p : people) {
        std::cout << p.name << ": " << p.age << std::endl;
    }
    
    std::cout << "\nPeople sorted by name (lambda):" << std::endl;
    quicksort::quicksort(people, [](const Person& a, const Person& b) {
        return a.name < b.name;
    });
    for (const auto& p : people) {
        std::cout << p.name << ": " << p.age << std::endl;
    }
    
    assert(std::ranges::is_sorted(vec1));
    assert(std::ranges::is_sorted(vec2, std::greater<>()));
    
    std::cout << "\n=== All tests passed ===" << std::endl;
    
    return 0;
}
