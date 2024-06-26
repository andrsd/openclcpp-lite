// SPDX-FileCopyrightText: 2024 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#include "openclcpp-lite/program.h"
#include "openclcpp-lite/error.h"
#include "openclcpp-lite/context.h"
#include "openclcpp-lite/utils.h"
#include "openclcpp-lite/exception.h"

namespace openclcpp_lite {

Program::Program() : prg(nullptr) {}

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

std::vector<size_t>
Program::binary_sizes() const
{
    return get_info<std::vector<size_t>>(CL_PROGRAM_BINARY_SIZES);
}

std::vector<std::vector<char>>
Program::binaries() const
{
    auto bin_size = binary_sizes();
    auto n = bin_size.size();
    std::vector<std::vector<char>> bins(n);
    for (int i = 0; i < n; i++)
        bins[i].resize(bin_size[i]);
    std::vector<char *> bin_ptrs(n);
    for (int i = 0; i < bin_size.size(); i++)
        bin_ptrs[i] = bins[i].data();
    auto err = clGetProgramInfo(this->prg,
                                CL_PROGRAM_BINARIES,
                                sizeof(char *) * n,
                                bin_ptrs.data(),
                                nullptr);
    OPENCL_CHECK(err);
    return bins;
}

void
Program::build(const std::vector<std::string> & options,
               void(CL_CALLBACK * pfn_notify)(cl_program program, void * user_data),
               void * user_data) const
{
    auto opts = utils::join(" ", options);
    OPENCL_CHECK(clBuildProgram(this->prg, 0, nullptr, opts.c_str(), pfn_notify, user_data));
}

void
Program::build(const std::vector<Device> & devices,
               const std::vector<std::string> & options,
               void(CL_CALLBACK * pfn_notify)(cl_program program, void * user_data),
               void * user_data) const
{
    std::vector<cl_device_id> ids;
    for (auto & d : devices)
        ids.push_back(d);
    auto opts = utils::join(" ", options);
    OPENCL_CHECK(clBuildProgram(this->prg,
                                ids.size(),
                                ids.size() == 0 ? nullptr : ids.data(),
                                opts.c_str(),
                                pfn_notify,
                                user_data));
}

void
Program::compile(const std::vector<std::string> & options,
                 void(CL_CALLBACK * pfn_notify)(cl_program program, void * user_data),
                 void * user_data) const
{
    auto opts = utils::join(" ", options);
    OPENCL_CHECK(clCompileProgram(this->prg,
                                  0,
                                  nullptr,
                                  opts.c_str(),
                                  0,
                                  nullptr,
                                  nullptr,
                                  pfn_notify,
                                  user_data));
}

void
Program::compile(const std::vector<Device> & devices,
                 const std::vector<std::string> & options,
                 void(CL_CALLBACK * pfn_notify)(cl_program program, void * user_data),
                 void * user_data) const
{
    std::vector<cl_device_id> ids;
    for (auto & d : devices)
        ids.push_back(d);
    auto opts = utils::join(" ", options);
    OPENCL_CHECK(clCompileProgram(this->prg,
                                  ids.size(),
                                  ids.empty() ? nullptr : ids.data(),
                                  opts.c_str(),
                                  0,
                                  nullptr,
                                  nullptr,
                                  pfn_notify,
                                  user_data));
}

Program::BuildStatus
Program::build_status(Device device) const
{
    return static_cast<BuildStatus>(
        get_build_info<cl_build_status>(device, CL_PROGRAM_BUILD_STATUS));
}

std::string
Program::build_options(Device device) const
{
    return get_build_info<std::string>(device, CL_PROGRAM_BUILD_OPTIONS);
}

std::string
Program::build_log(Device device) const
{
    return get_build_info<std::string>(device, CL_PROGRAM_BUILD_LOG);
}

Program::BinaryType
Program::binary_type(Device device) const
{
    return static_cast<BinaryType>(
        get_build_info<cl_program_binary_type>(device, CL_PROGRAM_BINARY_TYPE));
}

Program::operator cl_program() const
{
    return this->prg;
}

Program
Program::from_source(const std::string & source)
{
    auto context = Context::get_default();
    return from_source(context, source);
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
Program::from_source(const std::vector<std::string> & lines)
{
    auto context = Context::get_default();
    return from_source(context, lines);
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
    auto p = clCreateProgramWithSource(context, n, strs.data(), lens.data(), &err);
    OPENCL_CHECK(err);
    Program prg(p);
    return prg;
}

Program
Program::from_source(int n_lines, const char ** lines)
{
    auto context = Context::get_default();
    return from_source(context, n_lines, lines);
}

Program
Program::from_source(const Context & context, unsigned int n_lines, const char ** lines)
{
    cl_int err;
    auto p = clCreateProgramWithSource(context, n_lines, lines, nullptr, &err);
    OPENCL_CHECK(err);
    Program prg(p);
    return prg;
}

Program
Program::from_binary(const Context & context,
                     const std::vector<Device> & devices,
                     const std::vector<std::vector<char>> & binaries)
{
    if (devices.size() != binaries.size())
        throw Exception("The number of devices ({}) must be equal to the number of binaries ({})",
                        devices.size(),
                        binaries.size());
    std::vector<cl_device_id> dev_ids;
    for (auto & dev : devices)
        dev_ids.push_back(dev);
    std::vector<size_t> sizes;
    for (auto & bin : binaries)
        sizes.push_back(bin.size());
    std::vector<const unsigned char *> bins;
    for (auto & bin : binaries)
        bins.push_back(reinterpret_cast<const unsigned char *>(bin.data()));
    cl_int err;
    auto p = clCreateProgramWithBinary(context,
                                       dev_ids.size(),
                                       dev_ids.empty() ? nullptr : dev_ids.data(),
                                       sizes.data(),
                                       bins.data(),
                                       nullptr,
                                       &err);
    OPENCL_CHECK(err);
    return Program(p);
}

Program
Program::link(const std::vector<std::string> & options,
              const std::vector<Program> & programs,
              void(CL_CALLBACK * pfn_notify)(cl_program, void *),
              void * user_data)
{
    auto ctx = Context::get_default();
    return link(ctx, {}, options, programs, pfn_notify, user_data);
}

Program
Program::link(const Context & context,
              const std::vector<Device> & devices,
              const std::vector<std::string> & options,
              const std::vector<Program> & programs,
              void(CL_CALLBACK * pfn_notify)(cl_program program, void * user_data),
              void * user_data)
{
    std::vector<cl_device_id> devs;
    for (auto & d : devices)
        devs.push_back(d);
    std::vector<cl_program> prgs;
    for (auto & p : programs)
        prgs.push_back(p);
    auto opts = utils::join(" ", options);
    cl_int err;
    auto l = clLinkProgram(context,
                           devs.size(),
                           devs.empty() ? nullptr : devs.data(),
                           opts.c_str(),
                           prgs.size(),
                           prgs.data(),
                           pfn_notify,
                           user_data,
                           &err);
    OPENCL_CHECK(err);
    return Program(l);
}

} // namespace openclcpp_lite
