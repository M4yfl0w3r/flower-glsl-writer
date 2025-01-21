#pragma once

#include "variable.hpp"
#include "static_string.hpp"

namespace mfl
{
    static constexpr auto comma{ static_string{ "," } };
    static constexpr auto left_brace{ static_string{ "{" } };
    static constexpr auto right_brace{ static_string{ "}" } };
    static constexpr auto left_parenthesis{ static_string{ "(" } };
    static constexpr auto right_parenthesis{ static_string{ ")" } };

    template <Type t, static_string n>
    struct Param {
        static constexpr auto type{ t };
        static constexpr auto name{ n };
    };

    template <static_string fn_name, Type output_type, typename... Params>
    struct [[nodiscard]] function 
    {
        static constexpr auto input{
            concat(
                to_static_string<Keyword::in>(),
                space, 
                to_static_string<Params::type>(),
                space,
                Params::name
            )... 
        };
        
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
