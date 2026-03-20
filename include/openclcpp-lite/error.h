// SPDX-FileCopyrightText: 2024 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include "openclcpp-lite/cl.h"
#include <string>
#include <source_location>

namespace openclcpp_lite {

namespace internal {

std::string error_message(cl_int ierr);

void check_error(cl_int ierr,
                 const std::source_location location = std::source_location::current());

} // namespace internal

#define OPENCL_CHECK(ierr) openclcpp_lite::internal::check_error(ierr)

} // namespace openclcpp_lite
