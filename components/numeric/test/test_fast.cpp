#include <math.h>
#include <gtest/gtest.h>
#include <numeric/fast.h>

using namespace numeric;

class FastTest : public testing::Test {
protected:
};

TEST_F(FastTest, Trig) {
    constexpr float sqrt1_2f = (float)M_SQRT1_2;
    constexpr double sqrt1_2d = M_SQRT1_2;
    constexpr float abs_error = 0.002f; // 0.2%


    EXPECT_FLOAT_EQ( 0.0f,      fast::sin<float>(-M_PI));
    EXPECT_NEAR    (-sqrt1_2f,  fast::sin<float>(-M_PI_2 - M_PI_4), abs_error);
    EXPECT_FLOAT_EQ(-1.0f,      fast::sin<float>(-M_PI_2));
    EXPECT_NEAR    (-sqrt1_2f,  fast::sin<float>(-M_PI_4), abs_error);
    EXPECT_FLOAT_EQ( 0.0f,      fast::sin<float>(0.0));
    EXPECT_NEAR    ( sqrt1_2f,  fast::sin<float>(M_PI_4), abs_error);
    EXPECT_FLOAT_EQ( 1.0f,      fast::sin<float>(M_PI_2));
    EXPECT_NEAR    ( sqrt1_2f,  fast::sin<float>(M_PI_2 + M_PI_4), abs_error);
    EXPECT_FLOAT_EQ( 0.0f,      fast::sin<float>(M_PI));

    EXPECT_FLOAT_EQ(-1.0f,      fast::cos<float>(-M_PI));
    EXPECT_NEAR    (-sqrt1_2f,  fast::cos<float>(-M_PI_2 - M_PI_4), abs_error);
    EXPECT_FLOAT_EQ( 0.0f,      fast::cos<float>(-M_PI_2));
    EXPECT_NEAR    (sqrt1_2f,   fast::cos<float>(-M_PI_4), abs_error);
    EXPECT_FLOAT_EQ( 1.0f,      fast::cos<float>(0.0));
    EXPECT_NEAR    (sqrt1_2f,   fast::cos<float>(M_PI_4), abs_error);
    EXPECT_FLOAT_EQ( 0.0f,      fast::cos<float>(M_PI_2));
    EXPECT_NEAR    (-sqrt1_2f,  fast::cos<float>(M_PI_2 + M_PI_4), abs_error);
    EXPECT_FLOAT_EQ(-1.0f,      fast::cos<float>(M_PI));

    EXPECT_FLOAT_EQ( 0.0f,      fast::sin<double>(-M_PI));
    EXPECT_NEAR    (-sqrt1_2d,  fast::sin<double>(-M_PI_2 - M_PI_4), abs_error);
    EXPECT_FLOAT_EQ(-1.0f,      fast::sin<double>(-M_PI_2));
    EXPECT_NEAR    (-sqrt1_2d,  fast::sin<double>(-M_PI_4), abs_error);
    EXPECT_FLOAT_EQ( 0.0f,      fast::sin<double>(0.0));
    EXPECT_NEAR    ( sqrt1_2d,  fast::sin<double>(M_PI_4), abs_error);
    EXPECT_FLOAT_EQ( 1.0f,      fast::sin<double>(M_PI_2));
    EXPECT_NEAR    ( sqrt1_2d,  fast::sin<double>(M_PI_2 + M_PI_4), abs_error);
    EXPECT_FLOAT_EQ( 0.0f,      fast::sin<double>(M_PI));

    EXPECT_FLOAT_EQ(-1.0f,      fast::cos<double>(-M_PI));
    EXPECT_NEAR    (-sqrt1_2d,  fast::cos<double>(-M_PI_2 - M_PI_4), abs_error);
    EXPECT_FLOAT_EQ( 0.0f,      fast::cos<double>(-M_PI_2));
    EXPECT_NEAR    (sqrt1_2d,   fast::cos<double>(-M_PI_4), abs_error);
    EXPECT_FLOAT_EQ( 1.0f,      fast::cos<double>(0.0));
    EXPECT_NEAR    (sqrt1_2d,   fast::cos<double>(M_PI_4), abs_error);
    EXPECT_FLOAT_EQ( 0.0f,      fast::cos<double>(M_PI_2));
    EXPECT_NEAR    (-sqrt1_2d,  fast::cos<double>(M_PI_2 + M_PI_4), abs_error);
    EXPECT_FLOAT_EQ(-1.0f,      fast::cos<double>(M_PI));
}
