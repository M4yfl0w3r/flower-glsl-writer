#include <gtest/gtest.h>

#include "variable.hpp"
#include "statement.hpp"
#include "function.hpp"
#include "value.hpp"

using namespace mfl;
using enum mfl::Type;

template <out_var tex_coord>
static consteval auto main_body()
{
    constexpr auto pos{
        gl_position<gl_model_view_proj_matrix * gl_vertex>{}
    };

    return concat(pos.declaration);
}

TEST(Shaders, VertexShader)
{
static constexpr auto version{ set_version<"130">() };
    static constexpr auto tex_coord{ out_var<gl_vec2, "uvTexCoord">{} };

    static constexpr auto body{ main_body<tex_coord>() };
    static constexpr auto main_fn_impl{ main_fn<body>() };

    // static constexpr auto expected_result{ 
    //     "#version 130"
    //     "out vec2 uvTexCoord;\n"
    //     "void main() {\n"
    //     "gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
    //     "gl_TexCoord[0] = gl_MultiTexCoord0;\n"
    //     "uvTexCoord = gl_TexCoord[0].st;\n"
    //     "}\n"
    // };

    static constexpr auto result {
        concat(
            version,
            tex_coord.declaration,
            main_fn_impl.declaration
        )
    };

    print(result);
}

