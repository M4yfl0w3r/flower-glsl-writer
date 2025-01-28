#pragma once

#include "static_string.hpp"
#include "variable.hpp"
#include "symbols.hpp"
#include "value.hpp"

#include <tuple>

namespace mfl::detail
{
    template <auto expression>
    static consteval auto make_type_expression() 
    {
        return [&] { 
            if constexpr (is_static_string<decltype(expression)>)
                return expression;
            else if constexpr (is_glsl_type<decltype(expression)>) {
                return stringify<expression>();
            }
        }();
    }

    template <typename... Fields>
    static consteval auto process_members() {
        return concat((Fields::declaration)...);
    }

    template <static_string type, static_string name>
    static consteval auto type_name_declaration() {
        return concat(type, space, name);
    }

    template <static_string value>
    static consteval auto make_struct_body() {
        return concat(space, enclose_in_braces<new_line, value>(), line_end);
    }

    template <static_string type, static_string name, static_string size, typename... Fields>
    static consteval auto make_storage_declaration() 
    {
        if constexpr (enumify<type>() == Type::gl_struct) {
            return concat(
                type_name_declaration<type, name>(),
                make_struct_body<process_members<Fields...>()>()
            );
        }
        else {
            return concat(
                type_name_declaration<type, name>(),
                enclose_in_brackets<size>(),
                line_end
            );
        }
    } 
}

namespace mfl
{
    template <auto t_type, static_string t_name, auto t_size, typename... t_fields>
    struct [[nodiscard]] storage
    {
        static constexpr auto name{ t_name };
        static constexpr auto type{ detail::make_type_expression<t_type>() };
        static constexpr auto size{ detail::expression_value<t_size>() };
        static constexpr auto fields{ std::tuple<t_fields...>() };
        static constexpr auto declaration{ detail::make_storage_declaration<type, t_name, size, t_fields...>() };

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

        template <static_string field_name, auto access_expression>
        static consteval auto member_access_at() {
            return concat(
                name, 
                detail::enclose_in_brackets<
                    detail::expression_value<access_expression>()>(), 
                sym_dot, 
                get<field_name>().name
            );
        }
    };

    template <static_string name, typename... Fields>
    using structure = storage<Type::gl_struct, name, value(0), Fields...>;

    template <auto type, static_string name, auto size, typename... Fields>
    using array = storage<type, name, size, Fields...>;

    template <auto type, static_string name, auto size, typename source>
    static consteval auto make_array_with_fields() {
        return std::apply([](auto... fields) {
            return array<type, name, size, decltype(fields)...>{};
        }, source::fields);
    }

    static constexpr auto light_source{ 
        structure<
            "gl_LightSource",
            field<Type::gl_vec4, "position">,
            field<Type::gl_vec4, "ambient">,
            field<Type::gl_vec4, "diffuse">,
            field<Type::gl_vec3, "spotDirection">,
            field<Type::gl_float, "spotCutoff">,
            field<Type::gl_float, "spotExponent">,
            field<Type::gl_float, "constantAttenuation">,
            field<Type::gl_float, "linearAttenuation">,
            field<Type::gl_float, "quadraticAttenuation">
        >()
    };
}

