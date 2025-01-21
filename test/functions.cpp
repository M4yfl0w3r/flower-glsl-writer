#include <gtest/gtest.h>
#include "function.hpp"
#include "value.hpp"

using namespace mfl;

// vec3 toGamma(in vec3 v) {
//     vec3 res_toGamma = pow(v, vec3(1.0 / 2.2));
//     return res_toGamma;
// }

// #include <print>
// auto print(const auto& thing) {
//     for (auto e : thing) {
//         std::print("{}", e);
//     }
//     std::println("");
// }

TEST(Functions, BasicFunctionSignature)
{
    constexpr auto fn{ function<"to_gamma", Type::vec3, Param<Type::vec3, "v">>() };
    constexpr auto expected_declaration{ static_string{ "vec3 to_gamma(vec3 v) {}\n" } };

    EXPECT_TRUE(fn.declaration == expected_declaration);
}

TEST(Functions, MultiParamsFunction)
{
    // comma - super ugly fix for now, have to change it later
    constexpr auto fn{ function<"test_multiparam", Type::vec3, Param<Type::vec3, "a, ">, Param<Type::vec3, "b">>() };
    constexpr auto expected_declaration{ static_string{ "vec3 test_multiparam(vec3 a, vec3 b) {}\n"} };

    EXPECT_TRUE(fn.declaration == expected_declaration);
}

TEST(Functions, BuiltinFunction)
{
    constexpr auto fn{ function<"vec3", Type::empty, Param<Type::empty, value(1.0f / 2.2f)>>() };
    constexpr auto expected_declaration{ static_string{ "vec3(1.0f / 2.2f)" } };

    EXPECT_TRUE(fn.declaration == expected_declaration);
}
