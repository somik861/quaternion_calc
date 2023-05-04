#include "../common.hxx"
#include <ast/node.hpp>
#include <variant>

TEMPLATE_LIST_TEST_CASE("AST", "AST[template]", floatTypes) {
	using T = TestType;
	using ast::node::INode;
	using uptr_t = typename INode<T>::uptr_t;
	using result_t = typename INode<T>::result_t;
	using scalar_t = typename INode<T>::scalar_t;
	using vector_t = typename INode<T>::vector_t;
	using quaternion_t = typename INode<T>::quaternion_t;

	uptr_t vector_1 = ast::node::Vector<T>::make_unique(
	    ast::node::Scalar<T>::make_unique(1.f),
	    ast::node::Scalar<T>::make_unique(2.f),
	    ast::node::Scalar<T>::make_unique(3.f));

	vector_t vector_1_raw(1.f, 2.f, 3.f);

	uptr_t vector_2 = ast::node::Vector<T>::make_unique(
	    ast::node::Scalar<T>::make_unique(2.f),
	    ast::node::Scalar<T>::make_unique(3.f),
	    ast::node::Scalar<T>::make_unique(4.f));

	vector_t vector_2_raw(2.f, 3.f, 4.f);

	SECTION("Binary operators") {
		uptr_t node = ast::node::math::Plus<T>::make_unique(
		    std::move(vector_1), std::move(vector_2));
	};
}
