cmake_minimum_required(VERSION 3.21)
project(chess_engine CXX)

# Set C++ standard (using C++17 as a modern baseline)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define source files
set(SRC
    src/logic.cpp
    src/moveEvaluator.cpp
    src/main.cpp
    src/Renderer.cpp
    src/engine.cpp 
    src/evaluate.cpp
    src/gamemanager.cpp
)

# Create the executable
add_executable(chess_engine ${SRC})

# Include directories (-I)
target_include_directories(chess_engine PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include 
    C:/raylib/raylib/src
)

# Link directories (-L)
target_link_directories(chess_engine PRIVATE 
    C:/raylib/raylib/src
)

# Libraries to link (-l)
target_link_libraries(chess_engine PRIVATE 
    raylib 
    opengl32 
    gdi32 
    winmm
)

# Copy assets (models and shaders) to the executable's output directory
add_custom_command(TARGET chess_engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/models
        $<TARGET_FILE_DIR:chess_engine>/models
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders
        $<TARGET_FILE_DIR:chess_engine>/shaders
    COMMENT "Copying assets (models and shaders) to the output directory..."
)