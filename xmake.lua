set_project("flower-glsl-writer")
set_languages("c++26")

add_rules("mode.debug", "mode.release")
set_warnings("all", "error")

add_requires("glm", "gtest", "magic_enum")

target("app")
    add_files("src/*.cpp")
    add_packages("glm", "magic_enum")

target("test")
    set_kind("binary")
    add_files("test/*.cpp")
    add_includedirs("src")
    add_packages("glm", "gtest", "magic_enum")
