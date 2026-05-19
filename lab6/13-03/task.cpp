#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

bool is_blank_line(const std::string& line) {
    return std::all_of(line.begin(), line.end(), [](unsigned char c) {
        return std::isspace(c);
    });
}

int main() {
    const std::string input_file = "13-03_input.txt";
    const std::string output_file = "13-03_output.txt";

    std::ifstream input_stream(input_file);
    if (!input_stream.is_open()) {
        std::cerr << "Cannot open input file: " << input_file << '\n';
        return 1;
    }

    std::ofstream output_stream(output_file);
    if (!output_stream.is_open()) {
        std::cerr << "Cannot open output file: " << output_file << '\n';
        return 1;
    }

    std::string line;
    std::size_t total_lines = 0;
    std::size_t removed_lines = 0;

    while (std::getline(input_stream, line)) {
        ++total_lines;
        if (is_blank_line(line)) {
            ++removed_lines;
        } else {
            output_stream << line << '\n';
        }
    }

    std::cout << "Total lines: " << total_lines << '\n';
    std::cout << "Removed blank lines: " << removed_lines << '\n';

    return 0;
}
