#pragma once
#include <array>
#include <concepts>
#include <exception>
#include <fmt/core.h>
#include <memory>
#include <quaternion/quaternion.hpp>
#include <quaternion/vector3.hpp>
#include <variant>

namespace ast {
namespace node {

template <std::floating_point T>
class INode {
  public:
	using scalar_t = T;
	using vector_t = q::Vector3<T>;
	using quaternion_t = q::Quaternion<T>;
	using result_t = std::variant<T, vector_t, quaternion_t>;
	using uptr_t = std::unique_ptr<INode>;
	using ptr_t = INode*;

	virtual constexpr result_t evaluate() const = 0;
};

template <std::floating_point T>
class Scalar : public INode<T> {
  public:
	constexpr Scalar(T value) : _value(value) {}

	constexpr static INode<T>::uptr_t make_unique(T value) {
		return std::make_unique<Scalar>(value);
	}

	constexpr INode<T>::result_t evaluate() const override { return _value; }

  private:
	T _value;
};

template <std::floating_point T>
class Vector : public INode<T> {
  private:
	using INode<T>::uptr_t;
	using INode<T>::scalar_t;

  public:
	constexpr Vector(uptr_t x, uptr_t y, uptr_t z)
	    : _children{std::move(x), std::move(y), std::move(z)} {}

	constexpr static uptr_t make_unique(uptr_t x, uptr_t y, uptr_t z) {
		return std::make_unique<Vector>(std::move(x), std::move(y),
		                                std::move(z));
	}

	constexpr INode<T>::result_t evaluate() const override {
		std::array<T, 3> values;

		for (std::size_t i = 0; i < 3; ++i) {
			auto result = _children[i]->evaluate();
			T* value = std::get_if<scalar_t>(&result);
			if (value == nullptr)
				throw std::logic_error(fmt::format(
				    "Expected argument {} of Vector to be a Scalar", i));
			values[i] = *value;
		}

		return Vector3(values[0], values[1], values[2]);
	}

  private:
	std::array<typename INode<T>::uptr_t, 3> _children;
};

} // namespace node
} // namespace ast
