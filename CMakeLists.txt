cmake_minimum_required(VERSION 3.30)
project(RaylibGame C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(raylib REQUIRED)
find_package(tmx REQUIRED)
find_package(flecs REQUIRED)

add_library(tmx-loader STATIC src/tmx-loader.c)
target_include_directories(tmx-loader PUBLIC include)

add_executable(${PROJECT_NAME} src/main.c)
target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(tmx-loader PRIVATE raylib tmx flecs::flecs_static)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib tmx flecs::flecs_static tmx-loader)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(ASSETS_PATH "${CMAKE_SOURCE_DIR}/assets")
else()
    set(ASSETS_PATH "${CMAKE_CURRENT_BINARY_DIR}/assets")
    file(COPY "${CMAKE_SOURCE_DIR}/assets" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE
    "ASSETS_PATH=\"${ASSETS_PATH}/\"")
