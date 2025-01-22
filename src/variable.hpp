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

    template <Type type>
    static consteval auto type_or_none()
    {
        if constexpr (type == Type::empty) {
            return static_string{ "" };
        }
        else {
            return to_static_string<type>() + space;
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
    template <static_string var_name, Type var_type, Keyword key, static_string val = "">
    struct [[nodiscard]] variable_impl
    {
        static constexpr auto name{ var_name };
        static constexpr auto type{ detail::type_or_none<var_type>() };
        static constexpr auto keyword{ detail::keyword_or_none<key>() };
        static constexpr auto declaration{ detail::init_value_or_empty<name, type, keyword, val>() };
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

    template <static_string name, Type type>
    using uniform = variable_impl<name, type, Keyword::uniform>;

    template <static_string name, Type type>
    using in_var = variable_impl<name, type, Keyword::in>;

    template <static_string name, Type type, static_string value>
    using variable = variable_impl<name, type, Keyword::none, value>;

    template <static_string value>
    using gl_FragColor = variable_impl<"gl_FragColor", Type::empty, Keyword::none, value>;

    template <Keyword key, static_string value>
    using statement = variable_impl<"", Type::empty, key, value>;
}

