#include <regex>
#include <string>
#include <vector>

#include <gtest/gtest.h>

struct EmailMatch {
    std::string full;
    std::string domain;
};

std::vector<EmailMatch> extract_emails(const std::string& text) {
    std::regex email_regex(R"(([a-zA-Z0-9._%+-]+)@([a-zA-Z0-9.-]+\.[a-zA-Z]{2,}))");
    std::vector<EmailMatch> results;

    auto begin = std::sregex_iterator(text.begin(), text.end(), email_regex);
    auto end = std::sregex_iterator();

    for (auto it = begin; it != end; ++it) {
        std::smatch match = *it;
        results.push_back({match[0].str(), match[2].str()});
    }

    return results;
}

TEST(EmailExtraction, ValidEmails) {
    std::string text = "Contact us at support@example.com or sales@company.org.";
    auto results = extract_emails(text);
    ASSERT_EQ(results.size(), 2);
    EXPECT_EQ(results[0].full, "support@example.com");
    EXPECT_EQ(results[0].domain, "example.com");
    EXPECT_EQ(results[1].full, "sales@company.org");
    EXPECT_EQ(results[1].domain, "company.org");
}

TEST(EmailExtraction, ComplexEmails) {
    std::string text = "john.doe@university.edu and jane_smith@mail.ru";
    auto results = extract_emails(text);
    ASSERT_EQ(results.size(), 2);
    EXPECT_EQ(results[0].full, "john.doe@university.edu");
    EXPECT_EQ(results[0].domain, "university.edu");
    EXPECT_EQ(results[1].full, "jane_smith@mail.ru");
    EXPECT_EQ(results[1].domain, "mail.ru");
}

TEST(EmailExtraction, SubdomainsAndTags) {
    std::string text = "admin@sub.domain.co.uk and user+tag@gmail.com";
    auto results = extract_emails(text);
    ASSERT_EQ(results.size(), 2);
    EXPECT_EQ(results[0].domain, "sub.domain.co.uk");
    EXPECT_EQ(results[1].domain, "gmail.com");
}

TEST(EmailExtraction, InvalidEmails) {
    std::string text = "@nodomain.com noat sign.com spaces@domain .com";
    auto results = extract_emails(text);
    EXPECT_EQ(results.size(), 0);
}
