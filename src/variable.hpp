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
    template <Type t_type, static_string t_name, Keyword t_key, static_string t_value = "">
    struct [[nodiscard]] variable_impl
    {
        static constexpr auto name{ t_name };
        static constexpr auto type{ t_type };
        static constexpr auto value{ t_value };
        static constexpr auto key{ t_key };

        static constexpr auto declaration{ 
            [] {
                if constexpr (value == "") {
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
        static consteval auto y() { return component<"y">(); }
        static consteval auto z() { return component<"z">(); }
         
        static consteval auto rgb() requires detail::at_least_vec3<type> {
            return concat(name, sym_dot, static_string{ "rgb" } );
        }
        
        static consteval auto xyz() requires detail::at_least_vec3<type> {
            return concat(name, sym_dot, static_string{ "xyz" } );
        }

        template <auto expression>
        static consteval auto assign() 
        {
            static constexpr auto expr{ detail::expression_value<expression>() };
            return concat(name, equal, expr, line_end);
        }

        template <auto expression>
        static consteval auto add_assign() 
        {
            static constexpr auto expr{ detail::expression_value<expression>() };
            return concat(name, plusequal, expr, line_end);
        }

        template <auto expression>
        static consteval auto multiply_assign() 
        {
            static constexpr auto expr{ detail::expression_value<expression>() };
            return concat(name, timesequal, expr, line_end);
        }

        template <auto expression>
        static consteval auto multiply()
        {
            static constexpr auto expr{ detail::expression_value<expression>() };
            return concat(name, times, expr);
        }

        static consteval auto negative() {
            return concat(minus, name);
        }

        static consteval auto increment() {
            return concat(plusplus, name);
        }
        
        static consteval auto decrement() {
            return concat(minusminus, name);
        }

    private:    
        template <static_string access_name>
        static consteval auto component() {
            return concat(name, sym_dot, access_name);
        }
    };

    // TODO: a better structure for operators
    template <Type T, static_string N1, Keyword K1, static_string V1,
                      static_string N2, Keyword K2, static_string V2>
    consteval auto operator+(const variable_impl<T, N1, K1, V1>&, const variable_impl<T, N2, K2, V2>&)  {
        return concat(left_parenthesis, N1, plus, N2, right_parenthesis);
    }

    template <Type T1, static_string N1, Keyword K1, static_string V1,
              Type T2, static_string N2, Keyword K2, static_string V2>
    consteval auto operator*(const variable_impl<T1, N1, K1, V1>&, const variable_impl<T2, N2, K2, V2>&) {
        return concat(N1, times, N2);
    }

    template <Type T, static_string N, Keyword K, static_string V, std::size_t len>
    consteval auto operator*(const static_string<len>& str, const variable_impl<T, N, K, V>&) {
        return concat(left_parenthesis, str, times, N, right_parenthesis);
    }

    template <Type T, static_string N, Keyword K, static_string V, std::size_t len>
    consteval auto operator*(const variable_impl<T, N, K, V>&, const static_string<len>& str) {
        return concat(left_parenthesis, N, times, str, right_parenthesis);
    }

    template <Type t, static_string name, Keyword k, static_string val, std::size_t len>
    consteval auto operator-(const variable_impl<t, name, k, val>&, const static_string<len>& str) {
        return concat(left_parenthesis, name, minus, str, right_parenthesis);
    }

    template <Type t, static_string name, Keyword k, static_string val, std::size_t len>
    consteval auto operator-(const static_string<len>& str, const variable_impl<t, name, k, val>&) {
        return concat(left_parenthesis, str, minus, name, right_parenthesis);
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
}

