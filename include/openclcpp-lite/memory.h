// SPDX-FileCopyrightText: 2024 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include "openclcpp-lite/cl.h"
#include "openclcpp-lite/templ.h"

namespace openclcpp_lite {

class Context;

/// OpenCL memory. Base class for Buffers and Images
class Memory {
public:
    /// Increments the memory object reference count.
    void retain() const;

    /// Decrements the memory object reference count.
    void release() const;

    /// Return actual size of the data store associated with in bytes.
    size_t byte_size() const;

    /// Map count. The map count returned should be considered immediately stale. It is unsuitable
    /// for general use in applications. This feature is provided for debugging.
    cl_uint map_count() const;

    /// Return reference count. The reference count returned should be considered immediately stale.
    /// It is unsuitable for general use in applications. This feature is provided for identifying
    /// memory leaks.
    cl_uint reference_count() const;

    /// Return context specified when memory object is created. If `memobj` is created using
    /// `clCreateSubBuffer`, the context associated with the memory object specified as the buffer
    /// argument to `clCreateSubBuffer` is returned.
    Context context() const;

    /// Return offset if memobj is a sub-buffer object created using `clCreateSubBuffer`.
    /// This returns 0 if memobj is not a subbuffer object.
    size_t offset() const;

    operator cl_mem() const;

protected:
    template <typename T>
    T
    get_info(cl_mem_info name) const
    {
        T val;
        get_info_helper(clGetMemObjectInfo, this->mem, name, val);
        return val;
    }

    /// Underlying OpenCL memory
    cl_mem mem;
};

} // namespace openclcpp_lite
