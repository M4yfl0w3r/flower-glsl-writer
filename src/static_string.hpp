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

        consteval auto at(std::size_t i) const {
            return value[i];
        }

        constexpr bool operator==(const static_string& other) const {
            return std::equal(std::begin(value), std::end(value), std::begin(other.value));
        }

        constexpr auto begin() const { return value; }
        constexpr auto end() const { return value + size; }

        char value[N] {};
    };

    template <std::size_t N>
    static_string(const char (&str)[N]) -> static_string<N>;

    template <std::size_t... len>
    consteval auto concat(const static_string<len>&... strings)
    {
        constexpr std::size_t N{ (... + len) - sizeof...(len) };
        std::array<char, N+1> result{};
        std::size_t index{ 0 };
        ([&] {
            const auto sv{ strings.data() };
            std::copy(std::begin(sv), std::end(sv), std::begin(result) + index);
            index += std::size(sv);
        }(), ...);

        result[N] = '\0';
        return static_string{ result };
    }

    template <auto value>
    consteval auto to_static_string() {
        constexpr auto enum_val_str{ magic_enum::enum_name(value) };

        constexpr auto buffer{ 
            detail::fill_buffer<char>(
                enum_val_str, 
                std::make_index_sequence<std::size(enum_val_str)>()
            )
        };

        return static_string(buffer);
    }
}

