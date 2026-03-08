#include <gtest/gtest.h>

int ilog2_int(int x) {
    unsigned int u = static_cast<unsigned int>(x);
    int pos = -1;
    while (u) {
        u >>= 1;
        ++pos;
    }
    return pos;
}

int ilog2_float(float x) {
    union {
        float f;
        unsigned int u;
    } converter;
    converter.f = x;
    unsigned int bits = converter.u;
    int exp = (bits >> 23) & 0xFF;
    unsigned int mant = bits & 0x7FFFFF;
    if (exp == 255) {
        return 128;
    }
    if (exp == 0) {
        if (mant == 0) {
            return -128;
        }
        int pos = -1;
        while (mant) {
            mant >>= 1;
            ++pos;
        }
        return pos - 149;
    }
    return exp - 127;
}

TEST(ILog2IntTest, PowersOfTwo) {
    EXPECT_EQ(ilog2_int(1), 0);
    EXPECT_EQ(ilog2_int(2), 1);
    EXPECT_EQ(ilog2_int(4), 2);
    EXPECT_EQ(ilog2_int(8), 3);
    EXPECT_EQ(ilog2_int(16), 4);
    EXPECT_EQ(ilog2_int(1 << 30), 30);
}

TEST(ILog2IntTest, NonPowers) {
    EXPECT_EQ(ilog2_int(3), 1);
    EXPECT_EQ(ilog2_int(5), 2);
    EXPECT_EQ(ilog2_int(6), 2);
    EXPECT_EQ(ilog2_int(7), 2);
    EXPECT_EQ(ilog2_int(9), 3);
    EXPECT_EQ(ilog2_int(15), 3);
}

TEST(ILog2IntTest, Zero) {
    EXPECT_EQ(ilog2_int(0), -1);
}

TEST(ILog2FloatTest, NormalizedNumbers) {
    EXPECT_EQ(ilog2_float(1.0f), 0);
    EXPECT_EQ(ilog2_float(2.0f), 1);
    EXPECT_EQ(ilog2_float(0.5f), -1);
    EXPECT_EQ(ilog2_float(3.1415f), 1);
    EXPECT_EQ(ilog2_float(0.1f), -4);
}

TEST(ILog2FloatTest, DenormalizedNumbers) {
    float denorm = 1.0e-40f;
    int result = ilog2_float(denorm);
    EXPECT_LT(result, -126);
    
    float smallest_denorm = 1.4e-45f;
    EXPECT_EQ(ilog2_float(smallest_denorm), -149);
}

TEST(ILog2FloatTest, SpecialValues) {
    EXPECT_EQ(ilog2_float(0.0f), -128);
    EXPECT_EQ(ilog2_float(-0.0f), -128);
    
    float inf = std::numeric_limits<float>::infinity();
    EXPECT_EQ(ilog2_float(inf), 128);
    
    float nan = std::numeric_limits<float>::quiet_NaN();
    EXPECT_EQ(ilog2_float(nan), 128);
}

TEST(ILog2FloatTest, Boundaries) {
    float max_normal = std::numeric_limits<float>::max();
    EXPECT_EQ(ilog2_float(max_normal), 127);
    
    float min_normal = std::numeric_limits<float>::min();
    EXPECT_EQ(ilog2_float(min_normal), -126);
    
    float min_denorm = std::numeric_limits<float>::denorm_min();
    EXPECT_EQ(ilog2_float(min_denorm), -149);
}
