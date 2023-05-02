#pragma once
#include <array>
#include <cassert>
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
	using INode<T>::vector_t;

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

		return vector_t(values[0], values[1], values[2]);
	}

  private:
	std::array<uptr_t, 3> _children;
};

template <typename T>
class Quaternion : public INode<T> {
  private:
	using INode<T>::uptr_t;
	using INode<T>::scalar_t;
	using INode<T>::vector_t;
	using INode<T>::quaternion_t;
	using INode<T>::result_t;

  public:
	constexpr Quaternion(uptr_t scalar, uptr_t vector)
	    : _children{std::move(scalar), std::move(vector)}, _args_count(2) {}
	constexpr Quaternion(uptr_t x, uptr_t i, uptr_t j, uptr_t k)
	    : _children{std::move(x), std::move(i), std::move(j), std::move(k)},
	      _args_count(4) {}

	constexpr result_t evaluate() const override {
		result_t scalar_res = _children[0]->evaluate();
		scalar_t* scalar = std::get_if<scalar_t>(&scalar_res);
		if (scalar == nullptr)
			throw std::logic_error(
			    "Expected argument 0 of Quaternion to be Scalar");

		if (_args_count == 2) {
			result_t vector_res = _children[0]->evaluate();
			vector_t* vector = std::get_if<vector_t>(&vector_res);
			if (vector == nullptr)
				throw std::logic_error(
				    "Expected argument 1 of Quaternion to be Vector");

			return quaternion_t(*scalar, *vector);

		} else if (_args_count == 4) {
			std::array<scalar_t, 3> vector;

			for (std::size_t i = 1; i < 4; ++i) {
				auto result = _children[i]->evaluate();
				T* value = std::get_if<scalar_t>(&result);
				if (value == nullptr)
					throw std::logic_error(fmt::format(
					    "Expected argument {} of Quaternion to be a Scalar",
					    i));
				vector[i - 1] = *value;
			}

			return quaternion_t(*scalar, vector[0], vector[1], vector[2]);
		} else {
			assert(false);
		}
	}

  private:
	std::array<uptr_t, 4> _children;
	std::size_t _args_count;
};

} // namespace node
} // namespace ast
