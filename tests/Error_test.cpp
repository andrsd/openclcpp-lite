#include "gmock/gmock.h"
#include "openclcpp-lite/openclcpp-lite.h"

namespace ocl = openclcpp_lite;

TEST(ErrorTest, test)
{
    EXPECT_THROW({ OPENCL_CHECK(CL_INVALID_VALUE); }, ocl::Exception);
}
