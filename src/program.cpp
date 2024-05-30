// SPDX-FileCopyrightText: 2024 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#include "openclcpp-lite/program.h"
#include "openclcpp-lite/error.h"
#include "openclcpp-lite/context.h"
#include "openclcpp-lite/utils.h"

namespace openclcpp_lite {

Program::Program(cl_program prg) : prg(prg) {}

void
Program::retain() const
{
    OPENCL_CHECK(clRetainProgram(this->prg));
}

void
Program::release() const
{
    OPENCL_CHECK(clReleaseProgram(this->prg));
}

unsigned int
Program::reference_count() const
{
    return get_info<cl_uint>(CL_PROGRAM_REFERENCE_COUNT);
}

Context
Program::context() const
{
    auto ctx_id = get_info<cl_context>(CL_PROGRAM_CONTEXT);
    return Context(ctx_id);
}

std::vector<Device>
Program::devices() const
{
    auto dev_ids = get_info<std::vector<cl_device_id>>(CL_PROGRAM_DEVICES);
    std::vector<Device> devices;
    for (auto & id : dev_ids)
        devices.emplace_back(Device(id));
    return devices;
}

size_t
Program::num_of_kernels() const
{
    return get_info<size_t>(CL_PROGRAM_NUM_KERNELS);
}

std::vector<std::string>
Program::kernel_names() const
{
    auto names = get_info<std::string>(CL_PROGRAM_KERNEL_NAMES);
    names = utils::rtrim_null(names);
    return utils::split(names, ";");
}

void
Program::build() const
{
    OPENCL_CHECK(clBuildProgram(this->prg, 0, nullptr, nullptr, nullptr, nullptr));
}

Program::operator cl_program() const
{
    return this->prg;
}

Program
Program::from_source(const std::string & source)
{
    auto context = Context::get_default();
    const char * s = source.c_str();
    size_t len = source.length();
    cl_int err;
    auto p = clCreateProgramWithSource(context, 1, &s, &len, &err);
    OPENCL_CHECK(err);
    Program prg(p);
    return prg;
}

Program
Program::from_source(const Context & context, const std::string & source)
{
    const char * s = source.c_str();
    size_t len = source.length();
    cl_int err;
    auto p = clCreateProgramWithSource(context, 1, &s, &len, &err);
    OPENCL_CHECK(err);
    Program prg(p);
    return prg;
}

Program
Program::from_source(const Context & context, const std::vector<std::string> & lines)
{
    size_t n = lines.size();
    std::vector<const char *> strs;
    strs.reserve(n);
    std::vector<size_t> lens;
    lens.reserve(n);
    for (auto & s : lines) {
        strs.emplace_back(s.c_str());
        lens.emplace_back(s.size());
    }
    cl_int err;
    auto p = clCreateProgramWithSource(context, 1, strs.data(), lens.data(), &err);
    OPENCL_CHECK(err);
    Program prg(p);
    return prg;
}

} // namespace openclcpp_lite
