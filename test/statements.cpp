#include <gtest/gtest.h>

#include "static_string.hpp"
#include "statement.hpp"
#include "value.hpp"

using namespace mfl;

TEST(Statements, JumpStatements)
{
    {
        constexpr auto gl_return{ return_statement() };
        constexpr auto expected_result{ "return;\n" };
        EXPECT_TRUE(gl_return.declaration == expected_result);
    }

    {
        constexpr auto gl_continue{ continue_statement() };
        constexpr auto expected_result{ "continue;\n" };
        EXPECT_TRUE(gl_continue.declaration == expected_result);
    }

    {
        constexpr auto gl_break{ break_statement() };
        constexpr auto expected_result{ "break;\n" };
        EXPECT_TRUE(gl_break.declaration == expected_result);
    }

    {
        constexpr auto gl_discard{ discard_statement() };
        constexpr auto expected_result{ "discard;\n" };
        EXPECT_TRUE(gl_discard.declaration == expected_result);
    }
}

TEST(Statements, SelectionStatements)
{
    {
        constexpr auto active{ variable<Type::gl_bool, "active", value(true)>() };
        constexpr auto gl_if{ if_statement<active.name>() };
        constexpr auto expected_result{ "if (active) {}\n" };
        EXPECT_TRUE(gl_if.declaration == expected_result);
    }
}

TEST(Statements, IterationStatements)
{
    // for 
    // while
}

TEST(Statements, BuiltInVariables)
{
    {
        constexpr auto var{ variable<Type::gl_float, "test_var", value(1.0f)>() };
        constexpr auto gl_frag_color{ frag_color<var>() };
        constexpr auto expected_result{ "gl_FragColor = test_var;\n" };
        EXPECT_TRUE(gl_frag_color.declaration == expected_result);
    }

    {
        // constexpr auto var{ variable<Type::gl_float, "test_var", value(1.0f)>() };
        // constexpr auto gl_frag_color{ frag_color<var>() };
        // constexpr auto expected_result{ "gl_FragColor = test_var;\n" };
        // EXPECT_TRUE(gl_frag_color.declaration == expected_result);
    }
}
