#include "gmock/gmock.h"
#include "openclcpp-lite/context.h"
#include "openclcpp-lite/queue.h"
#include "openclcpp-lite/program.h"
#include "openclcpp-lite/kernel.h"
#include "openclcpp-lite/event.h"
#include "openclcpp-lite/ndrange.h"

namespace ocl = openclcpp_lite;

namespace {

// clang-format off
std::string src1 =
    "__kernel void vec_add(__global const float *A, __global const float *B, __global float *C) {\n"
    "    int i = get_global_id(0);\n"
    "    C[i] = A[i] + B[i];\n"
    "}";

}

TEST(KernelTest, create)
{
    auto ctx = ocl::default_context();
    ocl::Queue q(ctx);

    auto prg = ocl::Program::from_source(ctx, src1);
    prg.build();

    ocl::Kernel k(prg, "vec_add");
    EXPECT_EQ(k.reference_count(), 1);
    EXPECT_EQ(k.function_name(), "vec_add");
    EXPECT_EQ(k.num_of_args(), 3);
    EXPECT_EQ(k.context(), ctx);
    EXPECT_EQ(k.program(), prg);
    EXPECT_EQ(k.attributes().size(), 0);
}

TEST(KernelTest, execute)
{
    const int N = 10;
    std::vector<float> h_a(N);
    std::vector<float> h_b(N);
    std::vector<float> h_c(N);

    auto ctx = ocl::default_context();
    ocl::Queue q(ctx);

    auto prg = ocl::Program::from_source(ctx, src1);
    prg.build();

    for (int i = 0; i < N; i++) {
        h_a[i] = i + 1;
        h_b[i] = 100 - i;
    }

    auto d_a = ctx.alloc<float>(N);
    auto d_b = ctx.alloc<float>(N);
    auto d_c = ctx.alloc<float>(N);

    ocl::Kernel k(prg, "vec_add");
    k.set_arg(0, d_a);
    k.set_arg(1, d_b);
    k.set_arg(2, d_c);

    q.enqueue_write(d_a, 0, N * sizeof(float), h_a.data());
    q.enqueue_write(d_b, 0, N * sizeof(float), h_b.data());
    q.enqueue_kernel(k, ocl::NDRange(N));
    q.enqueue_read(d_c, 0, N * sizeof(float), h_c.data());
    for (auto & i : h_c) {
        EXPECT_FLOAT_EQ(i, 101);
    }
}
