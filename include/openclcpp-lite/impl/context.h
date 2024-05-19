#pragma once

#include "cl.h"
#include "exception.h"
#include "platform.h"
#include "device.h"
#include <vector>

namespace openclcpp_lite {

class Context {
public:
    Context(const Device & device);
    Context(const std::vector<Device> & devices);

    /// Increment the context reference count.
    void retain() const;

    /// Decrement the context reference count.
    void release() const;

    /// Return the context reference count. The reference count returned should be considered
    /// immediately stale. It is unsuitable for general use in applications. This feature is
    /// provided for identifying memory leaks.
    uint
    reference_count() const
    {
        return get_info<cl_uint>(CL_CONTEXT_REFERENCE_COUNT);
    }

    /// Return the number of devices in context.
    uint
    num_of_devices() const
    {
        return get_info<cl_uint>(CL_CONTEXT_NUM_DEVICES);
    }

    std::vector<Device>
    devices() const
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

    operator cl_context() const { return this->ctx; }

public:
    static Context default_context();

private:
    Context(cl_context context) : ctx(context) {}

    template <typename T>
    T
    get_info(cl_context_info name) const
    {
        std::size_t sz;
        OPENCL_CHECK(clGetContextInfo(this->ctx, name, 0, nullptr, &sz));
        T val;
        OPENCL_CHECK(clGetContextInfo(this->ctx, name, sizeof(T), &val, nullptr));
        return val;
    }

    cl_context ctx;

    friend class Queue;
};

inline Context::Context(const Device & device)
{
    cl_int err_code;
    cl_device_id id = device;
    this->ctx = clCreateContext(nullptr, 1, &id, nullptr, nullptr, &err_code);
    OPENCL_CHECK(err_code);
}

inline Context::Context(const std::vector<Device> & devices)
{
    cl_int err_code;
    std::vector<cl_device_id> ids;
    ids.reserve(devices.size());
    for (auto & d : devices)
        ids.emplace_back(d);
    this->ctx = clCreateContext(nullptr, ids.size(), ids.data(), nullptr, nullptr, &err_code);
    OPENCL_CHECK(err_code);
}

inline void
Context::retain() const
{
    OPENCL_CHECK(clRetainContext(this->ctx));
}

inline void
Context::release() const
{
    OPENCL_CHECK(clReleaseContext(this->ctx));
}

inline Context
default_context()
{
    auto platforms = Platform::platforms();
    if (platforms.empty())
        throw Exception("No OpenCL platofrms available");

    auto devices = platforms[0].devices();
    if (devices.empty())
        throw Exception("No OpenCL devices available");

    Context ctx(devices[0]);
    return ctx;
}

} // namespace openclcpp_lite
