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
    Range() : dims_(0)
    {
        for (int i = 0; i < N; i++)
            this->sz_[i] = 0;
    }

    /// Create N-dimensional range
    ///
    /// @param size Sizes of the range in each dimension
    explicit Range(std::initializer_list<size_t> size) : dims_(N)
    {
        for (auto it = size.begin(); it != size.end(); ++it) {
            auto i = it - size.begin();
            this->sz_[i] = *it;
        }
    }

    operator const size_t *() const { return this->sz_.data(); }

    /// Runtime number of dimensions
    ///
    /// @return Number of dimensions
    size_t
    dimensions() const
    {
        return this->dims_;
    }

    /// Number of indices in the whole range
    ///
    /// @return Number of indices in the whole range
    size_t
    size() const
    {
        if (this->dims_ == 0)
            return 0;
        else {
            size_t s = this->sz_[0];
            for (int i = 1; i < this->dims_; i++)
                s *= this->sz_[i];
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
        return this->sz_[idx];
    }

private:
    /// Size of the range in each dimension
    std::array<size_t, N> sz_;
    /// Number of dimensions
    cl_uint dims_;
};

} // namespace openclcpp_lite
