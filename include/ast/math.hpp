#pragma once
#include "node.hpp"

namespace ast::node::math {
namespace details {
class Cross {
  public:
	template <std::floating_point T>
	constexpr q::Vector3<T> operator()(const q::Vector3<T>& lhs,
	                                   const q::Vector3<T>& rhs) {
		return q::cross(lhs, rhs);
	}
};

class Dot {
  public:
	template <std::floating_point T>
	constexpr T operator()(const q::Vector3<T>& lhs, const q::Vector3<T>& rhs) {
		return q::dot(lhs, rhs);
	}
};

class Pow {
  public:
	template <std::floating_point T>
	constexpr q::Vector3<T> operator()(const q::Vector3<T>& arg, T exp) {
		return arg.powed(exp);
	}
	template <std::floating_point T>
	constexpr T operator()(T arg, T exp) {
		return std::pow(arg, exp);
	}
};

class Norm {
  public:
	template <std::floating_point T>
	constexpr T operator()(const q::Vector3<T>& arg) {
		return arg.norm();
	}
	template <std::floating_point T>
	constexpr T operator()(const q::Quaternion<T>& arg) {
		return arg.norm();
	}
};
class Sum {
  public:
	template <std::floating_point T>
	constexpr T operator()(const q::Vector3<T>& arg) {
		return arg.sum();
	}
};
class Conjugate {
  public:
	template <std::floating_point T>
	constexpr q::Quaternion<T> operator()(const q::Quaternion<T>& arg) {
		return arg.conjugated();
	}
};
class Reciprocal {
  public:
	template <std::floating_point T>
	constexpr q::Quaternion<T> operator()(const q::Quaternion<T>& arg) {
		return arg.reciprocaled();
	}
	template <std::floating_point T>
	constexpr T operator()(T arg) {
		return T(1) / arg;
	}
};
class Real {
  public:
	template <std::floating_point T>
	constexpr T operator()(const q::Quaternion<T>& arg) {
		return arg.real();
	}
};
class Imag {
  public:
	template <std::floating_point T>
	constexpr const q::Vector3<T>& operator()(const q::Quaternion<T>& arg) {
		return arg.imag();
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

template <>
constexpr std::string get_op_name<Pow>() {
	return "Pow";
};

template <>
constexpr std::string get_op_name<Norm>() {
	return "Norm";
};

template <>
constexpr std::string get_op_name<Sum>() {
	return "Sum";
};

template <>
constexpr std::string get_op_name<Conjugate>() {
	return "Conjugate";
};

template <>
constexpr std::string get_op_name<Reciprocal>() {
	return "Reciprocal";
};

template <>
constexpr std::string get_op_name<Real>() {
	return "Real";
};

template <>
constexpr std::string get_op_name<Imag>() {
	return "Imag";
};

template <std::floating_point T, typename bin_op>
class BinaryOperation final : public INode<T> {
  private:
	using uptr_t = typename INode<T>::uptr_t;
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
				        std::string("'") + get_op_name<bin_op>() +
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

template <std::floating_point T, typename unary_op>
class UnaryOperation final : public INode<T> {
  private:
	using uptr_t = typename INode<T>::uptr_t;
	using result_t = typename INode<T>::result_t;

  public:
	constexpr UnaryOperation(uptr_t arg) : _child(std::move(arg)) {}

	constexpr static uptr_t make_unique(uptr_t arg) {
		return std::make_unique<UnaryOperation<T, unary_op>>(std::move(arg));
	}

	constexpr uptr_t copy_unique() const override {
		return UnaryOperation<T, unary_op>::make_unique(_child->copy_unique());
	}

	constexpr result_t evaluate() const override {
		return std::visit(
		    [](auto arg) -> result_t {
			    if constexpr (std::is_invocable_v<unary_op, decltype(arg)>)
				    return unary_op{}(arg);
			    else
				    throw std::logic_error(
				        std::string("'") + get_op_name<unary_op>() +
				        "' cannot be invoked with "
				        "following argument: '" +
				        ast::node::details::get_type_name(arg) + "'.");
		    },
		    _child->evaluate());
	}

  private:
	uptr_t _child;
};

} // namespace details

template <std::floating_point T>
using Plus = details::BinaryOperation<T, std::plus<>>;

template <std::floating_point T>
using Minus = details::BinaryOperation<T, std::minus<>>;

template <std::floating_point T>
using Multiplies = details::BinaryOperation<T, std::multiplies<>>;

template <std::floating_point T>
using Divides = details::BinaryOperation<T, std::divides<>>;

template <std::floating_point T>
using Cross = details::BinaryOperation<T, details::Cross>;

template <std::floating_point T>
using Dot = details::BinaryOperation<T, details::Dot>;

template <std::floating_point T>
using Pow = details::BinaryOperation<T, details::Pow>;

template <std::floating_point T>
using Norm = details::UnaryOperation<T, details::Norm>;

template <std::floating_point T>
using Sum = details::UnaryOperation<T, details::Sum>;

template <std::floating_point T>
using Conjugate = details::UnaryOperation<T, details::Conjugate>;

template <std::floating_point T>
using Reciprocal = details::UnaryOperation<T, details::Reciprocal>;

template <std::floating_point T>
using Real = details::UnaryOperation<T, details::Real>;

template <std::floating_point T>
using Imag = details::UnaryOperation<T, details::Imag>;
} // namespace ast::node::math
