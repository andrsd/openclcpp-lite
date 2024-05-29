#pragma once

#include "openclcpp-lite/cl.h"
#include "openclcpp-lite/templ.h"
#include "openclcpp-lite/enums.h"
#include "openclcpp-lite/exception.h"
#include "openclcpp-lite/platform.h"
#include "openclcpp-lite/device.h"
#include "openclcpp-lite/buffer.h"
#include <vector>

namespace openclcpp_lite {

class Context {
public:
    explicit Context(const Device & device);
    explicit Context(const std::vector<Device> & devices);

    /// Increment the context reference count.
    void retain() const;

    /// Decrement the context reference count.
    void release() const;

    /// Return the context reference count. The reference count returned should be considered
    /// immediately stale. It is unsuitable for general use in applications. This feature is
    /// provided for identifying memory leaks.
    unsigned int reference_count() const;

    /// Return the number of devices in context.
    unsigned int num_of_devices() const;

    /// Devices attached to this context
    std::vector<Device> devices() const;

    /// Allocate
    template <typename T>
    TBuffer<T>
    alloc(int n_entries) const
    {
        throw Exception("Unsupported data type");
    }

    template <>
    TBuffer<double>
    alloc<double>(int n_entries) const
    {
        return alloc_helper<double>(n_entries);
    }

    template <>
    TBuffer<float>
    alloc<float>(int n_entries) const
    {
        return alloc_helper<float>(n_entries);
    }

    template <>
    TBuffer<int>
    alloc<int>(int n_entries) const
    {
        return alloc_helper<int>(n_entries);
    }

    operator cl_context() const { return this->ctx; }

public:
    static Context default_context();

private:
    explicit Context(cl_context context);

    template <typename T>
    TBuffer<T>
    alloc_helper(int n_entries) const
    {
        TBuffer<T> b(*this, READ_WRITE, n_entries);
        return b;
    }

    template <typename T>
    T
    get_info(cl_context_info name) const
    {
        T val;
        get_info_helper(clGetContextInfo, this->ctx, name, val);
        return val;
    }

    cl_context ctx;

    friend class Queue;
    friend class Memory;
    friend class Buffer;
    friend class Program;
    friend class Event;
    friend class Kernel;
};

Context default_context();

} // namespace openclcpp_lite