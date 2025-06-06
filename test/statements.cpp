#include <gtest/gtest.h>

#include "static_string.hpp"
#include "statement.hpp"
#include "function.hpp"
#include "value.hpp"

using namespace mfl;
using enum mfl::Type;

TEST(Statements, jump_statements)
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

TEST(Statements, selection_statements)
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
        static constexpr auto condition{ less_than<var, 1.0f>() };
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

TEST(Statements, iteration_statements)
{
    {
        static constexpr auto init{ variable<gl_int, "i", 0>() };
        static constexpr auto condition{ less_than<init, 5>() };
        static constexpr auto increment{ init.increment() };
        static constexpr auto gl_for{ for_loop<init, condition, increment, "">() };

        static constexpr auto expected_result{ 
            "for (int i = 0; i < 5; ++i) {\n" 
            "}\n"
        }; 

        EXPECT_TRUE(gl_for.declaration == expected_result);
    }

    {
        static constexpr auto init{ variable<gl_int, "i", 0>() };
        static constexpr auto condition{ less_than<init, 5>() };
        static constexpr auto increment{ init.increment() };

        static constexpr auto for_loop_body{
            [](auto i) {
                static constexpr auto test_var{ variable<gl_int, "test", i.name>() };
                return test_var.declaration;
            }(init)
        };

        static constexpr auto gl_for{ for_loop<init, condition, increment, for_loop_body>() };

        static constexpr auto expected_result{ 
            "for (int i = 0; i < 5; ++i) {\n" 
            "int test = i;\n"
            "}\n"
        }; 

        EXPECT_TRUE(gl_for.declaration == expected_result);
    }
}

TEST(Statements, define_statement)
{
    // The define statement is treated as a variable.
    {
        static constexpr auto def{ define<"NUM_LIGHTS", 2>() };
        static constexpr auto expected_result{ "#define NUM_LIGHTS 2\n" };
        EXPECT_TRUE(def.declaration == expected_result);
    }

}
