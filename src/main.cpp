#include <print>

#include "variable.hpp"
#include "function.hpp"

using namespace mfl;

#include <print>
auto print(const auto& thing) {
    for (auto e : thing) {
        std::print("{}", e);
    }
    std::println("");
}

auto main() -> int 
{
    constexpr auto colormap{ uniform<"colorMap", Type::sampler2D>() };
    constexpr auto normalmap{ uniform<"normalMap", Type::sampler2D>() };
    constexpr auto fog_color{ uniform<"fogColor", Type::vec3>() };
    constexpr auto fog_density{ uniform<"fogDensity", Type::f32>() };

    constexpr auto tex_coord{ in_var<"uvTexCoord", Type::vec2>() };
    constexpr auto position{ in_var<"position", Type::vec2>() };

    constexpr auto result{ 
        concat(
            colormap.declaration, 
            normalmap.declaration, 
            fog_color.declaration, 
            fog_density.declaration,
            tex_coord.declaration,
            position.declaration
        ) 
    };

    print(result);
}
