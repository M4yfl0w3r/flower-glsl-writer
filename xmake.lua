set_project("flower-glsl-writer")
set_languages("c++26")

add_rules("mode.debug", "mode.release")
set_warnings("all", "error")

add_requires("glm", "gtest")

target("app")
    add_files("src/*.cpp")
    add_packages("glm")

target("test")
    set_kind("binary")
    add_files("test/*.cpp")
    add_includedirs("src")
    add_packages("glm", "gtest")
