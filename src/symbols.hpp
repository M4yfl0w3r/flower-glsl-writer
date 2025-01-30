#pragma once 

#include "static_string.hpp"

#if __cplusplus >= 202302L
#include <print>
#endif

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
        gl_ivec2,
        gl_struct,
        gl_bool,
        empty 
    };

    enum class Keyword 
    {
        gl_uniform,
        gl_define,
        gl_in,
        gl_frag_color,
        gl_if,
        gl_for,
        gl_return,
        gl_continue,
        gl_break,
        gl_array,
        gl_discard,
        gl_position,
        none
    };

#if __cplusplus >= 202302L
    auto print(const auto& thing) 
    {
        for (auto e : thing) {
            std::print("{}", e);
        }
    }
#endif

    inline constexpr auto plus{ static_string{ " + " } };
    inline constexpr auto minus{ static_string{ " - " } };
    inline constexpr auto times{ static_string{ " * " } };
    inline constexpr auto divide{ static_string{ " / " } };
    inline constexpr auto space{ static_string{ " " } };
    inline constexpr auto equal{ static_string{ " = " } };
    inline constexpr auto less{ static_string{ " < " } };
    inline constexpr auto less_or_equal{ static_string{ " <= " } };
    inline constexpr auto greater{ static_string{ " > " } };
    inline constexpr auto greater_or_equal{ static_string{ " >= " } };
    inline constexpr auto semicolon{ static_string{ ";" } };
    inline constexpr auto plusplus{ static_string{ "++" } };
    inline constexpr auto plusequal{ static_string{ " += " } };
    inline constexpr auto timesequal{ static_string{ " *= " } };
    inline constexpr auto minusminus{ static_string{ "--" } };
    inline constexpr auto line_end{ static_string{ ";\n" } };
    inline constexpr auto new_line{ static_string{ "\n" } };
    inline constexpr auto comma{ static_string{ "," } };
    inline constexpr auto sym_dot{ static_string{ "." } };
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
    
    template <Type type>
    concept at_least_vec3 = (type == Type::gl_vec3 || type == Type::gl_vec4);

    template <Keyword key>
    concept is_array = (key == Keyword::gl_array);

    template <typename T>
    concept is_glsl_type = std::is_enum_v<T> && std::is_same_v<std::decay_t<T>, Type>;

    template <Type output_type>
    consteval auto stringify()
    {
        if constexpr (output_type == Type::gl_int)
            return static_string{ "int" };
        else if constexpr (output_type == Type::gl_float)
            return static_string{ "float" };
        else if constexpr (output_type == Type::gl_void)
            return static_string{ "void" };
        else if constexpr (output_type == Type::gl_vec2)
            return static_string{ "vec2" };
        else if constexpr (output_type == Type::gl_vec3)
            return static_string{ "vec3" };
        else if constexpr (output_type == Type::gl_vec4)
            return static_string{ "vec4" };
        else if constexpr (output_type == Type::gl_ivec2)
            return static_string{ "ivec2" };
        else if constexpr (output_type == Type::gl_sampler2D)
            return static_string{ "sampler2D" };
        else if constexpr (output_type == Type::gl_bool)
            return static_string{ "bool" };
        else if constexpr (output_type == Type::gl_struct)
            return static_string{ "struct" };
        else 
            return static_string{ "" };
    }

    template <static_string str>
    consteval auto enumify()
    {
        if constexpr (str == "int")
            return Type::gl_int;
        else if constexpr (str == "float")
            return Type::gl_float;
        else if constexpr (str == "void")
            return Type::gl_void;
        else if constexpr (str == "vec2")
            return Type::gl_vec2;
        else if constexpr (str == "vec3")
            return Type::gl_vec3;
        else if constexpr (str == "vec4")
            return Type::gl_vec4;
        else if constexpr (str == "ivec2")
            return Type::gl_ivec2;
        else if constexpr (str == "sampler2D")
            return Type::gl_sampler2D;
        else if constexpr (str == "bool")
            return Type::gl_bool;
        else if constexpr (str == "struct")
            return Type::gl_struct;
        else
            return Type::empty;
    }

    template <Keyword keyword>
    consteval auto stringify() 
    {
        if constexpr (keyword == Keyword::gl_uniform)
            return static_string{ "uniform" };
        else if constexpr (keyword == Keyword::gl_define)
            return static_string{ "#define" };
        else if constexpr (keyword == Keyword::gl_in)
            return static_string{ "in" };
        else if constexpr (keyword == Keyword::gl_frag_color)
            return static_string{ "gl_FragColor" };
        else if constexpr (keyword == Keyword::gl_position)
            return static_string{ "gl_Position" };
        else if constexpr (keyword == Keyword::gl_if)
            return static_string{ "if" };
        else if constexpr (keyword == Keyword::gl_for)
            return static_string{ "for" };
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

    template <auto expression>
    consteval auto expression_value() 
    {
        return [&] { 
            if constexpr (is_static_string<decltype(expression)>)
                return expression;
            else if constexpr (requires { expression.name; } ) {
                return expression.name;
            }
            else {
                static_assert(false, "Unsupported expression type in expression_value");
            }
        }();
    }

    template <static_string... value>
    consteval auto enclose_in_parenthesis() {
        return concat(left_parenthesis, value..., right_parenthesis);
    }

    template <static_string... value>
    consteval auto enclose_in_brackets() {
        return concat(left_bracket, value..., right_bracket);
    }

    template <static_string... value>
    consteval auto enclose_in_braces() {
        return concat(left_brace, value..., right_brace);
    }

    consteval auto empty_brackets() {
        return enclose_in_brackets();
    }

    template <static_string value>
    consteval auto create_body() {
        return concat(space, left_brace, new_line, value, right_brace, new_line);
    }

    template <static_string str_type, Type enum_type>
    static consteval auto are_types_equal() {
        return enumify<str_type>() == enum_type;
    }
}

// TODO: Create an utils namespace?
namespace mfl 
{
#define DEFINE_COMPARE_TEMPLATE(func_name, op)                            \
    template <auto st_expr, auto nd_expr>                                 \
    consteval auto func_name() {                                          \
        constexpr auto st{ detail::expression_value<st_expr>() };  \
        constexpr auto nd{ detail::expression_value<nd_expr>() };  \
        return concat(st, op, nd);                                        \
    }

    DEFINE_COMPARE_TEMPLATE(less_than, less)
    DEFINE_COMPARE_TEMPLATE(less_or_equal_to, less_or_equal)
    DEFINE_COMPARE_TEMPLATE(greater_than, greater)
    DEFINE_COMPARE_TEMPLATE(greater_or_equal_to, greater_or_equal)
}
