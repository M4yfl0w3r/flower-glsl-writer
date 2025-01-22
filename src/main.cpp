#include <print>

#include "variable.hpp"
#include "function.hpp"
#include "symbols.hpp"
#include "value.hpp"

using namespace mfl;

template <uniform depthmap, uniform colormap, in_var tex_coord, uniform deflection>
consteval auto main_body()
{
    constexpr auto depth_distortion{ 
        // vec4 depth_distortion = texture2D(depth_map, uv_tex_coord);
        variable<Type::vec4, "depth_distortion", sample<depthmap, tex_coord>()>()
    };

    constexpr auto parallax_multiplier{ 
        // float parallax_multiplier = depth_distortion.r;
        variable<Type::float_t, "parallax_multiplier", value(depth_distortion.r)>() 
    };

    constexpr auto parallax{ 
        // vec2 parallax = deflection * parallax_multiplier;
        variable<Type::vec2, "parallax", multiply<deflection, parallax_multiplier>()>() 
    };

    constexpr auto original{ 
	    // vec4 original = texture2D(color_map, uv_tex_coord + parallax);
        variable<Type::vec4, "original", sample<colormap, add<tex_coord, parallax>()>()>() };

    constexpr auto ret{ 
        // gl_FragColor = original;
        frag_color<original.name>() 
    };
    
    return concat_all(
        depth_distortion,
        parallax_multiplier,
        parallax,
        original,
        ret 
    );
}

auto main() -> int 
{
    constexpr auto colormap{ uniform<Type::sampler2D, "colorMap">() };
    constexpr auto depthmap{ uniform<Type::sampler2D, "depthMap">() };
    constexpr auto deflection{ uniform<Type::vec2, "deflection">() };
    constexpr auto tex_coord{ in_var<Type::vec2, "uvTexCoord">() };
    constexpr auto main_fn_impl{ main_fn<main_body<depthmap, colormap, tex_coord, deflection>()>() };
    
    constexpr auto result {
        concat_all(
            deflection,
            colormap, 
            depthmap, 
            tex_coord,
            main_fn_impl
        )
    };

    print(result);
}
