cmake_minimum_required(VERSION 3.15)

project(GameProgramming)

set(CMAKE_CXX_STANDARD 11)
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

find_package(RapidJSON REQUIRED)

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/extern/FMOD")
    set(FMOD_API_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extern/FMOD/api")
    include_directories("${FMOD_API_DIR}/core/inc" "${FMOD_API_DIR}/fsbank/inc" "${FMOD_API_DIR}/studio/inc")
    link_directories("${FMOD_API_DIR}/core/lib/x86_64" "${FMOD_API_DIR}/fsbank/lib/x86_64" "${FMOD_API_DIR}/studio/lib/x86_64")
endif()

add_subdirectory(Chapter01)
add_subdirectory(Chapter02)
add_subdirectory(Chapter03)
add_subdirectory(Chapter04)
add_subdirectory(Chapter05)
add_subdirectory(Chapter06)
add_subdirectory(Chapter07)