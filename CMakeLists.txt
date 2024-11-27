cmake_minimum_required(VERSION 3.20)
project(2DGameDev)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Add the SFML include directory
include_directories(/opt/homebrew/Cellar/sfml/2.6.2/include)
link_directories(/opt/homebrew/Cellar/sfml/2.6.2/lib)

# Find all source files
file(GLOB_RECURSE SOURCES src/*.cpp)

# Add executable
add_executable(2DGameDev ${SOURCES}
        src/objects/obstacles.h
        src/objects/obstacles.cpp
        src/player/character.h
        src/player/character.cpp
        src/player/camera.h
        src/player/camera.cpp)

# Link SFML libraries
target_link_libraries(2DGameDev
        sfml-graphics
        sfml-window
        sfml-system
)


