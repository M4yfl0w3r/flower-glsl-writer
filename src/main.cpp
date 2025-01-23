#include <print>

#include "statement.hpp"
#include "variable.hpp"

using namespace mfl;

auto main() -> int 
{
    constexpr auto if_color{ if_statement<"test">() };
    print(if_color.declaration);
}

