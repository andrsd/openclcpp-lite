#include "gmock/gmock.h"
#include "openclcpp-lite/range.h"

namespace ocl = openclcpp_lite;

TEST(RangeTest, empty)
{
    ocl::Range<1> rng;
    EXPECT_EQ(rng.dimensions(), 0);
    EXPECT_EQ(rng.size(), 0);
}

TEST(RangeTest, one_d)
{
    ocl::Range<1> rng { 10 };
    EXPECT_EQ(rng.dimensions(), 1);
    EXPECT_EQ(rng.size(), 10);
}

TEST(RangeTest, two_d)
{
    ocl::Range<2> rng { 10, 20 };
    EXPECT_EQ(rng.dimensions(), 2);
    EXPECT_EQ(rng.size(), 200);
}

TEST(RangeTest, three_d)
{
    ocl::Range<3> rng { 10, 20, 30 };
    EXPECT_EQ(rng.dimensions(), 3);
    EXPECT_EQ(rng.size(), 6000);
}
