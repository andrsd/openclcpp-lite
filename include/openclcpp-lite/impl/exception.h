#pragma once

#include <string>
#include <exception>

namespace openclcpp_lite {

class Exception : public std::exception {
public:
    Exception(const std::string & error_msg) : err(error_msg.c_str()) {}

    const char *
    what() const noexcept override
    {
        return this->err;
    }

private:
    const char * err;
};

} // namespace openclcpp_lite
