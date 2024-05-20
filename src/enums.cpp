#include "openclcpp-lite/enums.h"

namespace openclcpp_lite {

MemoryFlags::MemoryFlags(const MemoryFlag & flag) : Flags<MemoryFlag>(flag) {}

MemoryFlags::MemoryFlags(const Flags<MemoryFlag> & flags) : Flags<MemoryFlag>(flags) {}

MemoryFlags::operator cl_mem_flags() const
{
    return this->mask;
}

MapFlags::MapFlags(const MapFlag & flag) : Flags<MapFlag>(flag) {}

MapFlags::MapFlags(const Flags<MapFlag> & flags) : Flags<MapFlag>(flags) {}

MapFlags::operator cl_map_flags() const
{
    return this->mask;
}

} // namespace openclcpp_lite
