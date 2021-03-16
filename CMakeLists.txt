cmake_minimum_required(VERSION 3.16)
project(fluid)

find_package(SDL2 REQUIRED)

add_executable(fluid src/main.cpp src/graphics.cpp)
target_include_directories(fluid PUBLIC include)
target_link_libraries(fluid PUBLIC SDL2)

