#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include <gtest/gtest.h>

std::string to_hex_string(const std::vector<std::uint8_t>& data) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (std::uint8_t byte : data) {
        ss << std::setw(2) << static_cast<int>(byte);
    }
    return ss.str();
}

std::vector<std::uint8_t> from_hex_string(const std::string& hex) {
    std::vector<std::uint8_t> data;
    for (std::size_t i = 0; i + 1 < hex.size(); i += 2) {
        std::string byte_str = hex.substr(i, 2);
        std::uint8_t byte = static_cast<std::uint8_t>(std::stoul(byte_str, nullptr, 16));
        data.push_back(byte);
    }
    return data;
}

TEST(HexConversion, ToHexString) {
    std::vector<std::uint8_t> data = {0x48, 0x65, 0x6C, 0x6C, 0x6F};
    EXPECT_EQ(to_hex_string(data), "48656c6c6f");
}

TEST(HexConversion, FromHexString) {
    std::string hex = "48656c6c6f";
    auto data = from_hex_string(hex);
    ASSERT_EQ(data.size(), 5);
    EXPECT_EQ(data[0], 0x48);
    EXPECT_EQ(data[1], 0x65);
    EXPECT_EQ(data[2], 0x6C);
    EXPECT_EQ(data[3], 0x6C);
    EXPECT_EQ(data[4], 0x6F);
}

TEST(HexConversion, Roundtrip) {
    std::vector<std::uint8_t> original = {0x00, 0xFF, 0x7F, 0x80, 0x01};
    std::string hex = to_hex_string(original);
    auto restored = from_hex_string(hex);
    EXPECT_EQ(original, restored);
}

TEST(HexConversion, EmptyVector) {
    std::vector<std::uint8_t> empty;
    EXPECT_EQ(to_hex_string(empty), "");
    EXPECT_TRUE(from_hex_string("").empty());
}

TEST(HexConversion, SingleByte) {
    std::vector<std::uint8_t> single = {0xFF};
    EXPECT_EQ(to_hex_string(single), "ff");
    auto restored = from_hex_string("ff");
    ASSERT_EQ(restored.size(), 1);
    EXPECT_EQ(restored[0], 0xFF);
}

TEST(HexConversion, AllZeros) {
    std::vector<std::uint8_t> zeros = {0x00, 0x00, 0x00};
    EXPECT_EQ(to_hex_string(zeros), "000000");
    auto restored = from_hex_string("000000");
    ASSERT_EQ(restored.size(), 3);
    for (auto b : restored) EXPECT_EQ(b, 0x00);
}
