#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <unordered_set>
#include <vector>

static const char* hash_names[] = {
    "RS Hash",
    "JS Hash", 
    "PJW Hash",
    "ELF Hash",
    "BKDR Hash",
    "SDBM Hash",
    "DJB Hash",
    "DEK Hash",
    "FNV Hash"
};

std::uint32_t rs_hash(const std::string& str) {
    std::uint32_t b = 378551;
    std::uint32_t a = 63689;
    std::uint32_t hash = 0;
    for (char c : str) {
        hash = hash * a + static_cast<std::uint32_t>(c);
        a *= b;
    }
    return hash;
}

std::uint32_t js_hash(const std::string& str) {
    std::uint32_t hash = 1315423911;
    for (char c : str) {
        hash ^= ((hash << 5) + static_cast<std::uint32_t>(c) + (hash >> 2));
    }
    return hash;
}

std::uint32_t pjw_hash(const std::string& str) {
    std::uint32_t bits_in_unsigned_int = 32;
    std::uint32_t three_quarters = (bits_in_unsigned_int * 3) / 4;
    std::uint32_t one_eighth = bits_in_unsigned_int / 8;
    std::uint32_t high_bits = 0xFFFFFFFF << (bits_in_unsigned_int - one_eighth);
    std::uint32_t hash = 0;
    std::uint32_t test = 0;
    for (char c : str) {
        hash = (hash << one_eighth) + static_cast<std::uint32_t>(c);
        if ((test = hash & high_bits) != 0) {
            hash = ((hash ^ (test >> three_quarters)) & ~high_bits);
        }
    }
    return hash;
}

std::uint32_t elf_hash(const std::string& str) {
    std::uint32_t hash = 0;
    std::uint32_t x = 0;
    for (char c : str) {
        hash = (hash << 4) + static_cast<std::uint32_t>(c);
        if ((x = hash & 0xF0000000) != 0) {
            hash ^= (x >> 24);
        }
        hash &= ~x;
    }
    return hash;
}

std::uint32_t bkdr_hash(const std::string& str) {
    std::uint32_t seed = 131;
    std::uint32_t hash = 0;
    for (char c : str) {
        hash = (hash * seed) + static_cast<std::uint32_t>(c);
        seed *= 131;
    }
    return hash;
}

std::uint32_t sdbm_hash(const std::string& str) {
    std::uint32_t hash = 0;
    for (char c : str) {
        hash = static_cast<std::uint32_t>(c) + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}

std::uint32_t djb_hash(const std::string& str) {
    std::uint32_t hash = 5381;
    for (char c : str) {
        hash = ((hash << 5) + hash) + static_cast<std::uint32_t>(c);
    }
    return hash;
}

std::uint32_t dek_hash(const std::string& str) {
    std::uint32_t hash = static_cast<std::uint32_t>(str.length());
    for (std::size_t i = 0; i < str.length(); ++i) {
        hash = ((hash << 5) ^ (hash >> 27)) ^ static_cast<std::uint32_t>(str[i]);
    }
    return hash;
}

std::uint32_t fnv_hash(const std::string& str) {
    std::uint32_t fnv_prime = 0x811C9DC5;
    std::uint32_t hash = 0;
    for (char c : str) {
        hash *= fnv_prime;
        hash ^= static_cast<std::uint32_t>(c);
    }
    return hash;
}

using HashFunc = std::uint32_t(*)(const std::string&);

HashFunc hash_functions[] = {
    rs_hash,
    js_hash,
    pjw_hash,
    elf_hash,
    bkdr_hash,
    sdbm_hash,
    djb_hash,
    dek_hash,
    fnv_hash
};

std::string generate_random_string(std::mt19937& gen, std::size_t length) {
    static const char charset[] = 
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789";
    std::uniform_int_distribution<> dist(0, sizeof(charset) - 2);
    std::string result;
    result.reserve(length);
    for (std::size_t i = 0; i < length; ++i) {
        result += charset[dist(gen)];
    }
    return result;
}

struct CollisionResult {
    std::size_t strings_tested;
    std::size_t collisions;
    std::size_t unique_hashes;
};

CollisionResult test_hash_function(HashFunc func, const std::vector<std::string>& strings) {
    std::unordered_set<std::uint32_t> seen;
    std::size_t collisions = 0;
    
    for (const auto& s : strings) {
        std::uint32_t h = func(s);
        if (!seen.insert(h).second) {
            ++collisions;
        }
    }
    
    return {strings.size(), collisions, seen.size()};
}

void print_bar(std::size_t value, std::size_t max, std::size_t width = 50) {
    std::size_t bars = (value * width) / std::max(max, static_cast<std::size_t>(1));
    for (std::size_t i = 0; i < bars; ++i) {
        std::cout << '#';
    }
}

int main() {
    std::cout << "=== Hash Function Collision Analysis ===" << std::endl;
    std::cout << std::endl;
    
    std::vector<std::size_t> test_sizes = {100, 500, 1000, 5000, 10000, 50000, 100000};
    std::size_t string_length = 16;
    
    std::mt19937 gen(42);
    std::vector<std::string> strings;
    
    for (std::size_t size : test_sizes) {
        std::cout << "\nTest with " << size << " strings (length=" << string_length << "):\n";
        std::cout << std::string(70, '-') << std::endl;
        
        strings.clear();
        strings.reserve(size);
        for (std::size_t i = 0; i < size; ++i) {
            strings.push_back(generate_random_string(gen, string_length));
        }
        
        std::cout << std::endl;
        for (int i = 0; i < 9; ++i) { //where is this magic number 9 from?
            auto result = test_hash_function(hash_functions[i], strings);
            double collision_rate = (result.collisions * 100.0) / result.strings_tested;
            
            std::cout << std::left << std::setw(12) << hash_names[i] << ": ";
            print_bar(result.collisions, size / 10);
            std::cout << " collisions=" << std::setw(6) << result.collisions 
                      << " (" << std::fixed << std::setprecision(3) << collision_rate << "%)" << std::endl;
        }
    }
    
    std::cout << "\n=== Summary Statistics ===" << std::endl;
    std::cout << std::string(70, '-') << std::endl;
    
    const std::size_t final_size = 100000;
    strings.clear();
    strings.reserve(final_size);
    for (std::size_t i = 0; i < final_size; ++i) {
        strings.push_back(generate_random_string(gen, string_length));
    }
    
    std::vector<std::pair<std::size_t, std::string>> results;
    for (int i = 0; i < 9; ++i) {
        auto result = test_hash_function(hash_functions[i], strings);
        results.emplace_back(result.collisions, hash_names[i]);
    }
    
    std::sort(results.begin(), results.end());
    
    std::cout << "\nRanking (best to worst by collision count):\n" << std::endl;
    for (std::size_t rank = 0; rank < results.size(); ++rank) {
        const auto& [collisions, name] = results[rank];
        std::cout << rank + 1 << ". " << std::setw(12) << name 
                  << ": " << collisions << " collisions" << std::endl;
    }
    
    std::cout << "\n=== Analysis Complete ===" << std::endl;
    
    return 0;
}

/*
 * score is 4/10
 * 1. need to add graph to repo
 * 2. need to add conclusion here
 * 3. need to add discussion here
 * 
 * 4. make everything std::string, not mix of char * and string 
 * 
