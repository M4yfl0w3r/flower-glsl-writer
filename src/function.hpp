#pragma once

#include "symbols.hpp"

namespace mfl::detail 
{
    template <Type output_type>
    static consteval auto type_or_empty()
    {
        // TODO: handle it globally - f32 = float
        if constexpr (output_type == Type::empty) {
            return static_string{ "" };
        }
        else if constexpr (output_type == Type::f32) {
            return static_string{ "float" };
        }
        else {
            return to_static_string<output_type>();
        }
    }

    template <typename... Params>
    static consteval auto make_input() 
    {
        if constexpr ((... && (Params::type == Type::empty))) {
            return concat(concat(Params::name)...);
        }
        else {
            return concat(
                concat(
                    to_static_string<Params::type>(), 
                    space, 
                    Params::name
                )...
            );
        }
    }

    template <static_string fn_name, Type output_type, static_string output, static_string input>
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
                space,
                fn_name,
                left_parenthesis,
                input,
                right_parenthesis,
                space,
                left_brace,
                right_brace,
                line_end
            );
        }
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
        static constexpr auto input{ detail::make_input<Params...>() };
        static constexpr auto output{ detail::type_or_empty<output_type>() };
        static constexpr auto declaration{ detail::user_defined_or_builtin<fn_name, output_type, output, input>() };
    };
}
