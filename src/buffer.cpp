#include "openclcpp-lite/buffer.h"
#include "openclcpp-lite/cl.h"
#include "openclcpp-lite/error.h"
#include "openclcpp-lite/context.h"

namespace openclcpp_lite {

Buffer::Buffer(const Context & context, MemoryFlags mem_flags, size_t size, void * host_ptr)
{
    cl_int err_code;
    this->mem = clCreateBuffer(context, mem_flags, size, host_ptr, &err_code);
    OPENCL_CHECK(err_code);
}

void
Buffer::retain() const
{
    OPENCL_CHECK(clRetainMemObject(this->mem));
}

void
Buffer::release() const
{
    OPENCL_CHECK(clReleaseMemObject(this->mem));
}

size_t
Buffer::size() const
{
    return get_info<size_t>(CL_MEM_SIZE);
}

cl_uint
Buffer::map_count() const
{
    return get_info<cl_uint>(CL_MEM_MAP_COUNT);
}

cl_uint
Buffer::reference_count() const
{
    return get_info<cl_uint>(CL_MEM_REFERENCE_COUNT);
}

Context
Buffer::context() const
{
    auto ctx_id = get_info<cl_context>(CL_MEM_CONTEXT);
    return Context(ctx_id);
}

size_t
Buffer::offset() const
{
    return get_info<size_t>(CL_MEM_OFFSET);
}

} // namespace openclcpp_lite
