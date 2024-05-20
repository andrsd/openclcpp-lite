#include "openclcpp-lite/queue.h"
#include "openclcpp-lite/context.h"

namespace openclcpp_lite {

Queue::Queue(const Context & context, bool enable_profiling)
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

void
Queue::retain() const
{
    OPENCL_CHECK(clRetainCommandQueue(this->q));
}

void
Queue::release() const
{
    OPENCL_CHECK(clReleaseCommandQueue(this->q));
}

Context
Queue::context() const
{
    auto ctx_id = get_info<cl_context>(CL_QUEUE_CONTEXT);
    return Context(ctx_id);
}

Device
Queue::device() const
{
    auto dev_id = get_info<cl_device_id>(CL_QUEUE_DEVICE);
    return Device(dev_id);
}

unsigned int
Queue::reference_count() const
{
    return get_info<cl_uint>(CL_QUEUE_REFERENCE_COUNT);
}

template<typename... ARGS>
void
Queue::enqueue_kernel(const Kernel &kernel)
{
    //clEnqueueNDRangeKernel(this->q, kernel, );
}

void
Queue::flush() const
{
    OPENCL_CHECK(clFlush(this->q));
}

void
Queue::finish() const
{
    OPENCL_CHECK(clFinish(this->q));
}

} // namespace openclcpp_lite
