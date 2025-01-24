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

    template <static_string a, static_string b, static_string c>
    consteval auto vec3() {
        return builtin_fn<"vec3", Param<a>, Param<b>, Param<c>>();
    }
    
    template <static_string a>
    consteval auto vec3() {
        return builtin_fn<"vec3", Param<a>, Param<a>, Param<a>>();
    }

    template <uniform map, auto expression>
    consteval auto sample() {
        constexpr auto expression_value = [&] {
            if constexpr (is_static_string<decltype(expression)>)
                return expression;
            else 
                return expression.name;
        }();

        return builtin_fn<"texture2D", Param<map.name>, Param<expression_value>>().declaration;
    }
}

