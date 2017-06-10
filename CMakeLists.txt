cmake_minimum_required(VERSION 3.8 FATAL)

# Заставим MSVC использовать static версию C++ runtime вместо DLL
include(cmake/use-msvc-static-runtime.cmake)

include(cmake/add_gl_example.cmake)

project(ps-opengl-samples)

add_subdirectory(libs/libmath)
add_subdirectory(libs/libplatform)
add_subdirectory(libs/libshade)
