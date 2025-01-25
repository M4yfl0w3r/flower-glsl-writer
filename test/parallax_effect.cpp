#include <gtest/gtest.h>

#include "variable.hpp"
#include "function.hpp"
#include "statement.hpp"

using namespace mfl;
using enum mfl::Type;

template <uniform depthmap, uniform colormap, in_var tex_coord, uniform deflection>
consteval auto parallax_main_body()
{
    static constexpr auto depth_distortion{ 
        variable<gl_vec4, "depth_distortion", sample<depthmap, tex_coord>()>()
    };

    static constexpr auto parallax_multiplier{ 
        variable<gl_float, "parallax_multiplier", depth_distortion.r()>() 
    };

    static constexpr auto parallax{ 
        variable<gl_vec2, "parallax", deflection * parallax_multiplier>() 
    };

    static constexpr auto original{ 
        variable<gl_vec4, "original", sample<colormap, tex_coord + parallax>()>() 
    };

    static constexpr auto ret{ 
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
    static constexpr auto colormap{ uniform<gl_sampler2D, "colorMap">() };
    static constexpr auto depthmap{ uniform<gl_sampler2D, "depthMap">() };
    static constexpr auto deflection{ uniform<gl_vec2, "deflection">() };

    static constexpr auto tex_coord{ in_var<gl_vec2, "uvTexCoord">() };

    static constexpr auto body{ parallax_main_body<depthmap, colormap, tex_coord, deflection>() };
    static constexpr auto main_fn_impl{ main_fn<body>() };
    
    static constexpr auto result {
        concat_all(
            deflection,
            colormap, 
            depthmap, 
            tex_coord,
            main_fn_impl
        )
    };

    static constexpr auto expected_result{ 
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
