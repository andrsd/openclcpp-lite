#include "gmock/gmock.h"
#include "openclcpp-lite/platform.h"
#include "openclcpp-lite/context.h"
#include "openclcpp-lite/program.h"
#include <future>

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

const char * src3[] = {
    "__kernel void vec_sub(__global const int *A, __global const int *B, __global int *C) {",
    "    int i = get_global_id(0);",
    "    C[i] = A[i] - B[i];",
    "}"
};
// clang-format on

int done = false;

void
notify0(cl_program program, void * user_data)
{
    done = true;
}

int
wait_thread(const std::atomic_bool & cancelled)
{
    while (!done && !cancelled)
        usleep(1000);
    return cancelled ? 1 : 0;
}

} // namespace

TEST(ProgramTest, from_single_line_default_context)
{
    auto prg = ocl::Program::from_source(src1);
    prg.build();
    EXPECT_EQ(prg.num_of_kernels(), 1);
    EXPECT_THAT(prg.kernel_names(), testing::UnorderedElementsAre("vector_add"));
}

TEST(ProgramTest, from_single_line)
{
    auto ctx = ocl::Context::get_default();
    auto devs = ctx.devices();
    auto prg = ocl::Program::from_source(ctx, src1);
    prg.build();
    EXPECT_EQ(prg.num_of_kernels(), 1);
    EXPECT_THAT(prg.kernel_names(), testing::UnorderedElementsAre("vector_add"));
}

TEST(ProgramTest, from_lines)
{
    auto ctx = ocl::Context::get_default();
    auto prg = ocl::Program::from_source(ctx, src2);
    prg.build();
    EXPECT_EQ(prg.num_of_kernels(), 2);
    EXPECT_THAT(prg.kernel_names(), testing::UnorderedElementsAre("vec_add", "vec_sub"));
}

TEST(ProgramTest, from_lines_w_callback)
{
    auto ctx = ocl::Context::get_default();
    auto prg = ocl::Program::from_source(ctx, src2);

    std::atomic_bool cancel = ATOMIC_VAR_INIT(false);
    done = false;
    auto fut = std::async(std::launch::async, wait_thread, std::ref(cancel));
    prg.build({}, notify0, &done);
    auto ret = fut.wait_for(std::chrono::seconds(1));
    if (ret == std::future_status::ready) {
        EXPECT_TRUE(done);
        EXPECT_EQ(prg.num_of_kernels(), 2);
        EXPECT_THAT(prg.kernel_names(), testing::UnorderedElementsAre("vec_add", "vec_sub"));
    }
    else {
        cancel = true;
        FAIL();
    }
}

TEST(ProgramTest, from_const_char)
{
    auto ctx = ocl::Context::get_default();
    auto prg = ocl::Program::from_source(ctx, 4, src3);
    prg.build();
    EXPECT_EQ(prg.num_of_kernels(), 1);
    EXPECT_THAT(prg.kernel_names(), testing::UnorderedElementsAre("vec_sub"));
}

TEST(ProgramTest, ref_cnt)
{
    auto ctx = ocl::Context::get_default();
    auto prg = ocl::Program::from_source(ctx, src2);
    prg.build();
    auto n_refs = prg.reference_count();
    prg.retain();
    EXPECT_EQ(prg.reference_count(), n_refs + 1);
    prg.release();
    EXPECT_EQ(prg.reference_count(), n_refs);
    EXPECT_EQ(prg.context(), ctx);

    auto dev = prg.devices();
    EXPECT_GE(dev.size(), 1);
}

TEST(ProgramTest, build_info)
{
    auto platform = ocl::Platform::get_default();
    auto dev = platform.devices()[0];
    ocl::Context ctx({ dev });
    auto prg = ocl::Program::from_source(ctx, src2);
    prg.build();
    EXPECT_EQ(prg.build_status(dev), ocl::Program::BuildStatus::SUCCESS);
    auto nfo = prg.build_log(dev);
    EXPECT_EQ(nfo.size(), 0);
}
