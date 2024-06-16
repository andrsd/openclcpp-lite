// SPDX-FileCopyrightText: 2024 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include "openclcpp-lite/cl.h"
#include "openclcpp-lite/error.h"
#include "openclcpp-lite/memory.h"
#include "openclcpp-lite/program.h"
#include "openclcpp-lite/event.h"
#include <string>

namespace openclcpp_lite {

class Program;
class Memory;
class Context;

template <typename... Ts>
class KernelFunctor;

/// OpenCL kernel
class Kernel {
public:
    /// Create empty kernel
    Kernel();

    /// Create a kernel from an OpenCL kernel
    explicit Kernel(cl_kernel kernel);

    /// Create a kernel from a program and a kernel name
    ///
    /// @param program OpenCL program
    /// @param kernel_name Kernel name
    Kernel(const Program & program, const std::string & kernel_name);

    /// Return the kernel function name.
    std::string function_name() const;

    /// Return the number of arguments to kernel.
    unsigned int num_of_args() const;

    /// Return the kernel reference count.
    unsigned int reference_count() const;

    /// Return the context associated with kernel.
    Context context() const;

    /// Return the program object associated with kernel.
    Program program() const;

    /// Returns any attributes specified using the __attribute__ qualifier with the kernel function
    /// declaration in the program source
    std::vector<std::string> attributes() const;

    /// Set the argument value for a specific argument of a kernel.
    ///
    /// @param index The argument index
    /// @param value Memory object
    void set_arg(cl_uint index, const Memory & value);

    /// Set the argument value for a specific argument of a kernel.
    ///
    /// @param index The argument index
    /// @param value A pointer to data that should be used as the argument value for argument
    /// specified by arg_index
    void set_arg(cl_uint index, size_t size, const void * arg);

    operator cl_kernel() const;

private:
    template <typename T>
    T
    get_info(cl_kernel_info name) const
    {
        T val;
        get_info_helper(clGetKernelInfo, this->kern, name, val);
        return val;
    }

    /// Underlying OpenCL kernel
    cl_kernel kern;

public:
    /// Create a kernel functor
    template <typename... Ts>
    static KernelFunctor<Ts...>
    create(const Program & program, const std::string & kernel_name)
    {
        KernelFunctor<Ts...> fn(program, kernel_name);
        return fn;
    }
};

} // namespace openclcpp_lite
