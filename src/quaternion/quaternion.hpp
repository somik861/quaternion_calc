#pragma once

#include "details.hpp"
#include "vector3.hpp"

namespace q {
template <typename T>
class Quaternion {
  public:
	constexpr T& real() { return _real; }
	constexpr T real() const { return _real; }

	constexpr Vector3<T>& imag() { return _imag; }
	constexpr const Vector3<T>& imag() const { return _imag; }

	constexpr friend bool operator==(const Quaternion& lhs,
	                                 const Quaternion& rhs) {
		using details::isclose;

		return isclose(lhs.real(), rhs.real()) &&
		       isclose(lhs.imag(), rhs.imag());
	}

  private:
	T _real;
	Vector3<T> _imag;
};
} // namespace q