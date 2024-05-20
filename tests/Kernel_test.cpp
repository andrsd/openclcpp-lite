#include "gmock/gmock.h"
#include "openclcpp-lite/context.h"
#include "openclcpp-lite/queue.h"
#include "openclcpp-lite/program.h"
#include "openclcpp-lite/kernel.h"

namespace ocl = openclcpp_lite;

namespace {

// clang-format off
std::string src1 =
    "__kernel void vec_add(__global const int *A, __global const int *B, __global int *C) {\n"
    "    int i = get_global_id(0);\n"
    "    C[i] = A[i] + B[i];\n"
    "}";
}

TEST(KernelTest, create)
{
    const int N = 10;
    std::vector<float> h_a(N);
    std::vector<float> h_b(N);
    std::vector<float> h_c(N);

    auto ctx = ocl::default_context();
    ocl::Queue q(ctx);

    auto prg = ocl::Program::from_source(ctx, src1);
    prg.build();
    //auto v_add = ocl::Kernel::create<ocl::Buffer, ocl::Buffer, ocl::Buffer, int>(prg, "vector_add");
    //auto vadd<ocl::Buffer, ocl::Buffer, ocl::Buffer, int>(prg, "vector_add");

    auto vec_add = ocl::Kernel::create<ocl::Buffer, ocl::Buffer, ocl::Buffer, int>(prg, "vec_add");


    auto d_a = ctx.alloc<float>(N);//, READ_ONLY);
    auto d_b = ctx.alloc<float>(N);//, READ_ONLY);
    auto d_c = ctx.alloc<float>(N);

    // q.enqueue(vadd, d_a, d_b, d_c, count);
    // q.copy(d_c, h_c);
}
