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

    template <Type type, static_string name, typename... fields>
    static consteval auto make_storage_declaration() {
        if constexpr (type == Type::gl_struct) {
            return concat(
                stringify<type>(),
                space,
                name,
                make_struct_body<process_members<fields...>()>()
            );
        }
        else {
            return concat(static_string{""});
        }
    } 
}

/*
*   For array 
*   -> declaration -> if not fields -> Type name[ size ] 
*                  -> if fields -> TODO
*
*   For struct 
*   -> declaration -> struct name { fields... };
*/

namespace mfl
{
    template <Type t_type, static_string t_name, typename... t_fields>
    struct [[nodiscard]] storage
    {
        static constexpr auto name{ t_name };
        static constexpr auto type{ t_type };
        static constexpr auto fields{ std::tuple<t_fields...>() };
        static constexpr auto declaration{ detail::make_storage_declaration<t_type, t_name, t_fields...>() };

        template <static_string field_name, std::size_t index = 0>
        static consteval auto get()
        {
            if constexpr (index < sizeof...(t_fields)) {
                constexpr auto& field{ std::get<index>(fields) };

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
                static_assert(index < sizeof...(t_fields), "Field not found with the given name.");
            }
        }

        template <static_string field_name, static_string value>
        static consteval auto assign() {
            return concat(name, sym_dot, get<field_name>().template assign<value>());
        }

        template <static_string field_name>
        static consteval auto member_access() {
            return concat(name, sym_dot, get<field_name>().name);
        }
    };

    template <static_string name, typename... Fields>
    static consteval auto make_structure(Fields... fields) {
        return storage<Type::gl_struct, name, Fields...>{};
    }

    template <static_string name, static_string i, typename... Fields>
    static consteval auto make_array_of_structures(Fields... fields) {
        return storage<Type::empty, concat(name, left_bracket, i, right_bracket), Fields...>{};
    }

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

