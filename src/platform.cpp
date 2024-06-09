// SPDX-FileCopyrightText: 2024 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#include "openclcpp-lite/platform.h"
#include "openclcpp-lite/utils.h"

namespace openclcpp_lite {

std::string
Platform::name() const
{
    return get_info<std::string>(CL_PLATFORM_NAME);
}

std::string
Platform::vendor() const
{
    return get_info<std::string>(CL_PLATFORM_VENDOR);
}

std::string
Platform::version() const
{
    return get_info<std::string>(CL_PLATFORM_VERSION);
}

std::string
Platform::extensions() const
{
    return get_info<std::string>(CL_PLATFORM_EXTENSIONS);
}

std::string
Platform::profile() const
{
    return get_info<std::string>(CL_PLATFORM_PROFILE);
}

Platform::Platform() {}

Platform::Platform(cl_platform_id id) : id(id) {}

std::vector<Platform>
Platform::platforms()
{
    cl_uint n_platforms;
    OPENCL_CHECK(clGetPlatformIDs(0, nullptr, &n_platforms));
    std::vector<cl_platform_id> ids;
    ids.resize(n_platforms);
    OPENCL_CHECK(clGetPlatformIDs(n_platforms, ids.data(), nullptr));
    std::vector<Platform> plats;
    for (auto & id : ids)
        plats.emplace_back(Platform(id));
    return plats;
}

std::vector<Platform>
get_platforms()
{
    return Platform::platforms();
}

std::vector<Device>
Platform::devices(Device::Type type) const
{
    cl_uint num;
    OPENCL_CHECK(clGetDeviceIDs(this->id, type, 0, nullptr, &num));
    std::vector<cl_device_id> dev_ids;
    dev_ids.resize(num);
    OPENCL_CHECK(clGetDeviceIDs(this->id, type, num, dev_ids.data(), nullptr));
    std::vector<Device> devs;
    for (auto & id : dev_ids)
        devs.emplace_back(Device(id));
    return devs;
}

} // namespace openclcpp_lite
