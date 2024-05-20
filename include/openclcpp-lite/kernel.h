#pragma once

#include "openclcpp-lite/cl.h"
#include "openclcpp-lite/error.h"
#include "openclcpp-lite/program.h"
#include <string>

namespace openclcpp_lite {

class Program;
class Buffer;

struct LocalSpaceArg {
    std::size_t size_;
};

namespace detail {

template <typename T, class Enable = void>
struct KernelArgumentHandler;

// Enable for objects that are not subclasses of memory
// Pointers, constants etc
template <typename T>
struct KernelArgumentHandler<T, typename std::enable_if<!std::is_base_of<Buffer, T>::value>::type> {
    static size_t
    size(const T &)
    {
        return sizeof(T);
    }

    static const T *
    ptr(const T & value)
    {
        return &value;
    }
};

// Enable for subclasses of memory where we want to get a reference to the cl_mem out
// and pass that in for safety
template <typename T>
struct KernelArgumentHandler<T, typename std::enable_if<std::is_base_of<Buffer, T>::value>::type> {
    static size_t
    size(const T &)
    {
        return sizeof(cl_mem);
    }

    static const cl_mem *
    ptr(const T & value)
    {
        return &(value());
    }
};

// Specialization for DeviceCommandQueue defined later

template <>
struct KernelArgumentHandler<LocalSpaceArg, void> {
    static size_t
    size(const LocalSpaceArg & value)
    {
        return value.size_;
    }

    static const void *
    ptr(const LocalSpaceArg &)
    {
        return nullptr;
    }
};

} // namespace detail

class Kernel {
public:
    template <typename T>
    typename std::enable_if<!std::is_pointer<T>::value, cl_int>::type
    set_arg(cl_uint index, const T & value)
    {
        OPENCL_CHECK(clSetKernelArg(this->kern,
                                    index,
                                    detail::KernelArgumentHandler<T>::size(value),
                                    detail::KernelArgumentHandler<T>::ptr(value)));
    }

    void
    set_arg(cl_uint index, size_t size, const void * arg)
    {
        OPENCL_CHECK(clSetKernelArg(this->kern, index, size, arg));
    }

private:
    Kernel(cl_kernel kernel);

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
