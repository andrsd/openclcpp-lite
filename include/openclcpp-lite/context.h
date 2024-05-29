// SPDX-FileCopyrightText: 2024 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include "openclcpp-lite/cl.h"
#include "openclcpp-lite/templ.h"
#include "openclcpp-lite/device.h"
#include <vector>
#include <mutex>

namespace openclcpp_lite {

class Context {
public:
    /// Create a null context
    Context();

    /// Create a context from a OpenCL context
    explicit Context(cl_context context);

    /// Create a context on a device
    ///
    /// @param device Device to create the context on
    explicit Context(const Device & device);

    /// Create a context on devices
    ///
    /// @param devices Devices to create the context on
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

    operator cl_context() const { return this->ctx; }

private:
    template <typename T>
    T
    get_info(cl_context_info name) const
    {
        T val;
        get_info_helper(clGetContextInfo, this->ctx, name, val);
        return val;
    }

    cl_context ctx;

public:
    static Context get_default();

private:
    static std::once_flag have_default;
    static Context default_context;
};

} // namespace openclcpp_lite
