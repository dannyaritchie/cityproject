# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/danny/foot/cityproject

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/danny/foot/cityproject/build

# Include any dependencies generated for this target.
include libraries/mylibrary/CMakeFiles/mylibrary.dir/depend.make

# Include the progress variables for this target.
include libraries/mylibrary/CMakeFiles/mylibrary.dir/progress.make

# Include the compile flags for this target's objects.
include libraries/mylibrary/CMakeFiles/mylibrary.dir/flags.make

libraries/mylibrary/CMakeFiles/mylibrary.dir/src/mylibrary.cpp.o: libraries/mylibrary/CMakeFiles/mylibrary.dir/flags.make
libraries/mylibrary/CMakeFiles/mylibrary.dir/src/mylibrary.cpp.o: ../libraries/mylibrary/src/mylibrary.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/danny/foot/cityproject/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object libraries/mylibrary/CMakeFiles/mylibrary.dir/src/mylibrary.cpp.o"
	cd /home/danny/foot/cityproject/build/libraries/mylibrary && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mylibrary.dir/src/mylibrary.cpp.o -c /home/danny/foot/cityproject/libraries/mylibrary/src/mylibrary.cpp

libraries/mylibrary/CMakeFiles/mylibrary.dir/src/mylibrary.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mylibrary.dir/src/mylibrary.cpp.i"
	cd /home/danny/foot/cityproject/build/libraries/mylibrary && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/danny/foot/cityproject/libraries/mylibrary/src/mylibrary.cpp > CMakeFiles/mylibrary.dir/src/mylibrary.cpp.i

libraries/mylibrary/CMakeFiles/mylibrary.dir/src/mylibrary.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mylibrary.dir/src/mylibrary.cpp.s"
	cd /home/danny/foot/cityproject/build/libraries/mylibrary && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/danny/foot/cityproject/libraries/mylibrary/src/mylibrary.cpp -o CMakeFiles/mylibrary.dir/src/mylibrary.cpp.s

# Object files for target mylibrary
mylibrary_OBJECTS = \
"CMakeFiles/mylibrary.dir/src/mylibrary.cpp.o"

# External object files for target mylibrary
mylibrary_EXTERNAL_OBJECTS =

libraries/mylibrary/libmylibrary.a: libraries/mylibrary/CMakeFiles/mylibrary.dir/src/mylibrary.cpp.o
libraries/mylibrary/libmylibrary.a: libraries/mylibrary/CMakeFiles/mylibrary.dir/build.make
libraries/mylibrary/libmylibrary.a: libraries/mylibrary/CMakeFiles/mylibrary.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/danny/foot/cityproject/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libmylibrary.a"
	cd /home/danny/foot/cityproject/build/libraries/mylibrary && $(CMAKE_COMMAND) -P CMakeFiles/mylibrary.dir/cmake_clean_target.cmake
	cd /home/danny/foot/cityproject/build/libraries/mylibrary && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mylibrary.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
libraries/mylibrary/CMakeFiles/mylibrary.dir/build: libraries/mylibrary/libmylibrary.a

.PHONY : libraries/mylibrary/CMakeFiles/mylibrary.dir/build

libraries/mylibrary/CMakeFiles/mylibrary.dir/clean:
	cd /home/danny/foot/cityproject/build/libraries/mylibrary && $(CMAKE_COMMAND) -P CMakeFiles/mylibrary.dir/cmake_clean.cmake
.PHONY : libraries/mylibrary/CMakeFiles/mylibrary.dir/clean

libraries/mylibrary/CMakeFiles/mylibrary.dir/depend:
	cd /home/danny/foot/cityproject/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/danny/foot/cityproject /home/danny/foot/cityproject/libraries/mylibrary /home/danny/foot/cityproject/build /home/danny/foot/cityproject/build/libraries/mylibrary /home/danny/foot/cityproject/build/libraries/mylibrary/CMakeFiles/mylibrary.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : libraries/mylibrary/CMakeFiles/mylibrary.dir/depend
