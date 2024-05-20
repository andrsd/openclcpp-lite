#include "gmock/gmock.h"
#include "openclcpp-lite/context.h"
#include "openclcpp-lite/queue.h"
#include "openclcpp-lite/event.h"

namespace ocl = openclcpp_lite;

TEST(EventTest, wait_for_event)
{
    const int N = 5;
    auto ctx = ocl::default_context();
    std::vector<int> h_a(N);
    for (int i = 0; i < N; i++) {
        h_a[i] = 100 + i;
    }

    auto d_a = ctx.alloc<int>(N);

    ocl::Queue q(ctx);
    auto wr_evt1 = q.enqueue_iwrite(d_a, 0, N, h_a.data());
    ocl::wait_for_event(wr_evt1);
    EXPECT_EQ(wr_evt1.command_execution_status(), ocl::COMPLETE);
}

TEST(EventTest, wait_for_events)
{
    const int N = 5;
    auto ctx = ocl::default_context();
    std::vector<int> h_a(N);
    std::vector<int> h_b(N);
    for (int i = 0; i < N; i++) {
        h_a[i] = 100 + i;
        h_b[i] = 100 - i;
    }

    auto d_a = ctx.alloc<int>(N);
    auto d_b = ctx.alloc<int>(N);

    ocl::Queue q(ctx);
    auto wr_evt1 = q.enqueue_iwrite(d_a, 0, N, h_a.data());
    auto wr_evt2 = q.enqueue_iwrite(d_b, 0, N, h_b.data());
    ocl::wait_for_events({ wr_evt1, wr_evt2 });
    EXPECT_EQ(wr_evt1.command_execution_status(), ocl::COMPLETE);
    EXPECT_EQ(wr_evt2.command_execution_status(), ocl::COMPLETE);
}
