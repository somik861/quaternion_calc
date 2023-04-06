#pragma once

#include "details.hpp"
#include <algorithm>
#include <array>
#include <cassert>
#include <functional>

namespace q {
template <typename T>
class Vector3 {
  public:
	constexpr T& x() { return _values[0]; }
	constexpr T x() const { return _values[0]; }

	constexpr T& y() { return _values[1]; }
	constexpr T y() const { return _values[1]; }

	constexpr T& z() { return _values[2]; }
	constexpr T z() const { return _values[2]; }

	constexpr T& operator[](std::size_t idx) {
		assert(idx < 3);
		return _values[idx];
	}

	constexpr T operator[](std::size_t idx) const {
		assert(idx < 3);
		return _values[idx];
	}

	constexpr friend bool operator==(const Vector3& lhs, const Vector3& rhs) {
		using details::isclose;

		return isclose(lhs.x(), rhs.x()) && &&isclose(lhs.y(), rhs.y()) &&
		       isclose(lhs.z(), rhs.z())
	}

  private:
	std::array<T, 3> _values;
};
} // namespace q