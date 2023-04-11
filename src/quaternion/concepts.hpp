#pragma once
#include <concepts>
#include <sstream>

namespace q::details::concepts {
template <typename T>
concept Streamable = requires(std::ostream& os, T a) {
	                     { os << a } -> std::convertible_to<std::ostream&>;
                     };
}