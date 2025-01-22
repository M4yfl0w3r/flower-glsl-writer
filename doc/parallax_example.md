```c++
constexpr auto deflection{ uniform<Type::vec2, "deflection">() };
constexpr auto colormap{ uniform<Type::sampler2D, "colorMap">() };
constexpr auto depthmap{ uniform<Type::sampler2D, "depthMap">() };

constexpr auto tex_coord{ in_var<Type::vec2, "uvTexCoord">() };

template <uniform depthmap, uniform colormap, in_var tex_coord, uniform deflection>
consteval auto main_body()
{
    constexpr auto depth_distortion{ 
        variable<Type::vec4, "depth_distortion", sample<depthmap, tex_coord>()>()
    };

    constexpr auto parallax_multiplier{ 
        variable<Type::float_t, "parallax_multiplier", value(depth_distortion.r)>() 
    };

    constexpr auto parallax{ 
        variable<Type::vec2, "parallax", multiply<deflection, parallax_multiplier>()>() 
    };

    constexpr auto original{ 
        variable<Type::vec4, "original", sample<colormap, add<tex_coord, parallax>()>()>() 
    };

    constexpr auto ret{ 
        frag_color<original.name>() 
    };
    
    return concat_all(
        depth_distortion,
        parallax_multiplier,
        parallax,
        original,
        ret 
    );
}

constexpr auto body{ main_body<depthmap, colormap, tex_coord, deflection>() };
constexpr auto main_fn_impl{ main_fn<body>() };

constexpr auto result {
    concat_all(
        deflection,
        colormap, 
        depthmap, 
        tex_coord,
        main_fn_impl
    )
};
```

```glsl
uniform vec2 deflection;
uniform sampler2D colorMap;
uniform sampler2D depthMap;

in vec2 uvTexCoord;

void main() {
    vec4 depth_distortion = texture2D(depthMap, uvTexCoord);
    float parallax_multiplier = depth_distortion.r;
    vec2 parallax = deflection * parallax_multiplier;
    vec4 original = texture2D(colorMap, uvTexCoord + parallax);
    gl_FragColor = original;
}
```

