// SPDX-FileCopyrightText: 2024 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include <vector>
#include <string>

namespace openclcpp_lite {
namespace utils {

/**
 * Convert supplied string to lower case.
 * @param name The string to convert upper case.
 */
std::string to_lower(const std::string & name);

std::string join(const std::string & joiner, const std::vector<std::string> & items);

std::vector<std::string> split(const std::string & s, const std::string & delimiter);

std::string rtrim_null(const std::string & str);

} // namespace utils
} // namespace openclcpp_lite
