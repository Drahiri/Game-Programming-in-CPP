project(Chapter01 VERSION 1.0 LANGUAGES CXX)

set(SOURCE_FILES
    "src/main.cpp"
    "src/game.cpp"
)

set(HEADER_FILES
    "include/game.h"

)

add_executable(Chapter01 ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(Chapter01 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include") #SDL3::Headers)
target_link_libraries(Chapter01 PRIVATE SDL3::SDL3)