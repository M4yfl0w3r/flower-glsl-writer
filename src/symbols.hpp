#pragma once 

#include "static_string.hpp"

#include <print>

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

    auto print(const auto& thing) {
        for (auto e : thing) {
            std::print("{}", e);
        }
        std::println("");
    }

    inline constexpr auto plus{ static_string{ " + " } };
    inline constexpr auto times{ static_string{ " * " } };
    inline constexpr auto space{ static_string{ " " } };
    inline constexpr auto equal{ static_string{ "=" } };
    inline constexpr auto line_end{ static_string{ ";\n" } };
    inline constexpr auto new_line{ static_string{ "\n" } };
    inline constexpr auto comma{ static_string{ "," } };
    inline constexpr auto left_brace{ static_string{ "{" } };
    inline constexpr auto right_brace{ static_string{ "}" } };
    inline constexpr auto left_parenthesis{ static_string{ "(" } };
    inline constexpr auto right_parenthesis{ static_string{ ")" } };

}

namespace mfl::detail
{
    template <Type output_type>
    static consteval auto type_or_empty()
    {
        if constexpr (output_type == Type::empty) {
            return static_string{ "" };
        }
        else if constexpr (output_type == Type::int_t) {
            return static_string{ "int" } + space;
        }
        else if constexpr (output_type == Type::void_t) {
            return static_string{ "void" } + space;
        }
        else if constexpr (output_type == Type::float_t) {
            return static_string{ "float" } + space;
        }
        else {
            return to_static_string<output_type>() + space;
        }
    }
}

