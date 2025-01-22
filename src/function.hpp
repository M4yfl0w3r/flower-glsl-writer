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
                type_or_empty<Param::type>(),
                Param::name
            );
        } else {
            return concat(
                type_or_empty<Param::type>(),
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
        static constexpr auto output{ detail::type_or_empty<output_type>() };
        static constexpr auto declaration{ detail::user_defined_or_builtin<fn_name, output_type, output, body, input>() };
    };

    template <static_string body>
    using main_fn = function<"main", Type::void_t, body>;

    template <static_string fn_name, typename... Params>
    using builtin_fn = function<fn_name, Type::empty, "", Params...>;

    template <uniform map, static_string expression>
    consteval auto sample() {
        return builtin_fn<"texture2D", Param<map.name>, Param<expression>>().declaration;
    }

    template <uniform map, in_var var>
    consteval auto sample() {
        return builtin_fn<"texture2D", Param<map.name>, Param<var.name>>().declaration;
    }
}

