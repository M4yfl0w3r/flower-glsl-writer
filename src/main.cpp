#include <print>

#include "statement.hpp"
#include "variable.hpp"

using namespace mfl;

auto main() -> int 
{
    constexpr auto var_1{ variable<Type::gl_vec2, "var_1", "test_1">() };
    constexpr auto var_2{ variable<Type::gl_vec2, "var_2", "test_2">() };

    constexpr auto res{ var_1 + var_2 };

    print(res);
}

