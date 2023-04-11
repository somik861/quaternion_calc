#pragma once
#include "../src/quaternion/concepts.hpp"
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include <string>
#include <tuple>

using floatTypes = std::tuple<float, double>;

using q::details::to_string;