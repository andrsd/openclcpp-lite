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
std::vector<std::string> file_names;

void
print_help()
{
    fmt::print("OpenCL compiler\n");
    fmt::print("\n");
    fmt::print("Usage: ocl-compiler [options] <file>...\n");
    fmt::print("\n");
    // clang-format off
    fmt::print(
        "Options:\n"
        "\n"
        "* General\n"
        "\n"
        "  -c\n"
        "\n"
        "    Compile a file.\n"
        "\n"
        "  <file>\n"
        "\n"
        "    File name to compile/build.\n"
        "\n"
        "* Preprocessor Options\n"
        "\n"
        "  These options control the OpenCL C preprocessor which is run on each program source\n"
        "  before actual compilation.\n"
        "\n"
        "  -D name\n"
        "\n"
        "    Predefine name as a macro, with definition 1.\n"
        "\n"
        "  -D name=definition\n"
        "\n"
        "    The contents of definition are tokenized and processed as if they appeared during\n"
        "    translation phase three in a `#define' directive. In particular, the definition will\n"
        "    be truncated by embedded newline characters.\n"
        "\n"
        "  -I dir\n"
        "\n"
        "    Add the directory dir to the list of directories to be searched for header files.\n"
        "\n"
        "* Math Intrinsics Options\n"
        "\n"
        "  These options control compiler behavior regarding floating-point arithmetic. These \n"
        "  options trade off between speed and correctness.\n"
        "\n"
        "  -cl-single-precision-constant\n"
        "\n"
        "    Treat double precision floating-point constant as single precision constant.\n"
        "\n"
        "  -cl-denorms-are-zero\n"
        "\n"
        "    This option controls how single precision and double precision denormalized numbers\n"
        "    are handled. If specified as a build option, the single precision denormalized\n"
        "    numbers may be flushed to zero; double precision denormalized numbers may also be\n"
        "    flushed to zero if the optional extension for double precision is supported. This is\n"
        "    intended to be a performance hint and the OpenCL compiler can choose not to flush\n"
        "    denorms to zero if the device supports single precision (or double precision)\n"
        "    denormalized numbers.\n"
        "\n"
        "    This option is ignored for single precision numbers if the device does not support\n"
        "    single precision denormalized numbers i.e. CL_FP_DENORM bit is not set in\n"
        "    CL_DEVICE_SINGLE_FP_CONFIG.\n"
        "\n"
        "    This option is ignored for double precision numbers if the device does not support\n"
        "    double precision or if it does support double precison but CL_FP_DENORM bit is not\n"
        "    set in CL_DEVICE_DOUBLE_FP_CONFIG.\n"
        "\n"
        "    This flag only applies for scalar and vector single precision floating-point\n"
        "    variables and computations on these floating-point variables inside a program. It\n"
        "    does not apply to reading from or writing to image objects.\n"
        "\n"
        "  -cl-fp32-correctly-rounded-divide-sqrt\n"
        "\n"
        "    The -cl-fp32-correctly-rounded-divide-sqrt build option to clBuildProgram or\n"
        "    clCompileProgram allows an application to specify that single precision\n"
        "    floating-point divide (x/y and 1/x) and sqrt used in the program source are\n"
        "    correctly rounded. If this build option is not specified, the minimum numerical\n"
        "    accuracy of single precision floating-point divide and sqrt are as defined in\n"
        "    section 7.4 of the OpenCL specification.\n"
        "\n"
        "    This build option can only be specified if the CL_FP_CORRECTLY_ROUNDED_DIVIDE_SQRT\n"
        "    is set in CL_DEVICE_SINGLE_FP_CONFIG (as defined in in the table of allowed values\n"
        "    for param_name for clGetDeviceInfo) for devices that the program is being build.\n"
        "    clBuildProgram or clCompileProgram will fail to compile the program for a device if\n"
        "    the -cl-fp32-correctly-rounded-divide-sqrt option is specified and\n"
        "    CL_FP_CORRECTLY_ROUNDED_DIVIDE_SQRT is not set for the device.\n"
        "\n"
        "* Optimization Options\n"
        "\n"
        "  These options control various sorts of optimizations. Turning on optimization flags\n"
        "  makes the compiler attempt to improve the performance and/or code size at the\n"
        "  expense of compilation time and possibly the ability to debug the program.\n"
        "\n"
        "  -cl-opt-disable\n"
        "\n"
        "    This option disables all optimizations. The default is optimizations are enabled.\n"
        "\n"
        "  The following options control compiler behavior regarding floating-point arithmetic.\n"
        "  These options trade off between performance and correctness and must be specifically\n"
        "  enabled. These options are not turned on by default since it can result in incorrect\n"
        "  output for programs which depend on an exact implementation of IEEE 754\n"
        "  rules/specifications for math functions.\n"
        "\n"
        "  -cl-mad-enable\n"
        "\n"
        "    Allow a * b + c to be replaced by a mad. The mad computes a * b + c with reduced\n"
        "    accuracy. For example, some OpenCL devices implement mad as truncate the result of\n"
        "    a * b before adding it to c.\n"
        "\n"
        "  -cl-no-signed-zeros\n"
        "\n"
        "    Allow optimizations for floating-point arithmetic that ignore the signedness of\n"
        "    zero. IEEE 754 arithmetic specifies the distinct behavior of +0.0 and -0.0 values,\n"
        "    which then prohibits simplification of expressions such as x+0.0 or 0.0*x (even with\n"
        "    -clfinite-math only). This option implies that the sign of a zero result isn't\n"
        "    significant.\n"
        "\n"
        "  -cl-unsafe-math-optimizations\n"
        "\n"
        "    Allow optimizations for floating-point arithmetic that (a) assume that arguments and\n"
        "    results are valid, (b) may violate IEEE 754 standard and (c) may violate the OpenCL\n"
        "    numerical compliance requirements as defined in section 7.4 for single precision and\n"
        "    double precision floating-point, and edge case behavior in section 7.5. This option\n"
        "    includes the -cl-no-signed-zeros and -cl-mad-enable options.\n"
        "\n"
        "  -cl-finite-math-only\n"
        "\n"
        "    Allow optimizations for floating-point arithmetic that assume that arguments and\n"
        "    results are not NaNs or ±∞. This option may violate the OpenCL numerical compliance\n"
        "    requirements defined in section 7.4 for single precision and double precision\n"
        "    floating point, and edge case behavior in section 7.5.\n"
        "\n"
        "  -cl-fast-relaxed-math\n"
        "\n"
        "    Sets the optimization options -cl-finite-math-only and -cl-unsafe-math-optimizations.\n"
        "    This allows optimizations for floating-point arithmetic that may violate the IEEE 754\n"
        "    standard and the OpenCL numerical compliance requirements defined in the\n"
        "    specification in section 7.4 for single-precision and double precision floating-point,\n"
        "    and edge case behavior in section 7.5. This option causes the preprocessor macro\n"
        "    __FAST_RELAXED_MATH__ to be defined in the OpenCL program.\n"
        "\n"
        "* Options to Request or Suppress Warnings\n"
        "\n"
        "  Warnings are diagnostic messages that report constructions which are not inherently\n"
        "  erroneous but which are risky or suggest there may have been an error. The following\n"
        "  language independent options do not enable specific warnings but control the kinds of\n"
        "  diagnostics produced by the OpenCL compiler.\n"
        "\n"
        "  -w\n"
        "\n"
        "    Inhibit all warning messages.\n"
        "\n"
        "  -Werror\n"
        "\n"
        "    Make all warnings into errors.\n"
        "\n"
        "* Options Controlling the OpenCL C Version\n"
        "\n"
        "  The following option controls the version of OpenCL C that the compiler accepts.\n"
        "\n"
        "  -cl-std=\n"
        "\n"
        "    Determine the OpenCL C language version to use. A value for this option must be\n"
        "    provided. Valid values are:\n"
        "\n"
        "    - CL1.1 - Support all OpenCL C programs that use the OpenCL C language features\n"
        "      defined in section 6 of the OpenCL 1.1 specification.\n"
        "    - CL1.2 – Support all OpenCL C programs that use the OpenCL C language features\n"
        "      defined in section 6 of the OpenCL 1.2 specification.\n"
        "\n"
        "  Calls to clBuildProgram or clCompileProgram with the -cl-std=CL1.1 option will fail to\n"
        "  compile the program for any devices with CL_DEVICE_OPENCL_C_VERSION = OpenCL C 1.0.\n"
        "  Calls to clBuildProgram or clCompileProgram with the -cl-std=CL1.2 option will fail to\n"
        "  compile the program for any devices with CL_DEVICE_OPENCL_C_VERSION = OpenCL C 1.0 or\n"
        "  OpenCL C 1.1.\n"
        "\n"
        "  If the -cl-std build option is not specified, the CL_DEVICE_OPENCL_C_VERSION is used\n"
        "  to select the version of OpenCL C to be used when compiling the program for each\n"
        "  device.\n"
        "\n"
        "* Options for Querying Kernel Argument Information\n"
        "\n"
        "  -cl-kernel-arg-info\n"
        "\n"
        "    This option allows the compiler to store information about the arguments of a\n"
        "    kernel(s) in the program executable. The argument information stored includes the\n"
        "    argument name, its type, the address and access qualifiers used. Refer to\n"
        "    description of clGetKernelArgInfo on how to query this information.\n"
    );
    // clang-format on
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

std::string
create_output_file_name(const std::string & name)
{
    std::string str = name;
    return str.substr(0, str.find_last_of(".")) + ".o";
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

/// Compile single file and write into a file
///
/// @param input Input file name
/// @param output Output file name
/// @return 0 on success, non-zero on failure
int
compile_file(const std::string & input, const std::string & output)
{
    auto src = read_file(input);
    auto dev = target_platform.devices()[0];
    ocl::Context ctx(dev);
    auto prg = ocl::Program::from_source(ctx, src);
    try {
        prg.compile(ocl_compiler_opts);
        auto bins = prg.binaries();
        write_file(output, bins[0]);
        return 0;
    }
    catch (ocl::Exception & e) {
        auto status = prg.build_status(dev);
        if (status == ocl::Program::BuildStatus::ERROR) {
            auto log = prg.build_log(dev);
            fmt::print(stderr, "{}\n", input);
            fmt::print(stderr, "{}\n", log);
        }
        return 1;
    }
}

/// Compile files and produce binary outputs
int
compile()
{
    for (auto & ifn : file_names) {
        auto ofn = create_output_file_name(ifn);
        if (compile_file(ifn, ofn))
            return 1;
    }
    return 0;
}

int
build()
{
    const std::string & file_name = file_names[0];
    auto src = read_file(file_name);
    auto dev = target_platform.devices()[0];
    ocl::Context ctx(dev);
    auto prg = ocl::Program::from_source(ctx, src);
    try {
        prg.build({ dev }, ocl_compiler_opts);
        auto bins = prg.binaries();
        write_file("a.out", bins[0]);
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
                file_names.push_back(arg);
            }
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
        else if (command == COMPILE)
            return compile();
        else if (command == BUILD)
            return build();
    }
    catch (ocl::Exception & e) {
        fmt::print("{}\n", e.what());
        return 1;
    }
}
