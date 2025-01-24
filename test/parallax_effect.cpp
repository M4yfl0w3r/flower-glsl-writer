#include <gtest/gtest.h>

#include "variable.hpp"
#include "function.hpp"
#include "statement.hpp"

using namespace mfl;

template <uniform depthmap, uniform colormap, in_var tex_coord, uniform deflection>
consteval auto parallax_main_body()
{
    constexpr auto depth_distortion{ 
        variable<Type::gl_vec4, "depth_distortion", sample<depthmap, tex_coord>()>()
    };

    constexpr auto parallax_multiplier{ 
        variable<Type::gl_float, "parallax_multiplier", depth_distortion.r()>() 
    };

    constexpr auto parallax{ 
        variable<Type::gl_vec2, "parallax", deflection * parallax_multiplier>() 
    };

    constexpr auto original{ 
        variable<Type::gl_vec4, "original", sample<colormap, tex_coord + parallax>()>() 
    };

    constexpr auto ret{ 
        frag_color<original>()
    };
    
    return concat_all(
        depth_distortion,
        parallax_multiplier,
        parallax,
        original,
        ret 
    );
}

TEST(Effects, Parallax)
{
    constexpr auto colormap{ uniform<Type::gl_sampler2D, "colorMap">() };
    constexpr auto depthmap{ uniform<Type::gl_sampler2D, "depthMap">() };
    constexpr auto deflection{ uniform<Type::gl_vec2, "deflection">() };

    constexpr auto tex_coord{ in_var<Type::gl_vec2, "uvTexCoord">() };

    constexpr auto body{ parallax_main_body<depthmap, colormap, tex_coord, deflection>() };
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

    constexpr auto expected_result{ 
        static_string{ 
            "uniform vec2 deflection;\n"
            "uniform sampler2D colorMap;\n"
            "uniform sampler2D depthMap;\n"
            "in vec2 uvTexCoord;\n"
            "void main() {\n"
            "vec4 depth_distortion = texture2D(depthMap, uvTexCoord);\n"
            "float parallax_multiplier = depth_distortion.r;\n"
            "vec2 parallax = deflection * parallax_multiplier;\n"
            "vec4 original = texture2D(colorMap, uvTexCoord + parallax);\n"
            "gl_FragColor = original;\n"
            "}\n"
        }
    };

    EXPECT_TRUE(result == expected_result);
}
