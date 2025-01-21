#pragma once

#include "symbols.hpp"

namespace mfl::detail 
{
    template <typename... Params>
    static constexpr auto make_input() {
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
        static constexpr auto input{ detail::make_input<Params...>() };
        
        static constexpr auto declaration{ 
            concat(
                to_static_string<output_type>(),
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
