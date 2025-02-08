#pragma once

#include "static_string.hpp"
#include "value.hpp"

namespace mfl::detail
{
    template <auto expression>
    consteval auto expression_value() 
    {
        return [&] { 
            if constexpr (is_static_string<decltype(expression)>)
                return expression;
            else if constexpr (requires { expression.name; }) {
                return expression.name;
            }
            else if constexpr (std::is_integral_v<decltype(expression)>) {
                return convert_to_string<expression>();
            }
            else if constexpr (std::is_floating_point_v<decltype(expression)>) {
                return convert_to_string<expression>();
            }
            else if constexpr (requires { expression.declaration; }) {
                return expression.declaration;
            }
            else {
                static_assert(false, "Unsupported expression type in expression_value");
            }
        }();
    }
}

namespace mfl 
{
    template <auto st_expression, auto nd_expression>
    consteval auto less_than()
    {
        constexpr auto st{ detail::expression_value<st_expression>() };
        constexpr auto nd{ detail::expression_value<nd_expression>() };
        return concat(st, static_string{ " < " }, nd);
    }

    template <auto st_expression, auto nd_expression>
    consteval auto less_or_equal_to()
    {
        constexpr auto st{ detail::expression_value<st_expression>() };
        constexpr auto nd{ detail::expression_value<nd_expression>() };
        return concat(st, static_string{ " <= " }, nd);
    }

    template <auto st_expression, auto nd_expression>
    consteval auto greater_than()
    {
        constexpr auto st{ detail::expression_value<st_expression>() };
        constexpr auto nd{ detail::expression_value<nd_expression>() };
        return concat(st, static_string{ " > " }, nd);
    }

    template <auto st_expression, auto nd_expression>
    consteval auto greater_or_equal_to()
    {
        constexpr auto st{ detail::expression_value<st_expression>() };
        constexpr auto nd{ detail::expression_value<nd_expression>() };
        return concat(st, static_string{ " >= " }, nd);
    }
}
