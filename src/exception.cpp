#include "openclcpp-lite/exception.h"

namespace openclcpp_lite {

Exception::Exception(const std::string & error_msg) : err(error_msg) {}

const char *
Exception::what() const noexcept
{
    return this->err.c_str();
}


} // namespace openclcpp_lite
