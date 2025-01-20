#include <print>

#include "static_string.hpp"

using namespace mfl;

enum class Type 
{
    vec2,
    vec3,
    vec4,
    sampler2D
};

enum class Keyword 
{
    uniform,
    in
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

template <auto static_name, Type t, Keyword key>
struct variable 
{
    static constexpr auto name{ static_name };
    static constexpr auto type{ t };
    static constexpr auto keyword{ key };

    static constexpr auto declaration{ 
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

template <auto n1, auto n2, Type t, Keyword key>
consteval auto add(const variable<n1, t, key>& st, const variable<n2, t, key>& nd) {
    return concat(st.name, space, plus, space, nd.name, line_end);
}

template <auto n1, auto n2, Type t, Keyword key>
consteval auto assign(const variable<n1, t, key>& dest, const variable<n2, t, key>& src) {
    return concat(dest.name, space, equal, space, src.name, line_end);
}

template <auto n1, auto N, Type t, Keyword key>
consteval auto assign(const variable<n1, t, key>& dest, const static_string<N>& src) {
    return concat(dest.name, space, equal, space, src);
}

auto main() -> int 
{
    constexpr auto color_map{ 
        variable<
            static_string{ "color_map" }, 
            Type::sampler2D, 
            Keyword::uniform
        >() 
    };

    constexpr auto in_tex_coord{ 
        variable<
            static_string{ "uv_tex_coord" }, 
            Type::vec2, 
            Keyword::in
        >() 
    };

    // constexpr auto ambient{ uniform<static_string{ "ambient" }, Uniform_t::vec3>() };
    // constexpr auto diffuse{ uniform<static_string{ "diffuse" }, Uniform_t::vec3>() };
    // constexpr auto color{ uniform<static_string{ "color" }, Uniform_t::vec3>() };

    // constexpr auto add_res{ add(ambient, diffuse) };
    // constexpr auto ass_res{ assign(color, diffuse) };
    // constexpr auto test{ assign(color, add_res) };

    print(color_map.declaration);
    print(in_tex_coord.declaration);
    // print(add_res);
    // print(ass_res);
    // print(test);
}
