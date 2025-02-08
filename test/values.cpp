#include <gtest/gtest.h>

#include "value.hpp"

using namespace mfl;

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
        static constexpr auto test_float{ 12.431f };
        static constexpr auto test_str{ convert_to_string<test_float>() };
        EXPECT_TRUE(test_str == "12.4f");
    }
}
