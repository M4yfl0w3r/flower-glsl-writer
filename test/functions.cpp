#include <gtest/gtest.h>
#include "function.hpp"
#include "variable.hpp"
#include "value.hpp"

using namespace mfl;
using enum mfl::Type;

TEST(Functions, BasicFunctionSignature)
{
    static constexpr auto fn{ function<"to_gamma", gl_vec3, "", Param<"v", gl_vec3>>() };

    static constexpr auto expected_declaration{ 
        "vec3 to_gamma(vec3 v) {\n"
        "}\n" 
    };

    EXPECT_TRUE(fn.declaration == expected_declaration);
}

TEST(Functions, MultiParamsFunction)
{
    static constexpr auto fn{ function<"test_multiparam", gl_vec3, "", Param<"a", gl_vec3>, Param<"b", gl_vec3>>() };
    
    static constexpr auto expected_declaration{ 
        "vec3 test_multiparam(vec3 a, vec3 b) {\n"
        "}\n"
    };

    EXPECT_TRUE(fn.declaration == expected_declaration);
}

TEST(Functions, VecBuiltInFunctions)
{
    {
        static constexpr auto vec_33{ vec3<value(1.0f), value(2.0f), value(3.0f)>() };
        static constexpr auto expected_declaration{ "vec3(1.0f, 2.0f, 3.0f)" };
        EXPECT_TRUE(vec_33 == expected_declaration);
    }

    {
        static constexpr auto st_var{ variable<gl_float, "st_var">() };
        static constexpr auto nd_var{ variable<gl_float, "nd_var">() };
        static constexpr auto rd_var{ variable<gl_float, "rd_var">() };
        static constexpr auto vec_33{ vec3<st_var, nd_var, rd_var>() };
        static constexpr auto expected_declaration{ "vec3(st_var, nd_var, rd_var)" };
        EXPECT_TRUE(vec_33 == expected_declaration);
    }

    {
        static constexpr auto vec_32{ vec3<vec2<value(1.0f), value(2.0f)>(), value(3.0f)>() };
        static constexpr auto expected_declaration{ "vec3(vec2(1.0f, 2.0f), 3.0f)" };
        EXPECT_TRUE(vec_32 == expected_declaration);
    }

    {
        static constexpr auto st_var{ variable<gl_float, "st_var">() };
        static constexpr auto nd_var{ variable<gl_float, "nd_var">() };
        static constexpr auto vec_32{ vec3<st_var, nd_var>() };
        static constexpr auto expected_declaration{ "vec3(st_var, nd_var)" };
        EXPECT_TRUE(vec_32 == expected_declaration);
    }

    {
        static constexpr auto vec_31{ vec3<value(1.0f)>() };
        static constexpr auto expected_declaration{ "vec3(1.0f)" };
        EXPECT_TRUE(vec_31 == expected_declaration);
    }
    
    {
        static constexpr auto var{ variable<gl_float, "var">() };
        static constexpr auto vec_31{ vec3<var>() };
        static constexpr auto expected_declaration{ "vec3(var)" };
        EXPECT_TRUE(vec_31 == expected_declaration);
    }

    {
        static constexpr auto vec_21{ vec2<value(1.0f)>() };
        static constexpr auto expected_declaration{ "vec2(1.0f)" };
        EXPECT_TRUE(vec_21 == expected_declaration);
    }
    
    {
        static constexpr auto var{ variable<gl_float, "test">() };
        static constexpr auto vec_21{ vec2<var>() };
        static constexpr auto expected_declaration{ "vec2(test)" };
        EXPECT_TRUE(vec_21 == expected_declaration);
    }

    {
        static constexpr auto st_var{ variable<gl_float, "st_var">() };
        static constexpr auto nd_var{ variable<gl_float, "nd_var">() };
        static constexpr auto vec_22{ vec2<st_var, nd_var>() };
        static constexpr auto expected_declaration{ "vec2(st_var, nd_var)" };
        EXPECT_TRUE(vec_22 == expected_declaration);
    }
}

