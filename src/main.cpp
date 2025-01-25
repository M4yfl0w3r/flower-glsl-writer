#include <print>

#include "statement.hpp"
#include "variable.hpp"
#include "function.hpp"
#include "structure.hpp"
#include "value.hpp"

using namespace mfl;
using enum mfl::Type;

template <variable input, uniform gamma>
consteval auto to_linear_vec4_body() 
{
    static constexpr auto lhs{ pow<input.rgb(), vec3<gamma.name>()>() };
    static constexpr auto rhs{ input.a() };
    return return_value_statement<vec4<lhs, rhs>()>().declaration;
}

template <variable input, uniform gamma>
consteval auto to_linear_vec3_body() 
{
    static constexpr auto tmp{ pow<input.name, vec3<gamma.name>()>() };
    return return_value_statement<tmp>().declaration;
}

template <variable input, uniform gamma>
consteval auto to_gamma_body() 
{
    static constexpr auto tmp{ pow<input.name, vec3< value(1.0f / gamma)>()>() }; // TODO: super hacky, will change it
    return return_value_statement<tmp>().declaration;
}

template <variable color_with_alpha>
consteval auto color_if_body() {
    return concat_all(
        frag_color<color_with_alpha>(), 
        discard_statement()
    );
}

template <uniform colormap, uniform normalmap, in_var tex_coord, function to_linear_vec3>
consteval auto main_body() 
{
    static constexpr auto color_with_alpha{ variable<gl_vec4, "colorWithAlpha", sample<colormap, tex_coord>()>() };

    static constexpr auto if_alpha_statement{ 
        if_statement<
            less_than<color_with_alpha.a(), value(0.1f)>(), 
            color_if_body<color_with_alpha>()>() 
    };

    static constexpr auto fn_call{ to_linear_vec3.template call<color_with_alpha>() }; // TODO: might change it into a free fn
    static constexpr auto color{ variable<gl_vec3, "color", fn_call>() }; // TODO: rgb
    
    static constexpr auto normal{ variable<gl_vec3, "normal", sample<normalmap, tex_coord>()>() }; // TODO: rgb
    
    static constexpr auto normalize_op{ 
        normal.template assign<
            normalize<
                normal.template multiply<value(2.0f)>() - value(1.0f)
            >()
        >() 
    };


    // TODO: concat_all should accept both with declaration and without
    return concat(
        color_with_alpha.declaration,
        if_alpha_statement.declaration,
        color.declaration,
        normal.declaration,
        normalize_op
    );
}

auto main() -> int 
{
    static constexpr auto colormap{ uniform<gl_sampler2D, "colorMap">() };
    static constexpr auto normalmap{ uniform<gl_sampler2D, "normalMap">() };
    static constexpr auto depthmap{ uniform<gl_sampler2D, "depthMap">() };

    static constexpr auto ambient{ uniform<gl_float, "ambientStrength">() };
    static constexpr auto diffuse{ uniform<gl_float, "diffuseStrength">() };
    static constexpr auto gamma{ uniform<gl_float, "gamma">() };
    static constexpr auto depth_range{ uniform<gl_float, "depthRange">() };

    static constexpr auto fog_color{ uniform<gl_vec3, "fogColor">() };
    static constexpr auto fog_density{ uniform<gl_float, "fogDensity">() };

    static constexpr auto tex_coord{ in_var<gl_vec2, "uvTexCoord">() };


    static constexpr auto light_struct{ 
        make_structure<"Light">(
            field<gl_vec4, "position">(),
            field<gl_vec4, "ambient">(),
            field<gl_vec4, "diffuse">(),
            field<gl_vec3, "spotDirection">(),
            field<gl_float, "spotCutoff">(),
            field<gl_float, "spotExponent">(),
            field<gl_float, "constantAttenuation">(),
            field<gl_float, "linearAttenuation">(),
            field<gl_float, "quadraticAttenuation">()
        )
    };

    static constexpr auto num_lights{ define_statement<"NUM_LIGHTS", value(2)>() };
    static constexpr auto lights{ array<gl_light, "lights", num_lights.value>() };

//     // TODO: 
//     // - fn should take params differently
//     // - fn.name to use later 
    static constexpr auto st_fn_in{ variable<gl_vec4, "v", "">() };
    static constexpr auto st_fn_body{ to_linear_vec4_body<st_fn_in, gamma>() };
    static constexpr auto to_linear_vec4{ function<"toLinear", gl_vec4, st_fn_body, Param<st_fn_in.name, st_fn_in.type>>() };

    static constexpr auto nd_fn_in{ variable<gl_vec3, "v", "">() };
    static constexpr auto nd_fn_body{ to_linear_vec3_body<nd_fn_in, gamma>() };
    static constexpr auto to_linear_vec3{ function<"toLinear", gl_vec3, nd_fn_body, Param<nd_fn_in.name, nd_fn_in.type>>() };

    static constexpr auto rd_fn_in{ variable<gl_vec3, "v", "">() };
    static constexpr auto rd_fn_body{ to_gamma_body<rd_fn_in, gamma>() };
    static constexpr auto to_gamma{ function<"toGamma", gl_vec3, rd_fn_body, Param<rd_fn_in.name, rd_fn_in.type>>() };

    static constexpr auto body{ main_body<colormap, normalmap, tex_coord, to_linear_vec3>() };
    static constexpr auto main_fn_impl{ main_fn<body>() };

    static constexpr auto result{
        concat_all(
            colormap,
            normalmap,
            depthmap,
            ambient,
            diffuse, 
            gamma,
            depth_range,
            fog_color,
            fog_density,
            tex_coord,
            light_struct,
            num_lights,
            lights,
            to_linear_vec4,
            to_linear_vec3,
            to_gamma, 
            main_fn_impl
        )
    };

    print(result);
}

