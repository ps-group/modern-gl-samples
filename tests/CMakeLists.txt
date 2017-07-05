
function(custom_add_library_test TARGET LIBRARY)
    custom_add_executable_from_dirs(${TARGET} .)
    # Добавляем путь к заголовку Catch
    target_include_directories(${TARGET} PRIVATE ${CMAKE_SOURCE_DIR}/libs ${CMAKE_SOURCE_DIR}/libs/catch)
    # Добавляем компоновку с проверяемой библиотекой
    target_link_libraries(${TARGET} ${LIBRARY})
    # Регистрируем исполняемый файл в CMake как набор тестов.
    add_test(${TARGET} ${TARGET})
endfunction()

add_subdirectory(libmathtests)
