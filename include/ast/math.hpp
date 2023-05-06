#pragma once
#include "node.hpp"

namespace ast::node::math {
namespace details {
class Cross {
  public:
	template <typename T>
	constexpr q::Vector3<T> operator()(const q::Vector3<T>& lhs,
	                                   const q::Vector3<T>& rhs) {
		return q::cross(lhs, rhs);
	}
};

class Dot {
  public:
	template <typename T>
	constexpr T operator()(const q::Vector3<T>& lhs, const q::Vector3<T>& rhs) {
		return q::dot(lhs, rhs);
	}
};

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

template <>
constexpr std::string get_op_name<Cross>() {
	return "Cross";
};

template <>
constexpr std::string get_op_name<Dot>() {
	return "Dot";
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

template <std::floating_point T>
using Plus = BinaryOperation<T, std::plus<>>;

template <std::floating_point T>
using Minus = BinaryOperation<T, std::minus<>>;

template <std::floating_point T>
using Multiplies = BinaryOperation<T, std::multiplies<>>;

template <std::floating_point T>
using Divides = BinaryOperation<T, std::divides<>>;

template <std::floating_point T>
using Cross = BinaryOperation<T, details::Cross>;

template <std::floating_point T>
using Dot = BinaryOperation<T, details::Dot>;

} // namespace ast::node::math
