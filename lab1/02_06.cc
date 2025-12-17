#include <iostream>
#include <algorithm>
#include <numeric>
#include <cmath>

void BubbleSort(double arr[], size_t n) {
    for ( std::size_t i = 0; i < n - 1; i++ ) {
        for ( std::size_t j = 0; j < n - i - 1; j++ ) {
            if ( arr[j] > arr[j + 1] ) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

double FindMax(const double arr[], size_t n) {
    auto it = std::max_element(arr, arr + n);
    return *it;
}

double FindMin(const double arr[], size_t n) {
    auto it = std::min_element(arr, arr + n);
    return *it;
}

double CalcMean(const double *arr, size_t n) {
    double sum = std::accumulate(arr, arr + n, 0.0);
    return sum / static_cast<double>(n);
}

double CalcMedian(const double *arr, size_t n) {
    return n % 2 == 0
        ? (arr[n/2 - 1] + arr[n/2]) / 2.0
        : arr[n/2];
}

double CalcDeviation(const double* arr, size_t n, double mean) {
    double sumSquares = std::accumulate(arr, arr + n, 0.0,
        [mean](double acc, double x) {
            double diff = x - mean;
            return acc + diff * diff;
    });
    return std::sqrt(sumSquares / static_cast<double>(n));
}

int main() {
    constexpr std::size_t MAX_ARRAY_SIZE = 100;

    double data[MAX_ARRAY_SIZE];
    std::size_t count = 0;

    while ( std::cin >> data[count] && count < MAX_ARRAY_SIZE ) {
        count++;
    }

    if ( count == 0 ) {
        return 0;
    }

    double max = FindMax(data, count);
    double min = FindMin(data, count);
    double mean = CalcMean(data, count);
    double median = CalcMedian(data, count);
    double deviation = CalcDeviation(data, count, mean);

    std::cout << "Max: " << max << "\n";
    std::cout << "Min: " << min << "\n";
    std::cout << "Mean: " << mean << "\n";
    std::cout << "Median: " << median << "\n";
    std::cout << "Standard deviation: " << deviation << "\n";

    return 0;
}
