#include "gmock/gmock.h"
#include "openclcpp-lite/context.h"
#include "openclcpp-lite/queue.h"
#include "openclcpp-lite/event.h"
#include "openclcpp-lite/enums.h"
#include "openclcpp-lite/range.h"
#include "openclcpp-lite/buffer.h"

namespace ocl = openclcpp_lite;

TEST(QueueTest, create)
{
    auto ctx = ocl::Context::get_default();
    ocl::Queue q(ctx);
    EXPECT_EQ(q.context(), ctx);
    EXPECT_EQ(q.device(), ctx.devices()[0]);
}

TEST(QueueTest, ref_cnt)
{
    auto q = ocl::Queue::get_default();
    auto n_refs = q.reference_count();
    q.retain();
    EXPECT_EQ(q.reference_count(), n_refs + 1);
    q.release();
    EXPECT_EQ(q.reference_count(), n_refs);
}

TEST(QueueTest, read_write_buffer_winit)
{
    const int N = 5;
    std::vector<int> h_a(N);
    for (int i = 0; i < N; i++)
        h_a[i] = 100 + i;
    ocl::Range<1> rng { N };
    ocl::Buffer<int, 1> d_a { h_a.data(), rng };
    std::vector<int> h_b(N);
    auto q = ocl::Queue::get_default();
    q.enqueue_read(d_a, rng, h_b.data());

    EXPECT_EQ(h_b[0], 100);
    EXPECT_EQ(h_b[1], 101);
    EXPECT_EQ(h_b[2], 102);
    EXPECT_EQ(h_b[3], 103);
    EXPECT_EQ(h_b[4], 104);
}

TEST(QueueTest, read_write_buffer)
{
    const int N = 5;
    std::vector<int> h_a(N);
    for (int i = 0; i < N; i++)
        h_a[i] = 100 + i;
    ocl::Range<1> rng { N };
    ocl::Buffer<int, 1> d_a { rng };
    std::vector<int> h_b(N);
    auto q = ocl::Queue::get_default();
    q.enqueue_write(d_a, rng, h_a.data());
    q.enqueue_read(d_a, rng, h_b.data());

    EXPECT_EQ(h_b[0], 100);
    EXPECT_EQ(h_b[1], 101);
    EXPECT_EQ(h_b[2], 102);
    EXPECT_EQ(h_b[3], 103);
    EXPECT_EQ(h_b[4], 104);
}

TEST(QueueTest, copy_buffer)
{
    const int N = 5;
    std::vector<int> h_a(N);
    for (int i = 0; i < N; i++)
        h_a[i] = 100 + i;
    std::vector<int> h_b(N);
    ocl::Range<1> rng { N };
    ocl::Buffer<int, 1> d_a { rng };
    ocl::Buffer<int, 1> d_b { rng };
    auto q = ocl::Queue::get_default();
    q.enqueue_write(d_a, rng, h_a.data());
    q.enqueue_copy(d_a, d_b, rng);
    q.enqueue_read(d_b, rng, h_b.data());

    EXPECT_EQ(h_b[0], 100);
    EXPECT_EQ(h_b[1], 101);
    EXPECT_EQ(h_b[2], 102);
    EXPECT_EQ(h_b[3], 103);
    EXPECT_EQ(h_b[4], 104);
}

TEST(QueueTest, map_buffer)
{
    const int N = 5;
    std::vector<int> h_a(N);
    for (int i = 0; i < N; i++)
        h_a[i] = 100 + i;
    ocl::Range<1> rng { N };
    auto q = ocl::Queue::get_default();
    ocl::Buffer<int, 1> d_a { rng };
    q.enqueue_write(d_a, rng, h_a.data());
    auto * mapped_ints = q.enqueue_map_buffer(d_a, true, ocl::READ, rng);

    EXPECT_EQ(d_a.map_count(), 1);
    EXPECT_EQ(mapped_ints[0], 100);
    EXPECT_EQ(mapped_ints[1], 101);
    EXPECT_EQ(mapped_ints[2], 102);
    EXPECT_EQ(mapped_ints[3], 103);
    EXPECT_EQ(mapped_ints[4], 104);
}

TEST(QueueTest, iread_iwrite_buffer)
{
    const int N = 5;
    std::vector<int> h_a(N);
    for (int i = 0; i < N; i++)
        h_a[i] = 100 + i;
    std::vector<int> h_b(N);
    ocl::Range<1> rng { N };
    ocl::Buffer<int, 1> d_a { rng };
    auto q = ocl::Queue::get_default();
    auto wr_evt = q.enqueue_iwrite(d_a, rng, h_a.data());
    auto rd_evt = q.enqueue_iread(d_a, rng, h_b.data(), { wr_evt });
    rd_evt.wait();

    EXPECT_EQ(rd_evt.command_execution_status(), ocl::COMPLETE);
    EXPECT_EQ(h_b[0], 100);
    EXPECT_EQ(h_b[1], 101);
    EXPECT_EQ(h_b[2], 102);
    EXPECT_EQ(h_b[3], 103);
    EXPECT_EQ(h_b[4], 104);
}

