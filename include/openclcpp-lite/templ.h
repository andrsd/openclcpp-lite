#pragma once

#include "openclcpp-lite/utils.h"
#include "openclcpp-lite/error.h"
#include <type_traits>
#include <vector>
#include <string>

namespace openclcpp_lite {

template <typename>
struct is_std_vector : std::false_type {};

template <typename T, typename A>
struct is_std_vector<std::vector<T, A>> : std::true_type {};

template <typename>
struct is_std_string : std::false_type {};

template <>
struct is_std_string<std::string> : std::true_type {};

template <typename FN, typename OBJ, typename T>
cl_int
get_info_scalar(FN fn, OBJ obj, cl_uint name, T & val)
{
    OPENCL_CHECK(fn(obj, name, sizeof(T), &val, nullptr));
    return CL_SUCCESS;
}

template <typename FN, typename OBJ>
cl_int
get_info_string(FN fn, OBJ obj, cl_uint name, std::string & val)
{
    std::size_t n;
    OPENCL_CHECK(fn(obj, name, 0, nullptr, &n));
    std::vector<char> str(n);
    OPENCL_CHECK(fn(obj, name, n, str.data(), nullptr));
    val.assign(begin(str), prev(end(str)));
    return CL_SUCCESS;
}

template <typename FN, typename OBJ, typename T, typename A>
cl_int
get_info_vector(FN fn, OBJ obj, cl_uint name, std::vector<T, A> & val)
{
    std::size_t n_bytes;
    OPENCL_CHECK(fn(obj, name, 0, nullptr, &n_bytes));
    const size_t n = n_bytes / sizeof(T);
    std::vector<T, A> data(n);
    OPENCL_CHECK(fn(obj, name, n_bytes, data.data(), nullptr));
    val = std::move(data);
    return CL_SUCCESS;
}

template <typename FN, typename OBJ, typename TRET>
cl_int
get_info_helper(FN fn, OBJ obj, cl_uint name, TRET & val)
{
    if constexpr (is_std_vector<TRET>::value)
        return get_info_vector(fn, obj, name, val);
    else if constexpr (is_std_string<TRET>::value)
        return get_info_string(fn, obj, name, val);
    else
        return get_info_scalar(fn, obj, name, val);
}

} // namespace openclcpp_lite
