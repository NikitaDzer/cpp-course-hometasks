#include <cassert>
#include <cmath>
#include <iostream>

constexpr double EPSILON = 1e-7;

enum class SolutionCount {
    NO_SOLUTION = 0,
    ONE_SOLUTION = 1,
    TWO_SOLUTIONS = 2,
    INF_SOLUTIONS = -1
};

struct Solution {
    SolutionCount count = SolutionCount::NO_SOLUTION;
    double x1 = 0.0;
    double x2 = 0.0;

    void print() const {
        switch ( count ) {
            default: assert(0); break;
            case SolutionCount::NO_SOLUTION:
                std::cout << "no solutions\n";
                break;
            case SolutionCount::ONE_SOLUTION:
                std::cout << x1 << "\n";
                break;
            case SolutionCount::TWO_SOLUTIONS:
                std::cout << x1 << " " << x2 << "\n";
                break;
            case SolutionCount::INF_SOLUTIONS:
                std::cout << "infinite solutions" << "\n";
                break;
        }
    }
};

Solution SolveLinear(double b, double c) {
    if ( std::abs(b) < EPSILON ) {
        return std::abs(c) < EPSILON
            ? Solution{SolutionCount::INF_SOLUTIONS, 0.0, 0.0}
            : Solution{SolutionCount::NO_SOLUTION, 0.0, 0.0};
    }

    double x = -c / b;
    return {SolutionCount::ONE_SOLUTION, x, 0.0};
}

Solution SolveQuadratic(double a, double b, double c) {
    if ( std::abs(a) < EPSILON ) {
        return SolveLinear(b, c);
    }

    double discriminant = b * b - 4 * a * c;

    if ( std::abs(discriminant) < EPSILON ) {
        double x = -b / (2 * a);
        return {SolutionCount::ONE_SOLUTION, x, 0.0};
    }

    if ( discriminant > 0 ) {
        double sqrt_d = std::sqrt(discriminant);
        double x1 = (-b + sqrt_d) / (2 * a);
        double x2 = (-b - sqrt_d) / (2 * a);

        return {SolutionCount::TWO_SOLUTIONS, x1, x2};
    }

    return {SolutionCount::NO_SOLUTION, 0.0, 0.0};
}

int main() {
    double a = 0.0;
    double b = 0.0;
    double c = 0.0;

    std::cin >> a;
    std::cin >> b;
    std::cin >> c;

    Solution solution = SolveQuadratic(a, b, c);
    solution.print();

    return 0;
}
