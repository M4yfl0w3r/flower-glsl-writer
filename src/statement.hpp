#pragma once

#include "static_string.hpp"
#include "variable.hpp"
#include "symbols.hpp"

namespace mfl::detail
{
    template <Keyword key, bool assign, static_string value, static_string body>
    static consteval auto make_declaration()
    {
        if constexpr (key == Keyword::gl_if || key == Keyword::gl_for) {
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

    template <auto init_var, static_string condition, static_string increment>
    static consteval auto make_for_loop() {
        return concat(
            stringify<init_var.type>(), 
            space, 
            init_var.name, equal, init_var.value, semicolon, 
            space, 
            condition, semicolon,
            space, 
            increment);
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

    template <auto init_var, static_string condition, static_string increment, static_string body>
    using for_loop = statement<Keyword::gl_for, false, detail::make_for_loop<init_var, condition, increment>(), body>;

    using return_statement = statement<Keyword::gl_return, false>;
    using continue_statement = statement<Keyword::gl_continue, false>;
    using break_statement = statement<Keyword::gl_break, false>;
    using discard_statement = statement<Keyword::gl_discard, false>;

    template <static_string value>
    using return_value_statement = statement<Keyword::gl_return, false, value>;

    template <variable_impl var>
    using frag_color = statement<Keyword::gl_frag_color, true, var.name>;
}

