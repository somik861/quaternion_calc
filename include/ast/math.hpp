#pragma once
#include "node.hpp"

namespace ast::node::math {

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

} // namespace ast::node::math
