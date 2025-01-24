#include <print>

#include "statement.hpp"
#include "variable.hpp"
#include "function.hpp"
#include "structure.hpp"

using namespace mfl;

auto main() -> int 
{
    constexpr auto colormap{ uniform<Type::gl_sampler2D, "colorMap">() };
    constexpr auto normalmap{ uniform<Type::gl_sampler2D, "normalMap">() };
    constexpr auto depthmap{ uniform<Type::gl_sampler2D, "depthMap">() };

    constexpr auto ambient_strength{ uniform<Type::gl_float, "ambientStrength">() };
    constexpr auto diffuse_strength{ uniform<Type::gl_float, "diffuseStrength">() };
    constexpr auto gamma{ uniform<Type::gl_float, "gamma">() };
    constexpr auto depth_range{ uniform<Type::gl_float, "depthRange">() };

    constexpr auto fog_color{ uniform<Type::gl_vec3, "fogColor">() };
    constexpr auto fog_density{ uniform<Type::gl_float, "fogDensity">() };

    constexpr auto tex_coord{ in_var<Type::gl_vec2, "uvTexCoord">() };

    constexpr auto light_struct{ 
        make_structure<"Light">(
            field<Type::gl_vec4, "position">(),
            field<Type::gl_vec4, "ambient">(),
            field<Type::gl_vec4, "diffuse">(),
            field<Type::gl_vec3, "spotDirection">(),
            field<Type::gl_float, "spotCutoff">(),
            field<Type::gl_float, "spotExponent">(),
            field<Type::gl_float, "constantAttenuation">(),
            field<Type::gl_float, "linearAttenuation">(),
            field<Type::gl_float, "quadraticAttenuation">()
        )
    };

    constexpr auto num_lights{ define_statement<"NUM_LIGHTS", value(2)>() };
    constexpr auto lights{ array<Type::gl_light, "lights", num_lights.value>() };

    constexpr auto result{
        concat_all(
            colormap,
            normalmap,
            depthmap,
            ambient_strength,
            diffuse_strength, 
            gamma,
            depth_range,
            fog_color,
            fog_density,
            tex_coord,
            light_struct,
            num_lights,
            lights
        )
    };

    print(result);
}

