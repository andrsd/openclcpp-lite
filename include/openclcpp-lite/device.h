#pragma once

#include "openclcpp-lite/cl.h"
#include "openclcpp-lite/error.h"
#include <vector>
#include <string>

namespace openclcpp_lite {

class Platform;

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

    /// The default compute device address space size specified as an unsigned integer value in
    /// bits. Currently supported values are 32 or 64 bits.
    cl_uint address_bits() const;

    /// Is `true` if the device is available and `false` if the device is not available.
    bool available() const;

    /// Is `false` if the implementation does not have a compiler available to compile the program
    /// source. Is `true` if the compiler is available. This can be `false` for the embedded
    /// platform profile only.
    bool compiler_available() const;

    /// Size of global memory cache line in bytes
    unsigned long global_mem_cache_size() const;

    /// Size of global device memory in bytes
    unsigned long global_mem_size() const;

    /// Size of local memory arena in bytes. The minimum value is 32 KB for devices that are not of
    /// type `CUSTOM`.
    unsigned long local_mem_size() const;

    /// Maximum number of work-items in a work-group executing a kernel on a single compute unit,
    /// using the data parallel execution model. (Refer to `clEnqueueNDRangeKernel`). The minimum
    /// value is `1`.
    size_t max_work_group_size() const;

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

    /// OpenCL profile string. Returns the profile name supported by the device (see note). The
    /// profile name returned can be one of the following strings:
    ///
    /// `FULL_PROFILE` - if the device supports the OpenCL specification (functionality defined as
    /// part of the core specification and does not require any extensions to be supported).
    ///
    /// `EMBEDDED_PROFILE` - if the device supports the OpenCL embedded profile.
    std::string profile() const;

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
    Device(cl_device_id id);

    template <typename T>
    T get_info(cl_device_info name) const;

    template <>
    std::string get_info(cl_device_info name) const;

    cl_device_id id;

    friend class Platform;
    friend class Context;
    friend class Queue;
    friend class Program;
};

} // namespace openclcpp_lite
