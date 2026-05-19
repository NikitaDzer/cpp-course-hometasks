#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

class Calculator {
public:
    explicit Calculator(const std::string& expr) : m_stream(expr), m_ch(' ') {
        next_char();
    }

    double evaluate() {
        double result = parse_expression();
        if (m_ch != '\0') {
            throw std::runtime_error("Unexpected character at end of expression");
        }
        return result;
    }

private:
    std::istringstream m_stream;
    char m_ch;

    void next_char() {
        if (m_stream.get(m_ch)) {
            while (m_ch == ' ') {
                if (!m_stream.get(m_ch)) {
                    m_ch = '\0';
                    return;
                }
            }
        } else {
            m_ch = '\0';
        }
    }

    double parse_expression() {
        return parse_addition();
    }

    double parse_addition() {
        double left = parse_multiplication();
        while (m_ch == '+' || m_ch == '-') {
            char op = m_ch;
            next_char();
            double right = parse_multiplication();
            if (op == '+') {
                left += right;
            } else {
                left -= right;
            }
        }
        return left;
    }

    double parse_multiplication() {
        double left = parse_modulo();
        while (m_ch == '*' || m_ch == '/') {
            char op = m_ch;
            next_char();
            double right = parse_modulo();
            if (op == '*') {
                left *= right;
            } else {
                if (right == 0.0) {
                    throw std::runtime_error("Division by zero");
                }
                left /= right;
            }
        }
        return left;
    }

    double parse_modulo() {
        double left = parse_power();
        while (m_ch == '%') {
            next_char();
            double right = parse_power();
            long long il = static_cast<long long>(left);
            long long ir = static_cast<long long>(right);
            if (ir == 0) {
                throw std::runtime_error("Modulo by zero");
            }
            left = static_cast<double>(il % ir);
        }
        return left;
    }

    double parse_power() {
        double base = parse_factorial();
        if (m_ch == '^') {
            next_char();
            double exponent = parse_power();
            return std::pow(base, exponent);
        }
        return base;
    }

    double parse_factorial() {
        double value = parse_primary();
        while (m_ch == '!') {
            next_char();
            long long n = static_cast<long long>(value);
            if (n < 0) {
                throw std::runtime_error("Factorial of negative number");
            }
            double result = 1.0;
            for (long long i = 2; i <= n; ++i) {
                result *= i;
            }
            value = result;
        }
        return value;
    }

    double parse_primary() {
        if (m_ch == '(' || m_ch == '[' || m_ch == '{') {
            char open = m_ch;
            next_char();
            double value = parse_expression();
            char close = (open == '(') ? ')' : (open == '[') ? ']' : '}';
            if (m_ch != close) {
                throw std::runtime_error(std::string("Expected '") + close + "'");
            }
            next_char();
            return value;
        }

        if (m_ch == '-' || m_ch == '+') {
            char sign = m_ch;
            next_char();
            double value = parse_primary();
            return (sign == '-') ? -value : value;
        }

        if (std::isdigit(m_ch) || m_ch == '.') {
            std::string num_str;
            while (std::isdigit(m_ch) || m_ch == '.') {
                num_str += m_ch;
                next_char();
            }
            return std::stod(num_str);
        }

        throw std::runtime_error(std::string("Unexpected character: '") + m_ch + "'");
    }
};

int main() {
    const std::string input_file = "13-02_input.txt";

    std::ifstream input_stream(input_file);
    if (!input_stream.is_open()) {
        std::cerr << "Cannot open input file: " << input_file << '\n';
        return 1;
    }

    std::cout << "Reading expressions from: " << input_file << "\n\n";

    std::string line;
    while (std::getline(input_stream, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }

        try {
            Calculator calc(line);
            double result = calc.evaluate();
            std::cout << line << " = " << result << '\n';
        } catch (const std::exception& e) {
            std::cout << line << " -> ERROR: " << e.what() << '\n';
        }
    }

    return 0;
}
