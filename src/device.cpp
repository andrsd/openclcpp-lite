#include "openclcpp-lite/device.h"

namespace openclcpp_lite {

Device::Device(cl_device_id id) : id(id) {}

cl_uint
Device::address_bits() const
{
    return get_info<cl_uint>(CL_DEVICE_ADDRESS_BITS);
}

bool
Device::available() const
{
    return get_info<cl_bool>(CL_DEVICE_AVAILABLE);
}

bool
Device::compiler_available() const
{
    return get_info<cl_bool>(CL_DEVICE_COMPILER_AVAILABLE);
}

unsigned long
Device::global_mem_cache_size() const
{
    return get_info<cl_ulong>(CL_DEVICE_GLOBAL_MEM_CACHE_SIZE);
}

unsigned long
Device::global_mem_size() const
{
    return get_info<cl_ulong>(CL_DEVICE_GLOBAL_MEM_SIZE);
}

unsigned long
Device::local_mem_size() const
{
    return get_info<cl_ulong>(CL_DEVICE_LOCAL_MEM_SIZE);
}

size_t
Device::max_work_group_size() const
{
    return get_info<size_t>(CL_DEVICE_MAX_WORK_GROUP_SIZE);
}

std::string
Device::name() const
{
    return get_info<std::string>(CL_DEVICE_NAME);
}

std::string
Device::open_cl_version() const
{
    return get_info<std::string>(CL_DEVICE_OPENCL_C_VERSION);
}

std::string
Device::profile() const
{
    return get_info<std::string>(CL_DEVICE_PROFILE);
}

std::string
Device::vendor() const
{
    return get_info<std::string>(CL_DEVICE_VENDOR);
}

unsigned int
Device::vendor_id() const
{
    return get_info<cl_uint>(CL_DEVICE_VENDOR_ID);
}

std::string
Device::version() const
{
    return get_info<std::string>(CL_DEVICE_VERSION);
}

std::string
Device::driver_version() const
{
    return get_info<std::string>(CL_DRIVER_VERSION);
}

Device::Type
Device::type() const
{
    auto t = get_info<cl_device_type>(CL_DEVICE_TYPE);
    return static_cast<Type>(t);
}

template <typename T>
T
Device::get_info(cl_device_info name) const
{
    std::size_t sz;
    OPENCL_CHECK(clGetDeviceInfo(this->id, name, 0, nullptr, &sz));
    T val;
    OPENCL_CHECK(clGetDeviceInfo(this->id, name, sizeof(T), &val, nullptr));
    return val;
}

template <>
std::string
Device::get_info(cl_device_info name) const
{
    std::size_t sz;
    OPENCL_CHECK(clGetDeviceInfo(this->id, name, 0, nullptr, &sz));
    std::string val;
    val.resize(sz);
    OPENCL_CHECK(clGetDeviceInfo(this->id, name, sz, &val, nullptr));
    return val;
}

} // namespace openclcpp_lite
