project(Chapter04 VERSION 1.0 LANGUAGES CXX)


set(SOURCE_FILES
    "src/actor.cpp"
    "src/ai_state.cpp"
    "src/bullet.cpp"
    "src/circle_component.cpp"
    "src/component.cpp"
    "src/enemy.cpp"
    "src/game.cpp"
    "src/grid.cpp"
    "src/move_component.cpp"
    "src/sprite_component.cpp"
    "src/tile.cpp"
    "src/tower.cpp"
    
    "src/main.cpp"
    "src/math.cpp"
)

set(HEADER_FILES
    "include/actor.h"
    "include/ai_state.h"
    "include/bullet.h"
    "include/circle_component.h"
    "include/component.h"
    "include/enemy.h"
    "include/game.h"
    "include/grid.h"
    "include/move_component.h"
    "include/sprite_component.h"
    "include/tile.h"
    "include/tower.h"

    "include/math.h"
)

add_executable(Chapter04 ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(Chapter04 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(Chapter04 PRIVATE SDL3::SDL3 SDL3_image::SDL3_image)

file(CREATE_LINK "${CMAKE_CURRENT_SOURCE_DIR}/assets" "${CMAKE_CURRENT_BINARY_DIR}/assets" SYMBOLIC)