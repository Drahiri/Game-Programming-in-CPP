project(Chapter03 VERSION 1.0 LANGUAGES CXX)


set(SOURCE_FILES
    "src/main.cpp"
    "src/game.cpp"
    "src/actor.cpp"
    "src/component.cpp"
    "src/sprite_component.cpp"
    "src/anim_sprite_component.cpp"
    "src/bg_sprite_component.cpp"
    "src/ship.cpp"

    "src/math.cpp"
)

set(HEADER_FILES
    "include/game.h"
    "include/actor.h"
    "include/component.h"
    "include/sprite_component.h"
    "include/anim_sprite_component.h"
    "include/bg_sprite_component.h"
    "include/ship.h"

    "include/math.h"
)

add_executable(Chapter03 ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(Chapter03 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(Chapter03 PRIVATE SDL3::SDL3 SDL3_image::SDL3_image)

file(CREATE_LINK "${CMAKE_CURRENT_SOURCE_DIR}/assets" "${CMAKE_CURRENT_BINARY_DIR}/assets" SYMBOLIC)

# file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/assets" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")