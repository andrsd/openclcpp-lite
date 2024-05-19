#pragma once

#include "openclcpp-lite/cl.h"
#include "openclcpp-lite/flags.h"

namespace openclcpp_lite {

enum MemoryFlag {
    READ_WRITE = CL_MEM_READ_WRITE,
    WRITE_ONLY = CL_MEM_WRITE_ONLY,
    READ_ONLY = CL_MEM_READ_ONLY,
    USE_HOST_PTR = CL_MEM_USE_HOST_PTR,
    ALLOC_HOST_PTR = CL_MEM_ALLOC_HOST_PTR,
    COPY_HOST_PTR = CL_MEM_COPY_HOST_PTR,
    HOST_WRITE_ONLY = CL_MEM_HOST_WRITE_ONLY,
    HOST_READ_ONLY = CL_MEM_HOST_READ_ONLY,
    HOST_NO_ACCESS = CL_MEM_HOST_NO_ACCESS
};

struct MemoryFlags : public Flags<MemoryFlag>
{
    MemoryFlags(const Flags<MemoryFlag> & flags);

    operator cl_mem_flags() const;
};

} // namespace openclcpp_lite
