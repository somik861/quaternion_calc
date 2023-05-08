#pragma once
#include <ast/node.hpp>
#include <boost/container/flat_set.hpp>
#include <boost/lexical_cast.hpp>
#include <fmt/core.h>
#include <sstream>
#include <stdexcept>
#include <string_view>

namespace prsr {
class Parser {
  private:
	using T = float;
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
			return nullptr; // parse vector
		if (ch == 'Q')
			return nullptr; // parse quaternion
		return nullptr;     // parse operation
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

	std::string_view _buffer;
	std::size_t _original_size;
	const static inline boost::container::flat_set<char> WHITESPACES{
	    ' ', '\n', '\t', '\r', '\f', '\b'};
	const static inline boost::container::flat_set<char> DIGITS{
	    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
};
} // namespace prsr
