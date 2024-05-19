#pragma once

#include <type_traits>
#include <vector>

namespace openclcpp_lite {

template <typename>
struct is_std_vector : std::false_type {};

template <typename T, typename A>
struct is_std_vector<std::vector<T, A>> : std::true_type {};

} // namespace openclcpp_lite
