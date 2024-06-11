// SPDX-FileCopyrightText: 2024 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#include "openclcpp-lite/platform.h"
#include "openclcpp-lite/device.h"
#include "openclcpp-lite/utils.h"
#include "fmt/printf.h"
#include "cxxopts/cxxopts.hpp"

namespace ocl = openclcpp_lite;

bool verbose = false;

cxxopts::Options
build_options()
{
    cxxopts::Options options("ocl-info", "Get information about OpenCL");
    // clang-format off
    options.add_options()
        ("V,verbose", "Verbose output", cxxopts::value<bool>()->default_value("false"))
        ("h,help", "Print usage")
    ;
    // clang-format on
    return options;
}

std::string
device_type_str(ocl::Device::Type type)
{
    switch (type) {
    case ocl::Device::CPU:
        return "CPU";
    case ocl::Device::GPU:
        return "GPU";
    case ocl::Device::ACCELERATOR:
        return "Accelerator";
    case ocl::Device::CUSTOM:
        return "Custom";
    default:
        return "Unknown";
    }
}

std::string
human_size(unsigned long size)
{
    if (size < 1024)
        return fmt::format("{} B", size);
    else if (size < 1048576)
        return fmt::format("{:.2} kB", size / 1024.);
    else if (size < 1073741824)
        return fmt::format("{:.2} MB", size / 1048576.);
    else
        return fmt::format("{:.2} GB", size / 1073741824.);
}

std::string
bool_str(bool val)
{
    if (val)
        return "yes";
    else
        return "no";
}

template <typename T>
std::string
join(const std::string & joiner, const std::vector<T> & vals)
{
    if (vals.size() > 0) {
        std::string str = fmt::format("{}", vals[0]);
        for (int i = 1; i < vals.size(); i++) {
            str += joiner;
            str += fmt::format("{}", vals[i]);
        }
        return str;
    }
    else
        return "";
}

void
print_device_info(int id, const ocl::Device & dev)
{
    fmt::print("      - name: {}\n", dev.name());
    fmt::print("        id: {}\n", id);
    fmt::print("        type: {}\n", device_type_str(dev.type()));
    fmt::print("        OpenCL version: {}\n", dev.open_cl_version());
    if (verbose) {
        fmt::print("        vendor: {}\n", dev.vendor());
        fmt::print("        vendor id: {}\n", dev.vendor_id());
        fmt::print("        version: {}\n", dev.version());
        fmt::print("        profile: {}\n", dev.profile());
        fmt::print("        driver version: {}\n", dev.driver_version());
        fmt::print("        address bits: {}\n", dev.address_bits());
        fmt::print("        compiler available: {}\n", dev.compiler_available());
        fmt::print("        global mem size: {}\n", human_size(dev.global_mem_size()));
        fmt::print("        global mem cache size: {}\n", human_size(dev.global_mem_cache_size()));
        fmt::print("        global mem cache line size: {}\n",
                   human_size(dev.global_mem_cache_line_size()));
        fmt::print("        local mem size: {}\n", human_size(dev.local_mem_size()));
        fmt::print("        built-in kernels: {}\n",
                   dev.built_in_kernels().empty() ? "none"
                                                  : ocl::utils::join(" ", dev.built_in_kernels()));
        fmt::print("        endianess: {}\n", dev.endian_little() ? "little-endian" : "big-endian");
        fmt::print("        error correction support: {}\n",
                   bool_str(dev.error_correction_support()));
        fmt::print("        extensions:\n");
        for (auto & e : dev.extensions())
            fmt::print("          - {}\n", e);
        fmt::print("        host unified memory: {}\n", bool_str(dev.host_unified_memory()));
        fmt::print("        image support: {}\n", bool_str(dev.image_support()));
        fmt::print("        max 2D image size (W x H): {} x {}\n",
                   human_size(dev.image2d_max_width()),
                   human_size(dev.image2d_max_height()));
        fmt::print("        max 3D image size (D x H x W): {} x {} x {}\n",
                   human_size(dev.image3d_max_depth()),
                   human_size(dev.image3d_max_height()),
                   human_size(dev.image3d_max_width()));
        fmt::print("        image max buffer size: {}\n", human_size(dev.image_max_buffer_size()));
        fmt::print("        image max array size: {}\n", human_size(dev.image_max_array_size()));
        fmt::print("        linker available: {}\n", bool_str(dev.linker_available()));
        fmt::print("        max clock frequency: {} MHz\n", dev.max_clock_frequency());
        fmt::print("        max compute units: {}\n", dev.max_compute_units());
        fmt::print("        max constant args: {}\n", dev.max_constant_args());
        fmt::print("        max constant buffer size: {}\n",
                   human_size(dev.max_constant_buffer_size()));
        fmt::print("        max mem alloc size: {}\n", human_size(dev.max_mem_alloc_size()));
        fmt::print("        max parameter size: {}\n", human_size(dev.max_parameter_size()));
        fmt::print("        max read image args: {}\n", dev.max_read_image_args());
        fmt::print("        max work group size: {}\n", dev.max_work_group_size());
        fmt::print("        max work item dimensions: {}\n", dev.max_work_item_dimensions());
        fmt::print("        max work item sizes: {}\n", join(", ", dev.max_work_item_sizes()));
        fmt::print("        max write image args: {}\n", dev.max_write_image_args());
        fmt::print("        mem base address align: {} B\n", dev.mem_base_addr_align() / 8);
        fmt::print("        min data type align size: {} B\n", dev.min_data_type_align_size());
        fmt::print("        partition max sub devices: {}\n", dev.partition_max_sub_devices());
        fmt::print("        profiling timer resolution: {} ns\n", dev.profiling_timer_resolution());
    }
}

void
print_platform_info(int id, const ocl::Platform & platform)
{
    fmt::print("platforms:\n");
    fmt::print("  - name: {}\n", platform.name());
    fmt::print("    id: {}\n", id);
    if (verbose) {
        fmt::print("    version: {}\n", platform.version());
        fmt::print("    vendor: {}\n", platform.vendor());
        fmt::print("    profile: {}\n", platform.profile());
        fmt::print("    extensions:\n");
        for (auto & e : platform.extensions())
            fmt::print("      - {}\n", e);
    }
    fmt::print("    devices:\n");
    auto dev_id = 0;
    for (auto & d : platform.devices()) {
        print_device_info(dev_id, d);
        dev_id++;
    }
}

void
print_info()
{
    auto platforms = ocl::Platform::platforms();
    auto id = 0;
    for (auto & p : platforms) {
        print_platform_info(id, p);
        id++;
    }
}

int
main(int argc, char * argv[])
{
    try {
        auto options = build_options();
        auto result = options.parse(argc, argv);
        verbose = result["verbose"].as<bool>();
        if (result["help"].count() == 1)
            fmt::print("{}\n", options.help());
        else
            print_info();
        return 0;
    }
    catch (cxxopts::exceptions::exception & e) {
        fmt::print("{}\n", e.what());
        return 1;
    }
}
