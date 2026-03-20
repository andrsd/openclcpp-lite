#include "gmock/gmock.h"
#include "openclcpp-lite/exception.h"

namespace ocl = openclcpp_lite;

TEST(ExceptionTest, test)
{
    try {
        throw ocl::Exception("asdf");
        FAIL();
    }
    catch (ocl::Exception & e) {
        std::string text(e.what());
        EXPECT_TRUE(text.ends_with("asdf"));
    }
}
