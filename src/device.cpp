// SPDX-FileCopyrightText: 2024 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#include "openclcpp-lite/device.h"
#include "openclcpp-lite/utils.h"
#include "openclcpp-lite/context.h"

namespace openclcpp_lite {

std::once_flag Device::have_default;
Device Device::default_device;

Device::Device() : id(nullptr) {}

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

std::vector<std::string>
Device::built_in_kernels() const
{
    auto str = get_info<std::string>(CL_DEVICE_BUILT_IN_KERNELS);
    return utils::split(str, ";");
}

bool
Device::compiler_available() const
{
    return get_info<cl_bool>(CL_DEVICE_COMPILER_AVAILABLE);
}

bool
Device::endian_little() const
{
    return get_info<cl_bool>(CL_DEVICE_ENDIAN_LITTLE);
}

bool
Device::error_correction_support() const
{
    return get_info<cl_bool>(CL_DEVICE_ERROR_CORRECTION_SUPPORT);
}

std::vector<std::string>
Device::extensions() const
{
    auto str = get_info<std::string>(CL_DEVICE_EXTENSIONS);
    return utils::split(str, " ");
}

unsigned long
Device::global_mem_cache_size() const
{
    return get_info<cl_ulong>(CL_DEVICE_GLOBAL_MEM_CACHE_SIZE);
}

unsigned int
Device::global_mem_cache_line_size() const
{
    return get_info<cl_uint>(CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE);
}

unsigned long
Device::global_mem_size() const
{
    return get_info<cl_ulong>(CL_DEVICE_GLOBAL_MEM_SIZE);
}

bool
Device::host_unified_memory() const
{
    return get_info<cl_bool>(CL_DEVICE_HOST_UNIFIED_MEMORY);
}

bool
Device::image_support() const
{
    return get_info<cl_bool>(CL_DEVICE_IMAGE_SUPPORT);
}

size_t
Device::image2d_max_height() const
{
    return get_info<size_t>(CL_DEVICE_IMAGE2D_MAX_HEIGHT);
}

size_t
Device::image2d_max_width() const
{
    return get_info<size_t>(CL_DEVICE_IMAGE2D_MAX_WIDTH);
}

size_t
Device::image3d_max_depth() const
{
    return get_info<size_t>(CL_DEVICE_IMAGE3D_MAX_DEPTH);
}

size_t
Device::image3d_max_height() const
{
    return get_info<size_t>(CL_DEVICE_IMAGE3D_MAX_HEIGHT);
}

size_t
Device::image3d_max_width() const
{
    return get_info<size_t>(CL_DEVICE_IMAGE3D_MAX_WIDTH);
}

size_t
Device::image_max_buffer_size() const
{
    return get_info<size_t>(CL_DEVICE_IMAGE_MAX_BUFFER_SIZE);
}

size_t
Device::image_max_array_size() const
{
    return get_info<size_t>(CL_DEVICE_IMAGE_MAX_ARRAY_SIZE);
}

bool
Device::linker_available() const
{
    return get_info<cl_bool>(CL_DEVICE_LINKER_AVAILABLE);
}

unsigned long
Device::local_mem_size() const
{
    return get_info<cl_ulong>(CL_DEVICE_LOCAL_MEM_SIZE);
}

unsigned int
Device::max_clock_frequency() const
{
    return get_info<cl_uint>(CL_DEVICE_MAX_CLOCK_FREQUENCY);
}

unsigned int
Device::max_compute_units() const
{
    return get_info<cl_uint>(CL_DEVICE_MAX_COMPUTE_UNITS);
}

unsigned int
Device::max_constant_args() const
{
    return get_info<cl_uint>(CL_DEVICE_MAX_CONSTANT_ARGS);
}

unsigned long
Device::max_constant_buffer_size() const
{
    return get_info<cl_ulong>(CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE);
}

unsigned long
Device::max_mem_alloc_size() const
{
    return get_info<cl_ulong>(CL_DEVICE_MAX_MEM_ALLOC_SIZE);
}

size_t
Device::max_parameter_size() const
{
    return get_info<size_t>(CL_DEVICE_MAX_PARAMETER_SIZE);
}

unsigned int
Device::max_read_image_args() const
{
    return get_info<cl_uint>(CL_DEVICE_MAX_READ_IMAGE_ARGS);
}

unsigned int
Device::max_samplers() const
{
    return get_info<cl_uint>(CL_DEVICE_MAX_SAMPLERS);
}

size_t
Device::max_work_group_size() const
{
    return get_info<size_t>(CL_DEVICE_MAX_WORK_GROUP_SIZE);
}

unsigned int
Device::max_work_item_dimensions() const
{
    return get_info<size_t>(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS);
}

std::vector<size_t>
Device::max_work_item_sizes() const
{
    return get_info<std::vector<size_t>>(CL_DEVICE_MAX_WORK_ITEM_SIZES);
}

unsigned int
Device::max_write_image_args() const
{
    return get_info<cl_uint>(CL_DEVICE_MAX_WRITE_IMAGE_ARGS);
}

unsigned int
Device::mem_base_addr_align() const
{
    return get_info<cl_uint>(CL_DEVICE_MEM_BASE_ADDR_ALIGN);
}

unsigned int
Device::min_data_type_align_size() const
{
    return get_info<cl_uint>(CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE);
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

unsigned int
Device::partition_max_sub_devices() const
{
    return get_info<cl_uint>(CL_DEVICE_PARTITION_MAX_SUB_DEVICES);
}

std::string
Device::profile() const
{
    return get_info<std::string>(CL_DEVICE_PROFILE);
}

size_t
Device::profiling_timer_resolution() const
{
    return get_info<size_t>(CL_DEVICE_PROFILING_TIMER_RESOLUTION);
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

Device
Device::get_default()
{
    std::call_once(Device::have_default, []() {
        auto context = Context::get_default();
        auto dev = context.devices()[0];
        Device::default_device = Device(dev);
    });
    return default_device;
}

} // namespace openclcpp_lite
