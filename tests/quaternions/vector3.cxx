#include "../../src/quaternion/vector3.hpp"
#include "../common.hxx"
#include <fmt/core.h>

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

TEMPLATE_LIST_TEST_CASE("Vector3", "Vector3[template]", floatTypes) {
	TestType _xyz = 1.f;
	q::Vector3 one_val(_xyz);

	SECTION("One-value c'tor") { test_vec(one_val, _xyz, _xyz, _xyz); }

	TestType _x = 1.f, _y = 2.f, _z = 3.f;
	q::Vector3 three_val(_x, _y, _z);
	SECTION("Three-value c'tor") { test_vec(three_val, _x, _y, _z); }

	SECTION("C'tor equality") {
		REQUIRE(one_val == q::Vector3(_xyz, _xyz, _xyz));
	}

	SECTION("Equality") {
		REQUIRE(one_val == q::Vector3(_xyz));
		REQUIRE(one_val != q::Vector3(_xyz + 1, _xyz, _xyz));
		REQUIRE(one_val != q::Vector3(_xyz, _xyz + 1, _xyz));
		REQUIRE(one_val != q::Vector3(_xyz, _xyz, _xyz + 1));
	}

	SECTION("Indexing") {
		test_vec(one_val, one_val[0], one_val[1], one_val[2]);
		test_vec(three_val, three_val[0], three_val[1], three_val[2]);
	}

	SECTION("String representation") {
		REQUIRE(to_string(one_val) ==
		        fmt::format("[{}, {}, {}]", _xyz, _xyz, _xyz));
		REQUIRE(to_string(three_val) ==
		        fmt::format("[{}, {}, {}]", _x, _y, _z));
	}

	SECTION("Math operators") {
		using T = TestType;
		using vec_t = q::Vector3<TestType>;
		REQUIRE(one_val + three_val == vec_t(2.f, 3.f, 4.f));
		REQUIRE(one_val - three_val == vec_t(0.f, -1.f, -2.f));
		REQUIRE(one_val * three_val == three_val);
		REQUIRE(one_val / three_val == vec_t(1.f, T(1) / T(2), T(1) / T(3)));
		REQUIRE(-one_val == vec_t(-1.f, -1.f, -1.f));
		REQUIRE(-three_val == vec_t(-1.f, -2.f, -3.f));

		REQUIRE(three_val + T(2) == vec_t(3.f, 4.f, 5.f));
		REQUIRE(three_val + T(2) == T(2) + three_val);

		REQUIRE(three_val * T(2) == vec_t(2.f, 4.f, 6.f));
		REQUIRE(three_val * T(2) == T(2) * three_val);

		REQUIRE(three_val - T(2) == vec_t(-1.f, 0.f, 1.f));
		REQUIRE(T(2) - three_val == vec_t(1.f, 0.f, -1.f));

		REQUIRE(three_val / T(2) == vec_t(T(1) / T(2), 1.f, T(3) / T(2)));
		REQUIRE(T(2) / three_val == vec_t(T(2), 1.f, T(2) / T(3)));

		// Inplace operators
		vec_t cpy = three_val; // (1, 2, 3)
		cpy += one_val;        // += (1, 1, 1)
		REQUIRE(cpy == vec_t(2.f, 3.f, 4.f));
		REQUIRE(three_val == vec_t(1.f, 2.f, 3.f));
		REQUIRE(one_val == vec_t(1.f, 1.f, 1.f));

		cpy += T(2);
		REQUIRE(cpy == vec_t(4.f, 5.f, 6.f));

		cpy -= one_val; // -= (1, 1, 1)
		REQUIRE(cpy == vec_t(3.f, 4.f, 5.f));

		cpy -= T(2);
		REQUIRE(cpy == vec_t(1.f, 2.f, 3.f));

		cpy *= T(2);
		REQUIRE(cpy == vec_t(2.f, 4.f, 6.f));

		cpy *= cpy;
		REQUIRE(cpy == vec_t(4.f, 16.f, 36.f));

		cpy /= T(2);
		REQUIRE(cpy == vec_t(2.f, 8.f, 18.f));

		cpy /= three_val; // /= (1, 2, 3)
		REQUIRE(cpy == vec_t(2.f, 4.f, 6.f));

		cpy.pow(2);
		REQUIRE(cpy == vec_t(4.f, 16.f, 36.f));

		// math functions
		REQUIRE(isclose(q::dot(three_val, three_val), T(14)));
		REQUIRE(isclose(three_val.norm(), std::sqrt(T(14))));
		REQUIRE(q::cross(one_val, one_val) == vec_t(0.f, 0.f, 0.f));
		REQUIRE(q::cross(three_val, three_val) == vec_t(0.f, 0.f, 0.f));
		REQUIRE(q::cross(one_val, three_val) == vec_t(1.f, -2.f, 1.f));
		REQUIRE(q::cross(one_val, three_val) == -q::cross(three_val, one_val));

		REQUIRE(three_val.powed(2) == vec_t(1.f, 4.f, 9.f));
		REQUIRE(isclose(one_val.sum(), T(3)));
		REQUIRE(isclose(three_val.sum(), T(6)));
	}
}
