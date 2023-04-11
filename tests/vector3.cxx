#include "../src/quaternion/vector3.hpp"
#include "common.hxx"
#include <format>

auto test_vec = [](auto& vec, auto x, auto y, auto z) {
	auto _x = vec.x();
	auto _y = vec.y();
	auto _z = vec.z();

	REQUIRE(std::is_same_v<decltype(_x), decltype(x)>);
	REQUIRE(std::is_same_v<decltype(_y), decltype(y)>);
	REQUIRE(std::is_same_v<decltype(_z), decltype(z)>);

	REQUIRE(x == _x);
	REQUIRE(y == _y);
	REQUIRE(z == _z);
};

TEMPLATE_LIST_TEST_CASE("Vector3", "[construtor][template]", floatTypes) {
	TestType _xyz = 1.f;
	q::Vector3 one_val(_xyz);

	SECTION("One-value c'tor") { test_vec(one_val, _xyz, _xyz, _xyz); }

	TestType _x = 1.f, _y = 2.f, _z = 3.f;
	q::Vector3 three_val(_x, _y, _z);
	SECTION("Three-value c'tor") { test_vec(three_val, _x, _y, _z); }

	SECTION("C'tor equality") {
		REQUIRE(one_val == q::Vector3(_xyz, _xyz, _xyz));
	}

	SECTION("Indexing") {
		test_vec(one_val, one_val[0], one_val[1], one_val[2]);
		test_vec(three_val, three_val[0], three_val[1], three_val[2]);
	}

	SECTION("String representation") {
		REQUIRE(to_string(one_val) ==
		        std::format("[{}, {}, {}]", _xyz, _xyz, _xyz));
		REQUIRE(to_string(three_val) ==
		        std::format("[{}, {}, {}]", _x, _y, _z));
	}
}
