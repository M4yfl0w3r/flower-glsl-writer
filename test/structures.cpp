#include <gtest/gtest.h>

#include "static_string.hpp"
#include "structure.hpp"
#include "variable.hpp"

using namespace mfl;

consteval auto create_light_struct()
{
    return make_structure<"Light">(
        field<Type::vec4, "position">(),
        field<Type::vec4, "ambient">(),
        field<Type::vec4, "diffuse">(),
        field<Type::vec3, "spotDirection">(),
        field<Type::float_t, "spotCutoff">(),
        field<Type::float_t, "spotExponent">(),
        field<Type::float_t, "constantAttenuation">(),
        field<Type::float_t, "linearAttenuation">(),
        field<Type::float_t, "quadraticAttenuation">()
    );
}

TEST(Structs, LightStructDeclaration)
{
    constexpr auto light{ create_light_struct() };

    constexpr auto expected_result {
        static_string{ "struct Light {\n"
                       "vec4 position;\n"
                       "vec4 ambient;\n"
                       "vec4 diffuse;\n"
                       "vec3 spotDirection;\n"
                       "float spotCutoff;\n"
                       "float spotExponent;\n"
                       "float constantAttenuation;\n"
                       "float linearAttenuation;\n"
                       "float quadraticAttenuation;\n" 
                       "};\n" }
    };

    EXPECT_TRUE(light.declaration == expected_result);
}

TEST(Structs, LightStructAccessMembers)
{
    constexpr auto light{ create_light_struct() };
    constexpr auto position{ light.get<"position">() };
    constexpr auto diffuse{ light.get<"diffuse">() };
    constexpr auto const_att{ light.get<"constantAttenuation">() };

    EXPECT_EQ(position.type, Type::vec4);
    EXPECT_TRUE(position.name == "position");
    
    EXPECT_EQ(diffuse.type, Type::vec4);
    EXPECT_TRUE(diffuse.name == "diffuse");

    EXPECT_EQ(const_att.type, Type::float_t);
    EXPECT_TRUE(const_att.name == "constantAttenuation");
}

