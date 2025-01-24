#pragma once

#include "symbols.hpp"
#include "variable.hpp"

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
                left_parenthesis,
                input,
                right_parenthesis
            );
        }
        else {
            return concat(
                output,
                fn_name,
                left_parenthesis,
                input,
                right_parenthesis,
                space,
                left_brace,
                new_line,
                body,
                right_brace,
                new_line 
            );
        }
    }

    template <typename T>
    concept is_static_string = std::same_as<T, static_string<T::size + 1>>;
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

    template <uniform map, auto expression>
    consteval auto sample() {
        constexpr auto expression_value = [&] {
            if constexpr (detail::is_static_string<decltype(expression)>)
                return expression;
            else 
                return expression.name;
        }();

        return builtin_fn<"texture2D", Param<map.name>, Param<expression_value>>().declaration;
    }
}

