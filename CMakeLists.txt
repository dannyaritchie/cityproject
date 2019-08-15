# Minimum version of CMake required to build this project
cmake_minimum_required(VERSION 3.0)

# Name of the project
project(Project)

# Add all the source files needed to build the executable
add_executable(Project src/main.cpp src/helpers/functions.cpp
    src/models/ball.cpp src/models/frame.cpp src/models/frameobjects.cpp
    src/models/game.cpp src/models/player.cpp src/models/possessionfilereader/possessionfileline.cpp
    src/models/possessionfilereader/possessionfile.cpp src/models/possessionfilereader/possessionfilemaker.cpp
    src/models/wholeplayer.cpp)

# Include the directory where MyLibrary project is. Otherwise, we can't use the
# library
add_subdirectory(libraries/mylibrary)

# Link the executable and the library together
target_link_libraries(Project mylibrary)
