#pragma once

#include "symbols.hpp"

namespace mfl::detail
{
    template <Keyword keyword>
    static consteval auto keyword_or_none() 
    {
        if constexpr (keyword == Keyword::none) {
            return static_string{ "" };
        }
        else if constexpr (keyword == Keyword::ret) {
            return static_string{ "return" } + space;
        }
        else {
            return to_static_string<keyword>() + space;
        }
    }

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
        static constexpr auto type{ detail::type_or_empty<var_type>() };
        static constexpr auto keyword{ detail::keyword_or_none<key>() };
        static constexpr auto declaration{ detail::init_value_or_empty<name, type, keyword, val>() };

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
    using uniform = variable_impl<type, name, Keyword::uniform>;

    template <Type type, static_string name>
    using in_var = variable_impl<type, name, Keyword::in>;

    template <Type type, static_string name, static_string value>
    using variable = variable_impl<type, name, Keyword::none, value>;

    template <Type type, static_string name>
    using struct_field = variable_impl<type, name, Keyword::none, "">;

    template <static_string value>
    using frag_color = variable_impl<Type::empty, "gl_FragColor", Keyword::none, value>;

    template <Keyword key, static_string value>
    using statement = variable_impl<Type::empty, "", key, value>;
}

