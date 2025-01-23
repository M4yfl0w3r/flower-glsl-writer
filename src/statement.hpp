#pragma once

#include "static_string.hpp"
#include "symbols.hpp"

namespace mfl::detail
{

}

namespace mfl
{
    template <Keyword key, static_string val = "">
    struct [[nodiscard]] statement
    {
        static constexpr auto declaration{ 
            concat(
                detail::keyword_or_none<key>(),
                space,
                left_parenthesis, 
                val, 
                right_parenthesis, 
                space,
                left_brace, 
                right_brace
            ) 
        };
    };

    template <static_string val>
    using if_statement = statement<Keyword::condition, val>;

    template <static_string val = "">
    using return_statement = statement<Keyword::ret, val>;
}

// if (colorWithAlpha.w < 0.1f) {
//     gl_FragColor = colorWithAlpha;
//     return;
// }
