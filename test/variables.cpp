#include <gtest/gtest.h>

#include "variable.hpp"
#include "value.hpp"

using namespace mfl;

TEST(Variables, UniformsDeclaration)
{
    constexpr auto create_uniforms = []() {
        constexpr auto color_map{ uniform<Type::gl_sampler2D, "color_map">() };
        constexpr auto normal_map{ uniform<Type::gl_sampler2D, "normal_map">() };
        constexpr auto fog_color{ uniform<Type::gl_vec3, "fog_color">() };
        return concat(color_map.declaration, normal_map.declaration, fog_color.declaration);
    };

    constexpr auto result{ create_uniforms() };

    constexpr auto expected_result{ 
        static_string{ "uniform sampler2D color_map;\n"
                       "uniform sampler2D normal_map;\n"
                       "uniform vec3 fog_color;\n" } 
    };

    EXPECT_TRUE(result == expected_result);
} 

TEST(Variables, BaseTypeDeclarations)
{
    constexpr auto test_float{ variable<Type::gl_float, "test_float", value(1.0f)>() };
    constexpr auto expected_result{ static_string{ "float test_float = 1.0f;\n"} };
    EXPECT_TRUE(test_float.declaration == expected_result);
}
