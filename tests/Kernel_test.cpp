#include "gmock/gmock.h"
#include "openclcpp-lite/context.h"
#include "openclcpp-lite/queue.h"
#include "openclcpp-lite/program.h"
#include "openclcpp-lite/kernel.h"
#include "openclcpp-lite/range.h"
#include "openclcpp-lite/buffer.h"

namespace ocl = openclcpp_lite;

namespace {

// clang-format off
std::string src1 = R"(
__kernel void
vec_add(__global const float *A,
        __global const float *B,
        __global float *C)
{
    int i = get_global_id(0);
    C[i] = A[i] + B[i];
}
)";

}

TEST(KernelTest, create)
{
    auto ctx = ocl::Context::get_default();

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

    auto ctx = ocl::Context::get_default();
    auto q = ocl::Queue::get_default();

    auto prg = ocl::Program::from_source(ctx, src1);
    prg.build();

    for (int i = 0; i < N; i++) {
        h_a[i] = i + 1;
        h_b[i] = 100 - i;
    }

    ocl::Range<1> rng { N };
    ocl::Buffer<float> d_a { rng };
    ocl::Buffer<float> d_b { rng };
    ocl::Buffer<float> d_c { rng };

    ocl::Kernel k(prg, "vec_add");
    k.set_arg(0, d_a);
    k.set_arg(1, d_b);
    k.set_arg(2, d_c);

    q.submit([&](auto & h){
        h.copy(h_a.data(), d_a, rng);
        h.copy(h_b.data(), d_b, rng);
        h.kernel(k, rng);
        h.copy(d_c, h_c.data(), rng);
    });
    for (auto & i : h_c) {
        EXPECT_FLOAT_EQ(i, 101);
    }
}
