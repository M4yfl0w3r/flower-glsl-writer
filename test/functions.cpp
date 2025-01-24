#include <gtest/gtest.h>
#include "function.hpp"
#include "variable.hpp"
#include "value.hpp"

using namespace mfl;

TEST(Functions, BasicFunctionSignature)
{
    constexpr auto fn{ function<"to_gamma", Type::gl_vec3, "", Param<"v", Type::gl_vec3>>() };

    constexpr auto expected_declaration{ 
        static_string{ 
            "vec3 to_gamma(vec3 v) {\n"
            "}\n" 
        } 
    };

    EXPECT_TRUE(fn.declaration == expected_declaration);
}

TEST(Functions, MultiParamsFunction)
{
    constexpr auto fn{ function<"test_multiparam", Type::gl_vec3, "", Param<"a", Type::gl_vec3>, Param<"b", Type::gl_vec3>>() };
    
    constexpr auto expected_declaration{ 
        static_string{ 
            "vec3 test_multiparam(vec3 a, vec3 b) {\n"
            "}\n"
        } 
    };

    EXPECT_TRUE(fn.declaration == expected_declaration);
}

TEST(Functions, BuiltInFunctions)
{
    // TODO: value(1.0f) for now, float -> static_string conversion in the future
    {
        constexpr auto vec_33{ vec3<value(1.0f), value(2.0f), value(3.0f)>() };
        constexpr auto expected_declaration{ "vec3(1.0f, 2.0f, 3.0f)" };
        EXPECT_TRUE(vec_33== expected_declaration);
    }

    {
        constexpr auto vec_32{ vec3<vec2<value(1.0f), value(2.0f)>(), value(3.0f)>() };
        constexpr auto expected_declaration{ "vec3(vec2(1.0f, 2.0f), 3.0f)" };
        EXPECT_TRUE(vec_32 == expected_declaration);
    }

    {
        constexpr auto vec_31{ vec3<value(1.0f)>() };
        constexpr auto expected_declaration{ "vec3(1.0f)" };
        EXPECT_TRUE(vec_31 == expected_declaration);
    }

    {
        constexpr auto vec_21{ vec2<value(1.0f)>() };
        constexpr auto expected_declaration{ "vec2(1.0f)" };
        EXPECT_TRUE(vec_21 == expected_declaration);
    }

    {
        constexpr auto depthmap{ uniform<Type::gl_sampler2D, "depthMap">() };
        constexpr auto var{ variable<Type::gl_vec3, "var", vec3<value(1.0f)>()>() };
        constexpr auto sample_val{ sample<depthmap, var>() };
        constexpr auto expected_declaration{ "texture2D(depthMap, var)" };
        EXPECT_TRUE(sample_val == expected_declaration);
    }

    {
        constexpr auto st_var{ variable<Type::gl_vec3, "st_var", vec3<value(1.0f)>()>() };
        constexpr auto nd_var{ variable<Type::gl_vec3, "nd_var", vec3<value(3.0f)>()>() };
        constexpr auto pow_val{ pow<st_var, nd_var>() };
        constexpr auto expected_declaration{ "pow(st_var, nd_var)" };
        EXPECT_TRUE(pow_val == expected_declaration);
    }

    {
        constexpr auto st_var{ variable<Type::gl_vec3, "st_var", vec3<value(1.0f)>()>() };
        constexpr auto length_val{ length<st_var>() };
        constexpr auto expected_declaration{ "length(st_var)" };
        EXPECT_TRUE(length_val == expected_declaration);
    }
    
    {
        constexpr auto st_var{ variable<Type::gl_vec3, "st_var", vec3<value(1.0f)>()>() };
        constexpr auto radians_val{ radians<st_var>() };
        constexpr auto expected_declaration{ "radians(st_var)" };
        EXPECT_TRUE(radians_val == expected_declaration);
    }
}

TEST(Functions, FunctionWithBody)
{
    constexpr auto body{ variable<Type::gl_vec3, "result", value(1.0f / 2.0f)>() };
    constexpr auto fn{ function<"to_gamma", Type::gl_vec3, body.declaration, Param<"v", Type::gl_vec3>>() };

    constexpr auto expected_declaration{ 
        static_string{ 
            "vec3 to_gamma(vec3 v) {\n"
            "vec3 result = 1.0f / 2.0f;\n"
            "}\n" 
        } 
    };

    EXPECT_TRUE(fn.declaration == expected_declaration);
}
