// SPDX-FileCopyrightText: 2024 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#include "openclcpp-lite/platform.h"
#include "openclcpp-lite/device.h"
#include "openclcpp-lite/context.h"
#include "openclcpp-lite/program.h"
#include "openclcpp-lite/exception.h"
#include "openclcpp-lite/utils.h"
#include <map>
#include <iostream>
#include <fstream>

namespace ocl = openclcpp_lite;

enum Command { COMPILE, BUILD };

ocl::Platform target_platform = ocl::Platform::get_default();
ocl::Device target_device;
Command command = BUILD;

std::map<std::string, std::string> platform_name_map = {
    // clang-format off
    { "amd", "AMD Accelerated Parallel Processing" },
    { "intel", "Intel(R) OpenCL" },
    { "beignet", "Intel Gen OCL Driver" },
    { "nvidia", "NVIDIA CUDA" },
    { "apple", "Apple" },
    { "pocl", "Portable Computing Language" },
    { "qualcomm", "QUALCOMM Snapdragon(TM)" },
    { "arm", "ARM Platform" }
    // clang-format on
};

std::vector<std::string> ocl_compiler_opts;
std::string file_name;

/// Set target platform
///
/// @param platform_name Plaform name (from `platform_name_map`) to select
void
set_target_platform(const std::string & platform_name)
{
    auto it = platform_name_map.find(platform_name);
    if (it != platform_name_map.end()) {
        auto requested_platform = it->second;
        for (auto & p : ocl::get_platforms()) {
            if (p.name() == requested_platform) {
                target_platform = p;
                return;
            }
        }
        throw ocl::Exception("Requested platform '{}' is not available", platform_name);
    }
    else
        throw ocl::Exception("Unknown platform '{}' specified", platform_name);
}

void
set_target_device(const std::string & device_name)
{
}

/// Read a file into a string
///
/// @param file_name Name of the file to read
/// @return Content of the file
std::string
read_file(const std::string & file_name)
{
    std::string src;
    std::ifstream ifs;
    ifs.open(file_name, std::ifstream::in);
    char ch;
    while (ifs.get(ch))
        src += ch;
    ifs.close();
    return src;
}

int
compile(const std::string & file_name)
{
    auto src = read_file(file_name);
    auto dev = target_platform.devices()[0];
    ocl::Context ctx(dev);
    auto prg = ocl::Program::from_source(ctx, src);
    try {
        prg.compile(ocl_compiler_opts);
        return 0;
    }
    catch (ocl::Exception & e) {
        auto status = prg.build_status(dev);
        if (status == ocl::Program::BuildStatus::ERROR) {
            auto log = prg.build_log(dev);
            fmt::print(stderr, "{}\n", file_name);
            fmt::print(stderr, "{}\n", log);
        }
        return 1;
    }
}

int
build(const std::string & file_name)
{
    auto src = read_file(file_name);
    auto dev = target_platform.devices()[0];
    ocl::Context ctx(dev);
    auto prg = ocl::Program::from_source(ctx, src);
    try {
        prg.build({ dev }, ocl_compiler_opts);
        return 0;
    }
    catch (ocl::Exception & e) {
        auto status = prg.build_status(dev);
        if (status == ocl::Program::BuildStatus::ERROR) {
            auto log = prg.build_log(dev);
            fmt::print(stderr, "{}\n", file_name);
            fmt::print(stderr, "{}\n", log);
        }
        return 1;
    }
}

std::string
next_arg(int idx, int argc, char * argv[])
{
    if (idx < argc)
        return std::string(argv[idx]);
    else
        throw ocl::Exception("Missing argument");
}

void
parse_command_line(int argc, char * argv[])
{
    for (int i = 1; i < argc; i++) {
        std::string arg(argv[i]);
        if (arg == "-c") {
            command = COMPILE;
        }
        else if (arg == "-p" || arg == "--platform") {
            auto val = next_arg(i + 1, argc, argv);
            i++;
            set_target_platform(val);
        }
        else if (arg == "-d" || arg == "--device") {
            auto val = next_arg(i + 1, argc, argv);
            fmt::print("device: {}\n", val);
        }
        // Preprocessor Options
        else if (arg == "-D") {
            auto val = next_arg(i + 1, argc, argv);
            ocl_compiler_opts.push_back(arg);
            ocl_compiler_opts.push_back(val);
            i++;
        }
        else if (arg == "-I") {
            auto val = next_arg(i + 1, argc, argv);
            ocl_compiler_opts.push_back(arg);
            ocl_compiler_opts.push_back(val);
            i++;
        }
        // Math Intrinsics Options
        else if (arg == "-cl-single-precision-constant") {
            ocl_compiler_opts.push_back(arg);
        }
        else if (arg == "-cl-denorms-are-zero") {
            ocl_compiler_opts.push_back(arg);
        }
        else if (arg == "-cl-fp32-correctly-rounded-divide-sqrt") {
            ocl_compiler_opts.push_back(arg);
        }
        // Optimization Options
        else if (arg == "-cl-opt-disable") {
            ocl_compiler_opts.push_back(arg);
        }
        else if (arg == "-cl-mad-enable") {
            ocl_compiler_opts.push_back(arg);
        }
        else if (arg == "-cl-no-signed-zeros") {
            ocl_compiler_opts.push_back(arg);
        }
        else if (arg == "-cl-unsafe-math-optimizations") {
            ocl_compiler_opts.push_back(arg);
        }
        else if (arg == "-cl-finite-math-only") {
            ocl_compiler_opts.push_back(arg);
        }
        else if (arg == "-cl-fast-relaxed-math") {
            ocl_compiler_opts.push_back(arg);
        }
        // Options to Request or Suppress Warnings
        else if (arg == "-w") {
            ocl_compiler_opts.push_back(arg);
        }
        else if (arg == "-Werror") {
            ocl_compiler_opts.push_back(arg);
        }
        // Options Controlling the OpenCL C Version
        else if (ocl::utils::starts_with(arg, "-cl-std=")) {
            ocl_compiler_opts.push_back(arg);
        }
        // library linking options
        else if (arg == "-create-library") {
            ocl_compiler_opts.push_back(arg);
        }
        else if (arg == "-enable-link-options") {
            ocl_compiler_opts.push_back(arg);
        }
        // program linking options
        else if (arg == "-cl-denorms-are-zero") {
            ocl_compiler_opts.push_back(arg);
        }
        else if (arg == "-cl-no-signed-zeroes") {
            ocl_compiler_opts.push_back(arg);
        }
        else if (arg == "-cl-unsafe-math-optimizations") {
            ocl_compiler_opts.push_back(arg);
        }
        else if (arg == "-cl-finite-math-only") {
            ocl_compiler_opts.push_back(arg);
        }
        else if (arg == "-cl-fast-relaxed-mat") {
            ocl_compiler_opts.push_back(arg);
        }
        //
        else {
            if (arg[0] == '-')
                throw ocl::Exception("Unrecognized option '{}'", arg);
            else {
                file_name = arg;
            }
        }
    }
}

int
main(int argc, char * argv[])
{
    try {
        parse_command_line(argc, argv);
        if (command == COMPILE)
            return compile(file_name);
        else if (command == BUILD)
            return build(file_name);
    }
    catch (ocl::Exception & e) {
        fmt::print("{}\n", e.what());
        return 1;
    }
}
