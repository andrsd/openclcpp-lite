#pragma once

#include "openclcpp-lite/cl.h"
#include "openclcpp-lite/flags.h"
#include "openclcpp-lite/enums.h"

namespace openclcpp_lite {

class Context;

class Buffer {
public:
    Buffer(const Context & context, MemoryFlags mem_flags, size_t size, void * host_ptr);

    /// Increments the memory object reference count.
    void retain() const;

    /// Decrements the memory object reference count.
    void release() const;

    /// Return actual size of the data store associated with in bytes.
    size_t size() const;

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

private:
    template <typename T>
    T get_info(cl_mem_info name) const;

    cl_mem mem;
};

} // namespace openclcpp_lite
