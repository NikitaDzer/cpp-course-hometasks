#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <random>
#include <ranges>
#include <vector>

void test_ranges_algorithms() {
    std::vector<int> replace_data = {1, 2, 3, 2, 4, 2, 5};
    std::ranges::replace(replace_data, 2, 0);
    std::cout << "replace: ";
    for (int x : replace_data) std::cout << x << ' ';
    std::cout << '\n';

    std::vector<int> fill_data(5);
    std::ranges::fill(fill_data, 7);
    std::cout << "fill: ";
    for (int x : fill_data) std::cout << x << ' ';
    std::cout << '\n';

    std::vector<int> unique_data = {1, 2, 2, 3, 3, 3, 4};
    auto [unique_begin, unique_end] = std::ranges::unique(unique_data);
    unique_data.erase(unique_begin, unique_data.end());
    std::cout << "unique: ";
    for (int x : unique_data) std::cout << x << ' ';
    std::cout << '\n';

    std::vector<int> rotate_data = {1, 2, 3, 4, 5};
    std::ranges::rotate(rotate_data, rotate_data.begin() + 2);
    std::cout << "rotate: ";
    for (int x : rotate_data) std::cout << x << ' ';
    std::cout << '\n';

    std::vector<int> sample_data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> sample_result(3);
    std::ranges::sample(sample_data, sample_result.begin(), 3, std::mt19937{std::random_device{}()});
    std::cout << "sample: ";
    for (int x : sample_result) std::cout << x << ' ';
    std::cout << '\n';
}

void test_transform_if() {
    std::vector<int> data = {-3, -2, -1, 0, 1, 2, 3, 4, 5};
    std::vector<int> result;

    std::ranges::copy_if(
        data | std::views::transform([](int x) { return x * x; }),
        std::back_inserter(result),
        [](int x) { return x > 4; }
    );

    std::cout << "transform_if (square, keep > 4): ";
    for (int x : result) std::cout << x << ' ';
    std::cout << '\n';
}

double compute_mae(const std::vector<double>& actual, const std::vector<double>& predicted) {
    auto errors = std::views::zip(actual, predicted)
        | std::views::transform([](const auto& pair) {
            return std::abs(std::get<0>(pair) - std::get<1>(pair));
        });
    double sum = std::accumulate(errors.begin(), errors.end(), 0.0);
    return sum / static_cast<double>(actual.size());
}

double compute_mse(const std::vector<double>& actual, const std::vector<double>& predicted) {
    auto errors = std::views::zip(actual, predicted)
        | std::views::transform([](const auto& pair) {
            double diff = std::get<0>(pair) - std::get<1>(pair);
            return diff * diff;
        });
    double sum = std::accumulate(errors.begin(), errors.end(), 0.0);
    return sum / static_cast<double>(actual.size());
}

void test_mae_mse() {
    std::vector<double> actual = {3.0, -0.5, 2.0, 7.0};
    std::vector<double> predicted = {2.5, 0.0, 2.0, 8.0};

    std::cout << "MAE: " << compute_mae(actual, predicted) << '\n';
    std::cout << "MSE: " << compute_mse(actual, predicted) << '\n';
}

void test_views() {
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    std::cout << "filter (even): ";
    auto even = data | std::views::filter([](int x) { return x % 2 == 0; });
    for (int x : even) std::cout << x << ' ';
    std::cout << '\n';

    std::cout << "drop (3): ";
    auto dropped = data | std::views::drop(3);
    for (int x : dropped) std::cout << x << ' ';
    std::cout << '\n';

    std::vector<std::vector<int>> nested = {{1, 2}, {3, 4}, {5, 6}};
    std::cout << "join: ";
    auto joined = nested | std::views::join;
    for (int x : joined) std::cout << x << ' ';
    std::cout << '\n';

    std::vector<std::string> names = {"Alice", "Bob", "Charlie"};
    std::vector<int> ages = {30, 25, 35};
    std::cout << "zip: ";
    auto zipped = std::views::zip(names, ages);
    for (const auto& [name, age] : zipped) {
        std::cout << name << ":" << age << ' ';
    }
    std::cout << '\n';

    std::cout << "stride (2): ";
    auto strided = data | std::views::stride(2);
    for (int x : strided) std::cout << x << ' ';
    std::cout << '\n';
}

class FibonacciIterator {
public:
    using iterator_category = std::input_iterator_tag;
    using value_type = int;
    using difference_type = std::ptrdiff_t;
    using pointer = const int*;
    using reference = const int&;

    FibonacciIterator() : m_current(0), m_next(1) {}

    explicit FibonacciIterator(int count) : m_current(0), m_next(1), m_count(count) {}

    int operator*() const { return m_current; }

    FibonacciIterator& operator++() {
        int temp = m_current + m_next;
        m_current = m_next;
        m_next = temp;
        ++m_count;
        return *this;
    }

    FibonacciIterator operator++(int) {
        FibonacciIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    bool operator==(const FibonacciIterator& other) const {
        return m_count == other.m_count;
    }

    bool operator!=(const FibonacciIterator& other) const {
        return !(*this == other);
    }

private:
    int m_current;
    int m_next;
    int m_count = 0;
};

class Fibonacci : public std::ranges::view_interface<Fibonacci> {
public:
    Fibonacci() = default;

    explicit Fibonacci(int count) : m_count(count) {}

    FibonacciIterator begin() const {
        return FibonacciIterator{};
    }

    FibonacciIterator end() const {
        return FibonacciIterator{m_count};
    }

private:
    int m_count = 0;
};

void test_fibonacci_view() {
    Fibonacci fib(10);
    std::cout << "Fibonacci numbers: ";
    for (int n : fib) std::cout << n << ' ';
    std::cout << '\n';

    std::cout << "Filtered even: ";
    auto even_fib = Fibonacci{10} | std::views::filter([](int x) { return x % 2 == 0; });
    for (int n : even_fib) std::cout << n << ' ';
    std::cout << '\n';
}

int main() {
    test_ranges_algorithms();
    test_transform_if();
    test_mae_mse();
    test_views();
    test_fibonacci_view();

    return 0;
}
