#pragma once 

#include <array>
#include <algorithm>
#include <string_view>

namespace mfl::detail 
{
    template <typename T, std::size_t... Indices>
    static consteval auto fill_buffer(std::string_view str, std::index_sequence<Indices...>) {
        return std::array{ str.at(Indices)..., '\0' };
    }

    template <class Fn, std::size_t... Indices>
    static consteval auto unroll_impl(Fn&& fn, std::index_sequence<Indices...>) {
        ((fn(std::integral_constant<std::size_t, Indices>{})), ...);
    }

    template <std::size_t N, class Fn>
    static consteval auto unroll(Fn&& fn) {
       unroll_impl(std::forward<Fn>(fn), std::make_index_sequence<N>());
    }
}

namespace mfl
{
    template <std::size_t N>
    struct static_string 
    {
        static constexpr auto size{ N - 1 };

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

        consteval auto operator==(const static_string& other) const -> bool {
            return std::equal(std::begin(value), std::end(value), std::begin(other.value));
        }
        
        consteval auto operator==(const char* other) const -> bool {
            return std::string_view{ value } == other;
        }

        constexpr auto begin() const { 
            return value; 
        }

        constexpr auto end() const { 
            return value + size; 
        }

        char value[N] {};
    };

    template <std::size_t N>
    static_string(const char (&str)[N]) -> static_string<N>;

    template <typename T>
    concept is_static_string = std::same_as<T, static_string<T::size + 1>>;

    template <static_string string, std::size_t index, char delimiter>
    consteval auto insert_delimiter_at() 
    {
        constexpr auto N{ string.size + 1 };
        std::array<char, N + 1> result{};

        detail::unroll<index>([&](std::size_t i){ result.at(i) = string.at(i); });
        result.at(index) = delimiter;
        detail::unroll<N - index>([&](std::size_t i){ result.at(i + index + 1) = string.at(i + index); });

        result.at(N) = '\0';
        return static_string{ result };
    }

    template <static_string string, char character>
    consteval auto insert_at_back()
    {
        constexpr auto N{ string.size + 1 };
        std::array<char, N + 1> result{};
        detail::unroll<N - 1>([&](std::size_t i){ result.at(i) = string.at(i); });
        result.at(N - 1) = character;
        result.at(N) = '\0';
        return static_string{ result };
    }

    template <std::size_t... len>
    consteval auto concat(const static_string<len>&... strings)
    {
        constexpr auto N{ (... + len) - sizeof...(len) };
        std::array<char, N + 1> result{};
        std::size_t index{ 0u };
        ([&] {
            const auto& sv{ strings.data() };
            std::copy(std::begin(sv), std::end(sv), std::begin(result) + index);
            index += std::size(sv);
        }(), ...);

        result[N] = '\0';
        return static_string{ result };
    }

    template <typename... Vars>
    consteval auto concat_all(Vars... vars) {
        return concat(vars.declaration...);
    }
}

