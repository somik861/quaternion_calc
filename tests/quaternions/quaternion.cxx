#include "../common.hxx"
#include <fmt/core.h>
#include <quaternion/quaternion.hpp>

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
		REQUIRE(_scal_quat != q::Quaternion(_real + 1, _i, _j, _k));
		REQUIRE(_scal_quat != q::Quaternion(_real, _i + 1, _j, _k));
		REQUIRE(_scal_quat != q::Quaternion(_real, _i, _j + 1, _k));
		REQUIRE(_scal_quat != q::Quaternion(_real, _i, _j, _k + 1));
	}

	SECTION("String representation") {
		REQUIRE(to_string(_scal_quat) ==
		        fmt::format("Q({}, {})", _real, to_string(_imag)));
		REQUIRE(to_string(_vec_quat) ==
		        fmt::format("Q({}, {})", _real, to_string(_imag)));
	}

	SECTION("Math") {
		using T = TestType;
		using quat_t = q::Quaternion<T>;
		quat_t quat_1(1.f, 1.f, 1.f, 1.f);
		quat_t quat_2(2.f, 2.f, 2.f, 2.f);

		REQUIRE(_vec_quat.conjugated() == quat_t(1.f, -2.f, -3.f, -4.f));
		auto cpy = _vec_quat;
		cpy.conjugate();
		REQUIRE(cpy == quat_t(1.f, -2.f, -3.f, -4.f));

		REQUIRE(quat_1.reciprocaled() == quat_t(0.25f, -0.25f, -0.25f, -0.25f));

		cpy = quat_2;
		cpy.reciprocal();
		REQUIRE(cpy == quat_t(0.125f, -0.125f, -0.125f, -0.125f));

		REQUIRE(isclose(quat_1.norm(), T(2)));
		REQUIRE(isclose(quat_2.norm(), T(4)));

		REQUIRE(quat_1 + quat_2 == quat_t(3.f, 3.f, 3.f, 3.f));
		REQUIRE(quat_2 - quat_1 == quat_t(1.f, 1.f, 1.f, 1.f));
		REQUIRE(2 * quat_2 == quat_t(4.f, 4.f, 4.f, 4.f));
		REQUIRE(quat_1 * 3 == 3 * quat_1);
		REQUIRE(-quat_1 == quat_t(-1.f, -1.f, -1.f, -1.f));
		REQUIRE(-quat_2 == quat_t(-2.f, -2.f, -2.f, -2.f));
		REQUIRE(quat_1 * quat_2 == quat_t(-4.f, 4.f, 4.f, 4.f));

		cpy = quat_1;
		cpy += quat_2;
		REQUIRE(cpy == quat_t(3.f, 3.f, 3.f, 3.f));

		cpy -= quat_1;
		REQUIRE(cpy == quat_t(2.f, 2.f, 2.f, 2.f));

		cpy *= T(2);
		REQUIRE(cpy == quat_t(4.f, 4.f, 4.f, 4.f));

		cpy *= quat_2;
		REQUIRE(cpy == quat_t(-16.f, 16.f, 16.f, 16.f));
	}
}
