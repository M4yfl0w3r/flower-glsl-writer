#pragma once

#include "static_string.hpp"
#include "variable.hpp"
#include "symbols.hpp"
#include "utils.hpp"
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
#pragma once

    template <bool is_last, typename Param>
    static consteval auto format_param(Param param)
    {
        if constexpr (is_last) {
            return concat(Param::value);
        }
        else {
            return concat(Param::value, comma, space);
        }
    }

    template <typename... Params>
    static consteval auto format_array_members()
    {
        return []<auto... Indices>(std::index_sequence<Indices...>) {
            return concat(format_param<(Indices == sizeof...(Params) - 1), Params>(Params{})...);
        } (std::make_index_sequence<sizeof...(Params)>());
    }

    template <static_string type, static_string name>
    static consteval auto type_name_declaration() {
        return concat(type, space, name);
    }

    template <typename Fields>
    static consteval auto process_struct_members(const Fields& fields)
    {
        return std::apply([](auto... elems) consteval {
            return concat((elems.declaration)...);
        }, fields);
    }

    template <typename Fields>
    static consteval auto process_rvalue_array_members(const Fields& fields)
    {
        return std::apply([](auto... elems) consteval {
            return concat(
                format_array_members<decltype(elems)...>()
            );
        }, fields);
    }

    template <static_string value>
    static consteval auto make_struct_body() {
        return concat(space, enclose_in_braces<new_line, value>(), line_end);
    }

    template <static_string value>
    static consteval auto make_array_body() {
        return concat(enclose_in_parenthesis<value>(), line_end);
    }

    template <static_string type, static_string name, static_string size, typename... Args>
    static consteval auto make_storage_declaration(std::tuple<Args...> fields)
    {
        if constexpr (sizeof...(Args) == 0) {
            return concat(
                type_name_declaration<type, name>(),
                enclose_in_brackets<size>(),
                line_end
            );
        }
        else if constexpr (are_types_equal<type, Type::gl_struct>()) {
            return concat(
                type_name_declaration<type, name>(),
                make_struct_body<process_struct_members(fields)>()
            );
        }
        else if constexpr (is_glsl_type<decltype(enumify<type>())>) {
            return concat(
                type_name_declaration<type, name>(),
                enclose_in_brackets<size>(),
                equal,
                type,
                empty_brackets(),
                make_array_body<process_rvalue_array_members(fields)>()
            );
        }
    }

    template <static_string type, auto... fields>
    static consteval auto make_fields()
    {
        constexpr auto is_custom{ type == "Light" }; // TODO: tmp

        if constexpr (sizeof...(fields) == 0) {
            return std::tuple{};
        }
        else if constexpr ((is_supported_base_type<decltype(fields)> && ...)) {
            return std::tuple{ field<enumify<type>(), "", convert_to_string<fields>()>()... };
        }
        else if constexpr (are_types_equal<type, Type::gl_struct>() || is_custom) {
            return std::tuple{ fields... };
        }
        else if constexpr ((is_static_string<decltype(fields)> && ...)) {
            return std::tuple{ field<enumify<type>(), "", fields>()... };
        }
    }
}

namespace mfl
{
    template <auto t_type, static_string t_name, auto t_size, auto... t_fields>
    struct [[nodiscard]] storage
    {
        static constexpr auto name{ t_name };
        static constexpr auto type{ detail::make_type_expression<t_type>() };
        static constexpr auto size{ detail::expression_value<t_size>() };
        static constexpr auto fields{ detail::make_fields<type, t_fields...>() };
        static constexpr auto declaration{ detail::make_storage_declaration<type, t_name, size>(fields) };

        template <static_string field_name, std::size_t index = 0>
        static consteval auto get()
        {
            static_assert(index < sizeof...(t_fields), "Field not found with the given name.");

            constexpr auto& field{ std::get<index>(fields) };

            if constexpr (field.name.size == field_name.size) {
                if constexpr (field.name == field_name) {
                    return field;
                }
                else {
                    return get<field_name, index + 1>();
                }
            }
            else {
                return get<field_name, index + 1>();
            }
        }

        template <std::size_t index>
        static consteval auto at()
        {
            static_assert(index < sizeof...(t_fields), "Index out of range.");
            return std::get<index>(fields).value;
        }

        template <auto access_expression>
        static consteval auto at_expression()
        {
            return concat(
                name,
                detail::enclose_in_brackets<
                    detail::expression_value<access_expression>()
                >()
            );
        }

        template <static_string field_name, static_string value>
        static consteval auto assign() {
            return concat(name, sym_dot, get<field_name>().template assign<value>());
        }

        template <static_string field_name, auto value, auto access_expression>
        static consteval auto assign_at()
        {
            return concat(
                name,
                detail::enclose_in_brackets<
                    detail::expression_value<access_expression>()>(),
                sym_dot,
                get<field_name>().template assign<detail::expression_value<value>()>()
            );
        }

        template <auto value, auto access_expression>
        static constexpr auto assign_at()
        {
            return concat(
                name,
                detail::enclose_in_brackets<
                    detail::expression_value<access_expression>()>(),
                equal,
                detail::expression_value<value>(),
                line_end
            );
        }

        template <static_string field_name, auto access_expression>
        static consteval auto member_access_at()
        {
            return concat(
                name,
                detail::enclose_in_brackets<
                    detail::expression_value<access_expression>()>(),
                sym_dot,
                get<field_name>().name
            );
        }
    };

    template <static_string name, auto... fields>
    using structure = storage<Type::gl_struct, name, value(0), fields...>;

    template <auto type, auto size, static_string name, auto... fields>
    using array = storage<type, name, detail::expression_value<size>(), fields...>;

    template <structure type, auto size, static_string name>
    consteval auto make_array_of_structs() {
        return std::apply([](auto... elems) consteval {
            return array<type.name, size, name, elems...>{};
        }, type.fields);
    }

    static constexpr auto light_source{
        structure<
            "gl_LightSource",
            field<Type::gl_vec4, "position">{},
            field<Type::gl_vec4, "ambient">{},
            field<Type::gl_vec4, "diffuse">{},
            field<Type::gl_vec3, "spotDirection">{},
            field<Type::gl_float, "spotCutoff">{},
            field<Type::gl_float, "spotExponent">{},
            field<Type::gl_float, "constantAttenuation">{},
            field<Type::gl_float, "linearAttenuation">{},
            field<Type::gl_float, "quadraticAttenuation">{}
        >()
    };

    static constexpr auto gl_tex_coord{
        structure<"gl_TexCoord", field<Type::empty, "st">{}>{}
    };
}
