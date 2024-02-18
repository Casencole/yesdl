cmake_minimum_required(VERSION 3.27)
project(yesdl C)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)

set(CMAKE_C_STANDARD 99)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

add_executable(yesdl main.c
        Map.c
        Map.h
        Avatar.c
        Avatar.h
        assets/PISKEL_versions/Texture.h
        assets/PISKEL_versions/Texture.c
        Texture.c
        Texture.h
)

target_link_libraries(${PROJECT_NAME} ${SDL2_IMAGE_LIBRARY} ${SDL2_LIBRARY})
