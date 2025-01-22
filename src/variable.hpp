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
        else {
            return to_static_string<keyword>() + space;
        }
    }

    template <static_string name, Type type, static_string keyword, static_string value>
    static consteval auto init_value_or_empty()
    {
        if constexpr (value == "") {
            return concat(
                keyword,
                to_static_string<type>(), 
                space, 
                name, 
                line_end
            );
        }
        else {
            return concat(
                keyword,
                to_static_string<type>(),
                space,
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
    template <static_string name, Type type, Keyword key, static_string value = static_string{ "" }>
    struct [[nodiscard]] variable_impl
    {
        static constexpr auto keyword{ detail::keyword_or_none<key>() };
        static constexpr auto declaration{ detail::init_value_or_empty<name, type, keyword, value>() };
    };

    template <static_string st_name, static_string nd_name, Type type, Keyword key>
    consteval auto add(const variable_impl<st_name, type, key>& st, const variable_impl<nd_name, type, key>& nd) {
        return concat(st.name, space, plus, space, nd.name, line_end);
    }

    template <static_string dest_name, static_string src_name, Type type, Keyword key>
    consteval auto assign(const variable_impl<dest_name, type, key>& dest, const variable_impl<src_name, type, key>& src) {
        return concat(dest.name, space, equal, space, src.name, line_end);
    }

    template <static_string name, std::size_t len, Type type, Keyword key>
    consteval auto assign(const variable_impl<name, type, key>& dest, const static_string<len>& src) {
        return concat(dest.name, space, equal, space, src);
    }

    template <static_string name, Type t>
    using uniform = variable_impl<name, t, Keyword::uniform>;

    template <static_string name, Type t>
    using in_var = variable_impl<name, t, Keyword::in>;

    template <static_string name, Type t, static_string value>
    using variable = variable_impl<name, t, Keyword::none, value>;
}

