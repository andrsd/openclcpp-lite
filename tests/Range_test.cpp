#include "gmock/gmock.h"
#include "openclcpp-lite/range.h"

namespace ocl = openclcpp_lite;

TEST(RangeTest, empty)
{
    ocl::Range<1> rng;
    EXPECT_EQ(rng.dimensions(), 0);
    EXPECT_EQ(rng.size(), 0);
    EXPECT_EQ(rng.get()[0], 0);
}

TEST(RangeTest, one_d)
{
    ocl::Range<1> rng{ 10 };
    EXPECT_EQ(rng.dimensions(), 1);
    EXPECT_EQ(rng.size(), sizeof(size_t));
    EXPECT_EQ(rng.get()[0], 10);
}

TEST(RangeTest, two_d)
{
    ocl::Range<2> rng{ 10, 20 };
    EXPECT_EQ(rng.dimensions(), 2);
    EXPECT_EQ(rng.size(), 2 * sizeof(size_t));
    EXPECT_EQ(rng.get()[0], 10);
    EXPECT_EQ(rng.get()[1], 20);
}

TEST(RangeTest, three_d)
{
    ocl::Range<3> rng{ 10, 20, 30 };
    EXPECT_EQ(rng.dimensions(), 3);
    EXPECT_EQ(rng.size(), 3 * sizeof(size_t));
    EXPECT_EQ(rng.get()[0], 10);
    EXPECT_EQ(rng.get()[1], 20);
    EXPECT_EQ(rng.get()[2], 30);
}
