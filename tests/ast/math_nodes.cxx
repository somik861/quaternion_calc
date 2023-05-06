#include "../common.hxx"
#include <ast/math.hpp>
#include <variant>

TEMPLATE_LIST_TEST_CASE("AST", "AST[template]", floatTypes) {
	using T = TestType;
	using ast::node::INode;
	using uptr_t = typename INode<T>::uptr_t;
	using result_t = typename INode<T>::result_t;
	using scalar_t = typename INode<T>::scalar_t;
	using vector_t = typename INode<T>::vector_t;
	using quaternion_t = typename INode<T>::quaternion_t;

	scalar_t scalar_1_raw = 1.f;
	scalar_t scalar_2_raw = 2.f;
	uptr_t scalar_1 = ast::node::Scalar<T>::make_unique(scalar_1_raw);
	uptr_t scalar_2 = ast::node::Scalar<T>::make_unique(scalar_2_raw);

	vector_t vector_1_raw(1.f, 2.f, 3.f);
	uptr_t vector_1 = ast::node::Vector<T>::make_unique(
	    ast::node::Scalar<T>::make_unique(1.f),
	    ast::node::Scalar<T>::make_unique(2.f),
	    ast::node::Scalar<T>::make_unique(3.f));

	vector_t vector_2_raw(2.f, 3.f, 4.f);
	uptr_t vector_2 = ast::node::Vector<T>::make_unique(
	    ast::node::Scalar<T>::make_unique(2.f),
	    ast::node::Scalar<T>::make_unique(3.f),
	    ast::node::Scalar<T>::make_unique(4.f));

	quaternion_t quat_1_raw(scalar_1_raw, vector_1_raw);
	quaternion_t quat_2_raw(scalar_2_raw, vector_2_raw);
	uptr_t quat_1 = ast::node::Quaternion<T>::make_unique(
	    scalar_1->copy_unique(), vector_1->copy_unique());

	uptr_t quat_2 = ast::node::Quaternion<T>::make_unique(
	    scalar_2->copy_unique(), vector_2->copy_unique());

	SECTION("Binary operators on scalars") {
		{
			uptr_t node = ast::node::math::Plus<T>::make_unique(
			    scalar_1->copy_unique(), scalar_2->copy_unique());

			result_t result = node->evaluate();
			scalar_t* value = std::get_if<scalar_t>(&result);
			REQUIRE(value != nullptr);
			REQUIRE(isclose(*value, scalar_1_raw + scalar_2_raw));
		}

		{
			uptr_t node = ast::node::math::Minus<T>::make_unique(
			    scalar_1->copy_unique(), scalar_2->copy_unique());

			result_t result = node->evaluate();
			scalar_t* value = std::get_if<scalar_t>(&result);
			REQUIRE(value != nullptr);
			REQUIRE(isclose(*value, scalar_1_raw - scalar_2_raw));
		}

		{
			uptr_t node = ast::node::math::Multiplies<T>::make_unique(
			    scalar_1->copy_unique(), scalar_2->copy_unique());

			result_t result = node->evaluate();
			scalar_t* value = std::get_if<scalar_t>(&result);
			REQUIRE(value != nullptr);
			REQUIRE(isclose(*value, scalar_1_raw * scalar_2_raw));
		}

		{
			uptr_t node = ast::node::math::Divides<T>::make_unique(
			    scalar_1->copy_unique(), scalar_2->copy_unique());

			result_t result = node->evaluate();
			scalar_t* value = std::get_if<scalar_t>(&result);
			REQUIRE(value != nullptr);
			REQUIRE(isclose(*value, scalar_1_raw / scalar_2_raw));
		}
	}

	SECTION("Binary operators on vectors") {
		{
			uptr_t node = ast::node::math::Plus<T>::make_unique(
			    vector_1->copy_unique(), vector_2->copy_unique());

			result_t result = node->evaluate();
			vector_t* value = std::get_if<vector_t>(&result);
			REQUIRE(value != nullptr);
			REQUIRE(*value == (vector_1_raw + vector_2_raw));
		}

		{
			uptr_t node = ast::node::math::Minus<T>::make_unique(
			    vector_1->copy_unique(), vector_2->copy_unique());

			result_t result = node->evaluate();
			vector_t* value = std::get_if<vector_t>(&result);
			REQUIRE(value != nullptr);
			REQUIRE(*value == (vector_1_raw - vector_2_raw));
		}

		{
			uptr_t node = ast::node::math::Multiplies<T>::make_unique(
			    vector_1->copy_unique(), vector_2->copy_unique());

			result_t result = node->evaluate();
			vector_t* value = std::get_if<vector_t>(&result);
			REQUIRE(value != nullptr);
			REQUIRE(*value == (vector_1_raw * vector_2_raw));
		}

		{
			uptr_t node = ast::node::math::Divides<T>::make_unique(
			    vector_1->copy_unique(), vector_2->copy_unique());

			result_t result = node->evaluate();
			vector_t* value = std::get_if<vector_t>(&result);
			REQUIRE(value != nullptr);
			REQUIRE(*value == (vector_1_raw / vector_2_raw));
		}

		{
			uptr_t node = ast::node::math::Cross<T>::make_unique(
			    vector_1->copy_unique(), vector_2->copy_unique());

			result_t result = node->evaluate();
			vector_t* value = std::get_if<vector_t>(&result);
			REQUIRE(value != nullptr);
			REQUIRE(*value == (q::cross(vector_1_raw, vector_2_raw)));
		}

		{
			uptr_t node = ast::node::math::Dot<T>::make_unique(
			    vector_1->copy_unique(), vector_2->copy_unique());

			result_t result = node->evaluate();
			scalar_t* value = std::get_if<scalar_t>(&result);
			REQUIRE(value != nullptr);
			REQUIRE(isclose(*value, q::dot(vector_1_raw, vector_2_raw)));
		}
	}

	SECTION("Binary operators on quaternions") {
		{
			uptr_t node = ast::node::math::Plus<T>::make_unique(
			    quat_1->copy_unique(), quat_2->copy_unique());

			result_t result = node->evaluate();
			quaternion_t* value = std::get_if<quaternion_t>(&result);
			REQUIRE(value != nullptr);
			REQUIRE(*value == (quat_1_raw + quat_2_raw));
		}

		{
			uptr_t node = ast::node::math::Minus<T>::make_unique(
			    quat_1->copy_unique(), quat_2->copy_unique());

			result_t result = node->evaluate();
			quaternion_t* value = std::get_if<quaternion_t>(&result);
			REQUIRE(value != nullptr);
			REQUIRE(*value == (quat_1_raw - quat_2_raw));
		}

		{
			uptr_t node = ast::node::math::Multiplies<T>::make_unique(
			    quat_1->copy_unique(), quat_2->copy_unique());

			result_t result = node->evaluate();
			quaternion_t* value = std::get_if<quaternion_t>(&result);
			REQUIRE(value != nullptr);
			REQUIRE(*value == (quat_1_raw * quat_2_raw));
		}
	}

	SECTION("Binary operators on vectors with scalars") {
		{
			uptr_t node = ast::node::math::Plus<T>::make_unique(
			    vector_1->copy_unique(), scalar_2->copy_unique());

			result_t result = node->evaluate();
			vector_t* value = std::get_if<vector_t>(&result);
			REQUIRE(value != nullptr);
			REQUIRE(*value == (vector_1_raw + scalar_2_raw));
		}

		{
			uptr_t node = ast::node::math::Minus<T>::make_unique(
			    vector_1->copy_unique(), scalar_2->copy_unique());

			result_t result = node->evaluate();
			vector_t* value = std::get_if<vector_t>(&result);
			REQUIRE(value != nullptr);
			REQUIRE(*value == (vector_1_raw - scalar_2_raw));
		}

		{
			uptr_t node = ast::node::math::Multiplies<T>::make_unique(
			    vector_1->copy_unique(), scalar_2->copy_unique());

			result_t result = node->evaluate();
			vector_t* value = std::get_if<vector_t>(&result);
			REQUIRE(value != nullptr);
			REQUIRE(*value == (vector_1_raw * scalar_2_raw));
		}

		{
			uptr_t node = ast::node::math::Divides<T>::make_unique(
			    vector_1->copy_unique(), scalar_2->copy_unique());

			result_t result = node->evaluate();
			vector_t* value = std::get_if<vector_t>(&result);
			REQUIRE(value != nullptr);
			REQUIRE(*value == (vector_1_raw / scalar_2_raw));
		}
	}

	SECTION("Binary operators on scalars with vectors") {
		{
			uptr_t node = ast::node::math::Plus<T>::make_unique(
			    scalar_1->copy_unique(), vector_2->copy_unique());

			result_t result = node->evaluate();
			vector_t* value = std::get_if<vector_t>(&result);
			REQUIRE(value != nullptr);
			REQUIRE(*value == (scalar_1_raw + vector_2_raw));
		}

		{
			uptr_t node = ast::node::math::Minus<T>::make_unique(
			    scalar_1->copy_unique(), vector_2->copy_unique());

			result_t result = node->evaluate();
			vector_t* value = std::get_if<vector_t>(&result);
			REQUIRE(value != nullptr);
			REQUIRE(*value == (scalar_1_raw - vector_2_raw));
		}

		{
			uptr_t node = ast::node::math::Multiplies<T>::make_unique(
			    scalar_1->copy_unique(), vector_2->copy_unique());

			result_t result = node->evaluate();
			vector_t* value = std::get_if<vector_t>(&result);
			REQUIRE(value != nullptr);
			REQUIRE(*value == (scalar_1_raw * vector_2_raw));
		}

		{
			uptr_t node = ast::node::math::Divides<T>::make_unique(
			    scalar_1->copy_unique(), vector_2->copy_unique());

			result_t result = node->evaluate();
			vector_t* value = std::get_if<vector_t>(&result);
			REQUIRE(value != nullptr);
			REQUIRE(*value == (scalar_1_raw / vector_2_raw));
		}
	}

	SECTION("Binary operators on quaternions with scalars") {
		{
			uptr_t node = ast::node::math::Multiplies<T>::make_unique(
			    quat_1->copy_unique(), scalar_2->copy_unique());

			result_t result = node->evaluate();
			quaternion_t* value = std::get_if<quaternion_t>(&result);
			REQUIRE(value != nullptr);
			REQUIRE(*value == (quat_1_raw * scalar_2_raw));
		}
	}

	SECTION("Binary operators on scalars with quaternions") {
		{
			uptr_t node = ast::node::math::Multiplies<T>::make_unique(
			    scalar_1->copy_unique(), quat_2->copy_unique());

			result_t result = node->evaluate();
			quaternion_t* value = std::get_if<quaternion_t>(&result);
			REQUIRE(value != nullptr);
			REQUIRE(*value == (scalar_1_raw * quat_2_raw));
		}
	}
}
