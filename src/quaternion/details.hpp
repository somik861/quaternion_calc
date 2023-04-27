#pragma once

#include "concepts.hpp"
#include <cmath>
#include <concepts>
#include <limits>

namespace q::details {
template <std::floating_point T>
constexpr bool isclose(T a, T b) {
	return std::abs(a - b) <=
	           std::numeric_limits<T>::epsilon() * std::abs(a + b) * 4 ||
	       std::abs(a - b) <= std::numeric_limits<T>::min();
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
