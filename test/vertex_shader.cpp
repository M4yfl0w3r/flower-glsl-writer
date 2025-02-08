#include <gtest/gtest.h>

#include "variable.hpp"
#include "statement.hpp"
#include "function.hpp"
#include "storage.hpp"
#include "value.hpp"

using namespace mfl;
using enum mfl::Type;

template <out_var tex_coord>
static consteval auto main_body()
{
    return concat(
        gl_position<gl_model_view_proj_matrix * gl_vertex>{}.declaration,
        gl_tex_coord.assign_at<gl_multi_tex_coord_0, 0>(),
        tex_coord.template assign<gl_tex_coord.member_access_at<"st", 0>()>()
    );
}

TEST(Shaders, VertexShader)
{
    static constexpr auto version{ set_version<"130">() };
    static constexpr auto tex_coord{ out_var<gl_vec2, "uvTexCoord">{} };

    static constexpr auto body{ main_body<tex_coord>() };
    static constexpr auto main_fn_impl{ main_fn<body>() };

    static constexpr auto result{
        concat(
            version,
            tex_coord.declaration,
            main_fn_impl.declaration
        )
    };

    static constexpr auto expected_result{ 
        "#version 130\n"
        "out vec2 uvTexCoord;\n"
        "void main() {\n"
        "gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
        "gl_TexCoord[0] = gl_MultiTexCoord0;\n"
        "uvTexCoord = gl_TexCoord[0].st;\n"
        "}\n"
    };

    EXPECT_TRUE(result == expected_result);
}

