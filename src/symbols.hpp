#pragma once 

#include "static_string.hpp"

#include <print>

namespace mfl
{
    enum class Type 
    {
        gl_int,
        gl_float,
        gl_void,
        gl_vec2,
        gl_vec3,
        gl_vec4,
        gl_sampler2D,
        gl_bool,
        gl_light, // TODO: ugly hack for now - will have to think how to register custom types (struct instead of enum?)
        empty
    };

    enum class Keyword 
    {
        gl_uniform,
        gl_define,
        gl_in,
        gl_frag_color,
        gl_if,
        gl_return,
        gl_continue,
        gl_break,
        gl_array,
        gl_discard,
        gl_position,
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
    inline constexpr auto left_bracket{ static_string{ "[" } };
    inline constexpr auto right_bracket{ static_string{ "]" } };
    inline constexpr auto left_parenthesis{ static_string{ "(" } };
    inline constexpr auto right_parenthesis{ static_string{ ")" } };
}

namespace mfl::detail
{
    template <Type type>
    concept is_vec = (type == Type::gl_vec2 || type == Type::gl_vec3 || type == Type::gl_vec4);

    template <Type output_type>
    static consteval auto stringify()
    {
        if constexpr (output_type == Type::gl_int)
            return static_string{ "int" } + space;
        else if constexpr (output_type == Type::gl_float)
            return static_string{ "float" } + space;
        else if constexpr (output_type == Type::gl_void)
            return static_string{ "void" } + space;
        else if constexpr (output_type == Type::gl_vec2)
            return static_string{ "vec2" } + space;
        else if constexpr (output_type == Type::gl_vec3)
            return static_string{ "vec3" } + space;
        else if constexpr (output_type == Type::gl_vec4)
            return static_string{ "vec4" } + space;
        else if constexpr (output_type == Type::gl_sampler2D)
            return static_string{ "sampler2D" } + space;
        else if constexpr (output_type == Type::gl_bool)
            return static_string{ "bool" } + space;
        else if constexpr (output_type == Type::gl_light)
            return static_string{ "Light" };
        else 
            return static_string{ "" };
    }

    template <Keyword keyword>
    static consteval auto stringify() 
    {
        if constexpr (keyword == Keyword::gl_uniform)
            return static_string{ "uniform" } + space;
        else if constexpr (keyword == Keyword::gl_define)
            return static_string{ "#define" };
        else if constexpr (keyword == Keyword::gl_in)
            return static_string{ "in" } + space;
        else if constexpr (keyword == Keyword::gl_frag_color)
            return static_string{ "gl_FragColor" } + space;
        else if constexpr (keyword == Keyword::gl_position)
            return static_string{ "gl_Position" };
        else if constexpr (keyword == Keyword::gl_if)
            return static_string{ "if" };
        else if constexpr (keyword == Keyword::gl_return)
            return static_string{ "return" };
        else if constexpr (keyword == Keyword::gl_continue)
            return static_string{ "continue" };
        else if constexpr (keyword == Keyword::gl_break)
            return static_string{ "break" };
        else if constexpr (keyword == Keyword::gl_array)
            return static_string{ "array" };
        else if constexpr (keyword == Keyword::gl_discard)
            return static_string{ "discard" };
        else
            return static_string{ "" };
    }

    template <static_string val>
    consteval auto enclose_in_parenthesis() {
        return concat(left_parenthesis, val, right_parenthesis);
    }

}

