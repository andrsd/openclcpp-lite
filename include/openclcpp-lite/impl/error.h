#pragma once

#include "exception.h"
#include "cl.h"
#ifdef OPENCLCPP_LITE_WITH_FMT
    #include "fmt/printf.h"
#else
    #include <stringstream>
#endif
#include <map>
#include <string>

namespace openclcpp_lite {

namespace internal {

inline std::string
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
        { CL_DEVICE_NOT_AVAILABLE, "Device not available" }
    };
    // clang-format on

    auto it = error_msg.find(ierr);
    if (it != error_msg.end()) {
        return it->second;
    }
    else
        return "Unknown error";
}

inline void
check_error(cl_int ierr, const char * file, int line)
{
    if (ierr != CL_SUCCESS) {
#ifdef OPENCLCPP_LITE_WITH_FMT
        std::string err = fmt::format("[ERROR] OpenCL error {} at {}:{}: {}",
                                      ierr,
                                      file,
                                      line,
                                      error_message(ierr));
        throw Exception(err);
#else
        std::stringstream ss;
        ss << "[ERROR] OpenCL error " << ierr << " at " << file << ":" << line << ": "
           << error_message(ierr);
        throw Exception(ss.str());
#endif
    }
}

} // namespace internal

#define OPENCL_CHECK(ierr) openclcpp_lite::internal::check_error(ierr, __FILE__, __LINE__)

} // namespace openclcpp_lite
