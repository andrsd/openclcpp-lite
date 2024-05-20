#include "openclcpp-lite/memory.h"
#include "openclcpp-lite/context.h"

namespace openclcpp_lite {

void
Memory::retain() const
{
    OPENCL_CHECK(clRetainMemObject(this->mem));
}

void
Memory::release() const
{
    OPENCL_CHECK(clReleaseMemObject(this->mem));
}

size_t
Memory::size() const
{
    return get_info<size_t>(CL_MEM_SIZE);
}

cl_uint
Memory::map_count() const
{
    return get_info<cl_uint>(CL_MEM_MAP_COUNT);
}

cl_uint
Memory::reference_count() const
{
    return get_info<cl_uint>(CL_MEM_REFERENCE_COUNT);
}

Context
Memory::context() const
{
    auto ctx_id = get_info<cl_context>(CL_MEM_CONTEXT);
    return Context(ctx_id);
}

size_t
Memory::offset() const
{
    return get_info<size_t>(CL_MEM_OFFSET);
}

Memory::operator cl_mem() const
{
    return this->mem;
}

} // namespace openclcpp_lite
