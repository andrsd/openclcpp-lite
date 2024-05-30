// SPDX-FileCopyrightText: 2024 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include "openclcpp-lite/cl.h"
#include "openclcpp-lite/templ.h"
#include "openclcpp-lite/error.h"
#include <vector>
#include <string>
#include <mutex>

namespace openclcpp_lite {

/// OpenCL device
class Device {
public:
    enum Type {
        CPU = CL_DEVICE_TYPE_CPU,
        GPU = CL_DEVICE_TYPE_GPU,
        ACCELERATOR = CL_DEVICE_TYPE_ACCELERATOR,
        CUSTOM = CL_DEVICE_TYPE_CUSTOM,
        DEFAULT = CL_DEVICE_TYPE_DEFAULT,
        ALL = CL_DEVICE_TYPE_ALL
    };

    /// Create an null device
    Device();

    /// Create a device from OpenCL device ID
    explicit Device(cl_device_id id);

    /// The default compute device address space size specified as an unsigned integer value in
    /// bits. Currently supported values are 32 or 64 bits.
    cl_uint address_bits() const;

    /// Is `true` if the device is available and `false` if the device is not available.
    bool available() const;

    /// A list of built-in kernels supported by the device. An empty string is returned if no
    /// built-in kernels are supported by the device.
    std::vector<std::string> built_in_kernels() const;

    /// Is `false` if the implementation does not have a compiler available to compile the program
    /// source. Is `true` if the compiler is available. This can be `false` for the embedded
    /// platform profile only.
    bool compiler_available() const;

    /// Is `true` if the device implements error correction for all accesses to compute device
    /// memory (global and constant). Is `false` if the device does not implement such error
    /// correction.
    bool error_correction_support() const;

    /// Returns a list of extension names (the extension names themselves do not contain any spaces)
    /// supported by the device.
    std::vector<std::string> extensions() const;

    /// Is `true` if the OpenCL device is a little endian device and `false` otherwise.
    bool endian_little() const;

    /// Size of global memory cache line in bytes
    unsigned long global_mem_cache_size() const;

    /// Size of global memory cache line in bytes.
    unsigned int global_mem_cache_line_size() const;

    /// Size of global device memory in bytes
    unsigned long global_mem_size() const;

    /// Is `true` if the device and the host have a unified memory subsystem and is `false`
    /// otherwise.
    bool host_unified_memory() const;

    /// Is `true` if images are supported by the OpenCL device and `false` otherwise.
    bool image_support() const;

    /// Max height of 2D image in pixels. The minimum value is 8192 if `image_support` is `true`.
    size_t image2d_max_height() const;

    /// Max width of 2D image or 1D image not created from a buffer object in pixels. The minimum
    /// value is 8192 if `image_support()` is `true`.
    size_t image2d_max_width() const;

    /// Max depth of 3D image in pixels. The minimum value is 2048 if `image_support()` is `true`.
    size_t image3d_max_depth() const;

    /// Max height of 3D image in pixels. The minimum value is 2048 if `image_support()` is `true`.
    size_t image3d_max_height() const;

    /// Max width of 3D image in pixels. The minimum value is 2048 if `image_support()` is `true`.
    size_t image3d_max_width() const;

    /// Max number of pixels for a 1D image created from a buffer object. The minimum value is 65536
    /// if `image_support()` is `true`.
    size_t image_max_buffer_size() const;

    /// Max number of images in a 1D or 2D image array. The minimum value is 2048 if
    /// `image_support()` is `true`
    size_t image_max_array_size() const;

    /// Is `false` if the implementation does not have a linker available. Is `true` if the linker
    /// is available. This can be `false` for the embedded platform profile only. This must be
    /// `true` if `compiler_available` is `true`.
    bool linker_available() const;

    /// Size of local memory arena in bytes. The minimum value is 32 KB for devices that are not
    /// of type `CUSTOM`.
    unsigned long local_mem_size() const;

    /// Maximum configured clock frequency of the device in MHz.
    unsigned int max_clock_frequency() const;

    /// The number of parallel compute units on the OpenCL device. A work-group executes on a single
    /// compute unit. The minimum value is 1.
    unsigned int max_compute_units() const;

    /// Max number of arguments declared with the `__constant` qualifier in a kernel. The minimum
    /// value is 8 for devices that are not of type `CUSTOM`.
    unsigned int max_constant_args() const;

    /// Max size in bytes of a constant buffer allocation. The minimum value is 64 KB for devices
    /// that are not of type `CUSTOM`.
    unsigned long max_constant_buffer_size() const;

    /// Max size of memory object allocation in bytes. The minimum value is max (1/4th of
    /// `global_mem_size()`, 128*1024*1024) for devices that are not of type `CUSTOM`.
    unsigned long max_mem_alloc_size() const;

