#pragma once

#include "openclcpp-lite/cl.h"
#include "openclcpp-lite/error.h"

namespace openclcpp_lite {

class Context;
class Device;

class Queue {
public:
    Queue(const Context & context, bool enable_profiling = false);

    /// Increments the command_queue reference count
    void retain() const;

    /// Decrements the command_queue reference count.
    void release() const;

    /// Return the context specified when the command-queue is created.
    Context context() const;

    /// Return the device specified when the command-queue is created.
    Device device() const;

    /// Return the command-queue reference count.
    ///
    /// The reference count returned should be considered immediately stale. It is unsuitable for
    /// general use in applications. This feature is provided for identifying memory leaks.
    unsigned int reference_count() const;

private:
    template <typename T>
    T get_info(cl_context_info name) const;

    cl_command_queue q;
};

} // namespace openclcpp_lite
