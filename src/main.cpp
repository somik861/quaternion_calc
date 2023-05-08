#include <ast/math.hpp>
#include <ast/node.hpp>
#include <iostream>
#include <parser/parser.hpp>
#include <quaternion/quaternion.hpp>
#include <variant>

int main() {
	using T = float;
	prsr::Parser parser;

	try {
		auto result = parser.parse("123")->evaluate();
		std::visit([](auto x) { std::cout << x << '\n'; }, result);
	} catch (const std::logic_error& e) {
		std::cout << e.what() << '\n';
	}
}
