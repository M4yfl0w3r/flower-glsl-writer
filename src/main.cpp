#include <print>

#include "variable.hpp"
#include "function.hpp"
#include "value.hpp"

using namespace mfl;

#include <print>
auto print(const auto& thing) {
    for (auto e : thing) {
        std::print("{}", e);
    }
    std::println("");
}

consteval auto to_gamma_body() 
{
    constexpr auto vec{ 
        builtin_function<"vec3", Param<Type::empty, value(1.0f / 2.2f)>>() 
    };
    
    constexpr auto pow{ 
        builtin_function<"pow", Param<Type::empty, value(1.0f / 2.2f)>, 
                                Param<Type::empty, vec.declaration>>() 
    };

    constexpr auto result{ variable<"result", Type::vec3, pow.declaration>() };
    
    return result.declaration;
}

auto main() -> int 
{
    constexpr auto colormap{ uniform<"colorMap", Type::sampler2D>() };
    constexpr auto normalmap{ uniform<"normalMap", Type::sampler2D>() };
    constexpr auto fog_color{ uniform<"fogColor", Type::vec3>() };
    constexpr auto fog_density{ uniform<"fogDensity", Type::f32>() };

    constexpr auto tex_coord{ in_var<"uvTexCoord", Type::vec2>() };
    constexpr auto position{ in_var<"position", Type::vec2>() };

    constexpr auto to_gamma{ function<"toGamma", Type::vec3, to_gamma_body(), Param<Type::vec3, "v">>() };

    constexpr auto result{ 
        concat(
            colormap.declaration, 
            normalmap.declaration, 
            fog_color.declaration, 
            fog_density.declaration,
            tex_coord.declaration,
            position.declaration,
            to_gamma.declaration
        ) 
    };

    print(result);
}
