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
        gl_FragColor,
        condition,     // if
        ret,           // return
        none
    };

    auto print(const auto& thing) {
        for (auto e : thing) {
            std::print("{}", e);
        }
    }

    inline constexpr auto plus{ static_string{ " + " } };
    inline constexpr auto times{ static_string{ " * " } };
    inline constexpr auto space{ static_string{ " " } };
    inline constexpr auto equal{ static_string{ "=" } };
    inline constexpr auto line_end{ static_string{ ";\n" } };
    inline constexpr auto new_line{ static_string{ "\n" } };
    inline constexpr auto comma{ static_string{ "," } };
    inline constexpr auto dot{ static_string{ "." } };
    inline constexpr auto left_brace{ static_string{ "{" } };
    inline constexpr auto right_brace{ static_string{ "}" } };
    inline constexpr auto left_parenthesis{ static_string{ "(" } };
    inline constexpr auto right_parenthesis{ static_string{ ")" } };

}

namespace mfl::detail
{
    template <Type type>
    concept is_vec = (type == Type::vec2 || type == Type::vec3 || type == Type::vec4);

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

    template <Keyword keyword>
    static consteval auto keyword_or_none() 
    {
        if constexpr (keyword == Keyword::none) {
            return static_string{ "" };
        }
        else if constexpr (keyword == Keyword::ret) {
            return static_string{ "return" } + space;
        }
        else if constexpr (keyword == Keyword::condition) {
            return static_string{ "if" };
        }
        else {
            return to_static_string<keyword>() + space;
        }
    }

}

