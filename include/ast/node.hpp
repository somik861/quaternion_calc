#pragma once
#include <concepts>
#include <quaternion/quaternion.hpp>
#include <variant>

namespace ast {
namespace node {
template <std::floating_point T>
class INode {
  public:
	using scalar_t = T;
	using vector_t = q::Vector3<T>;
	using quaternion_t = q::Quaternion<T>;
	using result_type = std::variant<T, vector_t, quaternion_t>;

	virtual constexpr result_type evaluate() const = 0;
};

template <std::floating_point T>
class Scalar : public INode<T> {
  public:
	constexpr Scalar(T value) : _value(value) {}

	constexpr INode<T>::result_type evaluate() const override { return _value; }

  private:
	T _value;
};

// TODO build from Scalars
template <std::floating_point T>
class Vector : public INode<T> {
  public:
	constexpr INode<T>::result_type evaluate() const override { return T(1); }

  private:
};

} // namespace node
} // namespace ast
