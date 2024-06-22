#include "gmock/gmock.h"
#include "openclcpp-lite/utils.h"

namespace ocl = openclcpp_lite;

TEST(Utils, lo_lower)
{
    EXPECT_EQ(ocl::utils::to_lower("asdf"), "asdf");
    EXPECT_EQ(ocl::utils::to_lower("ASDF"), "asdf");
}

TEST(Utils, rtrim)
{
    EXPECT_EQ(ocl::utils::rtrim("asdf"), "asdf");
    EXPECT_EQ(ocl::utils::rtrim("as df "), "as df");
}

TEST(Utils, starts_with)
{
    EXPECT_TRUE(ocl::utils::starts_with("asdf", "asd"));
    EXPECT_FALSE(ocl::utils::starts_with("asdf", "zx"));
}

TEST(Utils, join)
{
    EXPECT_EQ(ocl::utils::join(" ", {}), "");
    EXPECT_EQ(ocl::utils::join(", ", { "one" }), "one");
    EXPECT_EQ(ocl::utils::join(", ", { "one", "two" }), "one, two");
}
