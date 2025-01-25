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

    template <static_string value>
    consteval auto create_struct_body() {
        return concat(space, left_brace, new_line, value, right_brace, line_end);
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
                detail::create_struct_body<detail::process_members<fields...>()>()
            ) 
        };

        template <static_string name, std::size_t index = 0>
        consteval auto get() const 
        {
            if constexpr (index < sizeof...(fields)) {
                constexpr auto& field{ std::get<index>(members) };

                if constexpr (field.name.size == name.size) {
                    if constexpr (field.name == name)
                        return field;
                    else
                        return get<name, index+1>();
                }
                else {
                    return get<name, index+1>();
                }
            }
            else {
                static_assert(index < sizeof...(fields), "Field not found with the given name.");
            }
        }
    };

    template <static_string struct_name, typename... Fields>
    static consteval auto make_structure(Fields... fields) {
        return structure<struct_name, Fields...>{};
    }
}

