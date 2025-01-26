#pragma once

#include "static_string.hpp"
#include "variable.hpp"
#include "symbols.hpp"

#include <tuple>

namespace mfl::detail
{
    template <typename... fields>
    static constexpr auto process_members() {
        return concat((fields::declaration)...);
    }

    template <static_string value>
    static consteval auto make_struct_body() {
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
                detail::make_struct_body<detail::process_members<fields...>()>()
            ) 
        };

        template <static_string field_name, std::size_t index = 0>
        static consteval auto get()
        {
            if constexpr (index < sizeof...(fields)) {
                constexpr auto& field{ std::get<index>(members) };

                if constexpr (field.name.size == field_name.size) {
                    if constexpr (field.name == field_name)
                        return field;
                    else
                        return get<field_name, index+1>();
                }
                else {
                    return get<field_name, index+1>();
                }
            }
            else {
                static_assert(index < sizeof...(fields), "Field not found with the given name.");
            }
        }

        template <static_string field_name, static_string value>
        static consteval auto assign() {
            return concat(struct_name, dot, get<field_name>().template assign<value>());
        }

        template <static_string field_name>
        static consteval auto member_access() {
            return concat(struct_name, dot, get<field_name>().name);
        }
    };

    template <static_string struct_name, typename... Fields>
    static consteval auto make_structure(Fields... fields) {
        return structure<struct_name, Fields...>{};
    }

    template <static_string struct_name, static_string i, typename... Fields>
    static consteval auto make_array_of_structures(Fields... fields) {
        // TODO: super hacky way to access elements - must be changed later
        return structure<concat(struct_name, left_bracket, i, right_bracket), Fields...>{};
    }

    // TODO: an array of structs should be a variable and return an lvalue reference
    static constexpr auto light_source{ 
        make_array_of_structures<"gl_LightSource", "i">(
            field<Type::gl_vec4, "position">(),
            field<Type::gl_vec4, "ambient">(),
            field<Type::gl_vec4, "diffuse">(),
            field<Type::gl_vec3, "spotDirection">(),
            field<Type::gl_float, "spotCutoff">(),
            field<Type::gl_float, "spotExponent">(),
            field<Type::gl_float, "constantAttenuation">(),
            field<Type::gl_float, "linearAttenuation">(),
            field<Type::gl_float, "quadraticAttenuation">()
        )
    };
}

