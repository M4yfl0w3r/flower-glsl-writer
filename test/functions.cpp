#include <gtest/gtest.h>
#include "function.hpp"
#include "variable.hpp"
#include "value.hpp"

using namespace mfl;

// #include <print>
// auto print(const auto& thing) {
//     for (auto e : thing) {
//         std::print("{}", e);
//     }
// }

TEST(Functions, BasicFunctionSignature)
{
    constexpr auto fn{ function<"to_gamma", Type::vec3, "", Param<"v", Type::vec3>>() };

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
    constexpr auto fn{ function<"test_multiparam", Type::vec3, "", Param<"a", Type::vec3>, Param<"b", Type::vec3>>() };
    
    constexpr auto expected_declaration{ 
        static_string{ 
            "vec3 test_multiparam(vec3 a, vec3 b) {\n"
            "}\n"
        } 
    };

    EXPECT_TRUE(fn.declaration == expected_declaration);
}

TEST(Functions, BuiltinFunction)
{
    constexpr auto fn{ builtin_fn<"vec3", Param<value(1.0f / 2.0f)>>() };
    constexpr auto expected_declaration{ static_string{ "vec3(1.0f / 2.0f)" } };
    EXPECT_TRUE(fn.declaration == expected_declaration);
}

TEST(Functions, FunctionWithBody)
{
    constexpr auto body{ variable<"result", Type::vec3, value(1.0f / 2.0f)>() };
    constexpr auto fn{ function<"to_gamma", Type::vec3, body.declaration, Param<"v", Type::vec3>>() };

    constexpr auto expected_declaration{ 
        static_string{ 
            "vec3 to_gamma(vec3 v) {\n"
            "vec3 result = 1.0f / 2.0f;\n"
            "}\n" 
        } 
    };

    EXPECT_TRUE(fn.declaration == expected_declaration);
}
