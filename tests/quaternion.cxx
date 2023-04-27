#include "../src/quaternion/quaternion.hpp"
#include "common.hxx"
#include <fmt/core.h>

auto test_quat = [](auto& quat, auto x, auto vec) {
	auto _x = quat.real();
	auto _vec = quat.imag();

	REQUIRE(std::is_same_v<decltype(_x), decltype(x)>);
	REQUIRE(std::is_same_v<decltype(_vec), decltype(vec)>);

	REQUIRE(_x == x);
	REQUIRE(_vec == vec);
};

TEMPLATE_LIST_TEST_CASE("Quaternion", "Quaternion[template]", floatTypes) {
	TestType _real = 1.f;
	TestType _i = 2.f, _j = 3.f, _k = 4.f;
	q::Vector3 _imag(_i, _j, _k);

	q::Quaternion _scal_quat = q::Quaternion(_real, _i, _j, _k);
	q::Quaternion _vec_quat = q::Quaternion(_real, _imag);

	SECTION("C'tor") {
		test_quat(_scal_quat, _real, _imag);
		test_quat(_vec_quat, _real, _imag);
	}

	SECTION("C'tor equality") { REQUIRE(_scal_quat == _vec_quat); }

	SECTION("Equality") {
		REQUIRE(_scal_quat == q::Quaternion(_real, _i, _j, _k));
		REQUIRE(one_val != q::Quaternion(_real + 1, _i, _j, _k));
		REQUIRE(one_val != q::Quaternion(_real, _i + 1, _j, _k));
		REQUIRE(one_val != q::Quaternion(_real, _i, _j + 1, _k));
		REQUIRE(one_val != q::Quaternion(_real, _i, _j, _k + 1));
	}

	SECTION("String representation") {
		REQUIRE(to_string(_scal_quat) ==
		        fmt::format("Q({}, {})", _real, to_string(_imag)));
		REQUIRE(to_string(_vec_quat) ==
		        fmt::format("Q({}, {})", _real, to_string(_imag)));
	}
}
