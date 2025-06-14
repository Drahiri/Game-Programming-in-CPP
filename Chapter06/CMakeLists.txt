project(Chapter06 VERSION 1.0 LANGUAGES CXX)


set(SOURCE_FILES
    "src/actor.cpp"
    "src/circle_component.cpp"
    "src/component.cpp"
    "src/game.cpp"
    "src/input_component.cpp"
    "src/mesh.cpp"
    "src/move_component.cpp"
    "src/renderer.cpp"
    "src/shader.cpp"
    "src/sprite_component.cpp"
    "src/texture.cpp"
    "src/vertex_array.cpp"
    "src/main.cpp"

    "src/math.cpp"
)

set(HEADER_FILES
    "include/actor.h"
    "include/circle_component.h"
    "include/component.h"
    "include/game.h"
    "include/input_component.h"
    "include/mesh.h"
    "include/move_component.h"
    "include/renderer.h"
    "include/shader.h"
    "include/sprite_component.h"
    "include/texture.h"
    "include/vertex_array.h"
    
    "include/math.h"
)

add_executable(Chapter06 ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(Chapter06 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include" "${RAPIDJSON_INCLUDE_DIRS}")
target_link_libraries(Chapter06 PRIVATE SDL3::SDL3 SDL3_image::SDL3_image GLEW::GLEW SOIL)

file(CREATE_LINK "${CMAKE_CURRENT_SOURCE_DIR}/assets" "${CMAKE_CURRENT_BINARY_DIR}/assets" SYMBOLIC)
file(CREATE_LINK "${CMAKE_CURRENT_SOURCE_DIR}/shaders" "${CMAKE_CURRENT_BINARY_DIR}/shaders" SYMBOLIC)