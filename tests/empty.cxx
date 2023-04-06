#include <catch2/catch_test_macros.hpp>

TEST_CASE("EMPTY") {
	static_assert(true);
	REQUIRE(true == true);
}

