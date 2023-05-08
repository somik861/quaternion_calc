#include "../common.hxx"
#include <parser/parser.hpp>
#include <string_view>

template <typename get_t, typename parser_t>
get_t parse_evaluate(std::string_view sv) {
	prsr::Parser<parser_t> parser;

	return std::get<get_t>(parser.parse(sv)->evaluate());
}

TEMPLATE_LIST_TEST_CASE("Parser", "Parser[template]", floatTypes) {
	using T = TestType;

	SECTION("Scalar") {
		REQUIRE(isclose(T(1), parse_evaluate<T, T>("1")));
		REQUIRE(isclose(T(1.453), parse_evaluate<T, T>("  1.453  ")));
		REQUIRE(isclose(T(0.453), parse_evaluate<T, T>("0.453  ")));
		REQUIRE(isclose(T(-15.222), parse_evaluate<T, T>("   -15.222")));
	}
}
