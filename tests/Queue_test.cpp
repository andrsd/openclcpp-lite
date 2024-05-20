#include "gmock/gmock.h"
#include "openclcpp-lite/context.h"
#include "openclcpp-lite/queue.h"
#include "openclcpp-lite/event.h"
#include "openclcpp-lite/enums.h"

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
    q.enqueue_write(d_a, 0, N * sizeof(int), h_a.data());
    q.enqueue_read(d_a, 0, N * sizeof(int), h_b.data());

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
    q.enqueue_write(d_a, 0, N * sizeof(int), h_a.data());
    q.enqueue_copy(d_a, d_b, 0, 0, N * sizeof(int));
    q.enqueue_read(d_b, 0, N * sizeof(int), h_b.data());

    EXPECT_EQ(h_b[0], 100);
    EXPECT_EQ(h_b[1], 101);
    EXPECT_EQ(h_b[2], 102);
    EXPECT_EQ(h_b[3], 103);
    EXPECT_EQ(h_b[4], 104);
}

TEST(BufferTest, map_buffer)
{
    const int N = 5;
    auto ctx = ocl::default_context();
    std::vector<int> h_a(N);
    for (int i = 0; i < N; i++)
        h_a[i] = 100 + i;
    ocl::Queue q(ctx);
    auto d_a = ctx.alloc<int>(N);
    q.enqueue_write(d_a, 0, N * sizeof(int), h_a.data());
    auto * mapped_ints = q.enqueue_map_buffer<int>(d_a, true, ocl::READ, 0, N);
    EXPECT_EQ(d_a.map_count(), 1);
    EXPECT_EQ(mapped_ints[0], 100);
    EXPECT_EQ(mapped_ints[1], 101);
    EXPECT_EQ(mapped_ints[2], 102);
    EXPECT_EQ(mapped_ints[3], 103);
    EXPECT_EQ(mapped_ints[4], 104);
}

TEST(BufferTest, iread_iwrite_buffer)
{
    const int N = 5;
    auto ctx = ocl::default_context();
    std::vector<int> h_a(N);
    for (int i = 0; i < N; i++)
        h_a[i] = 100 + i;
    auto d_a = ctx.alloc<int>(N);
    std::vector<int> h_b(N);

    ocl::Queue q(ctx);
    auto wr_evt = q.enqueue_iwrite(d_a, 0, N * sizeof(int), h_a.data());
    auto rd_evt = q.enqueue_iread(d_a, 0, N * sizeof(int), h_b.data(), { wr_evt });
    rd_evt.wait();
    EXPECT_EQ(rd_evt.command_execution_status(), ocl::COMPLETE);

    EXPECT_EQ(h_b[0], 100);
    EXPECT_EQ(h_b[1], 101);
    EXPECT_EQ(h_b[2], 102);
    EXPECT_EQ(h_b[3], 103);
    EXPECT_EQ(h_b[4], 104);
}

TEST(BufferTest, async_copy_buffer)
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
    auto wr_evt = q.enqueue_iwrite(d_a, 0, N * sizeof(int), h_a.data());
    auto cp_evt = q.enqueue_copy(d_a, d_b, 0, 0, N * sizeof(int), { wr_evt });
    auto rd_evt = q.enqueue_iread(d_b, 0, N * sizeof(int), h_b.data(), { cp_evt });
    rd_evt.wait();
    EXPECT_EQ(rd_evt.command_execution_status(), ocl::COMPLETE);

    EXPECT_EQ(h_b[0], 100);
    EXPECT_EQ(h_b[1], 101);
    EXPECT_EQ(h_b[2], 102);
    EXPECT_EQ(h_b[3], 103);
    EXPECT_EQ(h_b[4], 104);
}

TEST(EventTest, barrier)
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
    auto wr_evt1 = q.enqueue_iwrite(d_a, 0, N * sizeof(int), h_a.data());
    auto wr_evt2 = q.enqueue_iwrite(d_b, 0, N * sizeof(int), h_b.data());
    auto brr_evt = q.enqueue_barrier({ wr_evt1, wr_evt2 });
    brr_evt.wait();
    EXPECT_EQ(brr_evt.command_execution_status(), ocl::COMPLETE);
    EXPECT_EQ(wr_evt2.command_execution_status(), ocl::COMPLETE);
    EXPECT_EQ(wr_evt2.command_execution_status(), ocl::COMPLETE);
}
