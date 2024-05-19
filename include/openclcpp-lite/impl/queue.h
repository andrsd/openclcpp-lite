#pragma once

#include "cl.h"
#include "context.h"

namespace openclcpp_lite {

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
    uint
    reference_count() const
    {
        return get_info<cl_uint>(CL_QUEUE_REFERENCE_COUNT);
    }

private:
    template <typename T>
    T
    get_info(cl_context_info name) const
    {
        std::size_t sz;
        OPENCL_CHECK(clGetCommandQueueInfo(this->q, name, 0, nullptr, &sz));
        T val;
        OPENCL_CHECK(clGetCommandQueueInfo(this->q, name, sizeof(T), &val, nullptr));
        return val;
    }

    cl_command_queue q;
};

inline Queue::Queue(const Context & context, bool enable_profiling)
{
    auto devices = context.devices();
    cl_device_id device_id = devices[0];
    cl_int err_code;
    cl_command_queue_properties props = 0;
    if (enable_profiling)
        props |= CL_QUEUE_PROFILING_ENABLE;
    this->q = clCreateCommandQueue(context, device_id, props, &err_code);
    OPENCL_CHECK(err_code);
}

inline void
Queue::retain() const
{
    OPENCL_CHECK(clRetainCommandQueue(this->q));
}

inline void
Queue::release() const
{
    OPENCL_CHECK(clReleaseCommandQueue(this->q));
}

inline Context
Queue::context() const
{
    auto ctx_id = get_info<cl_context>(CL_QUEUE_CONTEXT);
    return Context(ctx_id);
}

inline Device
Queue::device() const
{
    auto dev_id = get_info<cl_device_id>(CL_QUEUE_DEVICE);
    return Device(dev_id);
}

} // namespace openclcpp_lite
