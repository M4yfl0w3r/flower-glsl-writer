#pragma once

#include "symbols.hpp"

namespace mfl 
{
    template <static_string name, Type type, Keyword keyword>
    struct [[nodiscard]] variable 
    {
        static constexpr auto declaration{ 
            concat(
                to_static_string<keyword>(), 
                space, 
                to_static_string<type>(), 
                space, 
                name, 
                line_end
            ) 
        };
    };

    template <static_string n1, static_string n2, Type t, Keyword key>
    consteval auto add(const variable<n1, t, key>& st, const variable<n2, t, key>& nd) {
        return concat(st.name, space, plus, space, nd.name, line_end);
    }

    template <static_string n1, static_string n2, Type t, Keyword key>
    consteval auto assign(const variable<n1, t, key>& dest, const variable<n2, t, key>& src) {
        return concat(dest.name, space, equal, space, src.name, line_end);
    }

    template <static_string n1, static_string N, Type t, Keyword key>
    consteval auto assign(const variable<n1, t, key>& dest, const static_string<N>& src) {
        return concat(dest.name, space, equal, space, src);
    }

    template <static_string name, Type t>
    using uniform = variable<name, t, Keyword::uniform>;

    template <static_string name, Type t>
    using in_var = variable<name, t, Keyword::in>;
}

