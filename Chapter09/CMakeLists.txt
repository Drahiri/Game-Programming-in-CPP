project(Chapter09 VERSION 1.0 LANGUAGES CXX)


set(SOURCE_FILES
    "src/actor.cpp"
    "src/audio_component.cpp"
    "src/audio_system.cpp"
    "src/camera_actor.cpp"
    "src/circle_component.cpp"
    "src/fps_actor.cpp"
    "src/component.cpp"
    "src/game.cpp"
    "src/mesh_component.cpp"
    "src/mesh.cpp"
    "src/move_component.cpp"
    "src/plane_actor.cpp"
    "src/renderer.cpp"
    "src/shader.cpp"
    "src/sound_event.cpp"
    "src/sprite_component.cpp"
    "src/texture.cpp"
    "src/vertex_array.cpp"
    "src/main.cpp"

    "src/math.cpp"
)

set(HEADER_FILES
    "include/actor.h"
    "include/audio_component.h"
    "include/audio_system.h"
    "include/camera_actor.h"
    "include/circle_component.h"
    "include/component.h"
    "include/fps_actor.h"
    "include/game.h"
    "include/mesh_component.h"
    "include/mesh.h"
    "include/move_component.h"
    "include/plane_actor.h"
    "include/renderer.h"
    "include/shader.h"
    "include/sound_event.h"
    "include/sprite_component.h"
    "include/texture.h"
    "include/vertex_array.h"
    
    "include/math.h"
)

add_executable(Chapter09 ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(Chapter09 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include" "${RAPIDJSON_INCLUDE_DIRS}")
target_link_libraries(Chapter09 PRIVATE SDL3::SDL3 SDL3_image::SDL3_image GLEW::GLEW SOIL fmod fmodstudio)

file(CREATE_LINK "${CMAKE_CURRENT_SOURCE_DIR}/assets" "${CMAKE_CURRENT_BINARY_DIR}/assets" SYMBOLIC)
file(CREATE_LINK "${CMAKE_CURRENT_SOURCE_DIR}/shaders" "${CMAKE_CURRENT_BINARY_DIR}/shaders" SYMBOLIC)