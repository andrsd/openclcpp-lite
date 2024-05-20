#include "openclcpp-lite/ndrange.h"

namespace openclcpp_lite {

NDRange::NDRange() : dims(0)
{
    this->sz[0] = 0;
    this->sz[1] = 0;
    this->sz[2] = 0;
}

NDRange::NDRange(size_t size0) : dims(1)
{
    this->sz[0] = size0;
    this->sz[1] = 1;
    this->sz[2] = 1;
}

NDRange::NDRange(size_t size0, size_t size1) : dims(2)
{
    this->sz[0] = size0;
    this->sz[1] = size1;
    this->sz[2] = 1;
}

NDRange::NDRange(size_t size0, size_t size1, size_t size2) : dims(3)
{
    this->sz[0] = size0;
    this->sz[1] = size1;
    this->sz[2] = size2;
}

NDRange::operator const size_t *() const
{
    return this->sz;
}

size_t
NDRange::dimensions() const
{
    return this->dims;
}

size_t
NDRange::size() const
{
    return this->dims * sizeof(size_t);
}

size_t *
NDRange::get()
{
    return this->sz;
}

const size_t *
NDRange::get() const
{
    return this->sz;
}

} // namespace openclcpp_lite
