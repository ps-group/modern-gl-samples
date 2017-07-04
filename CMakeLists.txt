cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(ps-opengl-samples)

# Инструкция enable_testing неявно объявляет опцию BUILD_TESTING,
#  по умолчанию BUILD_TESTING=ON.
enable_testing()

include(scripts/functions.cmake)

# Включаем анализатор clang-tidy
custom_clang_tidy_checks(
    "cppcoreguidelines-*"
    "clang-analyzer-*"
    "readability-*"
    "modernize-*"
    "-cppcoreguidelines-pro-type-union-access"
    "-cppcoreguidelines-pro-bounds-array-to-pointer-decay"
    "-cppcoreguidelines-pro-type-reinterpret-cast"
    "-cppcoreguidelines-pro-bounds-pointer-arithmetic")

add_subdirectory(libs/libmath)
add_subdirectory(libs/libplatform)
add_subdirectory(libs/libshade)
add_subdirectory(chapter1)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
