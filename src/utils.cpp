// SPDX-FileCopyrightText: 2024 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#include "openclcpp-lite/utils.h"
#include <algorithm>

namespace openclcpp_lite {
namespace utils {

std::string
to_lower(const std::string & name)
{
    std::string lower(name);
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower;
}

std::string
join(const std::string & joiner, const std::vector<std::string> & items)
{
    if (items.size() > 0) {
        std::string str = items[0];
        for (int i = 1; i < items.size(); i++) {
            str += joiner;
            str += items[i];
        }
        return str;
    }
    else
        return "";
}

std::vector<std::string>
split(const std::string & s, const std::string & delimiter)
{
    if (s.empty())
        return {};

    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}

std::string
rtrim_null(const std::string & str)
{
    std::string buffer = str;
    if (const auto pos = buffer.find('\0'); pos != std::string::npos)
        buffer.erase(pos);
    return buffer;
}

std::string
rtrim(const std::string & str)
{
    std::string buffer = str;
    while (buffer[buffer.length() - 1] == ' ')
        buffer.erase(buffer.length() - 1);
    return buffer;
}

bool
starts_with(const std::string & str, const std::string & prefix)
{
    return str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
}

} // namespace utils
} // namespace openclcpp_lite
