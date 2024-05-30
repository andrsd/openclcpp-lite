#include "gmock/gmock.h"
#include "openclcpp-lite/context.h"

namespace ocl = openclcpp_lite;

TEST(ContextTest, num_devices)
{
    auto ctx = ocl::Context::get_default();
    auto devices = ctx.devices();
    auto n_devs = ctx.num_of_devices();
    EXPECT_GE(n_devs, 1);
}

TEST(ContextTest, ref_cnt)
{
    auto ctx = ocl::Context::get_default();
    auto n_refs = ctx.reference_count();
    ctx.retain();
    EXPECT_EQ(ctx.reference_count(), n_refs + 1);
    ctx.release();
    EXPECT_EQ(ctx.reference_count(), n_refs);
}
