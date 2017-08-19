
# Функция добавляет пример из курса OpenGL
function(custom_add_ch1_example TARGET MAIN_DIR)
    # Добавляем цель - исполняемый файл
    custom_add_executable_from_dirs(${TARGET} ${MAIN_DIR} ${ARGN})
    # Добавляем к цели шаг копирования ресурсов
    custom_distribute_assets("res/${MAIN_DIR}" "res/${MAIN_DIR}" ${TARGET})
    
    # Составляем список библиотек
    set(SFML_LIBS sfml-graphics-s sfml-window-s sfml-system-s -lXrandr -lX11 -lGL -ljpeg -ludev)
    set(TARGET_EXTERNAL_LIBS glbinding "${SFML_LIBS}")
    set(TARGET_INTERNAL_LIBS libmath libplatform)
    # Составляем список путей поиска заголовочных файлов
    set(TARGET_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/libs)

    target_link_libraries(${TARGET} ${TARGET_EXTERNAL_LIBS} ${TARGET_INTERNAL_LIBS})
    target_include_directories(${TARGET} PRIVATE ${TARGET_INCLUDE_DIRS})

    custom_add_sanitizers(${TARGET})
endfunction()

custom_add_ch1_example(01 "01")
custom_add_ch1_example(02 "02")
custom_add_ch1_example(03 "03")
custom_add_ch1_example(04 "04" "shared")