TEST(Functions, BuiltInFunctions)
{
    {
        static constexpr auto depthmap{ uniform<gl_sampler2D, "depthMap">() };
        static constexpr auto var{ variable<gl_vec3, "var", vec3<value(1.0f)>()>() };
        static constexpr auto sample_val{ sample<depthmap, var>() };
        static constexpr auto expected_declaration{ "texture2D(depthMap, var)" };
        EXPECT_TRUE(sample_val == expected_declaration);
    }

    {
        static constexpr auto st_var{ variable<gl_vec3, "st_var", vec3<value(1.0f)>()>() };
        static constexpr auto nd_var{ variable<gl_vec3, "nd_var", vec3<value(3.0f)>()>() };
        static constexpr auto pow_val{ pow<st_var, nd_var>() };
        static constexpr auto expected_declaration{ "pow(st_var, nd_var)" };
        EXPECT_TRUE(pow_val == expected_declaration);
    }

    {
        static constexpr auto st_var{ variable<gl_vec3, "st_var", vec3<value(1.0f)>()>() };
        static constexpr auto length_val{ length<st_var>() };
        static constexpr auto expected_declaration{ "length(st_var)" };
        EXPECT_TRUE(length_val == expected_declaration);
    }

    {
        static constexpr auto var{ variable<gl_vec3, "var", vec3<value(1.0f)>()>() };
        static constexpr auto radians_val{ radians<var>() };
        static constexpr auto expected_declaration{ "radians(var)" };
        EXPECT_TRUE(radians_val == expected_declaration);
    }

    {
        static constexpr auto color_map{ uniform<gl_sampler2D, "colorMap">() };
        static constexpr auto tex_size{ texture_size<color_map, value(0)>() };
        static constexpr auto expected_declaration{ "textureSize(colorMap, 0)" };
        EXPECT_TRUE(tex_size == expected_declaration);
    }

    {
        static constexpr auto var{ variable<gl_vec3, "var", vec3<value(1.0f)>()>() };
        static constexpr auto norm{ normalize<var>() };
        static constexpr auto expected_declaration{ "normalize(var)" };
        EXPECT_TRUE(norm == expected_declaration);
    }
}

TEST(Functions, FunctionWithBody)
{
    static constexpr auto body{ variable<gl_vec3, "result", value(1.0f / 2.0f)>() };
    static constexpr auto fn{ function<"to_gamma", gl_vec3, body.declaration, Param<"v", gl_vec3>>() };

    static constexpr auto expected_declaration{ 
        "vec3 to_gamma(vec3 v) {\n"
        "vec3 result = 1.0f / 2.0f;\n"
        "}\n" 
    };

    EXPECT_TRUE(fn.declaration == expected_declaration);
}

TEST(Functions, SingleParamFunctionCall)
{
    static constexpr auto fn{ function<"fn", gl_vec3, "", Param<"v", gl_vec3>>() };
    static constexpr auto test_var{ variable<gl_vec3, "var", vec3<value(1.0f)>()>() };
    static constexpr auto res{ fn.call<test_var>() };
    static constexpr auto expected_declaration{ "fn(var)" };
    EXPECT_TRUE(res == expected_declaration);
}

TEST(Functions, MultiParamFunctionCall)
{
    static constexpr auto fn{ function<"fn", gl_vec3, "", Param<"v1", gl_vec3>, Param<"v2", gl_vec3>>() };
    static constexpr auto st_var{ variable<gl_vec3, "st_var", vec3<value(1.0f)>()>() };
    static constexpr auto nd_var{ variable<gl_vec3, "nd_var", vec3<value(2.0f)>()>() };
    static constexpr auto res{ fn.call<st_var, nd_var>() };
    static constexpr auto expected_declaration{ "fn(st_var, nd_var)" };
    EXPECT_TRUE(res == expected_declaration);
}
