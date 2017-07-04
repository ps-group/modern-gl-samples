cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

# Макрос включает статическую компоновку C++ Runtime при сборке с Visual Studio.
macro(custom_use_static_msvc_runtime)
    if(MSVC)
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
        set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /MT")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
        set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MT")
    endif(MSVC)
endmacro()

# В текущей версии CMake не может включить режим C++17 в некоторых компиляторах.
# Функция использует обходной манёвр.
function(custom_enable_cxx17 TARGET)
    # Включаем C++17 везде, где CMake может.
	target_compile_features(${TARGET} PUBLIC cxx_std_17)
    # Включаем режим C++latest в Visual Studio
	if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
		set_target_properties(${TARGET} PROPERTIES COMPILE_FLAGS "/std:c++latest")
    # Включаем компоновку с libc++, libc++experimental и pthread для Clang
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
		set_target_properties(${TARGET} PROPERTIES COMPILE_FLAGS "-stdlib=libc++ -pthread")
        target_link_libraries(${TARGET} c++experimental pthread)
    endif()
endfunction(custom_enable_cxx17)

function(custom_clang_tidy_checks)
    set(CLANG_TIDY_OPTIONS "clang-tidy;-checks=")
    set(SEPARATOR "")
    foreach(CHECK ${ARGN})
        set(CLANG_TIDY_OPTIONS "${CLANG_TIDY_OPTIONS}${SEPARATOR}${CHECK}")
        set(SEPARATOR ",")
    endforeach()
    set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_OPTIONS} PARENT_SCOPE)
endfunction(custom_clang_tidy_checks)

# Функция добавляет вспомогательную библиотеку из курса OpenGL.
# Использование: custom_add_library_from_dirs(myapp . ./detail ./helpers)
function(custom_add_library_from_dirs TARGET)
    # Добавляем цель - библиотеку
    # Собираем файлы с указанных каталогов
    foreach(DIR ${ARGN})
        file(GLOB TARGET_SRC_PART "${CMAKE_CURRENT_SOURCE_DIR}/${DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/${DIR}/*.h")
        list(APPEND TARGET_SRC ${TARGET_SRC_PART})
    endforeach()
    add_library(${TARGET} ${TARGET_SRC})
    custom_enable_cxx17(${TARGET})
endfunction()

# Функция добавляет пример из курса OpenGL
function(custom_add_executable_from_dir TARGET)
    # Собираем файлы с текущего каталога
    file(GLOB TARGET_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
    # Добавляем цель - исполняемый файл
    add_executable(${TARGET} ${TARGET_SRC})
    custom_enable_cxx17(${TARGET})
endfunction()
