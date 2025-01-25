#include <gtest/gtest.h>

#include "static_string.hpp"
#include "statement.hpp"
#include "value.hpp"

using namespace mfl;
using enum mfl::Type;

TEST(Statements, JumpStatements)
{
    {
        static constexpr auto gl_return{ return_statement() };
        static constexpr auto expected_result{ "return;\n" };
        EXPECT_TRUE(gl_return.declaration == expected_result);
    }

    {
        static constexpr auto gl_continue{ continue_statement() };
        static constexpr auto expected_result{ "continue;\n" };
        EXPECT_TRUE(gl_continue.declaration == expected_result);
    }

    {
        static constexpr auto gl_break{ break_statement() };
        static constexpr auto expected_result{ "break;\n" };
        EXPECT_TRUE(gl_break.declaration == expected_result);
    }

    {
        static constexpr auto gl_discard{ discard_statement() };
        static constexpr auto expected_result{ "discard;\n" };
        EXPECT_TRUE(gl_discard.declaration == expected_result);
    }
}

TEST(Statements, SelectionStatements)
{
    {
        static constexpr auto active{ variable<gl_bool, "active">() };
        static constexpr auto gl_if{ if_statement<active, "">() };
        
        static constexpr auto expected_result{ 
            "if (active) {\n"
            "}\n" 
        };

        EXPECT_TRUE(gl_if.declaration == expected_result);
    }

    {
        static constexpr auto var{ variable<gl_float, "var">() };
        static constexpr auto condition{ less_than<var, value(1.0f)>() };
        static constexpr auto body{ return_statement().declaration };
        static constexpr auto gl_if{ if_statement<condition, body>() };
        
        static constexpr auto expected_result{ 
            "if (var < 1.0f) {\n"
            "return;\n"
            "}\n" 
        };

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
        // static constexpr auto var{ variable<Type::gl_float, "test_var", value(1.0f)>() };
        // static constexpr auto gl_frag_color{ frag_color<var>() };
        // static constexpr auto expected_result{ "gl_FragColor = test_var;\n" };
        // EXPECT_TRUE(gl_frag_color.declaration == expected_result);
    }
}

TEST(Statements, DefineStatements)
{
    // The define statement is treated as a variable.
    {
        // static constexpr auto def{ define_statement<"NUM_LIGHTS", value(2)>() };
        // static constexpr auto expected_result{ "#define NUM_LIGHTS 2\n" };
        // EXPECT_TRUE(def.declaration == expected_result);
    }

}
