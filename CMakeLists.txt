cmake_minimum_required(VERSION 3.30)
project(RaylibGame C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(raylib REQUIRED)
find_package(tmx REQUIRED)

add_library(tmx-loader STATIC src/tmx-loader.c)
target_include_directories(tmx-loader PUBLIC include)

add_executable(${PROJECT_NAME} src/main.c)

target_link_libraries(tmx-loader PRIVATE raylib tmx)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib tmx tmx-loader)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(ASSETS_PATH "${CMAKE_SOURCE_DIR}/assets")
else()
    # 发布版本可以复制资源到输出目录
    set(ASSETS_PATH "${CMAKE_CURRENT_BINARY_DIR}/assets")
    file(COPY "${CMAKE_SOURCE_DIR}/assets" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
endif()

# 将资源路径定义为编译时常量
target_compile_definitions(${PROJECT_NAME} PRIVATE 
    "ASSETS_PATH=\"${ASSETS_PATH}/\"")
