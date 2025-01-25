#pragma once

#include "static_string.hpp"
#include "symbols.hpp"

namespace mfl
{
// TODO: An ugly macro for now, will change it later to compile-time convertion
#define value(x) static_string(#x)

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
