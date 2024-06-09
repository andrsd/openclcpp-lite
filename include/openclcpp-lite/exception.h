// SPDX-FileCopyrightText: 2024 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include "fmt/format.h"
#include <string>
#include <exception>

namespace openclcpp_lite {

class Exception : public std::exception {
public:
    template <typename... T>
    Exception(fmt::format_string<T...> format, T... args)
    {
        this->err = fmt::format(format, std::forward<T>(args)...);
    }

    Exception(const std::string & error_msg);

    const char * what() const noexcept override;

private:
    std::string err;
};

} // namespace openclcpp_lite
