#pragma once

#include "openclcpp-lite/cl.h"
#include "openclcpp-lite/error.h"
#include "openclcpp-lite/memory.h"
#include "openclcpp-lite/program.h"
#include <string>

namespace openclcpp_lite {

class Program;
class Memory;
class Context;

class Kernel {
public:
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
    Kernel(cl_kernel kernel);

    template <typename T>
    T
    get_info(cl_kernel_info name) const
    {
        T val;
        get_info_helper(clGetKernelInfo, this->kern, name, val);
        return val;
    }

    cl_kernel kern;

public:
    template <typename... Ts>
    static Kernel
    create(const Program & program, const std::string & kernel_name)
    {
        cl_int err_code;
        auto k = clCreateKernel(program, kernel_name.c_str(), &err_code);
        OPENCL_CHECK(err_code);
        return Kernel(k);
    }
};

//

template <typename... Ts>
class KernelFunctor {
public:
    KernelFunctor(Kernel kernel) : kern(kernel) {}

    //    KernelFunctor(const Program & program, const std::string & name, cl_int * err = nullptr) :
    //        kern(program, name.c_str(), err)
    //    {
    //    }

private:
    Kernel kern;

    template <int INDEX, typename T0, typename... T1s>
    void
    set_args(T0 && t0, T1s &&... t1s)
    {
        this->kern.setArg(INDEX, t0);
        set_args<INDEX + 1, T1s...>(std::forward<T1s>(t1s)...);
    }

    template <int INDEX, typename T0>
    void
    set_args(T0 && t0)
    {
        this->kern.set_arg(INDEX, t0);
    }

    template <int INDEX>
    void
    set_args()
    {
    }
};

} // namespace openclcpp_lite
