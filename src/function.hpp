#pragma once

#include "symbols.hpp"

namespace mfl::detail 
{
    template <auto value>
    static constexpr auto get_type_or_empty() {
        if constexpr (value == Type::empty) {
            return static_string("");
        }
        return to_static_string<value>();
    }

    template <typename... Params>
    static constexpr auto make_lvalue_input() {
        return concat(
            concat(
                to_static_string<Keyword::in>(), 
                space, 
                to_static_string<Params::type>(), 
                space, 
                Params::name
            )...
        );
    }
}

namespace mfl
{
    template <Type t, static_string n>
    struct Param {
        static constexpr auto type{ t };
        static constexpr auto name{ n };
    };

    template <static_string fn_name, Type output_type, typename... Params>
    struct [[nodiscard]] function 
    {
        static constexpr auto input{ detail::make_lvalue_input<Params...>() };
        static constexpr auto output{ detail::get_type_or_empty<output_type>() };
        
        static constexpr auto declaration{ 
            concat(
                output,
                space,
                fn_name,
                left_parenthesis,
                input,
                right_parenthesis,
                space,
                left_brace,
                right_brace,
                line_end
            )
        };
    };
}
