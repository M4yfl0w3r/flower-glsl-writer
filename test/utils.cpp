#include <gtest/gtest.h>

#include "value.hpp"
using namespace mfl;

TEST(Utils, Version)
{
    static constexpr auto version_declaration{ set_version<"130">() };
    static constexpr auto expected_declaration{ "#version 130" };
    EXPECT_TRUE(version_declaration == expected_declaration);
}
