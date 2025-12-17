#include <iostream>

void ClassifyChar(char c) {
    switch ( c ) {
        case 'A' ... 'Z':
            std::cout << "uppercase letter\n";
            break;
        case 'a' ... 'z':
            std::cout << "lowercase letter\n";
            break;
        case '0' ... '9':
            std::cout << "decimal digit\n";
            break;
        case '!':
        case '"':
        case '\'':
        case '(':
        case ')':
        case ',':
        case '-':
        case '.':
        case ':':
        case ';':
        case '?':
            std::cout << "punctuation mark\n";
            break;
        default:
            std::cout << "other character\n";
            break;
    }
}

int main() {
    int input = std::cin.get();
    ClassifyChar(static_cast<char>(input));

    return 0;
}

