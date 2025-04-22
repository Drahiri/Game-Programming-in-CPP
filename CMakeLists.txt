cmake_minimum_required(VERSION 3.15)

project(GameProgramming)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/extern")

find_package(SDL3 REQUIRED CONFIG)

add_subdirectory(Pong)