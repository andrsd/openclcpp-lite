// SPDX-FileCopyrightText: 2024 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include "openclcpp-lite/cl.h"
#include <array>
#include <initializer_list>

namespace openclcpp_lite {

/// N-dimensional range
///
/// @tparam N Number of dimensions of the range
template <int N>
class Range {
public:
    /// Create an empty range
    Range() : dims(0)
    {
        for (int i = 0; i < N; i++)
            this->sz[i] = 0;
    }

    /// Create N-dimensional range
    ///
    /// @param size Sizes of the range in each dimension
    explicit Range(std::initializer_list<size_t> size) : dims(N)
    {
        for (auto it = size.begin(); it != size.end(); ++it) {
            auto i = it - size.begin();
            this->sz[i] = *it;
        }
    }

    operator const size_t *() const { return this->sz.data(); }

    /// Runtime number of dimensions
    ///
    /// @return Number of dimensions
    size_t
    dimensions() const
    {
        return this->dims;
    }

    /// Number of indices in the whole range
    ///
    /// @return Number of indices in the whole range
    size_t
    size() const
    {
        if (this->dims == 0)
            return 0;
        else {
            size_t s = 1;
            for (int i = 0; i < this->dims; i++)
                s *= this->sz[i];
            return s;
        }
    }

    /// Number of indices in a specified dimension
    ///
    /// @param idx Dimension index
    /// @return Number of indices in the dimension
    size_t
    size(int idx) const
    {
        return this->sz[idx];
    }

private:
    /// Size of the range in each dimension
    std::array<size_t, N> sz;
    /// Number of dimensions
    cl_uint dims;
};

} // namespace openclcpp_lite
