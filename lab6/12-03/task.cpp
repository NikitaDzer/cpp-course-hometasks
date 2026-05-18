#include <string>
#include <string_view>
#include <vector>

#include <gtest/gtest.h>

std::string_view longest_palindrome(std::string_view text) {
    std::size_t n = text.size();
    if (n == 0) return text;

    std::vector<bool> cache(n * n, false);

    std::size_t start = 0;
    std::size_t max_len = 1;

    for (std::size_t i = 0; i < n; ++i) {
        cache[i * n + i] = true;
    }

    for (std::size_t i = 0; i + 1 < n; ++i) {
        if (text[i] == text[i + 1]) {
            cache[i * n + i + 1] = true;
            start = i;
            max_len = 2;
        }
    }

    for (std::size_t len = 3; len <= n; ++len) {
        for (std::size_t i = 0; i + len - 1 < n; ++i) {
            std::size_t j = i + len - 1;
            if (text[i] == text[j] && cache[(i + 1) * n + j - 1]) {
                cache[i * n + j] = true;
                if (len > max_len) {
                    start = i;
                    max_len = len;
                }
            }
        }
    }

    return text.substr(start, max_len);
}

TEST(LongestPalindrome, BasicCases) {
    EXPECT_EQ(longest_palindrome("babad"), "bab");
    EXPECT_EQ(longest_palindrome("cbbd"), "bb");
}

TEST(LongestPalindrome, SingleChar) {
    EXPECT_EQ(longest_palindrome("a"), "a");
    EXPECT_EQ(longest_palindrome("ac"), "a");
}

TEST(LongestPalindrome, FullPalindrome) {
    EXPECT_EQ(longest_palindrome("racecar"), "racecar");
    EXPECT_EQ(longest_palindrome("geeksskeeg"), "geeksskeeg");
}

TEST(LongestPalindrome, LongestFound) {
    EXPECT_EQ(longest_palindrome("forgeeksskeegfor"), "geeksskeeg");
    EXPECT_EQ(longest_palindrome("abacdfgdcaba"), "aba");
    EXPECT_EQ(longest_palindrome("abacdfgdcabba"), "abba");
}

TEST(LongestPalindrome, EmptyString) {
    EXPECT_EQ(longest_palindrome(""), "");
}
