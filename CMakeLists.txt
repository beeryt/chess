cmake_minimum_required(VERSION 3.16)
project(main)

find_package(SDL2 REQUIRED)

add_executable(main src/main.cpp src/graphics.cpp)
target_include_directories(main PUBLIC include)
target_link_libraries(main PRIVATE SDL2)

