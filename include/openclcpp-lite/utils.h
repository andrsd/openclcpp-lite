// SPDX-FileCopyrightText: 2024 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include "openclcpp-lite/exception.h"
#include <vector>
#include <string>
#include <fstream>

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

/// Trim whitespace characters from right
std::string rtrim(const std::string & str);

bool starts_with(const std::string & str, const std::string & start);

/// Read a text file into a `string`
///
/// @param file_name File name
/// @return Content of the file
std::string read_file_text(const std::string & file_name);

/// Read a binary file into a vector of `char`s
///
/// @param file_name File name
/// @return Content of the file
std::vector<char> read_file_bin(const std::string & file_name);

} // namespace utils
} // namespace openclcpp_lite
