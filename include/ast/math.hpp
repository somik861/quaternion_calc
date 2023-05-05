#pragma once
#include "node.hpp"

namespace ast::node::math {
namespace details {
template <typename T>
constexpr std::string get_op_name() {
	return "unkown";
}

template <>
constexpr std::string get_op_name<std::plus<>>() {
	return "Operator +";
};

template <>
constexpr std::string get_op_name<std::minus<>>() {
	return "Operator -";
};

template <>
constexpr std::string get_op_name<std::divides<>>() {
	return "Operator /";
};

template <>
constexpr std::string get_op_name<std::multiplies<>>() {
	return "Operator *";
};

} // namespace details

template <std::floating_point T, typename bin_op>
class BinaryOperation final : public INode<T> {
  private:
	using uptr_t = typename INode<T>::uptr_t;
	using vector_t = typename INode<T>::vector_t;
	using result_t = typename INode<T>::result_t;

  public:
	constexpr BinaryOperation(uptr_t lhs, uptr_t rhs)
	    : _children{std::move(lhs), std::move(rhs)} {}

	constexpr static uptr_t make_unique(uptr_t lhs, uptr_t rhs) {
		return std::make_unique<BinaryOperation<T, bin_op>>(std::move(lhs),
		                                                    std::move(rhs));
	}

	constexpr uptr_t copy_unique() const override {
		return BinaryOperation<T, bin_op>::make_unique(
		    _children[0]->copy_unique(), _children[1]->copy_unique());
	}

	constexpr result_t evaluate() const override {
		return std::visit(
		    [this](auto lhs, auto rhs) -> result_t {
			    if constexpr (std::is_invocable_v<bin_op, decltype(lhs),
			                                      decltype(rhs)>)
				    return bin_op{}(lhs, rhs);
			    else
				    throw std::logic_error(
				        std::string("'") + details::get_op_name<bin_op>() +
				        "' cannot be invoked with "
				        "following arguments: '" +
				        ast::node::details::get_type_name(lhs) + "', '" +
				        ast::node::details::get_type_name(rhs) + "'. ");
		    },
		    _children[0]->evaluate(), _children[1]->evaluate());
	}

  private:
	std::array<uptr_t, 2> _children;
};

template <typename T>
using Plus = BinaryOperation<T, std::plus<>>;

template <typename T>
using Minus = BinaryOperation<T, std::minus<>>;

template <typename T>
using Multiplies = BinaryOperation<T, std::multiplies<>>;

template <typename T>
using Divides = BinaryOperation<T, std::divides<>>;

template <std::floating_point T>
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

	constexpr uptr_t copy_unique() const override {
		return Cross<T>::make_unique(_children[0]->copy_unique(),
		                             _children[1]->copy_unique());
	}
	constexpr result_t evaluate() const override {
		vector_t lhs = ast::node::details::get_or_throw<vector_t>(
		    _children[0]->evaluate(), 0, "Cross");
		vector_t rhs = ast::node::details::get_or_throw<vector_t>(
		    _children[1]->evaluate(), 1, "Cross");
		return q::cross(lhs, rhs);
	}

  private:
	std::array<uptr_t, 2> _children;
};

template <std::floating_point T>
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

	constexpr uptr_t copy_unique() const override {
		return Dot<T>::make_unique(_children[0]->copy_unique(),
		                           _children[1]->copy_unique());
	}
	constexpr result_t evaluate() const override {
		vector_t lhs =
		    ast::node::details::get_or_throw<vector_t>(_children[0], 0, "Dot");
		vector_t rhs =
		    ast::node::details::get_or_throw<vector_t>(_children[1], 1, "Dot");

		return q::dot(lhs, rhs);
	}

  private:
	std::array<uptr_t, 2> _children;
};

} // namespace ast::node::math
