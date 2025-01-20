#include <print>
#include <ranges>
#include <type_traits>

#include "static_string.hpp"

using namespace mfl;

enum class Uniform_t
{
    vec3
};

enum class Keyword 
{
    Uniform
};

constexpr auto space{ static_string{ " " } };
constexpr auto equal{ static_string{ "=" } };
constexpr auto plus{ static_string{ "+" } };
constexpr auto line_end{ static_string{ ";" } };

auto print(const auto& thing) {
    for (auto e : thing) {
        std::print("{}", e);
    }
    std::println("");
}

template <auto static_name, Uniform_t t>
struct uniform 
{
    static constexpr auto name{ static_name };
    static constexpr auto type{ t };
    static constexpr auto key{ Keyword::Uniform };

    static constexpr auto init_expression{ 
        concat(
            to_static_string<key>(), 
            space, 
            to_static_string<type>(), 
            space, 
            name, 
            line_end
        ) 
    };
};

template <auto n1, auto n2, Uniform_t t>
consteval auto add(const uniform<n1, t>& st, const uniform<n2, t>& nd) {
    return concat(st.name, space, plus, space, nd.name, line_end);
}

template <auto n1, auto n2, Uniform_t t>
consteval auto assign(const uniform<n1, t>& dest, const uniform<n2, t>& src) {
    return concat(dest.name, space, equal, space, src.name, line_end);
}

template <auto n1, auto N, Uniform_t t>
consteval auto assign(const uniform<n1, t>& dest, const static_string<N>& src) {
    return concat(dest.name, space, equal, space, src);
}

auto main() -> int 
{
    constexpr auto ambient_name{ static_string{ "ambient" } };
    constexpr auto ambient{ uniform<ambient_name, Uniform_t::vec3>() };

    constexpr auto diffuse_name{ static_string{ "diffuse" } };
    constexpr auto diffuse{ uniform<diffuse_name, Uniform_t::vec3>() };

    constexpr auto color_name{ static_string{ "color" } };
    constexpr auto color{ uniform<color_name, Uniform_t::vec3>() };

    constexpr auto add_res{ add(ambient, diffuse) };
    constexpr auto ass_res{ assign(color, diffuse) };

    constexpr auto test{ assign(color, add_res) };

    print(add_res);
    print(ass_res);
    print(test);
}