TEST(QueueTest, async_copy_buffer)
{
    const int N = 5;
    std::vector<int> h_a(N);
    for (int i = 0; i < N; i++)
        h_a[i] = 100 + i;
    std::vector<int> h_b(N);
    ocl::Range<1> rng { N };
    ocl::Buffer<int, 1> d_a { rng };
    ocl::Buffer<int, 1> d_b { rng };
    auto q = ocl::Queue::get_default();
    auto wr_evt = q.enqueue_iwrite(d_a, rng, h_a.data());
    auto cp_evt = q.enqueue_copy(d_a, d_b, rng, { wr_evt });
    auto rd_evt = q.enqueue_iread(d_b, rng, h_b.data(), { cp_evt });
    rd_evt.wait();

    EXPECT_EQ(rd_evt.command_execution_status(), ocl::COMPLETE);
    EXPECT_EQ(h_b[0], 100);
    EXPECT_EQ(h_b[1], 101);
    EXPECT_EQ(h_b[2], 102);
    EXPECT_EQ(h_b[3], 103);
    EXPECT_EQ(h_b[4], 104);
}

TEST(QueueTest, barrier)
{
    const int N = 5;
    std::vector<int> h_a(N);
    std::vector<int> h_b(N);
    for (int i = 0; i < N; i++) {
        h_a[i] = 100 + i;
        h_b[i] = 100 - i;
    }
    ocl::Range<1> rng { N };
    ocl::Buffer<int, 1> d_a { rng };
    ocl::Buffer<int, 1> d_b { rng };
    auto q = ocl::Queue::get_default();
    auto wr_evt1 = q.enqueue_iwrite(d_a, rng, h_a.data());
    auto wr_evt2 = q.enqueue_iwrite(d_b, rng, h_b.data());
    auto brr_evt = q.enqueue_barrier({ wr_evt1, wr_evt2 });
    brr_evt.wait();

    EXPECT_EQ(brr_evt.command_execution_status(), ocl::COMPLETE);
    EXPECT_EQ(wr_evt2.command_execution_status(), ocl::COMPLETE);
    EXPECT_EQ(wr_evt2.command_execution_status(), ocl::COMPLETE);
}

TEST(QueueTest, marker)
{
    const int N = 5;
    std::vector<int> h_a(N);
    std::vector<int> h_b(N);
    for (int i = 0; i < N; i++) {
        h_a[i] = 100 + i;
        h_b[i] = 100 - i;
    }
    ocl::Range<1> rng { N };
    ocl::Buffer<int, 1> d_a { rng };
    ocl::Buffer<int, 1> d_b { rng };
    auto q = ocl::Queue::get_default();
    auto wr_evt1 = q.enqueue_iwrite(d_a, rng, h_a.data());
    auto wr_evt2 = q.enqueue_iwrite(d_b, rng, h_b.data());
    auto mrk_evt = q.enqueue_marker({ wr_evt1, wr_evt2 });
    mrk_evt.wait();

    EXPECT_EQ(mrk_evt.command_execution_status(), ocl::COMPLETE);
    EXPECT_EQ(wr_evt2.command_execution_status(), ocl::COMPLETE);
    EXPECT_EQ(wr_evt2.command_execution_status(), ocl::COMPLETE);
}

TEST(QueueTest, fill_buffer_scalar)
{
    const int N = 3;
    ocl::Range<1> rng { N };
    ocl::Buffer<int, 1> d_a { rng };
    auto q = ocl::Queue::get_default();
    q.enqueue_fill_buffer(d_a, 1234, rng);

    auto * vals = q.enqueue_map_buffer(d_a, true, ocl::READ, rng);
    EXPECT_EQ(vals[0], 1234);
    EXPECT_EQ(vals[1], 1234);
    EXPECT_EQ(vals[2], 1234);
}

TEST(QueueTest, fill_buffer_pattern)
{
    struct Pattern {
        float a;
        float b;
    };
    Pattern pattern { 1., 2. };

    const int N = 3;
    ocl::Range<1> rng { N };
    ocl::Buffer<Pattern, 1> d_a { rng };
    auto q = ocl::Queue::get_default();
    ocl::Range<1> fill_rng { 2 };
    q.enqueue_fill_buffer(d_a, pattern, fill_rng);

    auto * vals = q.enqueue_map_buffer(d_a, true, ocl::READ, rng);
    EXPECT_EQ(vals[0].a, 1.);
    EXPECT_EQ(vals[0].b, 2.);
    EXPECT_EQ(vals[1].a, 1.);
    EXPECT_EQ(vals[1].b, 2.);
    EXPECT_EQ(vals[2].a, 0.);
    EXPECT_EQ(vals[2].b, 0.);
}
