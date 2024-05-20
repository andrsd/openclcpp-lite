#include "gmock/gmock.h"
#include "openclcpp-lite/ndrange.h"

namespace ocl = openclcpp_lite;

TEST(NDRangeTest, empty)
{
    ocl::NDRange rng;
    EXPECT_EQ(rng.dimensions(), 0);
    EXPECT_EQ(rng.size(), 0);
    EXPECT_EQ(rng.get()[0], 0);
    EXPECT_EQ(rng.get()[1], 0);
    EXPECT_EQ(rng.get()[2], 0);
}

TEST(NDRangeTest, one_d)
{
    ocl::NDRange rng(10);
    EXPECT_EQ(rng.dimensions(), 1);
    EXPECT_EQ(rng.size(), sizeof(size_t));
    EXPECT_EQ(rng.get()[0], 10);
    EXPECT_EQ(rng.get()[1], 1);
    EXPECT_EQ(rng.get()[2], 1);
}

TEST(NDRangeTest, two_d)
{
    ocl::NDRange rng(10, 20);
    EXPECT_EQ(rng.dimensions(), 2);
    EXPECT_EQ(rng.size(), 2 * sizeof(size_t));
    EXPECT_EQ(rng.get()[0], 10);
    EXPECT_EQ(rng.get()[1], 20);
    EXPECT_EQ(rng.get()[2], 1);
}

TEST(NDRangeTest, three_d)
{
    ocl::NDRange rng(10, 20, 30);
    EXPECT_EQ(rng.dimensions(), 3);
    EXPECT_EQ(rng.size(), 3 * sizeof(size_t));
    EXPECT_EQ(rng.get()[0], 10);
    EXPECT_EQ(rng.get()[1], 20);
    EXPECT_EQ(rng.get()[2], 30);
}
