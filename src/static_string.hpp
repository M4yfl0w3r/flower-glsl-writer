#pragma once 

#include <array>
#include <algorithm>
#include <string_view>
#include <magic_enum/magic_enum.hpp>

namespace mfl::detail 
{
    template <auto N, class Fn>
    constexpr auto for_each(Fn&& fn) 
    {
        [&]<auto... indicies>(std::index_sequence<indicies...>) constexpr {
            (fn.template operator()<indicies>(), ...);
        } (std::make_index_sequence<N>());
    }

    template <typename T, std::size_t... Indices>
    constexpr auto fill_buffer(std::string_view str, std::index_sequence<Indices...>) {
        return std::array{ str.at(Indices)..., '\0' };
    }
}

namespace mfl
{
    template <std::size_t N>
    struct static_string 
    {
        static constexpr int size = N - 1;

        consteval static_string(const char (&str)[N])  {
            std::copy_n(str, N, value);
        }

        consteval static_string(const std::array<char, N>& str) {
            std::copy_n(std::begin(str), N, value);
        }

        consteval auto data() const {
            return std::string_view{ value };
        }

        constexpr auto begin() const { return value; }
        constexpr auto end() const { return value + size; }

        char value[N] {};
    };

    template <auto value>
    consteval auto to_static_string() {
        constexpr auto enum_val_str{ magic_enum::enum_name(value) };
        constexpr auto length{ std::size(enum_val_str) + 1 }; // \0

        constexpr auto buffer{ 
            detail::fill_buffer<char>(
                enum_val_str, 
                std::make_index_sequence<std::size(enum_val_str)>()
            )
        };

        return static_string<length>(buffer);
    }
}

