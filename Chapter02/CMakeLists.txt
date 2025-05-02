project(Chapter02 VERSION 1.0 LANGUAGES CXX)

set(SOURCE_FILES
    "src/main.cpp"
    "src/game.cpp"
    "src/actor.cpp"
    "src/component.cpp"
    "src/sprite_component.cpp"
    "src/anim_sprite_component.cpp"
    "src/bg_sprite_component.cpp"

    "src/math.cpp"
)

set(HEADER_FILES
    "include/game.h"
    "include/actor.h"
    "include/component.h"
    "include/sprite_component.h"
    "include/anim_sprite_component.h"
    "include/bg_sprite_component.h"

    "include/math.h"
)

add_executable(Chapter02 ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(Chapter02 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include") #SDL3::Headers)
target_link_libraries(Chapter02 PRIVATE SDL3::SDL3 SDL3_image::SDL3_image)
set_target_properties(Chapter02 PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build"
)