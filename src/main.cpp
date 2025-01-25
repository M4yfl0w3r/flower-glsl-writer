// #include <print>
//
// #include "statement.hpp"
// #include "variable.hpp"
// #include "function.hpp"
// #include "structure.hpp"
// #include "value.hpp"
//
// using namespace mfl;
//
// template <variable input, uniform gamma>
// consteval auto to_linear_vec4_body() 
// {
//     static constexpr auto lhs{ pow<input.rgb(), vec3<gamma.name>()>() };
//     static constexpr auto rhs{ input.a() };
//     return return_value_statement<vec4<lhs, rhs>()>().declaration;
// }
//
// template <variable input, uniform gamma>
// consteval auto to_linear_vec3_body() 
// {
//     static constexpr auto tmp{ pow<input.name, vec3<gamma.name>()>() };
//     return return_value_statement<tmp>().declaration;
// }
// template <variable input, uniform gamma>
// consteval auto to_gamma_body() 
// {
//     static constexpr auto tmp{ pow<input.name, vec3< value(1.0f / gamma)>()>() }; // TODO: super hacky, will change it
//     return return_value_statement<tmp>().declaration;
// }
//
// // vec4 colorWithAlpha = texture(colorMap, uvTexCoord);
// // if(colorWithAlpha.a < 0.1){
// //     gl_FragColor = colorWithAlpha;
// //     discard;
// // }
// // vec3 color = toLinear(colorWithAlpha).rgb;
//
// template <uniform colormap, in_var tex_coord>
// consteval auto main_body() 
// {
//     static constexpr auto color_with_alpha{ variable<Type::gl_vec4, "colorWithAlpha", sample<colormap, tex_coord>()>() };
//
//     static constexpr auto if_alpha_statement{ if_statement<color_with_alpha.a()>() }; // TODO: should construct a less than expression 
//
//     // static constexpr auto color{ variable<Type::gl_vec3, "color", "">().rgb() }; // TODO: should return string? 
//
//     return concat_all(
//         color_with_alpha,
//         if_alpha_statement
//     );
// }
//
auto main() -> int 
{
//     constexpr auto colormap{ uniform<Type::gl_sampler2D, "colorMap">() };
//     constexpr auto normalmap{ uniform<Type::gl_sampler2D, "normalMap">() };
//     constexpr auto depthmap{ uniform<Type::gl_sampler2D, "depthMap">() };
//
//     constexpr auto ambient{ uniform<Type::gl_float, "ambientStrength">() };
//     constexpr auto diffuse{ uniform<Type::gl_float, "diffuseStrength">() };
//     constexpr auto gamma{ uniform<Type::gl_float, "gamma">() };
//     constexpr auto depth_range{ uniform<Type::gl_float, "depthRange">() };
//
//     constexpr auto fog_color{ uniform<Type::gl_vec3, "fogColor">() };
//     constexpr auto fog_density{ uniform<Type::gl_float, "fogDensity">() };
//
//     constexpr auto tex_coord{ in_var<Type::gl_vec2, "uvTexCoord">() };
//
//     constexpr auto light_struct{ 
//         make_structure<"Light">(
//             field<Type::gl_vec4, "position">(),
//             field<Type::gl_vec4, "ambient">(),
//             field<Type::gl_vec4, "diffuse">(),
//             field<Type::gl_vec3, "spotDirection">(),
//             field<Type::gl_float, "spotCutoff">(),
//             field<Type::gl_float, "spotExponent">(),
//             field<Type::gl_float, "constantAttenuation">(),
//             field<Type::gl_float, "linearAttenuation">(),
//             field<Type::gl_float, "quadraticAttenuation">()
//         )
//     };
//
//     constexpr auto num_lights{ define_statement<"NUM_LIGHTS", value(2)>() };
//     constexpr auto lights{ array<Type::gl_light, "lights", num_lights.value>() };
//
//     // TODO: 
//     // - fn should take params differently
//     // - fn.name to use later 
//     constexpr auto st_fn_in{ variable<Type::gl_vec4, "v", "">() };
//     constexpr auto st_fn_body{ to_linear_vec4_body<st_fn_in, gamma>() };
//     constexpr auto to_linear_vec4{ function<"toLinear", Type::gl_vec4, st_fn_body, Param<st_fn_in.name, st_fn_in.type>>() };
//
//     constexpr auto nd_fn_in{ variable<Type::gl_vec3, "v", "">() };
//     constexpr auto nd_fn_body{ to_linear_vec3_body<nd_fn_in, gamma>() };
//     constexpr auto to_linear_vec3{ function<"toLinear", Type::gl_vec3, nd_fn_body, Param<nd_fn_in.name, nd_fn_in.type>>() };
//
//     constexpr auto rd_fn_in{ variable<Type::gl_vec3, "v", "">() };
//     constexpr auto rd_fn_body{ to_gamma_body<rd_fn_in, gamma>() };
//     constexpr auto to_gamma{ function<"toGamma", Type::gl_vec3, rd_fn_body, Param<rd_fn_in.name, rd_fn_in.type>>() };
//
//     constexpr auto body{ main_body<colormap, tex_coord>() };
//     constexpr auto main_fn_impl{ main_fn<body>() };
//
//     constexpr auto result{
//         concat_all(
//             colormap,
//             normalmap,
//             depthmap,
//             ambient,
//             diffuse, 
//             gamma,
//             depth_range,
//             fog_color,
//             fog_density,
//             tex_coord,
//             light_struct,
//             num_lights,
//             lights,
//             to_linear_vec4,
//             to_linear_vec3,
//             to_gamma, 
//             main_fn_impl
//         )
//     };
//
//     print(result);
}

