#include "../src/quaternion/vector3.hpp"
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <tuple>

using floatTypes = std::tuple<float, double>;

TEMPLATE_LIST_TEST_CASE("Constructors", "[construtor][template]", floatTypes) {
	SECTION("One-value c'tor") {
		TestType val = 1.f;
		q::Vector3 vec(val);

		auto x = vec.x();
		auto y = vec.y();
		auto z = vec.z();

		REQUIRE(std::is_same_v<decltype(x), TestType>);
		REQUIRE(std::is_same_v<decltype(y), TestType>);
		REQUIRE(std::is_same_v<decltype(z), TestType>);

		REQUIRE(x == val);
		REQUIRE(y == val);
		REQUIRE(z == val);
	}

	SECTION("Three-value c'tor") {
		TestType val_x = 1.f, val_y = 2.f, val_z = 3.f;
		q::Vector3 vec(val_x, val_y, val_z);

		auto x = vec.x();
		auto y = vec.y();
		auto z = vec.z();

		REQUIRE(std::is_same_v<decltype(x), TestType>);
		REQUIRE(std::is_same_v<decltype(y), TestType>);
		REQUIRE(std::is_same_v<decltype(z), TestType>);

		REQUIRE(x == val_x);
		REQUIRE(y == val_y);
		REQUIRE(z == val_z);
	}

	SECTION("C'tor equality") {
		TestType val = 1.f;

		q::Vector3 vec_a(val, val, val);
		q::Vector3 vec_b(val);

		REQUIRE(vec_a.x() == vec_b.x());
		REQUIRE(vec_a.y() == vec_b.y());
		REQUIRE(vec_a.z() == vec_b.z());
		REQUIRE(vec_a == vec_b);
	}
}
