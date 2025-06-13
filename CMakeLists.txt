cmake_minimum_required(VERSION 3.10)
project(GameOfLife)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# SFML
find_package(SFML 2 COMPONENTS graphics window system REQUIRED)

# Основное приложение
add_executable(GameOfLife
    src/main.cpp
    src/GameOfLifeCore.cpp
    src/GameOfLifeRenderer.cpp
)

target_include_directories(GameOfLife PRIVATE include)
target_link_libraries(GameOfLife PRIVATE sfml-graphics sfml-window sfml-system)

# Тестирование
option(BUILD_TESTS "Build unit tests" ON)

if(BUILD_TESTS)
    enable_testing()
    
    # Более современный способ подключения GTest
    find_package(GTest REQUIRED)
    
    add_executable(runUnitTests
        tests/GameOfLifeCoreTest.cpp
        src/GameOfLifeCore.cpp
    )

    target_include_directories(runUnitTests PRIVATE include)
    
    # Современный способ линковки GTest
    target_link_libraries(runUnitTests PRIVATE 
        GTest::GTest 
        GTest::Main 
        pthread 
        sfml-graphics sfml-window sfml-system
    )
    
    add_test(NAME GameOfLifeCoreTest COMMAND runUnitTests)
endif()