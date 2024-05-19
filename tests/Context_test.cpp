#include "gmock/gmock.h"
#include "openclcpp-lite/context.h"

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

TEST(ContextTest, alloc_buffer)
{
    auto ctx = ocl::default_context();
    auto b_f = ctx.alloc<float>(10);
    auto b_d = ctx.alloc<double>(10);

    EXPECT_THROW({ctx.alloc<int>(10);}, ocl::Exception);
}
