#pragma once

#include "openclcpp-lite/cl.h"
#include "openclcpp-lite/templ.h"
#include "openclcpp-lite/error.h"
#include "openclcpp-lite/device.h"
#include <vector>

namespace openclcpp_lite {

class Platform {
public:
    static std::vector<Platform> platforms();

    /// Platform name
    std::string name() const;

    /// Platform vendor
    std::string vendor() const;

    /// OpenCL version string
    std::string version() const;

    /// A space-separated list of extension names
    std::string extensions() const;

    /// OpenCL profile string
    std::string profile() const;

    /// List of devices available on a platform
    std::vector<Device> devices(Device::Type type = Device::ALL) const;

    operator cl_platform_id() const { return this->id; }

private:
    explicit Platform(cl_platform_id id);

    template <typename T>
    T
    get_info(cl_platform_info name) const
    {
        T val;
        get_info_helper(clGetPlatformInfo, this->id, name, val);
        return val;
    }

    cl_platform_id id;
};

std::vector<Platform> get_platforms();

} // namespace openclcpp_lite
