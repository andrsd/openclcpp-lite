#include "gmock/gmock.h"
#include "openclcpp-lite/context.h"
#include "openclcpp-lite/buffer.h"
#include "openclcpp-lite/queue.h"

namespace ocl = openclcpp_lite;

TEST(BufferTest, ref_cnt)
{
    auto ctx = ocl::default_context();
    auto b_f = ctx.alloc<float>(10);

    EXPECT_EQ(b_f.reference_count(), 1);
    b_f.retain();
    EXPECT_EQ(b_f.reference_count(), 2);
    b_f.release();
    EXPECT_EQ(b_f.reference_count(), 1);
}

TEST(BufferTest, test)
{
    auto ctx = ocl::default_context();
    auto b_f = ctx.alloc<float>(10);
    EXPECT_EQ(b_f.context(), ctx);
    EXPECT_EQ(b_f.size(), sizeof(float) * 10);
    EXPECT_EQ(b_f.map_count(), 0);
    EXPECT_EQ(b_f.offset(), 0);
}
