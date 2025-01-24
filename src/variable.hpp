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

    template <Type type, static_string st_name, Keyword st_key, static_string st_val,
                         static_string nd_name, Keyword nd_key, static_string nd_val>
    consteval auto operator+(const variable_impl<type, st_name, st_key, st_val>&,
                             const variable_impl<type, nd_name, nd_key, nd_val>&)
    {
        return concat(st_name, plus, nd_name);
    }

    template <Type st_type, static_string st_name, Keyword st_key, static_string st_val,
              Type nd_type, static_string nd_name, Keyword nd_key, static_string nd_val>
    consteval auto operator*(const variable_impl<st_type, st_name, st_key, st_val>&,
                             const variable_impl<nd_type, nd_name, nd_key, nd_val>&)
    {
        return concat(st_name, times, nd_name);
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

