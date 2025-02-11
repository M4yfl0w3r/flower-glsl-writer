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

TEST(Structs, declaration)
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

TEST(Structs, access_members)
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

TEST(Structs, global_structs)
{
    static constexpr auto position{ light_source.template get<"position">() };
    static constexpr auto diffuse{ light_source.template get<"diffuse">() };

    EXPECT_EQ(position.type, gl_vec4);
    EXPECT_TRUE(position.name == "position");

    EXPECT_EQ(diffuse.type, gl_vec4);
    EXPECT_TRUE(diffuse.name == "diffuse");
}

TEST(Arrays, declaration)
{
    static constexpr auto arr{ array<gl_int, 5, "test">() };
    static constexpr auto expected_result{ "int test[5];\n" };
    EXPECT_TRUE(arr.declaration == expected_result);
}

TEST(Arrays, base_type_definition)
{
    {
        static constexpr auto arr{ array<gl_int, 4, "test", 1, 2, 3, 4>() };
        static constexpr auto expected_result{ "int test[4] = int[](1, 2, 3, 4);\n" };
        EXPECT_TRUE(arr.declaration == expected_result);
    }

    {
        static constexpr auto arr{ array<gl_float, 3, "test", 1.0f, 2.0f, 3.0f>() };
        static constexpr auto expected_result{ "float test[3] = float[](1.0f, 2.0f, 3.0f);\n" };
        EXPECT_TRUE(arr.declaration == expected_result);
    }
}

TEST(Arrays, base_type_get_element)
{
    static constexpr auto arr{ 
        array<gl_int, 4, "test", 1, 2, 3, 4>() 
    };
    
    EXPECT_TRUE(arr.at<1>() == static_string{ "2" });   // TODO: should return an int
    EXPECT_TRUE(arr.at<3>() == static_string{ "4" });
}

TEST(Arrays, base_type_access_expression)
{
    static constexpr auto arr{ 
        array<gl_int, 4, "array", 1, 2>{}
    };

    static constexpr auto access_var{ variable<gl_int, "i">{} };

    EXPECT_TRUE(arr.at_expression<1>() == "array[1]");
    EXPECT_TRUE(arr.at_expression<access_var>() == "array[i]");
}

TEST(Arrays, custom_type_declaration)
{
    static constexpr auto light{ create_light_struct() };
    static constexpr auto num_lights{ define_statement<"NUM_LIGHTS", 2>() };
    static constexpr auto arr{ array<light.name, num_lights, "test">() };
    static constexpr auto expected_result{ "Light test[NUM_LIGHTS];\n" };
    EXPECT_TRUE(arr.declaration == expected_result);
}

TEST(Arrays, array_of_struct_access)
{
    static constexpr auto light{ create_light_struct() };
    static constexpr auto num_lights{ define_statement<"NUM_LIGHTS", static_string{ "2" }>() };
    static constexpr auto arr{ make_array_of_structs<light, num_lights, "test">() };

    static constexpr auto access_var{ variable<gl_int, "i">() };
    static constexpr auto test{ arr.template member_access_at<"position", access_var>() };

    EXPECT_TRUE(test == "test[i].position");
}

