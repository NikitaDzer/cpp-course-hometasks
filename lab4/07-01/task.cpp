#include <cmath>
#include <iostream>
#include <optional>
#include <utility>
#include <variant>

constexpr double EPSILON = 1e-7;

using Solution = std::optional<std::variant<std::monostate, double, std::pair<double, double>>>;

Solution SolveLinear(double b, double c) {
    if (std::abs(b) < EPSILON) {
        if (std::abs(c) < EPSILON)
            return std::make_optional(std::variant<std::monostate, double, std::pair<double, double>>(std::monostate{}));
        else
            return std::nullopt;
    }
    double x = -c / b;
    return std::make_optional(std::variant<std::monostate, double, std::pair<double, double>>(x));
}

Solution SolveQuadratic(double a, double b, double c) {
    if (std::abs(a) < EPSILON)
        return SolveLinear(b, c);

    double discriminant = b * b - 4 * a * c;

    if (std::abs(discriminant) < EPSILON) {
        double x = -b / (2 * a);
        return std::make_optional(std::variant<std::monostate, double, std::pair<double, double>>(x));
    }

    if (discriminant > 0) {
        double sqrt_d = std::sqrt(discriminant);
        double x1 = (-b + sqrt_d) / (2 * a);
        double x2 = (-b - sqrt_d) / (2 * a);
        return std::make_optional(std::variant<std::monostate, double, std::pair<double, double>>(std::pair(x1, x2)));
    }

    return std::nullopt;
}

void PrintSolution(const Solution& sol) {
    if (!sol.has_value()) {
        std::cout << "no solutions\n";
        return;
    }

    const auto& var = sol.value();
    if (std::holds_alternative<std::monostate>(var)) {
        std::cout << "infinite solutions\n";
    } else if (std::holds_alternative<double>(var)) {
        std::cout << std::get<double>(var) << '\n';
    } else {
        auto [x1, x2] = std::get<std::pair<double, double>>(var);
        std::cout << x1 << ' ' << x2 << '\n';
    }
}

int main() {
    double a = 0.0;
    double b = 0.0;
    double c = 0.0;
    std::cin >> a >> b >> c;

    Solution sol = SolveQuadratic(a, b, c);
    PrintSolution(sol);
    
    return 0;
}