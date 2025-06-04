project(Chapter05 VERSION 1.0 LANGUAGES CXX)


set(SOURCE_FILES
    "src/main.cpp"
    "src/game.cpp"
    "src/actor.cpp"
    "src/component.cpp"
    "src/sprite_component.cpp"
    "src/anim_sprite_component.cpp"
    "src/bg_sprite_component.cpp"
    "src/move_component.cpp"
    "src/input_component.cpp"
    "src/circle_component.cpp"
    "src/ship.cpp"
    "src/asteroid.cpp"
    "src/laser.cpp"
    "src/vertex_array.cpp"
    
    "src/random.cpp"
    "src/math.cpp"
)

set(HEADER_FILES
    "include/game.h"
    "include/actor.h"
    "include/component.h"
    "include/sprite_component.h"
    "include/anim_sprite_component.h"
    "include/bg_sprite_component.h"
    "include/move_component.h"
    "include/input_component.h"
    "include/circle_component.h"
    "include/ship.h"
    "include/asteroid.h"
    "include/laser.h"
    "include/vertex_array.h"
    
    "include/random.h"
    "include/math.h"
)

add_executable(Chapter05 ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(Chapter05 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(Chapter05 PRIVATE SDL3::SDL3 SDL3_image::SDL3_image GLEW::GLEW)

file(CREATE_LINK "${CMAKE_CURRENT_SOURCE_DIR}/assets" "${CMAKE_CURRENT_BINARY_DIR}/assets" SYMBOLIC)