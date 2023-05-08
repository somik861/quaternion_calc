#include <ast/math.hpp>
#include <ast/node.hpp>
#include <iostream>
#include <parser/parser.hpp>
#include <quaternion/quaternion.hpp>
#include <variant>

int main(int argc, char** argv) {
	using T = float;
	prsr::Parser<T> parser;

	try {
		auto result = parser.parse(argv[1])->evaluate();
		std::visit([](auto x) { std::cout << x << '\n'; }, result);
	} catch (const std::exception& e) {
		std::cout << e.what() << '\n';
	}
}
