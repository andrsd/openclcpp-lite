// SPDX-FileCopyrightText: 2024 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include "openclcpp-lite/cl.h"
#include "openclcpp-lite/templ.h"
#include <string>
#include <vector>

namespace openclcpp_lite {

class Context;
class Device;

/// OpenCL program
class Program {
public:
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

    /// Builds (compiles and links) a program executable from the program source or binary.
    void build() const;

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
    static Program from_source(const Context & context, const std::vector<std::string> & lines);

private:
    template <typename T>
    T
    get_info(cl_program_info name) const
    {
        T val;
        get_info_helper(clGetProgramInfo, this->prg, name, val);
        return val;
    }

    /// Underlying OpenCL program
    cl_program prg;
};

} // namespace openclcpp_lite
