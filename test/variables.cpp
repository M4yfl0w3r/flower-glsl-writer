#include <gtest/gtest.h>

#include "variable.hpp"
#include "value.hpp"

using namespace mfl;
using enum mfl::Type;

TEST(Variables, uniform_declaration)
{
    constexpr auto create_uniforms = []() {
        static constexpr auto color_map{ uniform<gl_sampler2D, "color_map">() };
        static constexpr auto normal_map{ uniform<gl_sampler2D, "normal_map">() };
        static constexpr auto fog_color{ uniform<gl_vec3, "fog_color">() };
        return concat(color_map.declaration, normal_map.declaration, fog_color.declaration);
    };

    static constexpr auto result{ create_uniforms() };

    static constexpr auto expected_result{ 
        "uniform sampler2D color_map;\n"
        "uniform sampler2D normal_map;\n"
        "uniform vec3 fog_color;\n"
    };

    EXPECT_TRUE(result == expected_result);
} 

TEST(Variables, in_variable_declaration)
{
    static constexpr auto tex_coord{ in_var<gl_vec2, "uvTexCoord">() };
    static constexpr auto expected_result{ "in vec2 uvTexCoord;\n" };
    EXPECT_TRUE(tex_coord.declaration == expected_result);
}

TEST(Variables, base_type)
{
    {
        static constexpr auto test_float{ variable<gl_float, "test">() };
        static constexpr auto expected_result{ "float test;\n" };
        EXPECT_TRUE(test_float.declaration == expected_result);
    }

    {
        static constexpr auto test_float{ variable<gl_float, "test", 1.0f>() };
        static constexpr auto expected_result{ "float test = 1.0f;\n" };
        EXPECT_TRUE(test_float.declaration == expected_result);
    }
}

TEST(Variables, operations)
{
    {
        static constexpr auto test_var{ variable<gl_float, "test_var", 1.0f>() };
        static constexpr auto test_float{ variable<gl_float, "test", 1.0f>() };
        static constexpr auto assignment_result{ test_float.assign<test_var>() };
        static constexpr auto expected_result{ "test = test_var;\n" };
        EXPECT_TRUE(assignment_result == expected_result);
    }

    {
        static constexpr auto test_var{ variable<gl_float, "test_var">() };
        static constexpr auto test_float{ variable<gl_float, "test">() };
        static constexpr auto assignment_result{ test_float.multiply<test_var>() };
        static constexpr auto expected_result{ "test * test_var" };
        EXPECT_TRUE(assignment_result == expected_result);
    }

    {
        static constexpr auto test_float{ variable<gl_float, "test", 1.0f>() };
        static constexpr auto assignment_result{ test_float.multiply<5.0f>() };
        static constexpr auto expected_result{ "test * 5.0f" };
        EXPECT_TRUE(assignment_result == expected_result);
    }
}

