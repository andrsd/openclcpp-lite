// SPDX-FileCopyrightText: 2024 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include "openclcpp-lite/cl.h"
#include "openclcpp-lite/device.h"
#include "openclcpp-lite/templ.h"
#include <string>
#include <vector>

namespace openclcpp_lite {

class Context;
class Device;

/// OpenCL program
class Program {
public:
    enum class BuildStatus {
        /// no build, compile or link has been performed
        NONE = CL_BUILD_NONE,
        /// build, compile or link generated an error
        ERROR = CL_BUILD_ERROR,
        /// build, compile or link was successful
        SUCCESS = CL_BUILD_SUCCESS,
        /// build, compile or link has not finished
        IN_PROGRESS = CL_BUILD_IN_PROGRESS
    };

    enum class BinaryType {
        ///
        NONE = CL_PROGRAM_BINARY_TYPE_NONE,
        ///
        COMPILED_OBJECT = CL_PROGRAM_BINARY_TYPE_COMPILED_OBJECT,
        ///
        LIBRARY = CL_PROGRAM_BINARY_TYPE_LIBRARY,
        ///
        EXECUTABLE = CL_PROGRAM_BINARY_TYPE_EXECUTABLE
    };

    /// Create a program from a OpenCL object
    explicit Program(cl_program prg);

    /// Increments the program reference count.
    void retain() const;

    /// Decrements the program reference count.
    void release() const;

    /// Return the program reference count.
    ///
    /// The reference count returned should be considered immediately stale. It is unsuitable for
    /// general use in applications. This feature is provided for identifying memory leaks.
    unsigned int reference_count() const;

    /// Return the context specified when the program object is created
    Context context() const;

    /// Return the list of devices associated with the program object. This can be the devices
    /// associated with context on which the program object has been created or can be a subset of
    /// devices that are specified when a progam object is created using clCreateProgramWithBinary.
    std::vector<Device> devices() const;

    /// Returns the number of kernels declared in program that can be created with clCreateKernel.
    /// This information is only available after a successful program executable has been built for
    /// at least one device in the list of devices associated with program.
    size_t num_of_kernels() const;

    /// Returns a list of kernel names in program that can be created with `clCreateKernel`. This
    /// information is only available after a successful program executable has been built for at
    /// least one device in the list of devices associated with program.
    std::vector<std::string> kernel_names() const;

    /// Returns an array that contains the size in bytes of the program binary
    std::vector<size_t> binary_sizes() const;

    /// Return the program binaries (could be an executable binary, compiled binary or library
    /// binary) for all devices associated with program.
    std::vector<std::vector<char>> binaries() const;

    /// Builds (compiles and links) a program executable from the program source or binary.
    ///
    /// @param options Build options to be used for building the program executable
    /// @param pfn_notify A function pointer to a function which will be called when the program
    ///        executable has been built (successfully or unsuccessfully).
    /// @param user_data Passed as an argument when `pfn_notify` is called. Can be `nullptr`.
    void build(const std::vector<std::string> & options = std::vector<std::string>(),
               void(CL_CALLBACK * pfn_notify)(cl_program program, void * user_data) = nullptr,
               void * user_data = nullptr) const;

    /// Builds (compiles and links) a program executable from the program source or binary.
    ///
    /// @param devices List of devices associated with program
    /// @param options Build options to be used for building the program executable
    /// @param pfn_notify A function pointer to a function which will be called when the program
    ///        executable has been built (successfully or unsuccessfully).
    /// @param user_data Passed as an argument when `pfn_notify` is called. Can be `nullptr`.
    void build(const std::vector<Device> & devices,
               const std::vector<std::string> & options = std::vector<std::string>(),
               void(CL_CALLBACK * pfn_notify)(cl_program program, void * user_data) = nullptr,
               void * user_data = nullptr) const;

    /// Compiles a program’s source for all the devices  in the OpenCL context associated with
    /// program
    ///
    /// @param options Compilation options to be used for building the program executable
    /// @param pfn_notify A function pointer to a function which will be called when the program
    ///        executable has been built (successfully or unsuccessfully).
    /// @param user_data Passed as an argument when `pfn_notify` is called. Can be `nullptr`.
    void compile(const std::vector<std::string> & options,
                 void(CL_CALLBACK * pfn_notify)(cl_program program, void * user_data) = nullptr,
                 void * user_data = nullptr) const;

