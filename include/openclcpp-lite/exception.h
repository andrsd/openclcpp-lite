#pragma once

#include <string>
#include <exception>

namespace openclcpp_lite {

class Exception : public std::exception {
public:
    Exception(const std::string & error_msg);

    const char * what() const noexcept override;

private:
    const std::string err;
};

} // namespace openclcpp_lite
