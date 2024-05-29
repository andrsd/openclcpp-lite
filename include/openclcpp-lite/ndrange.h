// SPDX-FileCopyrightText: 2024 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include "openclcpp-lite/cl.h"
#include <array>
#include <initializer_list>

namespace openclcpp_lite {

/// N-dimensional range
template <int N>
class NDRange {
public:
    /// Empty range
    NDRange() : dims(0)
    {
        for (int i = 0; i < N; i++)
            this->sz[i] = 0;
    }

    /// Create N-dimensional range
    ///
    /// @param size Size of the range in each dimension
    explicit NDRange(std::initializer_list<size_t> size) : dims(N)
    {
        for (auto it = size.begin(); it != size.end(); ++it) {
            auto i = it - size.begin();
            this->sz[i] = *it;
        }
    }

    operator const size_t *() const { return this->sz.data(); }

    size_t
    dimensions() const
    {
        return this->dims;
    }

    /// Runtime number of dimensions
    size_t
    size() const
    {
        return this->dims * sizeof(size_t);
    }

    size_t *
    get()
    {
        return this->sz.data();
    }

    const size_t *
    get() const
    {
        return this->sz.data();
    }

private:
    std::array<size_t, N> sz;
    cl_uint dims;
};

} // namespace openclcpp_lite
