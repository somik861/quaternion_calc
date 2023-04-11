#pragma once

#include "details.hpp"
#include "vector3.hpp"

namespace q {
template <typename T>
class Quaternion {
  public:
	Quaternion(T real, const Vector3<T>& imag) : _real(real), _imag(imag) {}
	Quaternion(T x, T i, T j, T k) : Quaternion(x, {i, j, k}) {}

	constexpr T& real() { return _real; }
	constexpr T real() const { return _real; }

	constexpr Vector3<T>& imag() { return _imag; }
	constexpr const Vector3<T>& imag() const { return _imag; }

	constexpr friend bool operator==(const Quaternion& lhs,
	                                 const Quaternion& rhs) {
		using details::isclose;

		return isclose(lhs.real(), rhs.real()) && lhs.imag() == rhs.imag();
	}

	constexpr friend std::ostream& operator<<(std::ostream& os,
	                                          const Quaternion& q) {
		return os << fmt::format("Q({}, {})", q.real(),
		                         details::to_string(q.imag()));
	}

  private:
	T _real;
	Vector3<T> _imag;
};
} // namespace q