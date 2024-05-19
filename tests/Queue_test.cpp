#include "gmock/gmock.h"
#include "openclcpp-lite/context.h"
#include "openclcpp-lite/queue.h"

namespace ocl = openclcpp_lite;

TEST(QueueTest, create)
{
    auto ctx = ocl::default_context();
    ocl::Queue q(ctx);
    EXPECT_EQ(q.context(), ctx);
    EXPECT_EQ(q.device(), ctx.devices()[0]);
}

TEST(QueueTest, ref_cnt)
{
    auto ctx = ocl::default_context();
    ocl::Queue q(ctx);
    EXPECT_EQ(q.reference_count(), 1);
    q.retain();
    EXPECT_EQ(q.reference_count(), 2);
    q.release();
    EXPECT_EQ(q.reference_count(), 1);
}
