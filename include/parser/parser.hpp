#pragma once
#include <ast/node.hpp>
#include <boost/container/flat_set.hpp>
#include <fmt/core.h>
#include <stdexcept>
#include <string_view>

namespace parser {
class Parser {
  private:
	using T = float;
	using uptr_t = typename ast::node::INode<T>::uptr_t;

  public:
	uptr_t parse(std::string_view sv) {
		_buffer = sv;
		_original_size = sv.size();

		uptr_t rv = _parse_elem();

		sv = {};
		return rv;
	}

  private:
	/* ======= parser logic ======== */
	uptr_t _parse_elem() { return nullptr; }

	constexpr void skip_whitespaces() {
		while (WHITESPACES.contains(_peek()))
			_skip();
	}

	/* ======= basic operations ======== */
	constexpr char _peek() const { return _buffer[0]; }
	constexpr void _skip(int n = 1) { _buffer.remove_prefix(n); }
	constexpr void _require(char ch) {
		if (_peek() != ch)
			throw std::logic_error(
			    fmt::format("Expected '{}' at position {}, got '{}'", ch,
			                _original_size - _buffer.size(), _peek()));
		_skip();
	}
	constexpr char _consume() {
		char rv = _peek();
		_skip();
		return rv;
	}

	std::string_view _buffer;
	std::size_t _original_size;
	const static inline boost::container::flat_set<char> WHITESPACES{
	    ' ', '\n', '\t', '\r', '\f', '\b'};
};
} // namespace parser
