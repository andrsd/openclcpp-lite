// SPDX-FileCopyrightText: 2024 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#include "openclcpp-lite/error.h"
#include "openclcpp-lite/exception.h"
#include "openclcpp-lite/cl.h"
#ifdef OPENCLCPP_LITE_WITH_FMT
    #include "fmt/printf.h"
#else
    #include <sstream>
#endif
#include <map>

namespace openclcpp_lite {

namespace internal {

std::string
error_message(cl_int ierr)
{
    // clang-format off
    static std::map<cl_int, std::string> error_msg = {
        { CL_DEVICE_NOT_FOUND, "Device not found"},
        { CL_DEVICE_NOT_AVAILABLE, "Device not available" },
        { CL_COMPILER_NOT_AVAILABLE, "Compiler not available" },
        { CL_MEM_OBJECT_ALLOCATION_FAILURE, "" },
        { CL_OUT_OF_RESOURCES, "Out of resources" },
        { CL_OUT_OF_HOST_MEMORY, "Out of host memory" },
        { CL_PROFILING_INFO_NOT_AVAILABLE, "" },
        { CL_MEM_COPY_OVERLAP, "Mem copy overlap" },
        { CL_IMAGE_FORMAT_MISMATCH, "Image format mismatch" },
        { CL_IMAGE_FORMAT_NOT_SUPPORTED, "Image format not supported" },
        { CL_BUILD_PROGRAM_FAILURE, "Build program failure" },
        { CL_MAP_FAILURE, "Map failure" },
        { CL_MISALIGNED_SUB_BUFFER_OFFSET, "Misaligned sub-buffer offset" },
        { CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST, "Execute status error for events in wait list" },
        { CL_COMPILE_PROGRAM_FAILURE, "Compile program failure" },
        { CL_LINKER_NOT_AVAILABLE, "Linker not available" },
        { CL_LINK_PROGRAM_FAILURE, "Link program failure" },
        { CL_DEVICE_PARTITION_FAILED, "Device partition failed" },
        { CL_KERNEL_ARG_INFO_NOT_AVAILABLE, "Kernel argument info not available" },

        { CL_INVALID_VALUE, "Invalid value" },
        { CL_INVALID_DEVICE_TYPE, "Invalid device type" },
        { CL_INVALID_PLATFORM, "Invalid platform" },
        { CL_INVALID_DEVICE, "Invalid device" },
        { CL_INVALID_CONTEXT, "Invalid context" },
        { CL_INVALID_QUEUE_PROPERTIES, "Invalid queue properties" },
        { CL_INVALID_COMMAND_QUEUE, "Invalid commnad queue" },
        { CL_INVALID_HOST_PTR, "Invalid host ptr" },
        { CL_INVALID_MEM_OBJECT, "Invalid memory object" },
        { CL_INVALID_IMAGE_FORMAT_DESCRIPTOR, "Invalid image format descriptor" },
        { CL_INVALID_IMAGE_SIZE, "Invalid image size" },
        { CL_INVALID_SAMPLER, "Invalid sampler" },
        { CL_INVALID_BINARY, "Invalid binary" },
        { CL_INVALID_BUILD_OPTIONS, "Invalid build options" },
        { CL_INVALID_PROGRAM, "Invalid program" },
        { CL_INVALID_PROGRAM_EXECUTABLE, "Invalid program executable" },
        { CL_INVALID_KERNEL_NAME, "Invalid kernel name" },
        { CL_INVALID_KERNEL_DEFINITION, "Invalid kernel definition" },
        { CL_INVALID_KERNEL, "Invalid kernel" },
        { CL_INVALID_ARG_INDEX, "Invalid argument index" },
        { CL_INVALID_ARG_VALUE, "Invalid argument value" },
        { CL_INVALID_ARG_SIZE, "Invalid argument size" },
        { CL_INVALID_KERNEL_ARGS, "Invalid kernel arguments" },
        { CL_INVALID_WORK_DIMENSION, "Invalid work dimension" },
        { CL_INVALID_WORK_GROUP_SIZE, "Invalid work group size" },
        { CL_INVALID_WORK_ITEM_SIZE, "invalid work item size" },
        { CL_INVALID_GLOBAL_OFFSET, "Invalid global offset" },
        { CL_INVALID_EVENT_WAIT_LIST, "Invalid event wait list" },
        { CL_INVALID_EVENT, "invalid event" },
        { CL_INVALID_OPERATION, "Invalid operation" },
        { CL_INVALID_GL_OBJECT, "Invalid GL object" },
        { CL_INVALID_BUFFER_SIZE, "Invalid buffer size" },
        { CL_INVALID_MIP_LEVEL, "invalid MIP level" },
        { CL_INVALID_GLOBAL_WORK_SIZE, "Invalid global work size" },
        { CL_INVALID_PROPERTY, "Invalid property" },
        { CL_INVALID_IMAGE_DESCRIPTOR, "Invalid image descriptor" },
        { CL_INVALID_COMPILER_OPTIONS, "Invalid compiler options" },
        { CL_INVALID_LINKER_OPTIONS, "Invalid linker options" },
        { CL_INVALID_DEVICE_PARTITION_COUNT, "Invalid device partition count" }
    };
    // clang-format on

    auto it = error_msg.find(ierr);
    if (it != error_msg.end())
        return it->second;
    else {
#ifdef OPENCLCPP_LITE_WITH_FMT
        return fmt::format("Unknown error {}", ierr);
#else
        std::stringstream ss;
        ss << "Unknown error " << ierr;
        return ss.str();
#endif
    }
}

void
check_error(cl_int ierr, const char * file, int line)
{
    if (ierr != CL_SUCCESS) {
#ifdef OPENCLCPP_LITE_WITH_FMT
        std::string err = fmt::format("OpenCL error at {}:{}: {}", file, line, error_message(ierr));
        throw Exception(err);
#else
        std::stringstream ss;
        ss << "OpenCL error at " << file << ":" << line << ": " << error_message(ierr);
        throw Exception(ss.str());
#endif
    }
}

} // namespace internal

} // namespace openclcpp_lite
