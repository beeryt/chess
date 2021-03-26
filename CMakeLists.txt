cmake_minimum_required(VERSION 3.16)
project(Chess
  VERSION 0.1.0
  DESCRIPTION "A chess program inspired by <a href='https://youtu.be/U4ogK0MIzqk'>Coding Adventure: Chess AI</a>")
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_executable(main
  src/graphics.cpp
  src/texture.cpp
  src/sprite.cpp
  src/chess.cpp
  src/main.cpp
)
target_include_directories(main PUBLIC include)
target_include_directories(main PRIVATE ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})
target_link_libraries(main PRIVATE SDL2 ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})

option(BUILD_DOC "Build documentation" OFF)
find_package(Doxygen)
if (BUILD_DOC AND DOXYGEN_FOUND)
  # Configure Doxyfile
  set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
  set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
  configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

  add_custom_target(docs ALL
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating documentation with Doxygen"
    VERBATIM)
elseif(BUILD_DOC)
  message("Doxygen needs to be installed to generate the documentation")
endif()
