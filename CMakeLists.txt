cmake_minimum_required(VERSION 3.15)

project(GameProgramming)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/extern")
    list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/extern")
endif()

find_package(SDL3 REQUIRED CONFIG)
find_package(SDL3_image REQUIRED CONFIG)

find_package(GLEW REQUIRED)

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/extern/SOIL/")
    # SOIL doesn't include cmake files
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/extern/SOIL/include")
    link_directories("${CMAKE_CURRENT_SOURCE_DIR}/extern/SOIL/lib")
endif()

add_subdirectory(Chapter01)
add_subdirectory(Chapter02)
add_subdirectory(Chapter03)
add_subdirectory(Chapter04)
add_subdirectory(Chapter05)