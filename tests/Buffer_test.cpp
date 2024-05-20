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

TEST(BufferTest, read_write_buffer)
{
    const int N = 5;
    auto ctx = ocl::default_context();
    std::vector<int> h_a(N);
    for (int i = 0; i < N; i++)
        h_a[i] = 100 + i;
    auto d_a = ctx.alloc<int>(N);
    std::vector<int> h_b(N);

    ocl::Queue q(ctx);
    q.enqueue_write_buffer(d_a, true, 0, N * sizeof(int), h_a.data());
    q.enqueue_read_buffer(d_a, true, 0, N * sizeof(int), h_b.data());

    EXPECT_EQ(h_b[0], 100);
    EXPECT_EQ(h_b[1], 101);
    EXPECT_EQ(h_b[2], 102);
    EXPECT_EQ(h_b[3], 103);
    EXPECT_EQ(h_b[4], 104);
}

TEST(BufferTest, copy_buffer)
{
    const int N = 5;
    auto ctx = ocl::default_context();
    std::vector<int> h_a(N);
    for (int i = 0; i < N; i++)
        h_a[i] = 100 + i;
    auto d_a = ctx.alloc<int>(N);
    auto d_b = ctx.alloc<int>(N);
    std::vector<int> h_b(N);

    ocl::Queue q(ctx);
    q.enqueue_write_buffer(d_a, true, 0, N * sizeof(int), h_a.data());
    q.enqueue_copy_buffer(d_a, d_b, 0, 0, N * sizeof(int));
    q.enqueue_read_buffer(d_b, true, 0, N * sizeof(int), h_b.data());

    EXPECT_EQ(h_b[0], 100);
    EXPECT_EQ(h_b[1], 101);
    EXPECT_EQ(h_b[2], 102);
    EXPECT_EQ(h_b[3], 103);
    EXPECT_EQ(h_b[4], 104);
}
