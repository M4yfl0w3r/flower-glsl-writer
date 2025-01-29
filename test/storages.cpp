#include <gtest/gtest.h>

#include "static_string.hpp"
#include "variable.hpp"
#include "storage.hpp"
#include "value.hpp"

using namespace mfl;
using enum mfl::Type;

static consteval auto create_light_struct()
{
    return structure<
        "Light",
        field<gl_vec4, "position">{},
        field<gl_vec4, "ambient">{},
        field<gl_vec4, "diffuse">{},
        field<gl_vec3, "spotDirection">{},
        field<gl_float, "spotCutoff">{},
        field<gl_float, "spotExponent">{},
        field<gl_float, "constantAttenuation">{},
        field<gl_float, "linearAttenuation">{},
        field<gl_float, "quadraticAttenuation">{}
    >();
}

TEST(Structs, LightStructDeclaration)
{
    static constexpr auto light{ create_light_struct() };

    static constexpr auto expected_result {
        "struct Light {\n"
        "vec4 position;\n"
        "vec4 ambient;\n"
        "vec4 diffuse;\n"
        "vec3 spotDirection;\n"
        "float spotCutoff;\n"
        "float spotExponent;\n"
        "float constantAttenuation;\n"
        "float linearAttenuation;\n"
        "float quadraticAttenuation;\n" 
        "};\n"
    };

    EXPECT_TRUE(light.declaration == expected_result);
}

TEST(Structs, LightStructAccessMembers)
{
    static constexpr auto light{ create_light_struct() };
    static constexpr auto position{ light.template get<"position">() };
    static constexpr auto diffuse{ light.template get<"diffuse">() };
    static constexpr auto const_att{ light.template get<"constantAttenuation">() };

    EXPECT_EQ(position.type, gl_vec4);
    EXPECT_TRUE(position.name == "position");

    EXPECT_EQ(diffuse.type, gl_vec4);
    EXPECT_TRUE(diffuse.name == "diffuse");

    EXPECT_EQ(const_att.type, gl_float);
    EXPECT_TRUE(const_att.name == "constantAttenuation");
}

TEST(Structs, GlobalStructs)
{
    static constexpr auto position{ light_source.template get<"position">() };
    static constexpr auto diffuse{ light_source.template get<"diffuse">() };

    EXPECT_EQ(position.type, gl_vec4);
    EXPECT_TRUE(position.name == "position");

    EXPECT_EQ(diffuse.type, gl_vec4);
    EXPECT_TRUE(diffuse.name == "diffuse");
}

TEST(Arrays, ArrayDeclaration)
{
    static constexpr auto arr{ array<gl_int, value(5), "test">() };
    static constexpr auto expected_result{ "int test[5];\n" };
    print(arr.declaration);
    EXPECT_TRUE(arr.declaration == expected_result);
}

TEST(Arrays, BaseTypeArrayDefinition)
{
    static constexpr auto arr{ 
        array<gl_int, value(4), "test", value(1), value(2), value(3), value(4)>() 
    };

    print(arr.declaration);

    static constexpr auto expected_result{ "int test[4] = int[](1, 2, 3, 4);\n" };
    EXPECT_TRUE(arr.declaration == expected_result);
}

TEST(Arrays, CustomTypeArrayDeclaration)    
{
    static constexpr auto light{ create_light_struct() };
    static constexpr auto num_lights{ define_statement<"NUM_LIGHTS", value(2)>() };
    static constexpr auto arr{ array<light.name, num_lights, "test">() };
    static constexpr auto expected_result{ "Light test[NUM_LIGHTS];\n" };
    EXPECT_TRUE(arr.declaration == expected_result);
}

TEST(Arrays, ArrayOfStructsAccess)
{
    static constexpr auto light{ create_light_struct() };
    static constexpr auto num_lights{ define_statement<"NUM_LIGHTS", value(2)>() };
    // TODO: a way to pass light.fields
    static constexpr auto arr{ array<light.name, num_lights, "test", field<gl_vec4, "position">{}>() };

    static constexpr auto access_var{ variable<gl_int, "i">() };
    static constexpr auto test{ arr.template member_access_at<"position", access_var>() };

    EXPECT_TRUE(test == "test[i].position");
}

