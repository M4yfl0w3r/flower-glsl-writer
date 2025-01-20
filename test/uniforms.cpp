#include <gtest/gtest.h>

#include "variable.hpp"

using namespace mfl;

// auto print(const auto& thing) {
//     for (auto e : thing) {
//         std::print("{}", e);
//     }
//     std::println("");
// }

TEST(Uniforms, UniformsDeclaration)
{
    constexpr auto create_uniforms = []() {
        constexpr auto color_map{ uniform<"color_map", Type::sampler2D>() };
        constexpr auto normal_map{ uniform<"normal_map", Type::sampler2D>() };
        constexpr auto fog_color{ uniform<"fog_color", Type::vec3>() };
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
