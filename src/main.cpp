#include <ast/math.hpp>
#include <ast/node.hpp>
#include <iostream>
#include <quaternion/quaternion.hpp>
#include <variant>

int main() {
	using T = float;

	auto scalar = ast::node::Scalar<T>::make_unique(4.f);

	auto vector = ast::node::Vector<T>::make_unique(
	    ast::node::Scalar<T>::make_unique(1.f),
	    ast::node::Scalar<T>::make_unique(2.f),
	    ast::node::Scalar<T>::make_unique(3.f));

	auto quaternion = ast::node::Quaternion<T>::make_unique(
	    scalar->copy_unique(), vector->copy_unique());

	auto root = ast::node::math::Plus<T>::make_unique(quaternion->copy_unique(),
	                                                  vector->copy_unique());

	auto root2 = root->copy_unique();

	try {
		auto result = root2->evaluate();
		std::visit([](auto x) { std::cout << x << '\n'; }, result);
	} catch (const std::logic_error& e) {
		std::cout << e.what() << '\n';
	}
}
