#pragma once

#include "static_string.hpp"
#include "variable.hpp"
#include "symbols.hpp"

namespace mfl::detail
{
    template <Keyword key, bool assign, static_string val>
    consteval auto make_declaration()
    {
        if constexpr (assign == true) { // gl_FragColor = value;
           return concat(stringify<key>(), equal, space, val, line_end);
        }
        else if constexpr (assign == false && val == "") { // return; continue;
            return concat(stringify<key>(), line_end);
        }
        else { // if (value) {}
            return concat(stringify<key>(), 
                          space,
                          enclose_in_parenthesis<val>(), 
                          space, 
                          left_brace, 
                          right_brace, 
                          new_line);
        }
    }
}

namespace mfl
{
    //
    //  The statement class represents actual statements like 
    //  return or continue, but also built-in variables for 
    //  which we do not define a type like gl_FragColor.
    //
    //  Args
    //      
    //    * key: return, if, for but also gl_FragColor
    //    * assign: is variable assignable? return is not, but gl_FragColor is
    //    * val: value to be assigned, should also be a variable, not only a static_string
    //
    template <Keyword key, bool assign, static_string val = "">
    struct [[nodiscard]] statement
    {
        static constexpr auto declaration{ detail::make_declaration<key, assign, val>() };
    };

    template <static_string val>
    using if_statement = statement<Keyword::gl_if, false, val>;

    using return_statement = statement<Keyword::gl_return, false, "">;
    using continue_statement = statement<Keyword::gl_continue, false, "">;
    using break_statement = statement<Keyword::gl_break, false, "">;
    using discard_statement = statement<Keyword::gl_discard, false, "">;

    template <variable_impl var>
    using frag_color = statement<Keyword::gl_frag_color, true, var.name>;
}

// if (colorWithAlpha.w < 0.1f) {
//     gl_FragColor = colorWithAlpha;
//     return;
// }
