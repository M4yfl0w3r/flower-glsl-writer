#pragma once

#include "static_string.hpp"
#include "symbols.hpp"

#include <tuple>

namespace mfl::detail
{
    template <typename... fields>
    constexpr auto process_members() {
        return concat((fields::declaration)...);
    }
}

namespace mfl
{
    template <static_string struct_name, typename... fields>
    struct [[nodiscard]] structure
    {
        static constexpr auto name{ struct_name };
        static constexpr auto members{ std::tuple<fields...>() };

        static constexpr auto declaration{ 
            concat(
                static_string{ "struct" }, 
                space, 
                name, 
                space,
                left_brace, 
                new_line,
                detail::process_members<fields...>(),
                right_brace, 
                line_end
            ) 
        };
    };

    template <static_string struct_name, typename... Fields>
    static consteval auto make_structure(Fields... fields) {
        return structure<struct_name, Fields...>{};
    }
}

