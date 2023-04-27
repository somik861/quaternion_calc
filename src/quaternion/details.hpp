#pragma once

#include "concepts.hpp"
#include <concepts>
#include <limits>

namespace q::details {
template <std::floating_point T>
constexpr bool isclose(T a, T b) {
	return std::fabs(a - b) <=
	           std::numeric_limits<T>::epsilon() * std::fabs(a + b) * 4 ||
	       std::fabs(a - b) <= std::numeric_limits<T>::min();
}

template <std::integral T>
constexpr bool isclose(T a, T b) {
	return a == b;
}

template <concepts::Streamable T>
std::string to_string(T elem) {
	std::stringstream ss;
	ss << elem;
	return ss.str();
}
} // namespace q::details
