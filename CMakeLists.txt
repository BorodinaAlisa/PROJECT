cmake_minimum_required(VERSION 3.10)
project(project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Путь к SFML
# set(SFML_DIR "C:/project/SFML-2.5.1-MSVC")
set(SFML_DIR "C:/project/SFML-2.6.0-dll")

# Подключаем заголовочные файлы
include_directories("${SFML_DIR}/include")

# Линкуем SFML
link_directories("${SFML_DIR}/lib")

add_executable(project src/main.cpp)

target_link_libraries(project
    "${SFML_DIR}/lib/sfml-graphics.lib"
    "${SFML_DIR}/lib/sfml-window.lib"
    "${SFML_DIR}/lib/sfml-system.lib"
)

# Копируем DLL после сборки
if(WIN32 AND NOT SFML_STATIC_LIBRARIES)
    add_custom_command(TARGET project POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SFML_DIR}/bin/sfml-graphics-2.dll"
            "${SFML_DIR}/bin/sfml-window-2.dll"
            "${SFML_DIR}/bin/sfml-system-2.dll"
            $<TARGET_FILE_DIR:project>
    )
endif()