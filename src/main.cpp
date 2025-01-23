#include <print>

#include "variable.hpp"
#include "function.hpp"
#include "symbols.hpp"

using namespace mfl;

auto main() -> int 
{
    constexpr auto colormap{ uniform<Type::sampler2D, "colorMap">() };
    constexpr auto res{ sample<colormap, static_string{ "test" }>() };
    print(res);
}
