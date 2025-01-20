#pragma once

#include "static_string.hpp"

namespace mfl 
{
    // TODO: move it somewhere
    constexpr auto space{ static_string{ " " } };
    constexpr auto equal{ static_string{ "=" } };
    constexpr auto plus{ static_string{ "+" } };
    constexpr auto line_end{ static_string{ ";\n" } };

    enum class Type 
    {
        vec2,
        vec3,
        vec4,
        sampler2D
    };

    enum class Keyword 
    {
        uniform,
        in
    };

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

    template <auto n1, auto n2, Type t, Keyword key>
    consteval auto add(const variable<n1, t, key>& st, const variable<n2, t, key>& nd) {
        return concat(st.name, space, plus, space, nd.name, line_end);
    }

    template <auto n1, auto n2, Type t, Keyword key>
    consteval auto assign(const variable<n1, t, key>& dest, const variable<n2, t, key>& src) {
        return concat(dest.name, space, equal, space, src.name, line_end);
    }

    template <auto n1, auto N, Type t, Keyword key>
    consteval auto assign(const variable<n1, t, key>& dest, const static_string<N>& src) {
        return concat(dest.name, space, equal, space, src);
    }

    template <static_string name, Type t>
    using uniform = variable<name, t, Keyword::uniform>;
}

