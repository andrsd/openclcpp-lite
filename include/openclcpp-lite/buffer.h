// SPDX-FileCopyrightText: 2024 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include "openclcpp-lite/cl.h"
#include "openclcpp-lite/templ.h"
#include "openclcpp-lite/flags.h"
#include "openclcpp-lite/enums.h"
#include "openclcpp-lite/memory.h"
#include "openclcpp-lite/range.h"
#include "openclcpp-lite/context.h"
#include "openclcpp-lite/queue.h"

namespace openclcpp_lite {

template <typename T, int D>
class Buffer : public Memory {
public:
    using type = T;
};

/// 1-dimensional buffer
///
/// @tparam T C++ type of the data element this buffer stores
template <typename T>
class Buffer<T, 1> : public Memory {
public:
    /// Create an uninitialized buffer in a default Context
    ///
    /// @param range Size of the buffer
    /// @param mem_flags Memory flags
    Buffer(const Range<1> & range, MemoryFlags mem_flags = READ_WRITE)
    {
        auto ctx = Context::get_default();
        cl_int err;
        this->mem = clCreateBuffer(ctx, mem_flags, sizeof(T) * range.size(), nullptr, &err);
        OPENCL_CHECK(err);
    }

    /// Create an uninitialized buffer in a context
    ///
    /// @param context OpenCL context
    /// @param range Size of the buffer
    /// @param mem_flags Memory flags
    Buffer(const Context & context, const Range<1> & range, MemoryFlags mem_flags = READ_WRITE)
    {
        cl_int err;
        this->mem = clCreateBuffer(context, mem_flags, sizeof(T) * range.size(), nullptr, &err);
        OPENCL_CHECK(err);
    }

    /// Create a buffer in the default Context and initialize it from host memory
    ///
    /// @param src Host memory with the initial values
    /// @param range Size of the buffer
    /// @param mem_flags Memory flags
    Buffer(const T * src, const Range<1> & range, MemoryFlags mem_flags = READ_WRITE)
    {
        auto ctx = Context::get_default();
        cl_int err;
        this->mem = clCreateBuffer(ctx, mem_flags, sizeof(T) * range.size(), nullptr, &err);
        OPENCL_CHECK(err);
        auto q = Queue::get_default();
        auto * dst = q.enqueue_map_buffer<T, 1>(*this, true, WRITE, range);
        memcpy(dst, src, byte_size());
        auto evt = q.enqueue_unmap_mem_object(*this, dst);
        evt.wait();
    }

    /// Create a buffer in a Context and initialize it from host memory
    ///
    /// @param context OpenCL context
    /// @param src Host memory with the initial values
    /// @param range Size of the buffer
    /// @param mem_flags Memory flags
    Buffer(const Context & context,
           const T * src,
           const Range<1> & range,
           MemoryFlags mem_flags = READ_WRITE)
    {
        cl_int err;
        this->mem = clCreateBuffer(context, mem_flags, sizeof(T) * range.size(), nullptr, &err);
        OPENCL_CHECK(err);
        auto q = Queue::get_default();
        auto * dst = q.enqueue_map_buffer<T, 1>(*this, true, WRITE, range);
        memcpy(dst, src, byte_size());
        auto evt = q.enqueue_unmap_mem_object(*this, dst);
        evt.wait();
    }
};

} // namespace openclcpp_lite
