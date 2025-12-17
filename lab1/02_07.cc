#include <iostream>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <vector>

void BubbleSort(double arr[], size_t n) {
    for (size_t i = 0; i < n - 1; i++) {
        for (size_t j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

double FindMax(const double arr[], size_t n) {
    return *std::max_element(arr, arr + n);
}

double FindMin(const double arr[], size_t n) {
    return *std::min_element(arr, arr + n);
}

double CalcMean(const double* arr, size_t n) {
    double sum = std::accumulate(arr, arr + n, 0.0);
    return sum / static_cast<double>(n);
}

double CalcMedian(const double* arr, size_t n) {
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
    std::vector<double> data;
    double value = 0.0;

    while ( std::cin >> value ) {
        data.push_back(value);
    }

    if ( data.empty() ) {
        return 0;
    }

    BubbleSort(data.data(), data.size());

    double max = FindMax(data.data(), data.size());
    double min = FindMin(data.data(), data.size());
    double mean = CalcMean(data.data(), data.size());
    double median = CalcMedian(data.data(), data.size());
    double deviation = CalcDeviation(data.data(), data.size(), mean);

    std::cout << "Max: " << max << "\n";
    std::cout << "Min: " << min << "\n";
    std::cout << "Mean: " << mean << "\n";
    std::cout << "Median: " << median << "\n";
    std::cout << "Standard deviation: " << deviation << "\n";

    return 0;
}
