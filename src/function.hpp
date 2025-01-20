#pragma once

#include "variable.hpp"
#include "static_string.hpp"

namespace mfl
{
    static constexpr auto left_brace{ static_string{ "{" } };
    static constexpr auto right_brace{ static_string{ "}" } };
    static constexpr auto left_parenthesis{ static_string{ "(" } };
    static constexpr auto right_parenthesis{ static_string{ ")" } };

    template <static_string fn_name, static_string param_name, Type input_type, Type output_type>
    struct [[nodiscard]] function 
    {
        static constexpr auto declaration{ 
            concat(
                to_static_string<output_type>(), 
                space, 
                fn_name,
                left_parenthesis,
                to_static_string<Keyword::in>(),
                space,
                to_static_string<input_type>(),
                space,
                param_name, 
                right_parenthesis,
                space,
                left_brace,
                right_brace,
                line_end
            )
        };
    };
}
