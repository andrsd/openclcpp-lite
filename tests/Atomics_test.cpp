#include "gmock/gmock.h"
#include "openclcpp-lite/platform.h"
#include "openclcpp-lite/context.h"
#include "openclcpp-lite/queue.h"
#include "openclcpp-lite/program.h"
#include "openclcpp-lite/kernel.h"
#include "openclcpp-lite/kernel_functor.h"
#include "openclcpp-lite/range.h"
#include "openclcpp-lite/buffer.h"

namespace ocl = openclcpp_lite;

namespace {

// From https://streamhpc.com/blog/2016-02-09/atomic-operations-for-floats-in-opencl-improved/

// clang-format off
std::string src =
    "void __attribute__((always_inline))\n"
    "atomic_add_f(volatile global float* addr,\n"
    "             const float val)\n"
    "{\n"
    "    union {\n"
    "        uint u32;\n"
    "        float f32;\n"
    "    } next, expected, current;\n"
    "    current.f32 = *addr;\n"
    "    do {\n"
    "        expected.f32 = current.f32;\n"
    "        next.f32 = expected.f32 + val;\n"
    "        current.u32 = atomic_cmpxchg((volatile global uint *) addr, expected.u32, next.u32);\n"
    "    } while (current.u32 != expected.u32);\n"
    "}\n"
    "\n"
    "__kernel void\n"
    "add(__global const int * connect,\n"
    "    __global float * C)\n"
    "{\n"
    "    int ie = get_global_id(0);\n"
    "    int3 elem = vload3(ie, connect);\n"
    "    atomic_add_f(C + elem[0], 1);\n"
    "    atomic_add_f(C + elem[1], 1);\n"
    "    atomic_add_f(C + elem[2], 1);\n"
    "}";
// clang-format on

} // namespace

TEST(AtomicsTest, atomic_add_float)
{
    const int N_ELEMS = 3;
    const int N_NODES = 5;
    std::vector<float> h_c(N_NODES);

    auto ctx = ocl::Context::get_default();
    auto q = ocl::Queue::get_default();

    auto prg = ocl::Program::from_source(ctx, src);
    prg.build();

    using ELEM = std::array<cl_int, 3>;
    ocl::Range<1> rng_node { N_NODES };
    ocl::Range<1> rng_elem { N_ELEMS };
    using BufferConnect = ocl::Buffer<ELEM, 1>;
    using BufferF = ocl::Buffer<cl_float, 1>;

    std::vector<ELEM> connect = { { 0, 1, 2 }, { 2, 1, 3 }, { 1, 4, 3 } };

    BufferConnect d_connect { connect.data(), rng_elem };
    BufferF d_C { rng_node };

    auto add = ocl::Kernel::create<BufferConnect, BufferF>(prg, "add");
    q.enqueue_kernel(add(d_connect, d_C), rng_elem);

    auto * C = q.enqueue_map_buffer(d_C, true, ocl::READ, rng_node);
    EXPECT_DOUBLE_EQ(C[0], 1.);
    EXPECT_DOUBLE_EQ(C[1], 3.);
    EXPECT_DOUBLE_EQ(C[2], 2.);
    EXPECT_DOUBLE_EQ(C[3], 2.);
    EXPECT_DOUBLE_EQ(C[4], 1.);
}
