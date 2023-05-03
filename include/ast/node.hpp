#pragma once
#include <array>
#include <cassert>
#include <concepts>
#include <exception>
#include <fmt/core.h>
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
constexpr std::string get_type_name(typename INode<T>::scalar_t) {
	return "Scalar";
}

template <typename T>
constexpr std::string get_type_name(typename INode<T>::vector_t) {
	return "Vector";
}

template <typename T>
constexpr std::string get_type_name(typename INode<T>::quaternion_t) {
	return "Quaternion";
}

constexpr inline std::string int2string(int num) {
	std::string out;
	bool sign = num < 0;
	num *= sign * -1;

	while (num != 0) {
		out.push_back('0' + num % 10);
		num /= 10;
	}

	if (sign)
		out.push_back('-');

	std::ranges::reverse(out);
	return out;
}

template <typename get_t, typename variant_t>
    requires std::is_same_v<get_t, typename INode<get_t>::scalar_t> ||
             std::is_same_v<
                 get_t,
                 typename INode<typename get_t::value_t>::vector_t> ||
             std::is_same_v<
                 get_t,
                 typename INode<typename get_t::value_t>::quaternion_t>
constexpr get_t get_or_throw(const variant_t& variant,
                             int arg_num,
                             const std::string& structure) {
	const get_t* value = std::get_if<get_t>(&variant);
	if (value == nullptr) {
		throw std::logic_error(
		    std::string("Expected argument ") + details::int2string(arg_num) +
		    " of " + structure + " to be " + details::get_type_name(*value) +
		    "; got " +
		    std::visit([](auto x) { return details::get_type_name(x); },
		               variant));
	}
	return *value;
}
} // namespace details

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
			values[i] = details::get_or_throw<scalar_t>(result, i, "Vector");
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

} // namespace node
} // namespace ast
