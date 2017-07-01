
# Функция добавляет пример из курса OpenGL
function(custom_add_ch1_example TARGET)
    custom_add_executable_from_dir(${TARGET})
    
    # Составляем список библиотек
    set(TARGET_EXTERNAL_LIBS SDL2 SDL2_image SDL2_ttf SDL2_mixer GL glbinding assimp)
    set(TARGET_INTERNAL_LIBS libshade libmath libplatform)
    # Составляем список путей поиска заголовочных файлов
    set(TARGET_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/libs)

    target_link_libraries(${TARGET} ${TARGET_EXTERNAL_LIBS} ${TARGET_INTERNAL_LIBS})
    target_include_directories(${TARGET} PRIVATE ${TARGET_INCLUDE_DIRS})
endfunction()

add_subdirectory(01_minimal)
add_subdirectory(02_tesselation)
