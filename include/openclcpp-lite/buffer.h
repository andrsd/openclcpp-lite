#pragma once

#include "openclcpp-lite/cl.h"
#include "openclcpp-lite/templ.h"
#include "openclcpp-lite/flags.h"
#include "openclcpp-lite/enums.h"
#include "openclcpp-lite/memory.h"

namespace openclcpp_lite {

class Context;

class Buffer : public Memory {
public:
    Buffer(const Context & context, MemoryFlags mem_flags, size_t size);
    Buffer(const Context & context, MemoryFlags mem_flags, size_t size, void * host_ptr);
};

/// Templated buffer
template <typename T>
class TBuffer : public Buffer {
public:
    using type = T;

    TBuffer(const Context & context, MemoryFlags mem_flags, size_t n_entries) :
        Buffer(context, mem_flags, sizeof(T) * n_entries)
    {
    }
};

} // namespace openclcpp_lite
