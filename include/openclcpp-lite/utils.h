#pragma once

#include <vector>
#include <string>

namespace openclcpp_lite {
namespace utils {

std::string join(const std::string & joiner, const std::vector<std::string> & items);

std::vector<std::string> split(const std::string & s, const std::string & delimiter);

std::string rtrim_null(const std::string & str);

} // namespace utils
} // namespace openclcpp_lite