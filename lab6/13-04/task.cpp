#include <filesystem>
#include <iostream>
#include <regex>
#include <string>

namespace fs = std::filesystem;

int main() {
    std::string directory = "..";
    std::string pattern = ".*\\.cpp";

    std::cout << "Directory: " << directory << '\n';
    std::cout << "Regex: " << pattern << "\n\n";

    std::regex regex(pattern);

    std::size_t total_count = 0;
    std::size_t match_count = 0;

    for (const auto& entry : fs::recursive_directory_iterator(directory)) {
        if (!entry.is_regular_file()) {
            continue;
        }
        ++total_count;
        std::string filename = entry.path().filename().string();
        std::string relative = entry.path().lexically_relative(directory).string();

        if (std::regex_match(filename, regex)) {
            ++match_count;
            std::cout << "  " << relative << " [" << entry.file_size() << " bytes]\n";
        }
    }

    std::cout << "\nTotal files: " << total_count << '\n';
    std::cout << "Matched: " << match_count << '\n';

    return 0;
}
