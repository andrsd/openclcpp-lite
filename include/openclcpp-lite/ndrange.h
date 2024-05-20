#pragma once

#include "openclcpp-lite/cl.h"

namespace openclcpp_lite {

/// N-dimensional range, but really maximum of 3 dimensions are possible
class NDRange {
public:
    /// Empty range
    NDRange();

    /// Create 1-dimensional range
    NDRange(size_t size0);

    /// Create 2-dimensional range
    NDRange(size_t size0, size_t size1);

    /// Create 3-dimensional range
    NDRange(size_t size0, size_t size1, size_t size2);

    operator const size_t *() const;

    size_t dimensions() const;

    /// Runtime number of dimensions
    size_t size() const;

    size_t * get();

    const size_t * get() const;

private:
    size_t sz[3];
    cl_uint dims;
};

} // namespace openclcpp_lite
