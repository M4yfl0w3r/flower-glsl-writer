GLSL writer is a library for generating GLSL shaders at compile time.

It is not a GLSL compiler and does not prevent you from writing incorrect GLSL code. It does not check types or function parameters. The library provides GLSL expressions represented as strings and allows you to concatenate them.

Example 

```c++
template <out_var tex_coord>
static consteval auto main_body()
{
    return concat(
        gl_position<gl_model_view_proj_matrix * gl_vertex>{}.declaration,
        gl_tex_coord.assign_at<gl_multi_tex_coord_0, 0>(),
        tex_coord.template assign<gl_tex_coord.member_access_at<"st", 0>()>()
    );
}

static constexpr auto version{ set_version<"130">() };
static constexpr auto tex_coord{ out_var<gl_vec2, "uvTexCoord">{} };
static constexpr auto body{ main_body<tex_coord>() };
static constexpr auto main_fn_impl{ main_fn<body>() };

static constexpr auto result{
    concat(
        version,
        tex_coord.declaration,
        main_fn_impl.declaration
    )
};
```

translates to

```c++
"#version 130\n"
"out vec2 uvTexCoord;\n"
"void main() {\n"
"gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
"gl_TexCoord[0] = gl_MultiTexCoord0;\n"
"uvTexCoord = gl_TexCoord[0].st;\n"
"}\n"
```


TODO

* a struct of all declared variables to return lvalues?
* struct uniforms
* lvalue arrays - structure class - struct + array
* custom type registration (Light)
* type system validation
* a compile-time float to string converter
* a simpler definition of a function
* check if an expression is passable to a function
* named template arguments
* Param should take a variable as an argument
