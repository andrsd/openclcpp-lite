#include "gmock/gmock.h"
#include "openclcpp-lite/openclcpp-lite.h"

namespace ocl = openclcpp_lite;

TEST(ContextTest, default_contex)
{
    auto ctx = ocl::default_context();
    EXPECT_EQ(ctx.reference_count(), 1);
    auto devices = ctx.devices();
    auto n_devs = ctx.num_of_devices();
}

TEST(ContextTest, ref_cnt)
{
    auto ctx = ocl::default_context();
    EXPECT_EQ(ctx.reference_count(), 1);
    ctx.retain();
    EXPECT_EQ(ctx.reference_count(), 2);
    ctx.release();
    EXPECT_EQ(ctx.reference_count(), 1);
}
