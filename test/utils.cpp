#include <gtest/gtest.h>

#include "value.hpp"
#include "utils.hpp"

using namespace mfl;
using namespace mfl::detail;

TEST(Utils, version)
{
    static constexpr auto version_declaration{ set_version<"130">() };
    static constexpr auto expected_declaration{ "#version 130\n" };
    EXPECT_TRUE(version_declaration == expected_declaration);
}

TEST(Utils, expression_value)
{
    static constexpr auto st_expr{ 5 };
    EXPECT_TRUE("5" == expression_value<st_expr>());

    static constexpr auto nd_expr{ 6.5f };
    EXPECT_TRUE("6.5" == expression_value<nd_expr>());

    static constexpr auto rd_expr{ static_string{ "test" } };
    EXPECT_TRUE("test" == expression_value<rd_expr>());
}
