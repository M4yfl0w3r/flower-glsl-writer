#pragma once

#include "static_string.hpp"

namespace mfl::detail
{
    static consteval auto is_digit(char c) {
        return c >= '0' && c <= '9';
    }

    static consteval auto pow(int number, unsigned n) -> int
    {
        return n == 0
               ? 1
               : number * pow(number, n - 1);
    }

    static consteval auto num_digits(int number, int count = 0) -> unsigned
    {
        return number == 0
               ? (count == 0 ? 1 : count)
               : num_digits(number / 10, count + 1);
    }

    static consteval auto convert_string_to_int_impl(const char* string, int value = 0) -> int
    {
        return is_digit(*string)
               ? convert_string_to_int_impl(string + 1, (*string - '0') + value * 10)
               : value;
    }

    static consteval auto extract_frac(const char* string) -> int
    {
        const std::string_view number{ string };
        const auto dot_it_pos{ std::ranges::find(number, '.') };
        const auto dot_pos{ std::distance(std::begin(number), dot_it_pos) };
        const auto frac_str{ number.substr(dot_pos + 1u) };
        return convert_string_to_int_impl(frac_str.data());
    }

    static consteval auto convert_string_to_float_impl(const char* string) -> float
    {
        /* TODO: Handle the *.01 case */
        const auto whole{ convert_string_to_int_impl(string) };
        const auto frac{ extract_frac(string) };
        const auto digits{ num_digits(frac) };
        return whole + (static_cast<float>(frac) / pow(10, digits));
    }

    template <int number, int... digits>
    struct to_digit_seq : to_digit_seq<number / 10, number % 10, digits...> {};

    template <int... digits>
    struct to_digit_seq<0, digits...> {
        using type = std::integer_sequence<int, digits...>;
    };

    template <>
    struct to_digit_seq<0> {
        using type = std::integer_sequence<int, 0>;
    };

    template <int number>
    using make_digit_seq = typename to_digit_seq<(number < 0 ? -number : number)>::type;

    template <int... digits>
    static consteval auto make_digit_array(std::integer_sequence<int, digits...>) {
        return std::array{ digits... };
    }

    template <float number, int precision>
    static consteval auto extract_frac() -> int
    {
        constexpr auto whole{ static_cast<int>(number) };
        auto val{ number };
        val -= whole;
        val *= pow(10, precision);
        return static_cast<int>(val);
    }

    template <int number>
    static consteval auto convert_int_to_string_impl()
    {
        /* TODO: Handle negative values */

        char buffer[num_digits(number) + 1] {};
        buffer[num_digits(number)] = '\0';

        constexpr auto chars{ std::array{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' } };
        constexpr auto digits{ make_digit_array(make_digit_seq<number>{}) };

        [&]<auto... indicies>(std::index_sequence<indicies...>) consteval {
            ((buffer[indicies] = chars.at(digits.at(indicies))), ...);
        } (std::make_index_sequence<num_digits(number)>());

        return static_string{ buffer };
    }

    template <float number, int precision = 1>
    static consteval auto convert_float_to_string_impl()
    {
        constexpr auto whole{ static_cast<int>(number) };
        constexpr auto frac{ extract_frac<number, precision>() };

        return concat(
            convert_int_to_string_impl<whole>(),
            static_string{ "." },
            convert_int_to_string_impl<frac>(),
            static_string{ "f" }
        );
    }
}

namespace mfl
{
    template <static_string string>
    consteval auto convert_to_int() {
        return detail::convert_string_to_int_impl(string.value);
    }

    template <static_string string>
    consteval auto convert_to_float() {
        return detail::convert_string_to_float_impl(string.value);
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

    template <std::size_t N>
    consteval auto operator+(auto value, const static_string<N>& string) {
        return value + detail::convert_string_to_float_impl(string.value);
    }

    template <std::size_t N>
    consteval auto operator+(const static_string<N>& string, auto value) {
        return detail::convert_string_to_float_impl(string.value) + value;
    }

    template <std::size_t N>
    consteval auto operator-(auto value, const static_string<N>& string) {
        return value - detail::convert_string_to_float_impl(string.value);
    }

    template <std::size_t N>
    consteval auto operator-(const static_string<N>& string, auto value) {
        return detail::convert_string_to_float_impl(string.value) - value;
    }

    template <std::size_t N>
    consteval auto operator*(auto value, const static_string<N>& string) {
        return value * detail::convert_string_to_float_impl(string.value);
    }

    template <std::size_t N>
    consteval auto operator*(const static_string<N>& string, auto value) {
        return detail::convert_string_to_float_impl(string.value) * value;
    }

    template <std::size_t N>
    consteval auto operator/(auto value, const static_string<N>& string) {
        return value / detail::convert_string_to_float_impl(string.value);
    }

    template <std::size_t N>
    consteval auto operator/(const static_string<N>& string, auto value) {
        return detail::convert_string_to_float_impl(string.value) / value;
    }
}
