#pragma once
#include <array>
#include <cassert>
#include <concepts>
#include <exception>
#include <fmt/core.h>
#include <iostream>
#include <memory>
#include <quaternion/quaternion.hpp>
#include <quaternion/vector3.hpp>
#include <string>
#include <string_view>
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

namespace details {

template <typename T>
constexpr std::string get_type_name(T) {
	return "unknown";
}

template <typename T>
    requires std::is_scalar_v<T>
constexpr std::string get_type_name(T) {
	return "Scalar";
}

template <typename T>
constexpr std::string get_type_name(q::Vector3<T>) {
	return "Vector";
}

template <typename T>
constexpr std::string get_type_name(q::Quaternion<T>) {
	return "Quaternion";
}

constexpr inline std::string int2string(int num) {
	if (num == 0)
		return "0";

	std::string out;
	bool sign = num < 0;
	if (sign) // make positive
		num *= -1;

	while (num != 0) {
		out.push_back('0' + (num % 10));
		num /= 10;
	}

	if (sign)
		out.push_back('-');

	std::ranges::reverse(out);
	return out;
}

template <typename get_t, typename variant_t>
constexpr get_t get_or_throw(const variant_t& variant,
                             int arg_num,
                             const std::string& structure) {

	const get_t* value = std::get_if<get_t>(&variant);
	if (value == nullptr) {
		throw std::logic_error(
		    std::string("Expected argument ") + details::int2string(arg_num) +
		    " of " + structure + " to be " + details::get_type_name(get_t{}) +
		    "; got " +
		    std::visit([](auto x) { return details::get_type_name(x); },
		               variant) +
		    ".");
	}
	return *value;
}
} // namespace details

template <std::floating_point T>
class Scalar final : public INode<T> {
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
class Vector final : public INode<T> {
  private:
	using uptr_t = typename INode<T>::uptr_t;
	using scalar_t = typename INode<T>::scalar_t;
	using vector_t = typename INode<T>::vector_t;

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
			values[i] = details::get_or_throw<scalar_t>(result, i, "Vector");
		}

		return vector_t(values[0], values[1], values[2]);
	}

  private:
	std::array<uptr_t, 3> _children;
};

template <typename T>
class Quaternion final : public INode<T> {
  private:
	using uptr_t = typename INode<T>::uptr_t;
	using scalar_t = typename INode<T>::scalar_t;
	using vector_t = typename INode<T>::vector_t;
	using quaternion_t = typename INode<T>::quaternion_t;
	using result_t = typename INode<T>::result_t;

  public:
	constexpr Quaternion(uptr_t scalar, uptr_t vector)
	    : _children{std::move(scalar), std::move(vector)}, _args_count(2) {}
	constexpr Quaternion(uptr_t scalar, uptr_t i, uptr_t j, uptr_t k)
	    : _children{std::move(scalar), std::move(i), std::move(j),
	                std::move(k)},
	      _args_count(4) {}

	constexpr static uptr_t make_unique(uptr_t scalar, uptr_t vector) {
		return std::make_unique<Quaternion>(std::move(scalar),
		                                    std::move(vector));
	}

	constexpr static uptr_t
	make_unique(uptr_t scalar, uptr_t i, uptr_t j, uptr_t k) {
		return std::make_unique<Quaternion>(std::move(scalar), std::move(i),
		                                    std::move(j), std::move(k));
	}

	constexpr result_t evaluate() const override {
		scalar_t scalar = details::get_or_throw<scalar_t>(
		    _children[0]->evaluate(), 0, "Quaternion");

		if (_args_count == 2) {
			vector_t vector = details::get_or_throw<vector_t>(
			    _children[1]->evaluate(), 1, "Quaternion");

			return quaternion_t(scalar, vector);
		}
		if (_args_count == 4) {
			std::array<scalar_t, 3> vector;

			for (std::size_t i = 1; i < 4; ++i)
				vector[i - 1] = details::get_or_throw<scalar_t>(
				    _children[i]->evaluate(), i, "Quaternion");

			return quaternion_t(scalar, vector[0], vector[1], vector[2]);
		}

		assert(false);
		throw std::runtime_error("Invalid number of arguments in Quaternion");
	}

  private:
	std::array<uptr_t, 4> _children;
	std::size_t _args_count;
};

namespace math {

template <typename T>
class Cross final : public INode<T> {
  private:
	using uptr_t = typename INode<T>::uptr_t;
	using vector_t = typename INode<T>::vector_t;
	using result_t = typename INode<T>::result_t;

  public:
	constexpr Cross(uptr_t lhs, uptr_t rhs)
	    : _children{std::move(lhs), std::move(rhs)} {}

	constexpr static uptr_t make_unique(uptr_t lhs, uptr_t rhs) {
		return std::make_unique<Cross>(std::move(lhs), std::move(rhs));
	}

	constexpr result_t evaluate() const override {
		vector_t lhs =
		    details::get_or_throw<vector_t>(_children[0], 0, "Cross");
		vector_t rhs =
		    details::get_or_throw<vector_t>(_children[1], 1, "Cross");

		return q::cross(lhs, rhs);
	}

  private:
	std::array<uptr_t, 2> _children;
};

template <typename T>
class Dot final : public INode<T> {
  private:
	using uptr_t = typename INode<T>::uptr_t;
	using vector_t = typename INode<T>::vector_t;
	using result_t = typename INode<T>::result_t;

  public:
	constexpr Dot(uptr_t lhs, uptr_t rhs)
	    : _children{std::move(lhs), std::move(rhs)} {}

	constexpr static uptr_t make_unique(uptr_t lhs, uptr_t rhs) {
		return std::make_unique<Dot>(std::move(lhs), std::move(rhs));
	}

	constexpr result_t evaluate() const override {
		vector_t lhs = details::get_or_throw<vector_t>(_children[0], 0, "Dot");
		vector_t rhs = details::get_or_throw<vector_t>(_children[1], 1, "Dot");

		return q::dot(lhs, rhs);
	}

  private:
	std::array<uptr_t, 2> _children;
};

template <typename T>
class Plus final : public INode<T> {
  private:
	using uptr_t = typename INode<T>::uptr_t;
	using vector_t = typename INode<T>::vector_t;
	using result_t = typename INode<T>::result_t;

  public:
	constexpr Plus(uptr_t lhs, uptr_t rhs)
	    : _children{std::move(lhs), std::move(rhs)} {}

	constexpr static uptr_t make_unique(uptr_t lhs, uptr_t rhs) {
		return std::make_unique<Plus>(std::move(lhs), std::move(rhs));
	}

	constexpr result_t evaluate() const override {
		return std::visit(
		    [](auto lhs, auto rhs) -> result_t {
			    if constexpr (std::is_invocable_v<std::plus<>, decltype(lhs),
			                                      decltype(rhs)>)
				    return lhs + rhs;
			    else
				    throw std::logic_error(
				        std::string("Operator+ cannot be invoked with "
				                    "following arguments: ") +
				        details::get_type_name(lhs) + "; " +
				        details::get_type_name(rhs));
		    },
		    _children[0]->evaluate(), _children[1]->evaluate());
	}

  private:
	std::array<uptr_t, 2> _children;
};

} // namespace math

} // namespace node
} // namespace ast
