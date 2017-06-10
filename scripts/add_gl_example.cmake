cmake_minimum_required(VERSION 3.8 FATAL)

# В текущей версии CMake не может включить режим C++17 в некоторых компиляторах.
# Функция использует обходной манёвр.
function(custom_enable_cxx17 TARGET)
    # Включае C++17 везде, где CMake может.
	target_compile_features(${TARGET} PUBLIC cxx_std_17)
    # Включаем режим C++latest в Visual Studio
	if (MSVC)
		set_target_properties(${TARGET} PROPERTIES COMPILE_FLAGS "/std:c++latest")
	endif()
    # Добавляем компоновку с экспериментальной libc++
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_link_libraries(${TARGET} c++experimental)
    endif()
endfunction(custom_enable_cxx17)

# Функция добавляет пример из курса OpenGL
function(add_gl_example TARGET)
    # Собираем файлы с текущего каталога
    file(GLOB TARGET_SRC "*.cpp" "*.h")
    # Составляем список библиотек
    set(TARGET_LIBS SDL2 SDL2_image SDL2_ttf SDL2_mixer GL glbinding assimp)
    # Составляем список путей поиска заголовочных файлов
    set(TARGET_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../libs)

    add_executable(${TARGET} ${TARGET_SRC})
    custom_enable_cxx17(${TARGET})
    target_link_libraries(${TARGET} ${TARGET_LIBS})
    target_include_directories(${TARGET ${TARGET_INCLUDE_DIRS}})
endfunction()
