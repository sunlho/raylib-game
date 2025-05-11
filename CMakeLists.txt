cmake_minimum_required(VERSION 3.30)
project(RaylibGame C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(raylib REQUIRED)
find_package(tmx REQUIRED)
find_package(flecs REQUIRED)
find_package(chipmunk2d REQUIRED)

add_executable(${PROJECT_NAME} src/main.c src/tmx-loader.c)
target_include_directories(${PROJECT_NAME} PRIVATE include src)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib tmx flecs::flecs_static chipmunk2d::chipmunk2d)


if(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
    set(ASSETS_PATH "${CMAKE_SOURCE_DIR}/assets")
else()
    set(ASSETS_PATH "${CMAKE_CURRENT_BINARY_DIR}/assets")
    file(COPY "${CMAKE_SOURCE_DIR}/assets" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE
    ASSETS_PATH="${ASSETS_PATH}/"
)