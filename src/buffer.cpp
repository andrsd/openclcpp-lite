#include "openclcpp-lite/buffer.h"
#include "openclcpp-lite/cl.h"
#include "openclcpp-lite/error.h"
#include "openclcpp-lite/context.h"

namespace openclcpp_lite {

Buffer::Buffer(const Context & context, MemoryFlags mem_flags, size_t size)
{
    cl_int err_code;
    this->mem = clCreateBuffer(context, mem_flags, size, nullptr, &err_code);
    OPENCL_CHECK(err_code);
}

Buffer::Buffer(const Context & context, MemoryFlags mem_flags, size_t size, void * host_ptr)
{
    cl_int err_code;
    this->mem = clCreateBuffer(context, mem_flags, size, host_ptr, &err_code);
    OPENCL_CHECK(err_code);
}

//

} // namespace openclcpp_lite
