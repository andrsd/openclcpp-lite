// SPDX-FileCopyrightText: 2024 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#include "openclcpp-lite/device.h"
#include "openclcpp-lite/context.h"
#include "openclcpp-lite/buffer.h"
#include "openclcpp-lite/platform.h"

namespace openclcpp_lite {

std::once_flag Context::have_default;
Context Context::default_context;

Context::Context() : ctx(nullptr) {}

Context::Context(const Device & device)
{
    cl_int err;
    cl_device_id id = device;
    this->ctx = clCreateContext(nullptr, 1, &id, nullptr, nullptr, &err);
    OPENCL_CHECK(err);
}

Context::Context(const std::vector<Device> & devices)
{
    cl_int err;
    std::vector<cl_device_id> ids;
    ids.reserve(devices.size());
    for (auto & d : devices)
        ids.emplace_back(d);
    this->ctx = clCreateContext(nullptr, ids.size(), ids.data(), nullptr, nullptr, &err);
    OPENCL_CHECK(err);
}

Context::Context(cl_context context) : ctx(context) {}

void
Context::retain() const
{
    OPENCL_CHECK(clRetainContext(this->ctx));
}

void
Context::release() const
{
    OPENCL_CHECK(clReleaseContext(this->ctx));
}

unsigned int
Context::reference_count() const
{
    return get_info<cl_uint>(CL_CONTEXT_REFERENCE_COUNT);
}

/// Return the number of devices in context.
unsigned int
Context::num_of_devices() const
{
    return get_info<cl_uint>(CL_CONTEXT_NUM_DEVICES);
}

std::vector<Device>
Context::devices() const
{
    std::size_t sz;
    OPENCL_CHECK(clGetContextInfo(this->ctx, CL_CONTEXT_DEVICES, 0, nullptr, &sz));
    std::vector<cl_device_id> ids;
    ids.resize(sz);
    OPENCL_CHECK(clGetContextInfo(this->ctx, CL_CONTEXT_DEVICES, sz, ids.data(), nullptr));
    std::vector<Device> devices;
    for (auto & id : ids)
        devices.emplace_back(Device(id));
    return devices;
}

Context
Context::get_default()
{
    std::call_once(Context::have_default, []() {
#if defined(__APPLE__) || defined(__MACOS)
        cl_context_properties * properties = nullptr;
#else
        const Platform & p = Platform::get_default();
        cl_platform_id default_platform = p;
        cl_context_properties properties[3] = { CL_CONTEXT_PLATFORM,
                                                (cl_context_properties) default_platform,
                                                0 };
#endif
        cl_int err;
        auto ctx =
            clCreateContextFromType(properties, CL_DEVICE_TYPE_DEFAULT, nullptr, nullptr, &err);
        OPENCL_CHECK(err);
        Context::default_context = Context(ctx);
    });
    return default_context;
}

} // namespace openclcpp_lite
