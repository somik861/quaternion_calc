#include "../src/quaternion/vector3.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("One value c'tor") {
	float val = 1.f;
	q::Vector3 vec(val);

	auto x = vec.x();
	auto y = vec.y();
	auto z = vec.z();

	REQUIRE(std::is_same_v<decltype(x), float>);
	REQUIRE(std::is_same_v<decltype(y), float>);
	REQUIRE(std::is_same_v<decltype(z), float>);

	REQUIRE(x == val);
	REQUIRE(y == val);
	REQUIRE(z == val);
}