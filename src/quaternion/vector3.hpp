#pragma once

#include "details.hpp"
#include <algorithm>
#include <array>
#include <cassert>
#include <fmt/core.h>
#include <functional>
#include <numeric>
#include <ostream>

namespace q {
template <typename T>
class Vector3 {
  public:
	constexpr Vector3(T x, T y, T z) noexcept : _values{x, y, z} {}
	constexpr Vector3(T xyz) noexcept : Vector3(xyz, xyz, xyz) {}

	constexpr T& x() noexcept { return _values[0]; }
	constexpr T x() const noexcept { return _values[0]; }

	constexpr T& y() noexcept { return _values[1]; }
	constexpr T y() const noexcept { return _values[1]; }

	constexpr T& z() noexcept { return _values[2]; }
	constexpr T z() const noexcept { return _values[2]; }

	constexpr Vector3& transform(auto unary_op) noexcept {
		std::ranges::transform(_values, _values.begin(), unary_op);
		return *this;
	}

	constexpr Vector3 transformed(auto unary_op) const noexcept {
		Vector3 cpy = *this;
		cpy.transform(unary_op);
		return cpy;
	}

	T norm() const noexcept { return std::sqrt(powed(2).sum()); }

	Vector3& pow(T exp) {
		return transform([exp](auto x) { return std::pow(x, exp); });
	}

	Vector3 powed(T exp) const {
		return transformed([exp](auto x) { return std::pow(x, exp); });
	}

	constexpr T sum() const noexcept {
		return std::reduce(_values.begin(), _values.end());
	}

	constexpr auto begin() noexcept { return _values.begin(); }
	constexpr auto begin() const noexcept { return _values.begin(); }
	constexpr auto end() noexcept { return _values.end(); }
	constexpr auto end() const noexcept { return _values.end(); }

	// ============= basic operators =============

	constexpr T& operator[](std::size_t idx) noexcept {
		assert(idx < 3);
		return _values[idx];
	}

	constexpr T operator[](std::size_t idx) const noexcept {
		assert(idx < 3);
		return _values[idx];
	}

	constexpr friend std::ostream& operator<<(std::ostream& os,
	                                          const Vector3& v) {
		return os << fmt::format("[{}, {}, {}]", v.x(), v.y(), v.z());
	}

	constexpr friend bool operator==(const Vector3& lhs,
	                                 const Vector3& rhs) noexcept {
		using details::isclose;

		return isclose(lhs.x(), rhs.x()) && isclose(lhs.y(), rhs.y()) &&
		       isclose(lhs.z(), rhs.z());
	}
	// ============= math operators =============
	constexpr friend Vector3 operator+(const Vector3& lhs,
	                                   const Vector3& rhs) noexcept {
		return lhs._elem_wised(rhs, std::plus{});
	}
	constexpr friend Vector3 operator-(const Vector3& lhs,
	                                   const Vector3& rhs) noexcept {
		return lhs._elem_wised(rhs, std::minus{});
	}
	constexpr friend Vector3 operator*(const Vector3& lhs,
	                                   const Vector3& rhs) noexcept {
		return lhs._elem_wised(rhs, std::multiplies{});
	}
	constexpr friend Vector3 operator/(const Vector3& lhs,
	                                   const Vector3& rhs) noexcept {
		return lhs._elem_wised(rhs, std::divides{});
	}
	constexpr friend Vector3 operator-(const Vector3& v) noexcept {
		return v.transformed([](auto x) { return -x; });
	}
	constexpr friend Vector3 operator+(T lhs, const Vector3& rhs) noexcept {
		return rhs.transformed([lhs](auto x) { return lhs + x; });
	}
	constexpr friend Vector3 operator-(T lhs, const Vector3& rhs) noexcept {
		return rhs.transformed([lhs](auto x) { return lhs - x; });
	}
	constexpr friend Vector3 operator*(T lhs, const Vector3& rhs) noexcept {
		return rhs.transformed([lhs](auto x) { return lhs * x; });
	}
	constexpr friend Vector3 operator/(T lhs, const Vector3& rhs) noexcept {
		return rhs.transformed([lhs](auto x) { return lhs / x; });
	}
	constexpr friend Vector3 operator+(const Vector3& lhs, T rhs) noexcept {
		return lhs.transformed([rhs](auto x) { return x + rhs; });
	}
	constexpr friend Vector3 operator-(const Vector3& lhs, T rhs) noexcept {
		return lhs.transformed([rhs](auto x) { return x - rhs; });
	}
	constexpr friend Vector3 operator*(const Vector3& lhs, T rhs) noexcept {
		return lhs.transformed([rhs](auto x) { return x * rhs; });
	}
	constexpr friend Vector3 operator/(const Vector3& lhs, T rhs) noexcept {
		return lhs.transformed([rhs](auto x) { return x / rhs; });
	}

	constexpr Vector3& operator+=(const Vector3& rhs) noexcept {
		return _elem_wise(rhs, std::plus{});
	}
	constexpr Vector3& operator-=(const Vector3& rhs) noexcept {
		return _elem_wise(rhs, std::minus{});
	}
	constexpr Vector3& operator*=(const Vector3& rhs) noexcept {
		return _elem_wise(rhs, std::multiplies{});
	}
	constexpr Vector3& operator/=(const Vector3& rhs) noexcept {
		return _elem_wise(rhs, std::divides{});
	}
	constexpr Vector3& operator+=(T rhs) noexcept {
		return transform([rhs](auto x) { return x + rhs; });
	}
	constexpr Vector3& operator-=(T rhs) noexcept {
		return transform([rhs](auto x) { return x - rhs; });
	}
	constexpr Vector3& operator*=(T rhs) noexcept {
		return transform([rhs](auto x) { return x * rhs; });
	}
	constexpr Vector3& operator/=(T rhs) noexcept {
		return transform([rhs](auto x) { return x / rhs; });
	}

  private:
	constexpr Vector3& _elem_wise(const Vector3& rhs, auto binary_op) noexcept {
		std::ranges::transform(_values, rhs._values, _values.begin(),
		                       binary_op);
		return *this;
	}

	constexpr Vector3 _elem_wised(const Vector3& rhs,
	                              auto binary_op) const noexcept {
		Vector3 cpy = *this;
		cpy._elem_wise(rhs, binary_op);
		return cpy;
	}

	std::array<T, 3> _values;
};

template <typename T>
constexpr Vector3<T> cross(const Vector3<T>& lhs,
                           const Vector3<T>& rhs) noexcept {
	return Vector3<T>(lhs.y() * rhs.z() - lhs.z() * rhs.y(),
	                  lhs.z() * rhs.x() - lhs.x() * rhs.z(),
	                  lhs.x() * rhs.y() - lhs.y() * rhs.x());
}

template <typename T>
constexpr T dot(const Vector3<T>& lhs, const Vector3<T>& rhs) noexcept {
	auto to_sum = lhs * rhs;
	return std::reduce(to_sum.begin(), to_sum.end());
}

} // namespace q
