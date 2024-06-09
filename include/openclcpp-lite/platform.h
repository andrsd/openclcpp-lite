// SPDX-FileCopyrightText: 2024 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include "openclcpp-lite/cl.h"
#include "openclcpp-lite/templ.h"
#include "openclcpp-lite/error.h"
#include "openclcpp-lite/device.h"
#include <vector>

namespace openclcpp_lite {

/// OpenCL platform
class Platform {
public:
    /// Get list of available platforms
    ///
    /// @return List of available platforms
    static std::vector<Platform> platforms();

    /// Create empty platform
    Platform();

    /// Create a platform from a OpenCL platform
    explicit Platform(cl_platform_id id);

    /// Platform name
    std::string name() const;

    /// Platform vendor
    std::string vendor() const;

    /// OpenCL version string
    std::string version() const;

    /// A space-separated list of extension names
    std::vector<std::string> extensions() const;

    /// OpenCL profile string
    std::string profile() const;

    /// List of devices available on a platform
    std::vector<Device> devices(Device::Type type = Device::ALL) const;

    operator cl_platform_id() const { return this->id; }

private:
    template <typename T>
    T
    get_info(cl_platform_info name) const
    {
        T val;
        get_info_helper(clGetPlatformInfo, this->id, name, val);
        return val;
    }

    /// Underlying OpenCL platform
    cl_platform_id id;
};

std::vector<Platform> get_platforms();

} // namespace openclcpp_lite
