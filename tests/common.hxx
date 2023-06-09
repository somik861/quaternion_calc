#pragma once
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <quaternion/concepts.hpp>
#include <quaternion/details.hpp>
#include <sstream>
#include <string>
#include <tuple>

using floatTypes = std::tuple<float, double>;

using q::details::isclose;
using q::details::to_string;
