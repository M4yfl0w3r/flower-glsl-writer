#include <gtest/gtest.h>

#include "value.hpp"
#include "variable.hpp"

using namespace mfl;
using enum mfl::Type;

TEST(Values, compile_time_static_string_to_int_conversion)
{
    {
        static constexpr auto test_str{ static_string{ "2" } };
        static constexpr auto test_int{ convert_to_int<test_str>() };
        EXPECT_EQ(test_int, 2);
    }

    {
        static constexpr auto test_str{ static_string{ "12" } };
        static constexpr auto test_int{ convert_to_int<test_str>() };
        EXPECT_EQ(test_int, 12);
    }

    {
        static constexpr auto test_str{ static_string{ "122" } };
        static constexpr auto test_int{ convert_to_int<test_str>() };
        EXPECT_EQ(test_int, 122);
    }
}

TEST(Values, compile_time_int_to_static_string_conversion)
{
    {
        static constexpr auto test_int{ 2 };
        static constexpr auto test_str{ convert_to_string<test_int>() };
        EXPECT_TRUE(test_str == "2");
    }

    {
        static constexpr auto test_int{ 12 };
        static constexpr auto test_str{ convert_to_string<test_int>() };
        EXPECT_TRUE(test_str == "12");
    }

    {
        static constexpr auto test_int{ 132 };
        static constexpr auto test_str{ convert_to_string<test_int>() };
        EXPECT_TRUE(test_str == "132");
    }
}

TEST(Values, compile_time_float_to_static_string_conversion)
{
    {
        static constexpr auto test_float{ 0.1f };
        static constexpr auto test_str{ convert_to_string<test_float>() };
        EXPECT_TRUE(test_str == "0.1f");
    }

    {
        static constexpr auto test_float{ 12.431f };
        static constexpr auto test_str{ convert_to_string<test_float>() };
        EXPECT_TRUE(test_str == "12.4f");
    }
}

TEST(Values, float_minus_variable)
{
    static constexpr auto test_float{ 0.5f };
    static constexpr auto test_var{ variable<gl_float, "test_var", 0.5f>{} };
    static constexpr auto result{ test_float - test_var };
    EXPECT_EQ(result, 0.0f);
}

TEST(Values, int_minus_variable)
{
    static constexpr auto test_int{ 10 };
    static constexpr auto test_var{ variable<gl_int, "test_var", 3>{} };
    static constexpr auto result{ test_int - test_var };
    EXPECT_EQ(result, 7);
}
