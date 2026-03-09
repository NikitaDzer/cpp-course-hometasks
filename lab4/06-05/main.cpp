#include <print>
#include <iostream>
#include <string>
#include <boost/dll.hpp>

int main()
{
        std::print("Choose library v1 or v2: ");

        std::string library_name;
        std::cin >> library_name;

        std::string library_path;
        if (library_name == "v2") {
                library_path = "lib_v1.so";
        }
        else if (library_name == "v2") {
                library_path = "lib_v2.so";
        }
        else {
                std::print("Bad library!\n");
                return 1;
        }

        try {
                auto test_function = boost::dll::import_symbol<void()>(
                        library_path,
                        "test"
                );

                test_function();
        } catch (const std::exception& exception) {
                std::print("Error during loading library: {}\n", exception.what());
                return 1;
        }

        return 0;
}