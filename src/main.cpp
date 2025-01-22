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

// TODO: pass lvalues
consteval auto main_body()
{
    // TODO: ugly comma fix
    constexpr auto depth_map_sample{ builtin_fn<"texture2D", Param<"depth_map, ">, Param<"uv_tex_coord">>() };
    constexpr auto color_map_sample{ builtin_fn<"texture2D", Param<"color_map, ">, Param<"uv_tex_coord + parallax">>() };

    constexpr auto depth_distortion{ variable<"depth_distortion", Type::vec4, depth_map_sample.declaration>() };
    constexpr auto parallax_multiplier{ variable<"parallax_multiplier", Type::float_t, value(depth_distortion.r)>() };
    constexpr auto parallax{ variable<"parallax", Type::vec2, value(deflection * parallax_multiplier)>() };
    constexpr auto original{ variable<"original", Type::vec4, color_map_sample.declaration>() };
    constexpr auto ret{ gl_FragColor<original.name>() };

    constexpr auto result{ 
        concat(
            depth_distortion.declaration,
            parallax_multiplier.declaration,
            parallax.declaration,
            original.declaration,
            ret.declaration
        )
    };

    return result;
}

auto main() -> int 
{
    constexpr auto colormap{ uniform<"colorMap", Type::sampler2D>() };
    constexpr auto depthmap{ uniform<"depthMap", Type::sampler2D>() };
    constexpr auto deflection{ uniform<"deflection", Type::vec2>() };

    constexpr auto tex_coord{ in_var<"uvTexCoord", Type::vec2>() };

    constexpr auto main_fn_impl{ main_fn<main_body()>() };

    constexpr auto result{ 
        concat(
            deflection.declaration,
            colormap.declaration, 
            depthmap.declaration, 
            tex_coord.declaration,
            main_fn_impl.declaration
        )
    };

    print(result);
}
