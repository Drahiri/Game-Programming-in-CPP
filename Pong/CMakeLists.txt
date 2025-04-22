project(Pong VERSION 1.0 LANGUAGES CXX)

set(SOURCE_FILES
    "src/main.cpp"
)

set(HEADER_FILES

)

add_executable(Pong ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(Pong PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include") #SDL3::Headers)
target_link_libraries(Pong PRIVATE SDL3::SDL3)
set_target_properties(Pong PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build"
)