    /// Compiles a program’s source for a specific device(s) in the OpenCL context associated with
    /// program
    ///
    /// @param options Compilation options to be used for building the program executable
    /// @param pfn_notify A function pointer to a function which will be called when the program
    ///        executable has been built (successfully or unsuccessfully).
    /// @param user_data Passed as an argument when `pfn_notify` is called. Can be `nullptr`.
    void compile(const std::vector<Device> & devices,
                 const std::vector<std::string> & options,
                 void(CL_CALLBACK * pfn_notify)(cl_program program, void * user_data) = nullptr,
                 void * user_data = nullptr) const;

    /// Links a set of compiled program objects and libraries for all the devices in the OpenCL
    /// context and creates an executable
    ///
    /// @param options Link options to be used for building the program executable
    /// @param programs Program objects that are compiled binaries or libraries that are to be
    ///        linked to create the program executable.
    /// @param pfn_notify A function pointer to a function which will be called when the program
    ///        executable has been built (successfully or unsuccessfully).
    /// @param user_data Passed as an argument when `pfn_notify` is called. Can be `nullptr`.
    /// @return `Program` object which contains the created executable
    Program link(const std::vector<std::string> & options,
                 const std::vector<Program> & programs,
                 void(CL_CALLBACK * pfn_notify)(cl_program program, void * user_data) = nullptr,
                 void * user_data = nullptr) const;

    /// Links a set of compiled program objects and libraries for a specific device(s) in the OpenCL
    /// context and creates an executable
    ///
    /// @param context Valid OpenCL context.
    /// @param devices List of devices that are in `context`
    /// @param options Link options to be used for building the program executable
    /// @param programs Program objects that are compiled binaries or libraries that are to be
    ///        linked to create the program executable.
    /// @param pfn_notify A function pointer to a function which will be called when the program
    ///        executable has been built (successfully or unsuccessfully).
    /// @param user_data Passed as an argument when `pfn_notify` is called. Can be `nullptr`.
    /// @return `Program` object which contains the created executable
    Program link(const Context & context,
                 const std::vector<Device> & devices,
                 const std::vector<std::string> & options,
                 const std::vector<Program> & programs,
                 void(CL_CALLBACK * pfn_notify)(cl_program program, void * user_data) = nullptr,
                 void * user_data = nullptr) const;

    /// Returns the build, compile or link status, whichever was performed last on program for
    /// device.
    BuildStatus build_status(Device device) const;

    /// Return the build, compile or link options specified by the options argument
    std::string build_options(Device device) const;

    /// Return the build, compile or link log
    std::string build_log(Device device) const;

    /// Return the program binary type
    BinaryType binary_type(Device device) const;

    operator cl_program() const;

public:
    /// Create a program from source in a default context
    ///
    /// @param source OpenCL source code
    /// @return Program object
    static Program from_source(const std::string & source);

    /// Create a program from source
    ///
    /// @param context OpenCL context
    /// @param source OpenCL source code
    /// @return Program object
    static Program from_source(const Context & context, const std::string & source);

    /// Create a program from source
    ///
    /// @param context OpenCL context
    /// @param lines OpenCL source code (as lines)
    /// @return Program object
    static Program from_source(const std::vector<std::string> & lines);
    static Program from_source(const Context & context, const std::vector<std::string> & lines);

    /// Create a program from source
    ///
    /// @param n_lines Number of lines
    /// @param lines Lines with code
    /// @return Program object
    static Program from_source(int n_lines, const char ** lines);
    static Program from_source(const Context & context, unsigned int n_lines, const char ** lines);

private:
    template <typename T>
    T
    get_info(cl_program_info name) const
    {
        T val;
        get_info_helper(clGetProgramInfo, this->prg, name, val);
        return val;
    }

    template <typename T>
    T
    get_build_info(Device device, cl_program_build_info name) const
    {
        T val;
        get_build_info_helper(this->prg, device, name, val);
        return val;
    }

    /// Underlying OpenCL program
    cl_program prg;
};

} // namespace openclcpp_lite
