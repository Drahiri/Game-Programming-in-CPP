project(Exercise-4.2 VERSION 1.0 LANGUAGES CXX)


set(SOURCE_FILES
    "src/Actor.cpp"
    "src/Board.cpp"
    "src/CircleComponent.cpp"
    "src/Component.cpp"
    "src/Game.cpp"
    "src/MoveComponent.cpp"
    "src/Random.cpp"
    "src/SpriteComponent.cpp"
    
    "src/Main.cpp"
    "src/Math.cpp"
)

set(HEADER_FILES
    "include/Actor.h"
    "include/Board.h"
    "include/CircleComponent.h"
    "include/Component.h"
    "include/Game.h"
    "include/MoveComponent.h"
    "include/Random.h"
    "include/SpriteComponent.h"
    
    "include/Math.h"
)

add_executable(Exercise-4.2 ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(Exercise-4.2 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(Exercise-4.2 PRIVATE SDL3::SDL3 SDL3_image::SDL3_image)

file(CREATE_LINK "${CMAKE_CURRENT_SOURCE_DIR}/Assets" "${CMAKE_CURRENT_BINARY_DIR}/Assets" SYMBOLIC)