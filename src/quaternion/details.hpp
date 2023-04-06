#pragma once

#include "concepts.hpp"
#include <concepts>

namespace q::details {
template <std::floating_point T>
bool isclose(T a, T b) {
	return a == b;
}

template <std::integral T>
bool isclose(T a, T b) {
	return a == b;
}
} // namespace q::details