#include "openclcpp-lite/error.h"
#include "openclcpp-lite/exception.h"
#include "openclcpp-lite/cl.h"
#ifdef OPENCLCPP_LITE_WITH_FMT
    #include "fmt/printf.h"
#else
    #include <stringstream>
#endif
#include <map>

namespace openclcpp_lite {

namespace internal {

std::string
error_message(cl_int ierr)
{
    // clang-format off
    static std::map<cl_int, std::string> error_msg = {
        { CL_OUT_OF_RESOURCES, "Out of resources" },
        { CL_OUT_OF_HOST_MEMORY, "Out of host memory" },
        { CL_INVALID_VALUE, "Invalid value" },
        { CL_INVALID_PLATFORM, "Invalid platform" },
        { CL_INVALID_PROPERTY, "Invalid property" },
        { CL_INVALID_DEVICE, "Invalid device" },
        { CL_INVALID_OPERATION, "Invalid operations" },
        { CL_INVALID_CONTEXT, "Invalid context" },
        { CL_INVALID_COMMAND_QUEUE, "Invalid commnad queue" },
        { CL_INVALID_PROGRAM, "Invalid program" },
        { CL_INVALID_PROGRAM_EXECUTABLE, "Invalid program executable" },
        { CL_DEVICE_NOT_AVAILABLE, "Device not available" },
        { CL_INVALID_BUFFER_SIZE, "Invalid buffer size" },
        { CL_INVALID_HOST_PTR, "Invalid host pointer" },
        { CL_MEM_OBJECT_ALLOCATION_FAILURE, "Memory object allocation failure" }
    };
    // clang-format on

    auto it = error_msg.find(ierr);
    if (it != error_msg.end())
        return it->second;
    else
        return "Unknown error";
}

void
check_error(cl_int ierr, const char * file, int line)
{
    if (ierr != CL_SUCCESS) {
#ifdef OPENCLCPP_LITE_WITH_FMT
        std::string err = fmt::format("OpenCL error at {}:{}: {}",
                                      file,
                                      line,
                                      error_message(ierr));
        throw Exception(err);
#else
        std::stringstream ss;
        ss << "OpenCL error at " << file << ":" << line << ": "
           << error_message(ierr);
        throw Exception(ss.str());
#endif
    }
}

} // namespace internal

} // namespace openclcpp_lite
