// SPDX-FileCopyrightText: 2024 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#include "openclcpp-lite/exception.h"
#include "fmt/core.h"

namespace openclcpp_lite {

Exception::Exception(const std::string & error_msg, const std::source_location location) :
    err(fmt::format("opencl-cpp-lite: exception raised at {}:{}: {}",
                    location.file_name(),
                    location.line(),
                    error_msg))
{
}

const char *
Exception::what() const noexcept
{
    return this->err.c_str();
}

} // namespace openclcpp_lite
