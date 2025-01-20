set_project("test")
set_languages("c++26")

add_rules("mode.debug", "mode.release")
set_warnings("all")

add_requires("glm", "gtest")

target("test")
    add_files("src/*.cpp")
    add_packages("glm")
