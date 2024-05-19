#include "openclcpp-lite/enums.h"

namespace openclcpp_lite {

MemoryFlags::MemoryFlags(const Flags<MemoryFlag> & flags) : Flags<MemoryFlag>(flags) {}

MemoryFlags::operator cl_mem_flags() const
{
    cl_mem_flags flags = 0;
    if (this->mask & READ_WRITE)
        flags |= CL_MEM_READ_WRITE;
    if (this->mask & WRITE_ONLY)
        flags |= CL_MEM_WRITE_ONLY;
    if (this->mask & READ_ONLY)
        flags |= CL_MEM_READ_ONLY;
    if (this->mask & USE_HOST_PTR)
        flags |= CL_MEM_USE_HOST_PTR;
    if (this->mask & ALLOC_HOST_PTR)
        flags |= CL_MEM_ALLOC_HOST_PTR;
    if (this->mask & COPY_HOST_PTR)
        flags |= CL_MEM_COPY_HOST_PTR;
    if (this->mask & HOST_WRITE_ONLY)
        flags |= CL_MEM_HOST_WRITE_ONLY;
    if (this->mask & HOST_READ_ONLY)
        flags |= CL_MEM_HOST_READ_ONLY;
    if (this->mask & HOST_NO_ACCESS)
        flags |= CL_MEM_HOST_NO_ACCESS;
    return flags;
}

} // namespace openclcpp_lite
