#include "gmock/gmock.h"
#include "openclcpp-lite/context.h"
#include "openclcpp-lite/queue.h"
#include "openclcpp-lite/event.h"
#include "openclcpp-lite/range.h"
#include "openclcpp-lite/buffer.h"

namespace ocl = openclcpp_lite;

TEST(EventTest, wait_for_event)
{
    const int N = 5;
    std::vector<int> h_a(N);
    for (int i = 0; i < N; i++) {
        h_a[i] = 100 + i;
    }

    ocl::Range<1> rng { N };
    ocl::Buffer<int, 1> d_a { rng };
    auto q = ocl::Queue::get_default();
    auto wr_evt1 = q.enqueue_iwrite(d_a, rng, h_a.data());
    ocl::wait_for_event(wr_evt1);
    EXPECT_EQ(wr_evt1.command_execution_status(), ocl::COMPLETE);
}

TEST(EventTest, wait_for_events)
{
    const int N = 5;
    auto ctx = ocl::Context::get_default();
    std::vector<int> h_a(N);
    std::vector<int> h_b(N);
    for (int i = 0; i < N; i++) {
        h_a[i] = 100 + i;
        h_b[i] = 100 - i;
    }

    ocl::Range<1> rng { N };
    ocl::Buffer<int, 1> d_a { rng };
    ocl::Buffer<int, 1> d_b { rng };

    ocl::Queue q(ctx);
    auto wr_evt1 = q.enqueue_iwrite(d_a, rng, h_a.data());
    auto wr_evt2 = q.enqueue_iwrite(d_b, rng, h_b.data());
    ocl::wait_for_events({ wr_evt1, wr_evt2 });
    EXPECT_EQ(wr_evt1.command_execution_status(), ocl::COMPLETE);
    EXPECT_EQ(wr_evt2.command_execution_status(), ocl::COMPLETE);
}
