// SPDX-FileCopyrightText: 2024 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#include "openclcpp-lite/kernel.h"
#include "openclcpp-lite/context.h"
#include "openclcpp-lite/program.h"
#include "openclcpp-lite/utils.h"

namespace openclcpp_lite {

Kernel::Kernel() {}

Kernel::Kernel(const Program & program, const std::string & kernel_name)
{
    cl_int err;
    this->kern = clCreateKernel(program, kernel_name.c_str(), &err);
    OPENCL_CHECK(err);
}

Kernel::Kernel(cl_kernel kernel) : kern(kernel) {}

std::string
Kernel::function_name() const
{
    return get_info<std::string>(CL_KERNEL_FUNCTION_NAME);
}

unsigned int
Kernel::num_of_args() const
{
    return get_info<cl_uint>(CL_KERNEL_NUM_ARGS);
}

unsigned int
Kernel::reference_count() const
{
    return get_info<cl_uint>(CL_KERNEL_REFERENCE_COUNT);
}

Context
Kernel::context() const
{
    auto ctx = get_info<cl_context>(CL_KERNEL_CONTEXT);
    return Context(ctx);
}

Program
Kernel::program() const
{
    auto prg = get_info<cl_program>(CL_KERNEL_PROGRAM);
    return Program(prg);
}

std::vector<std::string>
Kernel::attributes() const
{
    auto str = get_info<std::string>(CL_KERNEL_ATTRIBUTES);
    return utils::split(str, " ");
}

void
Kernel::set_arg(cl_uint index, const Memory & value)
{
    OPENCL_CHECK(clSetKernelArg(this->kern, index, sizeof(cl_mem), &value));
}

void
Kernel::set_arg(cl_uint index, size_t size, const void * arg)
{
    OPENCL_CHECK(clSetKernelArg(this->kern, index, size, arg));
}

Kernel::operator cl_kernel() const
{
    return this->kern;
}

} // namespace openclcpp_lite
