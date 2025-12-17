#include <gtest/gtest.h>
#include <array>
#include <cmath>
#include <limits>

namespace {

consteval double compute_pi(double epsilon) {
    double pi_over_4 = 0.0;
    double term = 1.0;
    int n = 0;

    while (std::abs(term) >= epsilon) {
        if (n % 2 == 0) {
            term = 1.0 / (2.0 * n + 1.0);
        } else {
            term = -1.0 / (2.0 * n + 1.0);
        }
        pi_over_4 += term;
        n++;
    }

    return pi_over_4 * 4.0;
}

consteval double compute_e(double epsilon) {
    double e = 1.0;
    double term = 1.0;
    int n = 1;
    
    while (term >= epsilon) {
        term = term / n;
        e += term;
        n++;
    }
    
    return e;
}

constexpr std::array<double, 4> epsilons = {1e-1, 1e-2, 1e-3, 1e-4};

} // namespace

static_assert(compute_pi(epsilons[0]) > 2.9   && compute_pi(epsilons[0]) < 3.2);
static_assert(compute_pi(epsilons[1]) > 3.1   && compute_pi(epsilons[1]) < 3.2);
static_assert(compute_pi(epsilons[2]) > 3.14  && compute_pi(epsilons[2]) < 3.15);
static_assert(compute_pi(epsilons[3]) > 3.141 && compute_pi(epsilons[3]) < 3.142);

static_assert(compute_e(epsilons[0]) > 2.6    && compute_e(epsilons[0]) < 2.8);
static_assert(compute_e(epsilons[1]) > 2.70   && compute_e(epsilons[1]) < 2.74);
static_assert(compute_e(epsilons[2]) > 2.718  && compute_e(epsilons[2]) < 2.719);
static_assert(compute_e(epsilons[3]) > 2.7182 && compute_e(epsilons[3]) < 2.7183);
