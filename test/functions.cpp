#include <gtest/gtest.h>

#include "function.hpp"

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
    constexpr auto expected_result{ static_string{ "vec3 to_gamma(in vec3 v) {};\n" } };

    EXPECT_TRUE(fn.declaration == expected_result);
}
