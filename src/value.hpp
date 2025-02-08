#pragma once

#include "static_string.hpp"

namespace mfl::detail
{
    static consteval auto is_digit(char c) {
        return c >= '0' && c <= '9';
    }

    static consteval auto convert_string_to_int_impl(const char* str, int value = 0) -> int
    {
        return is_digit(*str) 
               ? convert_string_to_int_impl(str + 1, (*str - '0') + value * 10)
               : value;
    }

    static consteval auto num_digits(int num, int count = 0) -> unsigned {
        return num == 0 
               ? (count == 0 ? 1 : count) 
               : num_digits(num / 10, count + 1);
    }

    template <int num, int... digits>
    struct to_digit_seq : to_digit_seq<num / 10, num % 10, digits...> {};

    template <int... digits>
    struct to_digit_seq<0, digits...> {
        using type = std::integer_sequence<int, digits...>;
    };
    
    template <>
    struct to_digit_seq<0> {
        using type = std::integer_sequence<int, 0>;
    };

    template <int num>
    using make_digit_seq = typename to_digit_seq<(num < 0 ? -num : num)>::type;

    template <int... digits>
    static consteval auto make_digit_array(std::integer_sequence<int, digits...>) {
        return std::array{ digits... };
    }

    static consteval auto pow(int num, unsigned n) -> int {
        return n == 0
               ? 1 
               : num * pow(num, n - 1);
    }

    template <int num>
    static consteval auto convert_int_to_string_impl()
    {
        // TODO: Handle negative values 
        char buffer[num_digits(num) + 1] {};
        buffer[num_digits(num)] = '\0';

        constexpr auto chars{ std::array{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' } };
        constexpr auto digits{ make_digit_array(make_digit_seq<num>{}) };

        [&]<auto... indicies>(std::index_sequence<indicies...>) consteval {
            ((buffer[indicies] = chars.at(digits.at(indicies))), ...);
        } (std::make_index_sequence<num_digits(num)>());
        
        return static_string{ buffer };
    }

    template <float num, int precision = 1>
    static consteval auto convert_float_to_string_impl()
    {
        constexpr auto whole{ num_digits(static_cast<int>(num)) };
        constexpr auto multiplied{ num * pow(10, precision)};
        constexpr auto full_number{ convert_int_to_string_impl<static_cast<int>(multiplied)>() };
        constexpr auto result{ insert_delimiter_at<full_number, whole, '.'>() };
        return result;
    }
}

namespace mfl
{
// TODO: An ugly macro for now, will change it later to compile-time convertion
#define value(x) static_string(#x)

    template <static_string str>
    consteval auto convert_to_int() {
        return detail::convert_string_to_int_impl(str.value);
    }

    template <auto number>
    consteval auto convert_to_string() 
    {
        if constexpr (std::is_integral_v<decltype(number)>)
            return detail::convert_int_to_string_impl<number>();
        else if constexpr (std::is_floating_point_v<decltype(number)>)
            return detail::convert_float_to_string_impl<number>();
    }

    template <static_string glsl_version>
    consteval auto set_version() {
        return concat(static_string{ "#version " }, glsl_version, static_string{ "\n" });
    }

    template <std::size_t N1, std::size_t N2>
    consteval auto operator+(const static_string<N1>& a, const static_string<N2>& b) {
        return concat(a, static_string{ "+" }, b);
    }

    template <std::size_t N1, std::size_t N2>
    consteval auto operator-(const static_string<N1>& a, const static_string<N2>& b) {
        return concat(a, static_string{ "-" }, b);
    }

    template <std::size_t N1, std::size_t N2>
    consteval auto operator/(const static_string<N1>& a, const static_string<N2>& b) {
        return concat(a, static_string{ "/" }, b);
    }

    template <std::size_t N1, std::size_t N2>
    consteval auto operator*(const static_string<N1>& a, const static_string<N2>& b) {
        return concat(a, static_string{ "*" }, b);
    }
}

