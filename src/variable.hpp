#pragma once

#include "symbols.hpp"

namespace mfl::detail
{
    template <static_string name, Type type, Keyword key>
    static consteval auto make_declaration()
    {
        static constexpr auto type_str{ stringify<type>() };
        static constexpr auto key_str{ stringify<key>() };

        if constexpr (key == Keyword::none) {
            return concat(type_str, space, name, line_end);
        }
        else {
            return concat(key_str, space, type_str, space, name, line_end);
        }
    }

    template <static_string name, Type type, Keyword key, static_string value>
    static consteval auto make_definition()
    {
        static constexpr auto type_str{ stringify<type>() };
        static constexpr auto key_str{ stringify<key>() };

        if constexpr (key == Keyword::none) {
            return concat(type_str, space, name, equal, value, line_end);
        }
        else if constexpr (key == Keyword::gl_define) {
            // #define name value\n
            return concat(key_str, space, name, space, value, new_line);
        }
        else if constexpr (key == Keyword::gl_uniform || key == Keyword::gl_in) {
            return concat(key_str, space, type_str, space, name, line_end);
        }
        else if constexpr (key == Keyword::gl_array) {
            // type name[value];\n
            return concat(type_str, space, name, enclose_in_brackets<value>(), line_end);
        }
    }
}

namespace mfl 
{
    template <Type var_type, static_string var_name, Keyword var_key, static_string var_value = "">
    struct [[nodiscard]] variable_impl
    {
        static constexpr auto name{ var_name };
        static constexpr auto type{ var_type };
        static constexpr auto value{ var_value };
        static constexpr auto key{ var_key };

        static constexpr auto declaration{ 
            [] {
                if constexpr (var_value == "") {
                    return detail::make_declaration<name, type, key>();
                }
                else {
                    return detail::make_definition<name, type, key, value>();
                }
            }()
        };

        static consteval auto r() { return component<"r">(); }
        static consteval auto g() { return component<"g">(); }
        static consteval auto b() { return component<"b">(); }
        static consteval auto a() { return component<"a">(); }
        static consteval auto x() { return component<"x">(); }
         
        static consteval auto rgb() requires detail::at_least_vec3<var_type> {
            return concat(name, dot, static_string{ "rgb" } );
        }

        template <auto expression>
        static consteval auto assign() 
        {
            static constexpr auto expr{ detail::expression_value<expression>() };
            return concat(name, equal, expr, line_end);
        }

        template <auto expression>
        static consteval auto multiply()
        {
            static constexpr auto expr{ detail::expression_value<expression>() };
            return concat(name, times, expr);
        }

    private:    
        template <static_string access_name>
        static consteval auto component() {
            return concat(var_name, dot, access_name);
        }
    };

    template <Type type, static_string st_name, Keyword st_key, static_string st_val,
                         static_string nd_name, Keyword nd_key, static_string nd_val>
    consteval auto operator+(const variable_impl<type, st_name, st_key, st_val>&,
                             const variable_impl<type, nd_name, nd_key, nd_val>&) 
    {
        return concat(st_name, plus, nd_name);
    }

    template <Type st_type, static_string st_name, Keyword st_key, static_string st_val,
              Type nd_type, static_string nd_name, Keyword nd_key, static_string nd_val>
    consteval auto operator*(const variable_impl<st_type, st_name, st_key, st_val>&,
                             const variable_impl<nd_type, nd_name, nd_key, nd_val>&)
    {
        return concat(st_name, times, nd_name);
    }

    template <Type type, static_string name>
    using uniform = variable_impl<type, name, Keyword::gl_uniform>;

    template <Type type, static_string name>
    using in_var = variable_impl<type, name, Keyword::gl_in>;

    template <Type type, static_string name, static_string value = "">
    using variable = variable_impl<type, name, Keyword::none, value>;

    template <Type type, static_string name>
    using field = variable_impl<type, name, Keyword::none, "">;

    template <static_string name, static_string value>
    using define_statement = variable_impl<Type::empty, name, Keyword::gl_define, value>;

    template <Type type, static_string name, static_string size>
    using array = variable_impl<Type::gl_light, name, Keyword::gl_array, size>;
}

