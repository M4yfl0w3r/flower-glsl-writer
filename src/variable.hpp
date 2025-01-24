#pragma once

#include "symbols.hpp"

namespace mfl::detail
{
    template <static_string name, static_string type, static_string keyword, static_string value>
    static consteval auto init_value_or_empty()
    {
        // TODO: space bug
        if constexpr (keyword == "return ") {
            return concat(
                keyword,
                value,
                line_end
            );
        }
        else if constexpr (value == "") {
            return concat(
                keyword,
                type, 
                name,
                line_end
            );
        }
        else {
            return concat(
                keyword,
                type,
                name,
                space,
                equal,
                space,
                value,
                line_end
            );
        }
    }
}

namespace mfl 
{
    template <Type var_type, static_string var_name, Keyword key, static_string val = "">
    struct [[nodiscard]] variable_impl
    {
        static constexpr auto name{ var_name };
        static constexpr auto type{ var_type };
        static constexpr auto str_type{ detail::stringify<var_type>() };
        static constexpr auto keyword{ detail::stringify<key>() };
        static constexpr auto declaration{ detail::init_value_or_empty<name, str_type, keyword, val>() };

        static consteval auto r() requires detail::is_vec<var_type> {
            return concat(name, dot, static_string{ "r" } );
        }
    };

    template <variable_impl st, variable_impl nd>
    consteval auto add() {
        return concat(st.name, plus, nd.name);
    }

    template <variable_impl st, variable_impl nd>
    consteval auto multiply() {
        return concat(st.name, times, nd.name);
    }

    template <Type type, static_string name>
    using uniform = variable_impl<type, name, Keyword::gl_uniform>;

    template <Type type, static_string name>
    using in_var = variable_impl<type, name, Keyword::gl_in>;

    template <Type type, static_string name, static_string value>
    using variable = variable_impl<type, name, Keyword::none, value>;

    template <Type type, static_string name>
    using field = variable_impl<type, name, Keyword::none, "">;

    template <static_string value>
    using frag_color = variable_impl<Type::empty, "gl_FragColor", Keyword::none, value>;
}

