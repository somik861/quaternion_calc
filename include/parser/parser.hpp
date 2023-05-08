#pragma once
#include <array>
#include <ast/math.hpp>
#include <ast/node.hpp>
#include <boost/container/flat_set.hpp>
#include <boost/lexical_cast.hpp>
#include <fmt/core.h>
#include <sstream>
#include <stdexcept>
#include <string_view>

namespace prsr {
template <typename T>
class Parser {
  private:
	using uptr_t = typename ast::node::INode<T>::uptr_t;

  public:
	uptr_t parse(std::string_view sv) {
		_buffer = sv;
		_original_size = sv.size();

		_skip_whitespaces();
		uptr_t rv = _parse_elem();
		_skip_whitespaces();

		if (_available())
			throw std::logic_error(fmt::format(
			    "Expected the line to end at position {}", _current_idx()));

		sv = "";
		return rv;
	}

  private:
	/* ======= parser logic ======== */
	uptr_t _parse_elem() {
		char ch = _peek();

		if (DIGITS.contains(ch) || ch == '-')
			return _parse_scalar();
		if (ch == '[')
			return _parse_vector(); // parse vector
		if (ch == 'Q')
			return _parse_quaternion();   // parse quaternion
		return _parse_prefix_operation(); // parse operation
	}

	uptr_t _parse_scalar() {
		bool dot_seen = false;
		std::string str;

		while (_available()) {
			char ch = _peek();
			// not a digit, dot or a sign
			if (!(DIGITS.contains(ch) || (ch == '.' && !dot_seen) ||
			      (ch == '-' && str.empty())))
				break;

			if (ch == '.')
				dot_seen = true;
			str.push_back(ch);

			_skip();
		}

		return ast::node::Scalar<T>::make_unique(boost::lexical_cast<T>(str));
	}

	uptr_t _parse_vector() {
		_require('[');

		auto nodes = _get_nodes<3>();

		_require(']');
		return ast::node::Vector<T>::make_unique(
		    std::move(nodes[0]), std::move(nodes[1]), std::move(nodes[2]));
	}

	uptr_t _parse_quaternion() {
		uptr_t rv;
		_require('Q');
		_require('(');

		auto nodes = _get_nodes<2>();

		if (_peek() != ',') { // 2-value c'tor
			rv = ast::node::Quaternion<T>::make_unique(std::move(nodes[0]),
			                                           std::move(nodes[1]));
		} else { // 4-value c'tor
			_skip();
			auto second_nodes = _get_nodes<2>();
			rv = ast::node::Quaternion<T>::make_unique(
			    std::move(nodes[0]), std::move(nodes[1]),
			    std::move(second_nodes[0]), std::move(second_nodes[1]));
		}

		_require(')');
		return rv;
	}

	uptr_t _parse_prefix_operation() {
		std::string op_name;
		char ch;
		while (_is_letter(_peek()))
			op_name.push_back(_consume());

		_require('(');
		uptr_t rv;
		// Unary prefix functions
		if (op_name == "Norm")
			rv = ast::node::math::Norm<T>::make_unique(
			    std::move(std::move(_get_nodes<1>()[0])));
		else if (op_name == "Sum")
			rv = ast::node::math::Sum<T>::make_unique(
			    std::move(_get_nodes<1>()[0]));
		else if (op_name == "Reciprocal")
			rv = ast::node::math::Reciprocal<T>::make_unique(
			    std::move(_get_nodes<1>()[0]));
		else if (op_name == "Conjugate")
			rv = ast::node::math::Conjugate<T>::make_unique(
			    std::move(_get_nodes<1>()[0]));
		else if (op_name == "Real")
			rv = ast::node::math::Real<T>::make_unique(
			    std::move(_get_nodes<1>()[0]));
		else if (op_name == "Imag")
			rv = ast::node::math::Imag<T>::make_unique(
			    std::move(_get_nodes<1>()[0]));

		// Binary prefix functions
		_require(')');

		if (rv == nullptr)
			throw std::logic_error(
			    fmt::format("Unknown function: '{}'", op_name));

		return rv;
	}

	void _skip_whitespaces() {
		while (_available() && WHITESPACES.contains(_peek()))
			_skip();
	}

	/* ======= basic operations ======== */
	constexpr char _peek() const {
		_require_available();
		return _buffer[0];
	}
	constexpr void _skip(int n = 1) {
		_require_available();
		_buffer.remove_prefix(n);
	}
	constexpr void _require(char ch) {
		_require_available();
		if (_peek() != ch)
			throw std::logic_error(
			    fmt::format("Expected '{}' at position {}, got '{}'", ch,
			                _current_idx(), _peek()));
		_skip();
	}
	constexpr bool _available() const { return !_buffer.empty(); }
	constexpr void _require_available() const {
		if (!_available())
			throw std::logic_error(
			    fmt::format("Unexpected end at position {}", _current_idx()));
	}
	constexpr char _consume() {
		_require_available();
		char rv = _peek();
		_skip();
		return rv;
	}
	constexpr std::size_t _current_idx() const {
		return _original_size - _buffer.size();
	}

	template <std::size_t N>
	constexpr std::array<uptr_t, N> _get_nodes() {
		std::array<uptr_t, N> nodes;
		for (std::size_t i = 0; i < nodes.size(); ++i) {
			_skip_whitespaces();
			nodes[i] = _parse_elem();
			_skip_whitespaces();
			if (i != N - 1)
				_require(',');
		}

		return nodes;
	}

	constexpr bool _is_letter(char ch) const {
		return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z');
	}

	std::string_view _buffer;
	std::size_t _original_size;
	const static inline boost::container::flat_set<char> WHITESPACES{
	    ' ', '\n', '\t', '\r', '\f', '\b'};
	const static inline boost::container::flat_set<char> DIGITS{
	    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
};
} // namespace prsr
