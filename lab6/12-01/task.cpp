#include <iomanip>
#include <iostream>
#include <locale>
#include <sstream>
#include <string>

int main() {
    std::locale ru_locale("ru_RU.utf8");
    std::locale us_locale("en_US.utf8");

    std::string input_str;
    std::cout << "Enter amount in RUB (e.g., 1000,00 руб.): ";
    std::getline(std::cin, input_str);

    std::istringstream input_stream(input_str);
    input_stream.imbue(ru_locale);

    long double rub_amount = 0;
    input_stream >> std::get_money(rub_amount);

    if (input_stream.fail()) {
        std::cerr << "Failed to parse amount\n";
        return 1;
    }

    double exchange_rate = 71.5;
    long double usd_amount = rub_amount / exchange_rate;

    std::ostringstream output_stream;
    output_stream.imbue(us_locale);
    output_stream << std::put_money(usd_amount);

    std::cout << "RUB: " << input_str << '\n';
    std::cout << "USD: " << output_stream.str() << '\n';
    std::cout << "Exchange rate: 1 USD = " << exchange_rate << " RUB\n";

    return 0;
}
