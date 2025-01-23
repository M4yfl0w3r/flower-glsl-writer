#include <print>

#include "structure.hpp"
#include "variable.hpp"

using namespace mfl;

auto main() -> int 
{
    constexpr auto light{
        make_structure<"Light">(
            field<Type::vec4, "position">(),
            field<Type::vec4, "ambient">(),
            field<Type::vec4, "diffuse">(),
            field<Type::vec3, "spotDirection">(),
            field<Type::float_t, "spotCutoff">(),
            field<Type::float_t, "spotExponent">(),
            field<Type::float_t, "constantAttenuation">(),
            field<Type::float_t, "linearAttenuation">(),
            field<Type::float_t, "quadraticAttenuation">()
        )
    };

    print(light.declaration);
}

