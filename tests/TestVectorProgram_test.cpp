#include "gmock/gmock.h"
#include "test_vector_program.cl.h"
#include "openclcpp-lite/program.h"
#include "openclcpp-lite/queue.h"
#include "openclcpp-lite/buffer.h"
#include "openclcpp-lite/kernel_functor.h"

namespace ocl = openclcpp_lite;

TEST(TestVectorProgram, test)
{
    TestVectorProgram prg;
    prg.build();
    EXPECT_EQ(prg.num_of_kernels(), 2);

    const int N = 10;
    std::vector<float> h_a(N);
    std::vector<float> h_b(N);
    std::vector<float> h_c(N);
    for (int i = 0; i < N; i++) {
        h_a[i] = i + 1;
        h_b[i] = 100 - i;
    }

    ocl::Range<1> rng { N };
    using FloatBuffer = ocl::Buffer<float, 1>;

    FloatBuffer d_a { h_a.data(), rng };
    FloatBuffer d_b { h_b.data(), rng };
    FloatBuffer d_c { rng };

    auto vec_add = ocl::Kernel::create<FloatBuffer, FloatBuffer, FloatBuffer>(prg, "vec_add");

    auto q = ocl::Queue::get_default();
    q.enqueue_kernel(vec_add(d_a, d_b, d_c), rng);
    q.enqueue_read(d_c, rng, h_c.data());
    for (auto & i : h_c) {
        EXPECT_FLOAT_EQ(i, 101);
    }
}
