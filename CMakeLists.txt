cmake_minimum_required(VERSION 3.16)
project(main)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

add_executable(main
  src/graphics.cpp
  src/texture.cpp
  src/sprite.cpp
  src/chess.cpp
  src/main.cpp
)
target_include_directories(main PUBLIC include)
target_include_directories(main PRIVATE ${SDL2_IMAGE_INCLUDE_DIRS})
target_link_libraries(main PRIVATE SDL2 ${SDL2_IMAGE_LIBRARIES})

