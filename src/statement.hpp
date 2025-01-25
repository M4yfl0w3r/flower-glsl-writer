#pragma once

#include "static_string.hpp"
#include "variable.hpp"
#include "symbols.hpp"

namespace mfl::detail
{
    template <Keyword key, bool assign, static_string value, static_string body>
    consteval auto make_declaration()
    {
        if constexpr (key == Keyword::gl_if) {
            return concat(stringify<key>(), space, enclose_in_parenthesis<value>(), create_body<body>());
        }
        else if constexpr (assign == true) { // gl_FragColor = value;
           return concat(stringify<key>(), equal, value, line_end);
        }
        else if constexpr (assign == false && value != "") {
            return concat(stringify<key>(), space, value, line_end);
        }
        else if constexpr (assign == false && value == "") { // return; continue;
            return concat(stringify<key>(), line_end);
        }
        else { // if (value) {}
            return concat(stringify<key>(), 
                          space,
                          enclose_in_parenthesis<value>(),
                          space, 
                          left_brace, 
                          right_brace, 
                          new_line);
        }
    }
}

namespace mfl
{
    template <Keyword key, bool assign, static_string value = "", static_string body = "">
    struct [[nodiscard]] statement
    {
        static constexpr auto declaration{ detail::make_declaration<key, assign, value, body>() };
    };

    template <auto condition, static_string body>
    using if_statement = statement<Keyword::gl_if, false, detail::expression_value<condition>(), body>;

    using return_statement = statement<Keyword::gl_return, false>;
    using continue_statement = statement<Keyword::gl_continue, false>;
    using break_statement = statement<Keyword::gl_break, false>;
    using discard_statement = statement<Keyword::gl_discard, false>;

    template <static_string value>
    using return_value_statement = statement<Keyword::gl_return, false, value>;

    template <variable_impl var>
    using frag_color = statement<Keyword::gl_frag_color, true, var.name>;
}

// if (colorWithAlpha.w < 0.1f) {
//     gl_FragColor = colorWithAlpha;
//     return;
// }
