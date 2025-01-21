#include <print>

#include "variable.hpp"

using namespace mfl;

auto print(const auto& thing) {
    for (auto e : thing) {
        std::print("{}", e);
    }
    std::println("");
}

auto main() -> int 
{
    constexpr auto color_map{ 
        variable<
            { "color_map" }, 
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
