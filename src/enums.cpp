// SPDX-FileCopyrightText: 2024 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#include "openclcpp-lite/enums.h"

namespace openclcpp_lite {

FPConfigFlags::FPConfigFlags(const FPConfigFlags & flag) : Flags<FPConfig>(flag) {}

FPConfigFlags::FPConfigFlags(const Flags<FPConfig> & flags) : Flags<FPConfig>(flags) {}

FPConfigFlags::operator cl_device_fp_config() const
{
    return this->mask;
}

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
