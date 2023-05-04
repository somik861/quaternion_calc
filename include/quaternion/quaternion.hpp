#pragma once

#include "details.hpp"
#include "vector3.hpp"
#include <cmath>

namespace q {
template <typename T>
class Quaternion {
  public:
	using value_t = T;

	constexpr Quaternion() noexcept = default;
	constexpr Quaternion(T real, const Vector3<T>& imag) noexcept
	    : _real(real), _imag(imag) {}
	constexpr Quaternion(T x, T i, T j, T k) noexcept
	    : Quaternion(x, {i, j, k}) {}

	constexpr T& real() noexcept { return _real; }
	constexpr T real() const noexcept { return _real; }

	constexpr Vector3<T>& imag() noexcept { return _imag; }
	constexpr const Vector3<T>& imag() const noexcept { return _imag; }

	T norm() const noexcept {
		return std::sqrt(_real * _real + dot(_imag, _imag));
	};

	constexpr Quaternion& conjugate() noexcept {
		_imag = -_imag;
		return *this;
	}

	constexpr Quaternion conjugated() const noexcept {
		auto cpy = *this;
		return cpy.conjugate();
	}

	Quaternion& reciprocal() noexcept {
		conjugate();
		(*this) *= (1 / std::pow(norm(), T(2)));
		return *this;
	}

	Quaternion reciprocaled() const noexcept {
		auto cpy = *this;
		return cpy.reciprocal();
	}

	// ============= basic operators =============

	constexpr friend bool operator==(const Quaternion& lhs,
	                                 const Quaternion& rhs) noexcept {
		using details::isclose;

		return isclose(lhs.real(), rhs.real()) && lhs.imag() == rhs.imag();
	}

	constexpr friend std::ostream&
	operator<<(std::ostream& os, const Quaternion& q) noexcept(false) {
		return os << fmt::format("Q({}, {})", q.real(),
		                         details::to_string(q.imag()));
	}

	// ============= math operators =============
	constexpr friend Quaternion operator+(const Quaternion& lhs,
	                                      const Quaternion& rhs) noexcept {
		return {lhs.real() + rhs.real(), lhs.imag() + rhs.imag()};
	}

	constexpr friend Quaternion operator-(const Quaternion& lhs,
	                                      const Quaternion& rhs) noexcept {
		return {lhs.real() - rhs.real(), lhs.imag() - rhs.imag()};
	}

	constexpr friend Quaternion operator*(T lhs,
	                                      const Quaternion& rhs) noexcept {
		return {lhs * rhs.real(), lhs * rhs.imag()};
	}

	constexpr friend Quaternion operator*(const Quaternion& lhs,
	                                      T rhs) noexcept {
		return {lhs.real() * rhs, lhs.imag() * rhs};
	}

	constexpr friend Quaternion operator-(const Quaternion& op) noexcept {
		return {-op.real(), -op.imag()};
	}

	constexpr friend Quaternion operator*(const Quaternion& lhs,
	                                      const Quaternion& rhs) noexcept {
		T real = lhs.real() * rhs.real() - dot(lhs.imag(), rhs.imag());
		Vector3<T> imag = cross(lhs.imag(), rhs.imag()) +
		                  lhs.real() * rhs.imag() + rhs.real() * lhs.imag();
		return {real, imag};
	}

	constexpr Quaternion& operator+=(const Quaternion& rhs) noexcept {
		_real += rhs.real();
		_imag += rhs.imag();
		return *this;
	}

	constexpr Quaternion& operator-=(const Quaternion& rhs) noexcept {
		_real -= rhs.real();
		_imag -= rhs.imag();
		return *this;
	}

	constexpr Quaternion& operator*=(T rhs) noexcept {
		_real *= rhs;
		_imag *= rhs;
		return *this;
	}

	constexpr Quaternion& operator*=(const Quaternion& rhs) noexcept {
		*this = *this * rhs;
		return *this;
	}

  private:
	T _real;
	Vector3<T> _imag;
};
} // namespace q
