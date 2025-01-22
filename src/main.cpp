#include <print>

#include "variable.hpp"
#include "function.hpp"
#include "symbols.hpp"

using namespace mfl;

constexpr auto colormap{ uniform<Type::sampler2D, "colorMap">() };
constexpr auto depthmap{ uniform<Type::sampler2D, "depthMap">() };
constexpr auto deflection{ uniform<Type::vec2, "deflection">() };

constexpr auto tex_coord{ in_var<Type::vec2, "uvTexCoord">() };

template <uniform depthmap, uniform colormap, in_var tex_coord, uniform deflection>
consteval auto main_body()
{
    constexpr auto depth_distortion{ 
        variable<Type::vec4, "depth_distortion", sample<depthmap, tex_coord>()>()
    };

    constexpr auto parallax_multiplier{ 
        variable<Type::float_t, "parallax_multiplier", depth_distortion.r()>() 
    };

    constexpr auto parallax{ 
        variable<Type::vec2, "parallax", multiply<deflection, parallax_multiplier>()>() 
    };

    constexpr auto original{ 
        variable<Type::vec4, "original", sample<colormap, add<tex_coord, parallax>()>()>() 
    };

    constexpr auto ret{ 
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
    constexpr auto body{ main_body<depthmap, colormap, tex_coord, deflection>() };
    constexpr auto main_fn_impl{ main_fn<body>() };
    
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
