#pragma once

#include "static_string.hpp"
#include "symbols.hpp"

namespace mfl::detail
{
    static consteval auto is_digit(char c) {
        return c >= '0' && c <= '9';
    }

    static consteval int convert_to_int_impl(const char* str, int value = 0) 
    {
        return is_digit(*str) 
               ? convert_to_int_impl(str + 1, (*str - '0') + value * 10)
               : value;
    }
}

namespace mfl
{
// TODO: An ugly macro for now, will change it later to compile-time convertion
#define value(x) static_string(#x)

    template <static_string str>
    consteval auto convert_to_int() {
        return detail::convert_to_int_impl(str.value);
    }

    template <static_string glsl_version>
    static consteval auto set_version() {
        return concat(static_string{ "#version " }, glsl_version);
    }

    template <std::size_t N1, std::size_t N2>
    consteval auto operator+(const static_string<N1>& a, const static_string<N2>& b) {
        return concat(a, plus, b);
    }

    template <std::size_t N1, std::size_t N2>
    consteval auto operator-(const static_string<N1>& a, const static_string<N2>& b) {
        return concat(a, minus, b);
    }

    template <std::size_t N1, std::size_t N2>
    consteval auto operator/(const static_string<N1>& a, const static_string<N2>& b) {
        return concat(a, divide, b);
    }

    template <std::size_t N1, std::size_t N2>
    consteval auto operator*(const static_string<N1>& a, const static_string<N2>& b) {
        return concat(a, times, b);
    }
}

