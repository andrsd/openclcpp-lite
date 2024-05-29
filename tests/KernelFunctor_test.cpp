#include "gmock/gmock.h"
#include "openclcpp-lite/context.h"
#include "openclcpp-lite/queue.h"
#include "openclcpp-lite/program.h"
#include "openclcpp-lite/kernel.h"
#include "openclcpp-lite/kernel_functor.h"
#include "openclcpp-lite/range.h"
#include "openclcpp-lite/buffer.h"

namespace ocl = openclcpp_lite;

namespace {

// clang-format off
std::string src1 =
    "__kernel void vec_add(__global const float *A, __global const float *B, __global float *C) {\n"
    "    int i = get_global_id(0);\n"
    "    C[i] = A[i] + B[i];\n"
    "}";

}

TEST(KernelTest, execute_functor)
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
    using FloatBuffer = ocl::Buffer<float, 1>;

    FloatBuffer d_a{h_a.data(), rng };
    FloatBuffer d_b{h_b.data(), rng};
    FloatBuffer d_c { rng };

    auto vec_add = ocl::Kernel::create<FloatBuffer, FloatBuffer, FloatBuffer>(prg, "vec_add");

    q.enqueue_write(d_a, rng, h_a.data());
    q.enqueue_write(d_b, rng, h_b.data());
    q.enqueue_kernel(vec_add(d_a, d_b, d_c), rng);
    q.enqueue_read(d_c, rng, h_c.data());
    for (auto & i : h_c) {
        EXPECT_FLOAT_EQ(i, 101);
    }
}
