#include "../common.hxx"
#include <parser/parser.hpp>
#include <string_view>

template <typename get_t>
get_t parse_evaluate(std::string_view sv) {
	prsr::Parser parser;

	return std::get<get_t>(parser.parse(sv)->evaluate());
}

TEMPLATE_LIST_TEST_CASE("Parser", "Parser[template]", floatTypes) {
	using T = TestType;

	SECTION("Scalar") {
		REQUIRE(isclose(1.f, parse_evaluate<float>("1")));
		REQUIRE(isclose(1.453f, parse_evaluate<float>("  1.453  ")));
		REQUIRE(isclose(0.453f, parse_evaluate<float>("0.453  ")));
		REQUIRE(isclose(-15.222f, parse_evaluate<float>("   -15.222")));
	}
}
