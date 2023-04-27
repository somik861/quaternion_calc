#pragma once

#include "details.hpp"
#include <algorithm>
#include <array>
#include <cassert>
#include <fmt/core.h>
#include <functional>
#include <ostream>

namespace q {
template <typename T>
class Vector3 {
  public:
	constexpr Vector3(T x, T y, T z) : _values{x, y, z} {}
	constexpr Vector3(T xyz) : Vector3(xyz, xyz, xyz) {}

	constexpr T& x() { return _values[0]; }
	constexpr T x() const { return _values[0]; }

	constexpr T& y() { return _values[1]; }
	constexpr T y() const { return _values[1]; }

	constexpr T& z() { return _values[2]; }
	constexpr T z() const { return _values[2]; }

	constexpr Vector3& transform(auto unary_op) {
		std::ranges::transform(_values, _values.begin(), unary_op);
		return *this;
	}

	constexpr Vector3 transformed(auto unary_op) {
		Vector3 cpy = *this;
		cpy.transform(unary_op);
		return cpy;
	}

	// ============= basic operators =============

	constexpr T& operator[](std::size_t idx) {
		assert(idx < 3);
		return _values[idx];
	}

	constexpr T operator[](std::size_t idx) const {
		assert(idx < 3);
		return _values[idx];
	}

	constexpr friend std::ostream& operator<<(std::ostream& os,
	                                          const Vector3& v) {
		return os << fmt::format("[{}, {}, {}]", v.x(), v.y(), v.z());
	}

	constexpr friend bool operator==(const Vector3& lhs, const Vector3& rhs) {
		using details::isclose;

		return isclose(lhs.x(), rhs.x()) && isclose(lhs.y(), rhs.y()) &&
		       isclose(lhs.z(), rhs.z());
	}
	// ============= math operators =============
	constexpr friend Vector3 operator+(const Vector3& lhs, const Vector3& rhs) {
		return lhs._elem_wised(rhs, std::plus{});
	}
	constexpr friend Vector3 operator-(const Vector3& lhs, const Vector3& rhs) {
		return lhs._elem_wised(rhs, std::minus{});
	}
	constexpr friend Vector3 operator*(const Vector3& lhs, const Vector3& rhs) {
		return lhs._elem_wised(rhs, std::multiplies{});
	}
	constexpr friend Vector3 operator/(const Vector3& lhs, const Vector3& rhs) {
		return lhs._elem_wised(rhs, std::divides{});
	}
	constexpr friend Vector3 operator-(const Vector& v) {
		return v.transformed([](auto x) { return -x; });
	}

	constexpr Vector3& operator+(const Vector3& rhs) {
		return _elem_wise(rhs, std::plus{});
	}
	constexpr Vector3& operator-(const Vector3& rhs) {
		return _elem_wise(rhs, std::minus{});
	}
	constexpr Vector3& operator*(const Vector3& rhs) {
		return _elem_wise(rhs, std::multiplies{});
	}
	constexpr Vector3& operator/(const Vector3& rhs) {
		return _elem_wise(rhs, std::divides{});
	}

  private:
	constexpr Vector3& _elem_wise(const Vector3& rhs, auto binary_op) {
		std::ranges::transform(_values, rhs._values, _values.begin(),
		                       binary_op);
		return *this;
	}

	constexpr friend Vector3
	_elem_wised(const Vector3& lhs, const Vector3& rhs, auto binary_op) {
		Vectro3 cpy = lhs;
		cpy._elem_wise(rhs, binary_op);
		return cpy;
	}

	std::array<T, 3> _values;
};

} // namespace q
