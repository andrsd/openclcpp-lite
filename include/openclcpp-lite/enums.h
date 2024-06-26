// SPDX-FileCopyrightText: 2024 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include "openclcpp-lite/cl.h"
#include "openclcpp-lite/flags.h"

namespace openclcpp_lite {

enum FPConfig {
    DENORM = CL_FP_DENORM,
    INF_NAN = CL_FP_INF_NAN,
    ROUND_TO_NEAREST = CL_FP_ROUND_TO_NEAREST,
    ROUND_TO_ZERO = CL_FP_ROUND_TO_ZERO,
    ROUND_TO_INF = CL_FP_ROUND_TO_INF,
    FMA = CL_FP_FMA,
    SOFT_FLOAT = CL_FP_SOFT_FLOAT
};

struct FPConfigFlags : public Flags<FPConfig> {
    FPConfigFlags(const FPConfigFlags & flag);
    FPConfigFlags(const Flags<FPConfig> & flags);
    operator cl_device_fp_config() const;
};

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

struct MemoryFlags : public Flags<MemoryFlag> {
    MemoryFlags(const MemoryFlag & flag);
    MemoryFlags(const Flags<MemoryFlag> & flags);
    operator cl_mem_flags() const;
};

enum MapFlag {
    READ = CL_MAP_READ,
    WRITE = CL_MAP_WRITE,
    WRITE_INVALIDATE_REGION = CL_MAP_WRITE_INVALIDATE_REGION
};

struct MapFlags : public Flags<MapFlag> {
    MapFlags(const MapFlag & flag);
    MapFlags(const Flags<MapFlag> & flags);
    operator cl_map_flags() const;
};

enum CommandExecutionStatus {
    QUEUED = CL_QUEUED,
    SUBMITTED = CL_SUBMITTED,
    RUNNING = CL_RUNNING,
    COMPLETE = CL_COMPLETE,
    ERROR
};

} // namespace openclcpp_lite
