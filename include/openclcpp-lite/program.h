#pragma once

#include "openclcpp-lite/cl.h"
#include "openclcpp-lite/templ.h"
#include <string>
#include <vector>

namespace openclcpp_lite {

class Context;
class Device;

class Program {
public:
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
    static Program from_source(const Context & ctx, const std::string & source);
    static Program from_source(const Context & ctx, const std::vector<std::string> & lines);

private:
    explicit Program(cl_program prg);

    template <typename T>
    T
    get_info(cl_program_info name) const
    {
        T val;
        get_info_helper(clGetProgramInfo, this->prg, name, val);
        return val;
    }

    cl_program prg;

    friend class Kernel;
};

} // namespace openclcpp_lite