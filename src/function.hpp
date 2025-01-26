#pragma once

#include "symbols.hpp"
#include "variable.hpp"

namespace mfl::detail 
{
    template <bool is_last, typename Param>
    static consteval auto format_no_type_param() 
    {
        if constexpr (is_last) {
            return concat(Param::name);
        }
        else {
            return concat(Param::name, comma, space);
        }
    }

    template <bool is_last, typename Param>
    static consteval auto format_type_param() 
    {
        static constexpr auto type_str{ stringify<Param::type>() };

        if constexpr (is_last) {
            return concat(type_str, space, Param::name);
        }
        else {
            return concat(type_str, space, Param::name, comma, space);
        }
    }

    template <bool is_last, bool is_builtin, typename Param>
    static consteval auto format_param(Param param) 
    {
        static constexpr auto is_param_without_type{ is_builtin || Param::type == Type::empty };

        if constexpr (is_param_without_type) {
            return format_no_type_param<is_last, Param>();
        }
        else {
            return format_type_param<is_last, Param>();
        }
    }

    template <bool is_builtin, typename... Params>
    static consteval auto make_input() 
    {
        if constexpr (sizeof...(Params) == 0) {
            return static_string{ "" };
        }
        else {
            return []<auto... Indices>(std::index_sequence<Indices...>) {
                return concat(format_param<(Indices == sizeof...(Params) - 1), is_builtin, Params>(Params{})...);
            } (std::make_index_sequence<sizeof...(Params)>());
        }
    }

    template <Type output_type, static_string fn_name, static_string body, static_string input>
    static consteval auto user_defined_or_builtin() 
    {
        static constexpr auto is_fn_builtin{ output_type == Type::empty };
        static constexpr auto type_str{ stringify<output_type>() };

        if constexpr (is_fn_builtin) {
            return concat(fn_name, enclose_in_parenthesis<input>());
        }
        else {
            return concat(type_str, space, fn_name, enclose_in_parenthesis<input>(), create_body<body>());
        }
    }
}

namespace mfl
{
    template <static_string n, Type t = Type::empty>
    struct Param 
    {
        static constexpr auto type{ t };
        static constexpr auto name{ n };
    };

    template <Type output_type, static_string fn_name, static_string body, typename... Params>
    struct [[nodiscard]] function 
    {
        static constexpr auto name{ fn_name };
        static constexpr auto is_builtin{ output_type == Type::empty };
        static constexpr auto input{ detail::make_input<is_builtin, Params...>() };
        static constexpr auto declaration{ detail::user_defined_or_builtin<output_type, fn_name, body, input>() };

        template <auto... expressions>
        consteval auto call() const {
            return concat(
                fn_name, 
                detail::enclose_in_parenthesis<
                    detail::make_input<
                        is_builtin, 
                        Param<detail::expression_value<expressions>()>... 
                    >()
                >()
            );
        }
    };

    template <static_string body>
    using main_fn = function<Type::gl_void, "main", body>;

    template <static_string fn_name, typename... Params>
    using builtin_fn = function<Type::empty, fn_name, "", Params...>;

    template <auto... expressions>
    consteval auto vec2() 
    {
        static_assert(sizeof...(expressions) > 0 && sizeof...(expressions) <= 2);
        return builtin_fn<"vec2", Param<detail::expression_value<expressions>()>...>().declaration;
    }

    template <auto... expressions>
    consteval auto vec3() 
    {
        static_assert(sizeof...(expressions) > 0 && sizeof...(expressions) <= 3);
        return builtin_fn<"vec3", Param<detail::expression_value<expressions>()>...>().declaration;
    }

    template <auto... expressions>
    consteval auto vec4() 
    {
        static_assert(sizeof...(expressions) > 0 && sizeof...(expressions) <= 4);
        return builtin_fn<"vec4", Param<detail::expression_value<expressions>()>...>().declaration;
    }

    template <auto... expressions>
    consteval auto mix() 
    {
        static_assert(sizeof...(expressions) > 0 && sizeof...(expressions) <= 3);
        return builtin_fn<"mix", Param<detail::expression_value<expressions>()>...>().declaration;
    }

    template <auto expression>
    consteval auto length() {
        return builtin_fn<"length", Param<detail::expression_value<expression>()>>().declaration;
    }

    template <auto expression>
    consteval auto radians() {
        return builtin_fn<"radians", Param<detail::expression_value<expression>()>>().declaration;
    }

    template <auto expression>
    consteval auto normalize() {
        return builtin_fn<"normalize", Param<detail::expression_value<expression>()>>().declaration;
    }

    template <auto expression>
    consteval auto exp() {
        return builtin_fn<"exp", Param<detail::expression_value<expression>()>>().declaration;
    }

    template <auto expression>
    consteval auto acos() {
        return builtin_fn<"acos", Param<detail::expression_value<expression>()>>().declaration;
    }
    
    template <auto st_expression, auto nd_expression, auto rd_expression>
    consteval auto clamp() 
    {
        return builtin_fn<
            "clamp", 
            Param<detail::expression_value<st_expression>()>,
            Param<detail::expression_value<nd_expression>()>,
            Param<detail::expression_value<rd_expression>()>
        >().declaration;
    }

    template <auto st_expression, auto nd_expression>
    consteval auto pow()
    {
        constexpr auto st_expr{ detail::expression_value<st_expression>() };
        constexpr auto nd_expr{ detail::expression_value<nd_expression>() };
        return builtin_fn<"pow", Param<st_expr>, Param<nd_expr>>().declaration;
    }

    template <auto st_expression, auto nd_expression>
    consteval auto dot()
    {
        constexpr auto st_expr{ detail::expression_value<st_expression>() };
        constexpr auto nd_expr{ detail::expression_value<nd_expression>() };
        return builtin_fn<"dot", Param<st_expr>, Param<nd_expr>>().declaration;
    }

    template <uniform map, auto expression>
    consteval auto sample() 
    {
        constexpr auto expr{ detail::expression_value<expression>() };
        return builtin_fn<"texture2D", Param<map.name>, Param<expr>>().declaration;
    }

    template <uniform texture, auto expression>
    consteval auto texture_size() 
    {
        constexpr auto expr{ detail::expression_value<expression>() };
        return builtin_fn<"textureSize", Param<texture.name>, Param<expr>>().declaration;
    }
}

