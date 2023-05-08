#include "../common.hxx"
#include <parser/parser.hpp>
#include <quaternion/quaternion.hpp>
#include <quaternion/vector3.hpp>
#include <string_view>

template <typename get_t, typename parser_t>
get_t parse_evaluate(std::string_view sv) {
	prsr::Parser<parser_t> parser;

	return std::get<get_t>(parser.parse(sv)->evaluate());
}

TEMPLATE_LIST_TEST_CASE("Parser", "Parser[template]", floatTypes) {
	using T = TestType;
	using vec_t = q::Vector3<T>;
	using quat_t = q::Quaternion<T>;

	SECTION("Scalar") {
		REQUIRE(isclose(T(1), parse_evaluate<T, T>("1")));
		REQUIRE(isclose(T(1.453), parse_evaluate<T, T>("  1.453  ")));
		REQUIRE(isclose(T(0.453), parse_evaluate<T, T>("0.453  ")));
		REQUIRE(isclose(T(-15.222), parse_evaluate<T, T>("   -15.222")));
	}

	SECTION("Vector") {
		REQUIRE(vec_t(1, 2, 3) == parse_evaluate<vec_t, T>("[ 1, 2  , 3  ]"));
		REQUIRE(vec_t(-1.453, -98, -12.5) ==
		        parse_evaluate<vec_t, T>("[-1.453, -98, -12.5  ]"));
		REQUIRE(vec_t(3, 1, 2) == parse_evaluate<vec_t, T>("  [3,1,2]  "));
		REQUIRE(vec_t(4, 3, 3.123) ==
		        parse_evaluate<vec_t, T>("[  4,        3,      3.123]"));
	}

	SECTION("Quaternion") {
		REQUIRE(quat_t(1, 2, 3, 4) ==
		        parse_evaluate<quat_t, T>("Q(1, 2, 3, 4)"));
		REQUIRE(quat_t(1, 2, 3, 4) ==
		        parse_evaluate<quat_t, T>("Q(1, [2, 3, 4])"));
		REQUIRE(quat_t(-1, 2, 3, 4) ==
		        parse_evaluate<quat_t, T>("Q(-1, 2, 3, 4)"));
		REQUIRE(quat_t(-1, 2, 3, 4) ==
		        parse_evaluate<quat_t, T>("Q(-1, [2, 3, 4])"));
	}

	SECTION("Unary prefix functions") {
		REQUIRE(isclose(vec_t(1, 2, 3).norm(),
		                parse_evaluate<T, T>("Norm([1,2,3])")));
		REQUIRE(isclose(quat_t(1, 2, 3, 4).norm(),
		                parse_evaluate<T, T>("Norm(Q(1, [2,3,4]))")));
		REQUIRE(isclose(vec_t(1, 2, 3).sum(),
		                parse_evaluate<T, T>("Sum([1,2,3])")));
		REQUIRE(quat_t(1, 2, 3, 4).reciprocaled() ==
		        parse_evaluate<quat_t, T>("Reciprocal(Q(1, [2,3,4]))"));
		REQUIRE(quat_t(1, 2, 3, 4).conjugated() ==
		        parse_evaluate<quat_t, T>("Conjugate(Q(1, [2,3,4]))"));
		REQUIRE(isclose(quat_t(1, 2, 3, 4).real(),
		                parse_evaluate<T, T>("Real(Q(1, [2,3,4]))")));
		REQUIRE(quat_t(1, 2, 3, 4).imag() ==
		        parse_evaluate<vec_t, T>("Imag(Q(1, [2,3,4]))"));
	}
}
