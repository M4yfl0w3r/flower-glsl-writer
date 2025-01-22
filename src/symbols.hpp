#pragma once 

#include "static_string.hpp"

namespace mfl
{
    enum class Type 
    {
        int_t,
        float_t,
        void_t,
        vec2,
        vec3,
        vec4,
        sampler2D,
        empty
    };

    enum class Keyword 
    {
        uniform,
        in,
        ret,
        none
    };

    inline constexpr auto space{ static_string{ " " } };
    inline constexpr auto equal{ static_string{ "=" } };
    inline constexpr auto plus{ static_string{ "+" } };
    inline constexpr auto line_end{ static_string{ ";\n" } };
    inline constexpr auto new_line{ static_string{ "\n" } };
    inline constexpr auto comma{ static_string{ "," } };
    inline constexpr auto left_brace{ static_string{ "{" } };
    inline constexpr auto right_brace{ static_string{ "}" } };
    inline constexpr auto left_parenthesis{ static_string{ "(" } };
    inline constexpr auto right_parenthesis{ static_string{ ")" } };
}

