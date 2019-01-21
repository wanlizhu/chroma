#include <math.h>
#include <gtest/gtest.h>
#include <math/half.h>

using namespace math;

class HalfTest : public testing::Test {
protected:
};

TEST_F(HalfTest, Basic) {
    half h(0.1234567890123f);
    float f = (float) h;
    half h2(f);
 
    EXPECT_FLOAT_EQ(h, h2);

    h = NAN;
    EXPECT_TRUE(std::isnan((float) h));

    h = INFINITY;
    EXPECT_TRUE(std::isinf((float) h));
}
