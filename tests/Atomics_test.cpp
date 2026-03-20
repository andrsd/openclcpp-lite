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

std::string src_dbl = R"(
/// atomic add for `double`
void __attribute__((always_inline))
atomic_add_d(volatile global double* addr,
             const double val)
{
    union {
        ulong u64;
        double f64;
    } next, expected, current;
    current.f64 = *addr;
    do {
        expected.f64 = current.f64;
        next.f64 = expected.f64 + val;
        current.u64 = atomic_cmpxchg((volatile global ulong *) addr, expected.u64, next.u64);
    } while (current.u64 != expected.u64);
}

__kernel void
add_dbl(__global const int * connect,
        __global double * C)
{
    int ie = get_global_id(0);
    int3 elem = vload3(ie, connect);
    atomic_add_d(C + elem[0], 1);
    atomic_add_d(C + elem[1], 1);
    atomic_add_d(C + elem[2], 1);
}
)";

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
    using BufferConnect = ocl::Buffer<ELEM>;
    using BufferF = ocl::Buffer<cl_float>;

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

TEST(AtomicsTest, atomic_add_double)
{
    auto ctx = ocl::Context::get_default();
    auto q = ocl::Queue::get_default();
    auto dev = q.device();
    auto extensions = dev.extensions();
    if (!(extensions.contains("cl_khr_fp64") && extensions.contains("cl_khr_int64_base_atomics")))
        SUCCEED();

    const int N_ELEMS = 3;
    const int N_NODES = 5;
    std::vector<double> h_c(N_NODES);

    auto prg = ocl::Program::from_source(ctx, src_dbl);
    prg.build();

    using ELEM = std::array<cl_int, 3>;
    ocl::Range<1> rng_node { N_NODES };
    ocl::Range<1> rng_elem { N_ELEMS };
    using BufferConnect = ocl::Buffer<ELEM>;
    using BufferD = ocl::Buffer<cl_double>;

    std::vector<ELEM> connect = { { 0, 1, 2 }, { 2, 1, 3 }, { 1, 4, 3 } };

    BufferConnect d_connect { connect.data(), rng_elem };
    BufferD d_C { rng_node };

    auto add = ocl::Kernel::create<BufferConnect, BufferD>(prg, "add_dbl");
    q.enqueue_kernel(add(d_connect, d_C), rng_elem);

    auto * C = q.enqueue_map_buffer(d_C, true, ocl::READ, rng_node);
    EXPECT_DOUBLE_EQ(C[0], 1.);
    EXPECT_DOUBLE_EQ(C[1], 3.);
    EXPECT_DOUBLE_EQ(C[2], 2.);
    EXPECT_DOUBLE_EQ(C[3], 2.);
    EXPECT_DOUBLE_EQ(C[4], 1.);
}
