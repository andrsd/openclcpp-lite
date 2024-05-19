#pragma once

#include "cl.h"
#include "error.h"
#include "device.h"
#include <vector>

namespace openclcpp_lite {

class Platform {
public:
    static std::vector<Platform> platforms();

    /// Platform name
    std::string
    name() const
    {
        return get_info_string(CL_PLATFORM_NAME);
    }

    /// Platform vendor
    std::string
    vendor() const
    {
        return get_info_string(CL_PLATFORM_VENDOR);
    }

    /// OpenCL version string
    std::string
    version() const
    {
        return get_info_string(CL_PLATFORM_VERSION);
    }

    /// A space-separated list of extension names
    std::string
    extensions() const
    {
        return get_info_string(CL_PLATFORM_EXTENSIONS);
    }

    /// OpenCL profile string
    std::string
    profile() const
    {
        return get_info_string(CL_PLATFORM_PROFILE);
    }

    /// List of devices available on a platform
    std::vector<Device> devices(Device::Type type = Device::ALL) const;

    operator cl_platform_id() const { return this->id; }

private:
    Platform(cl_platform_id id) : id(id) {}

    std::string
    get_info_string(cl_platform_info info) const
    {
        std::size_t sz;
        std::string str;
        OPENCL_CHECK(clGetPlatformInfo(this->id, info, 0, nullptr, &sz));
        str.resize(sz + 1);
        OPENCL_CHECK(clGetPlatformInfo(this->id, info, sz, str.data(), nullptr));
        return str;
    }

    cl_platform_id id;
};

inline std::vector<Platform>
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

inline std::vector<Platform>
get_platforms()
{
    return Platform::platforms();
}

inline std::vector<Device>
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
