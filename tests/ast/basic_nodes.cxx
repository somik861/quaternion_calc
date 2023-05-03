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

	SECTION("Scalar") {
		uptr_t node = ast::node::Scalar<T>::make_unique(2.f);
		result_t result = node->evaluate();

		scalar_t* value = std::get_if<scalar_t>(&result);
		REQUIRE(value != nullptr);
		REQUIRE(isclose(*value, T(2)));
	}

	SECTION("Vector") {
		uptr_t node = ast::node::Vector<T>::make_unique(
		    ast::node::Scalar<T>::make_unique(1.f),
		    ast::node::Scalar<T>::make_unique(2.f),
		    ast::node::Scalar<T>::make_unique(3.f));

		result_t result = node->evaluate();
		vector_t* value = std::get_if<vector_t>(&result);
		REQUIRE(value != nullptr);
		REQUIRE(*value == vector_t(1.f, 2.f, 3.f));
	}

	SECTION("Quaternion") {
		uptr_t node = ast::node::Quaternion<T>::make_unique(
		    ast::node::Scalar<T>::make_unique(1.f),
		    ast::node::Vector<T>::make_unique(
		        ast::node::Scalar<T>::make_unique(2.f),
		        ast::node::Scalar<T>::make_unique(3.f),
		        ast::node::Scalar<T>::make_unique(4.f)));

		result_t result = node->evaluate();
		quaternion_t* value = std::get_if<quaternion_t>(&result);
		REQUIRE(value != nullptr);
		REQUIRE(*value == quaternion_t(1.f, 2.f, 3.f, 4.f));

		node = ast::node::Quaternion<T>::make_unique(
		    ast::node::Scalar<T>::make_unique(1.f),
		    ast::node::Scalar<T>::make_unique(2.f),
		    ast::node::Scalar<T>::make_unique(3.f),
		    ast::node::Scalar<T>::make_unique(4.f));

		result = node->evaluate();
		value = std::get_if<quaternion_t>(&result);
		REQUIRE(value != nullptr);
		REQUIRE(*value == quaternion_t(1.f, 2.f, 3.f, 4.f));
	}
}
