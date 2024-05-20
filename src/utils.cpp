#include "openclcpp-lite/utils.h"

namespace openclcpp_lite {
namespace utils {

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

} // namespace utils
} // namespace openclcpp_lite
