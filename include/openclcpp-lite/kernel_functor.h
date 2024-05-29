// SPDX-FileCopyrightText: 2024 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include "openclcpp-lite/cl.h"
#include "openclcpp-lite/error.h"
#include "openclcpp-lite/memory.h"
#include "openclcpp-lite/program.h"
#include "openclcpp-lite/event.h"
#include "openclcpp-lite/kernel.h"
#include <string>

namespace openclcpp_lite {

/// OpenCL kernel functor
template <typename... ARGS>
class KernelFunctor {
public:
    /// Create a functor from a program and a kernel name
    ///
    /// @param program OpenCL program
    /// @param kernel_name Kernel name
    KernelFunctor(const Program & program, const std::string & kernel_name) :
        kern(program, kernel_name)
    {
    }

    /// Set up the underlying kernel with its arguments and get the Kernel instance. This a
    /// syntactic sugar for putting kernels into queues
    ///
    /// @param args Kernel arguments
    /// @return Kernel object
    Kernel
    operator()(ARGS... args)
    {
        set_args<0>(std::forward<ARGS>(args)...);
        return this->kern;
    }

private:
    /// OpenCL kernel
    Kernel kern;

    template <int INDEX, typename T0, typename... T1S>
    void
    set_args(T0 && t0, T1S &&... t1s)
    {
        this->kern.set_arg(INDEX, t0);
        set_args<INDEX + 1, T1S...>(std::forward<T1S>(t1s)...);
    }

    template <int INDEX, typename T0>
    void
    set_args(T0 && t0)
    {
        this->kern.set_arg(INDEX, t0);
    }

    template <int>
    void
    set_args()
    {
    }
};

} // namespace openclcpp_lite
