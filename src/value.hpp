#pragma once

#include "static_string.hpp"
#include "symbols.hpp"

namespace mfl
{
// TODO: An ugly macro for now, will change it later to compile-time convertion
#define value(x) static_string(#x)

    template <std::size_t st_len, std::size_t nd_len>
    consteval auto operator-(const static_string<st_len>& a, const static_string<nd_len>& b) {
        return concat(a, minus, b);
    }
}
