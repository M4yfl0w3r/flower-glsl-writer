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
        constexpr auto color_map{ variable<static_string{ "color_map" }, Type::sampler2D, Keyword::uniform>() };
        constexpr auto normal_map{ variable<static_string{ "normal_map" }, Type::sampler2D, Keyword::uniform>() };
        constexpr auto fog_color{ variable<static_string{ "fog_color" }, Type::vec3, Keyword::uniform>() };
        return concat(color_map.declaration, normal_map.declaration, fog_color.declaration);
    };

    constexpr auto result{ create_uniforms() };

    constexpr auto expected_result{ 
        static_string{ "uniform sampler2D color_map;\nuniform sampler2D normal_map;\nuniform vec3 fog_color;\n" } 
    };

    EXPECT_TRUE(result == expected_result);
}
