#include "gmock/gmock.h"
#include "openclcpp-lite/utils.h"

namespace ocl = openclcpp_lite;

TEST(Utils, lo_lower)
{
    EXPECT_EQ(ocl::utils::to_lower("asdf"), "asdf");
    EXPECT_EQ(ocl::utils::to_lower("ASDF"), "asdf");
}
