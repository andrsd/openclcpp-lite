// SPDX-FileCopyrightText: 2024 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

// NOLINTNEXTLINE
#define CL_TARGET_OPENCL_VERSION 120
#ifdef __APPLE__
    #include <OpenCL/cl.h>
#else
    #include <CL/cl.h>
#endif
