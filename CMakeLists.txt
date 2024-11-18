cmake_minimum_required(VERSION 3.20)
project(GameDev)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Add the SFML include directory
include_directories(SFML)  # Use system-wide `/usr/include/SFML` if not in project

# Find all source files
file(GLOB_RECURSE SOURCES src/*.cpp)

# Add executable
add_executable(GameDev ${SOURCES})

# Link SFML libraries
target_link_libraries(MySFMLProject
        sfml-graphics
        sfml-window
        sfml-system
)

# Link additional libraries (optional)
target_link_libraries(MySFMLProject X11 pthread)
