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

TEST(Values, compile_time_static_string_to_float_conversion)
{
    {
        static constexpr auto test_str{ static_string{ "123.12f"} };
        static constexpr auto test_float{ convert_to_float<test_str>() };
        EXPECT_EQ(test_float, 123.12f);
    }
    
    {
        static constexpr auto test_str{ static_string{ "0.12f"} };
        static constexpr auto test_float{ convert_to_float<test_str>() };
        EXPECT_EQ(test_float, 0.12f);
    }

    // {
    //     static constexpr auto test_str{ static_string{ "123.01f"} };
    //     static constexpr auto test_float{ convert_to_float<test_str>() };
    //     EXPECT_EQ(test_float, 123.01f);
    // }
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
