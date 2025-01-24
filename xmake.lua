set_project("flower-glsl-writer")
set_languages("c++26")

add_rules("mode.debug", "mode.release")
set_warnings("all", "error")

add_requires("gtest")

target("app")
    set_kind("binary")
    add_files("src/*.cpp")

target("test")
    set_kind("binary")
    add_files("test/*.cpp")
    add_includedirs("src")
    add_packages("gtest")
