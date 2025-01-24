#pragma once

#include "symbols.hpp"
#include "variable.hpp"
#include "value.hpp"

namespace mfl::detail 
{
    template <bool is_last, typename Param>
    static consteval auto format_param(Param param) 
    {
        if constexpr (is_last) {
            return concat(
                stringify<Param::type>(),
                Param::name
            );
        } else {
            return concat(
                stringify<Param::type>(),
                Param::name,
                comma,
                space
            );
        }
    }

    template <typename... Params>
    static consteval auto make_input() 
    {
        if constexpr (sizeof...(Params) == 0) {
            return static_string{ "" };
        }
        else {
            return []<auto... Indices>(std::index_sequence<Indices...>) {
                return concat(
                    format_param<(Indices == sizeof...(Params) - 1), Params>(Params{})...
                );
            } (std::make_index_sequence<sizeof...(Params)>());
        }
    }

    template <static_string fn_name, Type output_type, static_string output, static_string body, static_string input>
    static consteval auto user_defined_or_builtin() 
    {
        if constexpr (output_type == Type::empty) {
            return concat( 
                fn_name,
                enclose_in_parenthesis<input>()
            );
        }
        else {
            return concat(
                output,
                fn_name,
                enclose_in_parenthesis<input>(),
                space,
                left_brace,
                new_line,
                body,
                right_brace,
                new_line 
            );
        }
    }

    template <auto expression>
    static consteval auto expression_value() {
        return [&] { 
            if constexpr (is_static_string<decltype(expression)>)
                return expression;
            else 
                return expression.name;
        }();
    }
}

namespace mfl
{
    template <static_string n, Type t = Type::empty>
    struct Param {
        static constexpr auto type{ t };
        static constexpr auto name{ n };
    };

    template <static_string fn_name, Type output_type, static_string body, typename... Params>
    struct [[nodiscard]] function 
    {
        static constexpr auto name{ fn_name };
        static constexpr auto input{ detail::make_input<Params...>() };
        static constexpr auto output{ detail::stringify<output_type>() };
        static constexpr auto declaration{ detail::user_defined_or_builtin<fn_name, output_type, output, body, input>() };
    };

    template <static_string body>
    using main_fn = function<"main", Type::gl_void, body>;

    template <static_string fn_name, typename... Params>
    using builtin_fn = function<fn_name, Type::empty, "", Params...>;

    // TODO: should take expression to handle variables
    template <static_string... expressions>
    consteval auto vec2() {
        static_assert(sizeof...(expressions) > 0 && sizeof...(expressions) <= 2);
        return builtin_fn<"vec2", Param<expressions>...>().declaration;
    }

    template <static_string... expressions>
    consteval auto vec3() {
        static_assert(sizeof...(expressions) > 0 && sizeof...(expressions) <= 3);
        return builtin_fn<"vec3", Param<expressions>...>().declaration;
    }

    template <static_string... expressions>
    consteval auto vec4() {
        static_assert(sizeof...(expressions) > 0 && sizeof...(expressions) <= 4);
        return builtin_fn<"vec4", Param<expressions>...>().declaration;
    }

    template <auto expression>
    consteval auto length() {
        constexpr auto expr{ detail::expression_value<expression>() };
        return builtin_fn<"length", Param<expr>>().declaration;
    }

    template <auto expression>
    consteval auto radians() {
        constexpr auto expr{ detail::expression_value<expression>() };
        return builtin_fn<"radians", Param<expr>>().declaration;
    }

    template <auto st_expression, auto nd_expression>
    consteval auto pow()
    {
        constexpr auto st_expr{ detail::expression_value<st_expression>() };
        constexpr auto nd_expr{ detail::expression_value<nd_expression>() };
        return builtin_fn<"pow", Param<st_expr>, Param<nd_expr>>().declaration;
    }

    template <uniform map, auto expression>
    consteval auto sample() 
    {
        constexpr auto expr{ detail::expression_value<expression>() };
        return builtin_fn<"texture2D", Param<map.name>, Param<expr>>().declaration;
    }

    template <uniform texture, auto expression>
    consteval auto texture_size() 
    {
        constexpr auto expr{ detail::expression_value<expression>() };
        return builtin_fn<"textureSize", Param<texture.name>, Param<expr>>().declaration;
    }
}

