#include "gmock/gmock.h"
#include "openclcpp-lite/context.h"
#include "openclcpp-lite/program.h"

namespace ocl = openclcpp_lite;

namespace {

// clang-format off
std::string src1 =
    "__kernel void vector_add(__global const int *A, __global const int *B, __global int *C) {\n"
    "    int i = get_global_id(0);\n"
    "    C[i] = A[i] + B[i];\n"
    "}";

std::vector<std::string> src2 = {
    "__kernel void vec_add(__global const int *A, __global const int *B, __global int *C) {"
    "    int i = get_global_id(0);"
    "    C[i] = A[i] + B[i];"
    "}"
    "__kernel void vec_sub(__global const int *A, __global const int *B, __global int *C) {"
    "    int i = get_global_id(0);"
    "    C[i] = A[i] - B[i];"
    "}"
};
// clang-format on

} // namespace

TEST(ProgramTest, from_single_line)
{
    auto ctx = ocl::default_context();
    auto devs = ctx.devices();
    auto prg = ocl::Program::from_source(ctx, src1);
    prg.build();
    EXPECT_EQ(prg.num_of_kernels(), 1);
    EXPECT_THAT(prg.kernel_names(), testing::UnorderedElementsAre("vector_add"));
}

TEST(ProgramTest, from_lines)
{
    auto ctx = ocl::default_context();
    auto prg = ocl::Program::from_source(ctx, src2);
    prg.build();
    EXPECT_EQ(prg.num_of_kernels(), 2);
    EXPECT_THAT(prg.kernel_names(), testing::UnorderedElementsAre("vec_add", "vec_sub"));
}

TEST(ProgramTest, ref_cnt)
{
    auto ctx = ocl::default_context();
    auto prg = ocl::Program::from_source(ctx, src2);
    prg.build();
    EXPECT_EQ(prg.reference_count(), 1);
    prg.retain();
    EXPECT_EQ(prg.reference_count(), 2);
    prg.release();
    EXPECT_EQ(prg.reference_count(), 1);
    EXPECT_EQ(prg.context(), ctx);

    auto dev = prg.devices();
}
