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

std::vector<std::string> options;
std::vector<std::string> file_names;
std::string output_file_name = "a.out";

void
print_help()
{
    fmt::print("OpenCL linker\n");
    fmt::print("\n");
    fmt::print("Usage: ocl-linker [options] <file>...\n");
    fmt::print("\n");
    // clang-format off
    fmt::print(
        "Options:\n"
        "\n"
        "* Linker Options\n"
        "\n"
        "  This specification defines a standard set of linker options that must be supported by\n"
        "  the OpenCL C compiler when linking compiled programs online or offline. These linker\n"
        "  options are categorized as library linking options and program linking options. These\n"
        "  may be extended by a set of vendor- or platform-specific options.\n"
        "\n"
        "* Library Linking Options\n"
        "\n"
        "  The following options can be specified when creating a library of compiled binaries.\n"
        "\n"
        "  -create-library\n"
        "\n"
        "    Create a library of compiled binaries specified in input_programs argument to\n"
        "    clLinkProgram.\n"
        "\n"
        "  -enable-link-options\n"
        "\n"
        "    Allows the linker to modify the library behavior based on one or more link options\n"
        "    (described in Program Linking Options, below) when this library is linked with a\n"
        "    program executable. This option must be specified with the –create-library option.\n"
        "\n"
        "* Program Linking Options\n"
        "\n"
        "  The following options can be specified when linking a program executable.\n"
        "\n"
        "  -cl-denorms-are-zero\n"
        "  -cl-no-signed-zeroes\n"
        "  -cl-unsafe-math-optimizations\n"
        "  -cl-finite-math-only\n"
        "  -cl-fast-relaxed-mat\n"
        "\n"
        "  The linker may apply these options to all compiled program objects specified to\n"
        "  clLinkProgram. The linker may apply these options only to libraries which were created\n"
        "  with the –enable-link-option.\n"
    );
}

/// Set target platform
///
/// @param platform_name Platform name (from `platform_name_map`) to select
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

/// Write binary
///
/// @param file_name Output file name
/// @param bin Binary blob to write
void
write_file(const std::string & file_name, const std::vector<char> & bin)
{
    std::ofstream ofs;
    ofs.open(file_name, std::ofstream::out | std::ofstream::binary);
    ofs.write(bin.data(), bin.size());
    ofs.close();
}

int
link()
{
    auto dev = target_platform.devices()[0];
    ocl::Context ctx(dev);

    std::vector<ocl::Program> objs;
    for (auto & fn : file_names) {
        std::vector<std::vector<char>> bins(1);
        bins[0] = ocl::utils::read_file_bin(fn);
        objs.emplace_back(ocl::Program::from_binary(ctx, { dev }, bins));
    }

    ocl::Program prg;
    try {
        prg = ocl::Program::link(ctx, { dev }, options, objs);
        auto bins = prg.binaries();
        write_file(output_file_name, bins[0]);
        return 0;
    }
    catch (ocl::Exception & e) {
        auto status = prg.build_status(dev);
        if (status == ocl::Program::BuildStatus::ERROR) {
            auto log = prg.build_log(dev);
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
        if (arg == "-o") {
            output_file_name = next_arg(i + 1, argc, argv);
            i++;
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
        // library linking options
        else if (arg == "-create-library") {
            options.push_back(arg);
        }
        else if (arg == "-enable-link-options") {
            options.push_back(arg);
        }
        // program linking options
        else if (arg == "-cl-denorms-are-zero") {
            options.push_back(arg);
        }
        else if (arg == "-cl-no-signed-zeroes") {
            options.push_back(arg);
        }
        else if (arg == "-cl-unsafe-math-optimizations") {
            options.push_back(arg);
        }
        else if (arg == "-cl-finite-math-only") {
            options.push_back(arg);
        }
        else if (arg == "-cl-fast-relaxed-mat") {
            options.push_back(arg);
        }
        //
        else {
            if (arg[0] == '-')
                throw ocl::Exception("Unrecognized option '{}'", arg);
            else
                file_names.push_back(arg);
        }
    }
}

int
main(int argc, char * argv[])
{
    try {
        parse_command_line(argc, argv);
        if (file_names.empty())
            print_help();
        else
            link();
    }
    catch (ocl::Exception & e) {
        fmt::print("{}\n", e.what());
        return 1;
    }
}
