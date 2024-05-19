#pragma once

#include "openclcpp-lite/cl.h"
#include <string>

namespace openclcpp_lite {

namespace internal {

std::string error_message(cl_int ierr);

void check_error(cl_int ierr, const char * file, int line);

} // namespace internal

#define OPENCL_CHECK(ierr) openclcpp_lite::internal::check_error(ierr, __FILE__, __LINE__)

} // namespace openclcpp_lite
