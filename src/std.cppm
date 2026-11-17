module;

#include <array>
#include <charconv>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <expected>
#include <format>
#include <functional>
#include <optional>
#include <print>
#include <string>
#include <utility>

export module std;

export namespace std {
using std::array;
using std::to_chars;
using std::to_chars_result;
using std::size_t;
using std::ptrdiff_t;
using std::byte;
using std::int8_t;
using std::int16_t;
using std::int32_t;
using std::int64_t;
using std::uint8_t;
using std::uint16_t;
using std::uint32_t;
using std::uint64_t;
using std::intptr_t;
using std::uintptr_t;
using std::deque;
using std::expected;
using std::unexpected;
using std::function;
using std::optional;
using std::nullopt;
using std::nullopt_t;
using std::string;
using std::string_view;
using std::wstring;
using std::to_wstring;
using std::format;
using std::print;
using std::println;
using std::move;
using std::forward;
using std::operator+;
using std::operator-;
using std::operator==;
using std::operator!=;
using std::operator<;
using std::operator<=;
using std::operator>;
using std::operator>=;
}
