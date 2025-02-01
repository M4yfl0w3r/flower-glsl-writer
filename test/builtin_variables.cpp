#include <gtest/gtest.h>

#include "statement.hpp"
#include "function.hpp"
#include "storage.hpp"
#include "value.hpp"

using namespace mfl;
using enum mfl::Type;

TEST(BuiltIn, gl_FragColor)
{
    {
        static constexpr auto var{ variable<gl_float, "test_var", value(1.0f)>() };
        static constexpr auto frag_color{ gl_frag_color<var>() };
        static constexpr auto expected_result{ "gl_FragColor = test_var;\n" };
        EXPECT_TRUE(frag_color.declaration == expected_result);
    }

    {
        static constexpr auto frag_color{ gl_frag_color<vec3<value(1.0f)>()>() };
        static constexpr auto expected_result{ "gl_FragColor = vec3(1.0f);\n" };
        EXPECT_TRUE(frag_color.declaration == expected_result);
    }
}

TEST(BuiltIn, ReadOnlyBuiltInVariables)
{
    EXPECT_TRUE(gl_model_view_proj_matrix.declaration == "gl_ModelViewProjectionMatrix");
    EXPECT_TRUE(gl_multi_tex_coord_0.declaration == "gl_MultiTexCoord0");
    EXPECT_TRUE(gl_vertex.declaration == "gl_Vertex");
}

TEST(BuiltIn, Access)
{
    static constexpr auto tex_coord{ gl_tex_coord };
    static constexpr auto access_expression{ tex_coord.at_expression<value(0)>() };
    static constexpr auto expected_result{ "gl_TexCoord[0]" };
    EXPECT_TRUE(access_expression == expected_result);
}

TEST(BuiltIn, Assign)
{
    static constexpr auto tex_coord{ gl_tex_coord };
    static constexpr auto assign_expression{ tex_coord.assign_at<gl_multi_tex_coord_0, value(0)>() };
    static constexpr auto expected_result{ "gl_TexCoord[0] = gl_MultiTexCoord0;\n" };
    EXPECT_TRUE(assign_expression == expected_result);
}

