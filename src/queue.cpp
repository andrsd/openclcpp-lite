#include "openclcpp-lite/queue.h"
#include "openclcpp-lite/context.h"
#include "openclcpp-lite/buffer.h"
#include "openclcpp-lite/event.h"

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

Queue::Queue(cl_command_queue q) : q(q) {}

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

void
Queue::enqueue_read(const Buffer & buffer,
                    size_t offset,
                    size_t size,
                    void * ptr,
                    const std::vector<Event> & wait_list) const
{
    OPENCL_CHECK(clEnqueueReadBuffer(this->q,
                                     buffer,
                                     CL_TRUE,
                                     offset,
                                     size,
                                     ptr,
                                     wait_list.size(),
                                     wait_list.empty() ? nullptr : (cl_event *) &wait_list.front(),
                                     nullptr));
}

Event
Queue::enqueue_iread(const Buffer & buffer,
                     size_t offset,
                     size_t size,
                     void * ptr,
                     const std::vector<Event> & wait_list) const
{
    cl_event evt;
    OPENCL_CHECK(clEnqueueReadBuffer(this->q,
                                     buffer,
                                     CL_FALSE,
                                     offset,
                                     size,
                                     ptr,
                                     wait_list.size(),
                                     wait_list.empty() ? nullptr : (cl_event *) &wait_list.front(),
                                     &evt));
    return Event(evt);
}

void
Queue::enqueue_write(const Buffer & buffer,
                     size_t offset,
                     size_t size,
                     const void * ptr,
                     const std::vector<Event> & wait_list) const
{
    OPENCL_CHECK(clEnqueueWriteBuffer(this->q,
                                      buffer,
                                      CL_TRUE,
                                      offset,
                                      size,
                                      ptr,
                                      wait_list.size(),
                                      wait_list.empty() ? nullptr : (cl_event *) &wait_list.front(),
                                      nullptr));
}

Event
Queue::enqueue_iwrite(const Buffer & buffer,
                      size_t offset,
                      size_t size,
                      const void * ptr,
                      const std::vector<Event> & wait_list) const
{
    cl_event evt;
    OPENCL_CHECK(clEnqueueWriteBuffer(this->q,
                                      buffer,
                                      CL_FALSE,
                                      offset,
                                      size,
                                      ptr,
                                      wait_list.size(),
                                      wait_list.empty() ? nullptr : (cl_event *) &wait_list.front(),
                                      &evt));
    return Event(evt);
}

Event
Queue::enqueue_copy(const Buffer & src,
                    const Buffer & dest,
                    size_t src_offset,
                    size_t dest_offset,
                    size_t size,
                    const std::vector<Event> & wait_list)
{
    cl_event evt;
    OPENCL_CHECK(clEnqueueCopyBuffer(this->q,
                                     src,
                                     dest,
                                     src_offset,
                                     dest_offset,
                                     size,
                                     wait_list.size(),
                                     wait_list.empty() ? nullptr : (cl_event *) &wait_list.front(),
                                     &evt));
    return Event(evt);
}

void *
Queue::enqueue_map_buffer(const Buffer & buffer,
                          bool blocking,
                          MapFlags flags,
                          size_t offset,
                          size_t size) const
{
    cl_int err_code;
    auto ret = clEnqueueMapBuffer(this->q,
                                  buffer,
                                  blocking ? CL_TRUE : CL_FALSE,
                                  flags,
                                  offset,
                                  size,
                                  0,
                                  nullptr,
                                  nullptr,
                                  &err_code);
    OPENCL_CHECK(err_code);
    return ret;
}

template <typename... ARGS>
void
Queue::enqueue_kernel(const Kernel & kernel)
{
    // clEnqueueNDRangeKernel(this->q, kernel, );
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