    /// Max size in bytes of the arguments that can be passed to a kernel. The minimum value is 1024
    /// for devices that are not of type `CUSTOM`. For this minimum value, only a maximum of 128
    /// arguments can be passed to a kernel.
    size_t max_parameter_size() const;

    /// Max number of simultaneous image objects that can be read by a kernel. The minimum value is
    /// 128 if `image_support()` is `true`.
    unsigned int max_read_image_args() const;

    /// Maximum number of samplers that can be used in a kernel. The minimum value is 16 if
    /// `image_support()` is `true`. (Also see Sampler.)
    unsigned int max_samplers() const;

    /// Maximum number of work-items in a work-group executing a kernel on a single compute unit,
    /// using the data parallel execution model. (Refer to `clEnqueueNDRangeKernel`). The minimum
    /// value is `1`.
    size_t max_work_group_size() const;

    /// Maximum dimensions that specify the global and local work-item IDs used by the data parallel
    /// execution model. (Refer to `clEnqueueNDRangeKernel`). The minimum value is 3 for devices
    /// that are not of type `CUSTOM`.
    unsigned int max_work_item_dimensions() const;

    /// Maximum number of work-items that can be specified in each dimension of the work-group to
    /// `clEnqueueNDRangeKernel`.
    ///
    /// Returns n size_t entries, where n is the value returned by the query for
    /// `max_work_item_dimensions`.
    ///
    /// The minimum value is (1, 1, 1) for devices that are not of type `CUSTOM`.
    std::vector<size_t> max_work_item_sizes() const;

    /// Max number of simultaneous image objects that can be written to by a kernel. The minimum
    /// value is 8 if `image_support` is `true`.
    unsigned int max_write_image_args() const;

    /// The minimum value is the size (in bits) of the largest OpenCL built-in data type supported
    /// by the device (`long16` in `FULL` profile, `long16` or `int16` in `EMBEDDED` profile) for
    /// devices that are not of type `CUSTOM`.
    unsigned int mem_base_addr_align() const;

    /// Deprecated in OpenCL 1.2. The smallest alignment in bytes which can be used for any data
    /// type.
    unsigned int min_data_type_align_size() const;

    /// Device name
    std::string name() const;

    /// OpenCL C version string. Returns the highest OpenCL C version supported by the compiler for
    /// this device that is not of type CL_DEVICE_TYPE_CUSTOM. This version string has the following
    /// format:
    ///
    /// ```
    /// OpenCL<space>C<space><major_version.minor_version><space><vendor-specific information>
    /// ```
    ///
    /// The `major_version`.`minor_version` value returned must be `1.2` if `CL_DEVICE_VERSION` is
    /// OpenCL 1.2.
    ///
    /// The `major_version`.`minor_version` value returned must be `1.1` if `CL_DEVICE_VERSION` is
    /// OpenCL 1.1.
    std::string open_cl_version() const;

    /// Returns the maximum number of sub-devices that can be created when a device is partitioned.
    /// The value returned cannot exceed `max_compute_units`.
    unsigned int partition_max_sub_devices() const;

    /// OpenCL profile string. Returns the profile name supported by the device (see note). The
    /// profile name returned can be one of the following strings:
    ///
    /// `FULL_PROFILE` - if the device supports the OpenCL specification (functionality defined as
    /// part of the core specification and does not require any extensions to be supported).
    ///
    /// `EMBEDDED_PROFILE` - if the device supports the OpenCL embedded profile.
    std::string profile() const;

    /// Describes the resolution of device timer. This is measured in nanoseconds.
    size_t profiling_timer_resolution() const;

    /// Vendor name
    std::string vendor() const;

    /// A unique device vendor identifier. An example of a unique device identifier could be the
    /// PCIe ID.
    unsigned int vendor_id() const;

    /// OpenCL version string. Returns the OpenCL version supported by the device. This version
    /// string has the following format:
    ///
    /// ```
    /// OpenCL<space><major_version.minor_version><space><vendor-specific information>
    /// ```
    ///
    /// The `major_version`.`minor_version` value returned will be `1.2`.
    std::string version() const;

    /// OpenCL software driver version string in the form `major_number`.`minor_number`.
    std::string driver_version() const;

    /// The OpenCL device type. Currently supported values are one of or a combination of: `CPU`,
    /// `GPU`, `ACCELERATOR`, `DEFAULT`, a combination of the above types, or `CUSTOM`.
    Type type() const;

    operator cl_device_id() const { return this->id; }

private:
    template <typename T>
    T
    get_info(cl_device_info name) const
    {
        T val;
        get_info_helper(clGetDeviceInfo, this->id, name, val);
        return val;
    }

    cl_device_id id;

public:
    static Device get_default();

private:
    static std::once_flag have_default;
    static Device default_device;
};

} // namespace openclcpp_lite
