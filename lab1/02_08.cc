#include <iostream>
#include <vector>


std::size_t calcCollatzLength(unsigned long long int n, std::vector<size_t> &cache) {
    if ( n < cache.size() && cache[n] != 0 ) {
        return cache[n];
    }

    unsigned long long int next = (n % 2 == 0)
        ? n / 2
        : 3 * n + 1;
    size_t len = 1 + calcCollatzLength(next, cache);

    if ( n >= cache.size() ) {
        cache.resize(n + 1, 0);
    }

    cache[n] = len;
    return len;
}

int main() {
    constexpr std::size_t CACHE_SIZE = 100;

    std::vector<std::size_t> cache(CACHE_SIZE, 0);
    cache[1] = 1;

    std::size_t maxLength = 0;
    unsigned long long int initialValue = 0;

    for ( unsigned long long int i = 1; i <= 100; i++ ) {
        std::size_t len = calcCollatzLength(i, cache);
        if ( len > maxLength ) {
            maxLength = len;
            initialValue = i;
        }
    }

    std::cout << "Max length: " << maxLength << "\n";
    std::cout << "Initial value: " << initialValue << "\n";

    return 0;
}
