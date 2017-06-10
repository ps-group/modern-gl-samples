cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

# Заставим MSVC использовать static версию C++ runtime вместо DLL
include(scripts/use-msvc-static-runtime.cmake)

include(scripts/functions.cmake)

project(ps-opengl-samples)

add_subdirectory(libs/libmath)
add_subdirectory(libs/libplatform)
add_subdirectory(libs/libshade)
