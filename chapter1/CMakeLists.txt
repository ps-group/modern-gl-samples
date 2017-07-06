
# Функция добавляет пример из курса OpenGL
function(custom_add_ch1_example TARGET DIR)
    custom_add_executable_from_dirs(${TARGET} ${DIR})
    
    # Составляем список библиотек
    #set(TARGET_EXTERNAL_LIBS SDL2 SDL2_image SDL2_ttf SDL2_mixer GL glbinding assimp)
    set(TARGET_EXTERNAL_LIBS glbinding sfml-graphics-s sfml-window-s sfml-system-s -lXrandr -lX11 -lGL -ljpeg -ludev)
    #set(TARGET_INTERNAL_LIBS libshade libmath libplatform)
    # Составляем список путей поиска заголовочных файлов
    set(TARGET_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/libs)

    target_compile_options(${TARGET} PRIVATE "-fsanitize=address" "-fsanitize=undefined")
    target_link_libraries(${TARGET} ${TARGET_EXTERNAL_LIBS} ${TARGET_INTERNAL_LIBS} "-fsanitize=address" "-fsanitize=undefined")
    target_include_directories(${TARGET} PRIVATE ${TARGET_INCLUDE_DIRS})

    custom_add_sanitizers(${TARGET})
endfunction()

custom_add_ch1_example(01 "01")
custom_add_ch1_example(02 "02")
