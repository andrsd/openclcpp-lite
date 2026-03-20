// SPDX-FileCopyrightText: 2024 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include <string>
#include <exception>
#include <source_location>

namespace openclcpp_lite {

class Exception : public std::exception {
public:
    Exception(const std::string & error_msg,
              const std::source_location location = std::source_location::current());

    const char * what() const noexcept override;

private:
    std::string err;
};

} // namespace openclcpp_lite
