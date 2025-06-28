project(Chapter08 VERSION 1.0 LANGUAGES CXX)


set(SOURCE_FILES
    "src/actor.cpp"
    "src/asteroid.cpp"
    "src/circle_component.cpp"
    "src/component.cpp"
    "src/game.cpp"
    "src/input_component.cpp"
    "src/laser.cpp"
    "src/main.cpp"
    "src/move_component.cpp"
    "src/ship.cpp"
    "src/shader.cpp"
    "src/sprite_component.cpp"
    "src/texture.cpp"
    "src/vertex_array.cpp"
    
    "src/random.cpp"
    "src/math.cpp"
)

set(HEADER_FILES
    "include/actor.h"
    "include/asteroid.h"
    "include/component.h"
    "include/circle_component.h"
    "include/game.h"
    "include/input_component.h"
    "include/laser.h"
    "include/move_component.h"
    "include/ship.h"
    "include/shader.h"
    "include/sprite_component.h"
    "include/texture.h"
    "include/vertex_array.h"
    
    "include/random.h"
    "include/math.h"
)

add_executable(Chapter08 ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(Chapter08 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(Chapter08 PRIVATE SDL3::SDL3 SDL3_image::SDL3_image GLEW::GLEW SOIL)

file(CREATE_LINK "${CMAKE_CURRENT_SOURCE_DIR}/assets" "${CMAKE_CURRENT_BINARY_DIR}/assets" SYMBOLIC)
file(CREATE_LINK "${CMAKE_CURRENT_SOURCE_DIR}/shaders" "${CMAKE_CURRENT_BINARY_DIR}/shaders" SYMBOLIC)