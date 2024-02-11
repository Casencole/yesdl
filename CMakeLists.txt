cmake_minimum_required(VERSION 3.27)
project(yesdl C)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)

set(CMAKE_C_STANDARD 99)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

add_executable(yesdl main.c
        Map.c
        Map.h)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})
