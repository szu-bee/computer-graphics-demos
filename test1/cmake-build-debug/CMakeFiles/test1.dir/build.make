# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

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
CMAKE_COMMAND = /home/bee/Apps/apps/CLion/ch-0/171.4694.4/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/bee/Apps/apps/CLion/ch-0/171.4694.4/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/bee/Projects/computer-graphics-demos/test1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bee/Projects/computer-graphics-demos/test1/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/test1.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/test1.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test1.dir/flags.make

CMakeFiles/test1.dir/main.cpp.o: CMakeFiles/test1.dir/flags.make
CMakeFiles/test1.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bee/Projects/computer-graphics-demos/test1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test1.dir/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test1.dir/main.cpp.o -c /home/bee/Projects/computer-graphics-demos/test1/main.cpp

CMakeFiles/test1.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test1.dir/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bee/Projects/computer-graphics-demos/test1/main.cpp > CMakeFiles/test1.dir/main.cpp.i

CMakeFiles/test1.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test1.dir/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bee/Projects/computer-graphics-demos/test1/main.cpp -o CMakeFiles/test1.dir/main.cpp.s

CMakeFiles/test1.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/test1.dir/main.cpp.o.requires

CMakeFiles/test1.dir/main.cpp.o.provides: CMakeFiles/test1.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/test1.dir/build.make CMakeFiles/test1.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/test1.dir/main.cpp.o.provides

CMakeFiles/test1.dir/main.cpp.o.provides.build: CMakeFiles/test1.dir/main.cpp.o


CMakeFiles/test1.dir/Common/InitShader.cpp.o: CMakeFiles/test1.dir/flags.make
CMakeFiles/test1.dir/Common/InitShader.cpp.o: ../Common/InitShader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bee/Projects/computer-graphics-demos/test1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/test1.dir/Common/InitShader.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test1.dir/Common/InitShader.cpp.o -c /home/bee/Projects/computer-graphics-demos/test1/Common/InitShader.cpp

CMakeFiles/test1.dir/Common/InitShader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test1.dir/Common/InitShader.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bee/Projects/computer-graphics-demos/test1/Common/InitShader.cpp > CMakeFiles/test1.dir/Common/InitShader.cpp.i

CMakeFiles/test1.dir/Common/InitShader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test1.dir/Common/InitShader.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bee/Projects/computer-graphics-demos/test1/Common/InitShader.cpp -o CMakeFiles/test1.dir/Common/InitShader.cpp.s

CMakeFiles/test1.dir/Common/InitShader.cpp.o.requires:

.PHONY : CMakeFiles/test1.dir/Common/InitShader.cpp.o.requires

CMakeFiles/test1.dir/Common/InitShader.cpp.o.provides: CMakeFiles/test1.dir/Common/InitShader.cpp.o.requires
	$(MAKE) -f CMakeFiles/test1.dir/build.make CMakeFiles/test1.dir/Common/InitShader.cpp.o.provides.build
.PHONY : CMakeFiles/test1.dir/Common/InitShader.cpp.o.provides

CMakeFiles/test1.dir/Common/InitShader.cpp.o.provides.build: CMakeFiles/test1.dir/Common/InitShader.cpp.o


# Object files for target test1
test1_OBJECTS = \
"CMakeFiles/test1.dir/main.cpp.o" \
"CMakeFiles/test1.dir/Common/InitShader.cpp.o"

# External object files for target test1
test1_EXTERNAL_OBJECTS =

test1: CMakeFiles/test1.dir/main.cpp.o
test1: CMakeFiles/test1.dir/Common/InitShader.cpp.o
test1: CMakeFiles/test1.dir/build.make
test1: /usr/lib/x86_64-linux-gnu/libGLU.so
test1: /usr/lib/x86_64-linux-gnu/libGL.so
test1: /usr/lib/x86_64-linux-gnu/libGLEW.so
test1: /usr/lib/x86_64-linux-gnu/libglut.so
test1: /usr/lib/x86_64-linux-gnu/libXmu.so
test1: /usr/lib/x86_64-linux-gnu/libXi.so
test1: CMakeFiles/test1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/bee/Projects/computer-graphics-demos/test1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable test1"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test1.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test1.dir/build: test1

.PHONY : CMakeFiles/test1.dir/build

CMakeFiles/test1.dir/requires: CMakeFiles/test1.dir/main.cpp.o.requires
CMakeFiles/test1.dir/requires: CMakeFiles/test1.dir/Common/InitShader.cpp.o.requires

.PHONY : CMakeFiles/test1.dir/requires

CMakeFiles/test1.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test1.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test1.dir/clean

CMakeFiles/test1.dir/depend:
	cd /home/bee/Projects/computer-graphics-demos/test1/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bee/Projects/computer-graphics-demos/test1 /home/bee/Projects/computer-graphics-demos/test1 /home/bee/Projects/computer-graphics-demos/test1/cmake-build-debug /home/bee/Projects/computer-graphics-demos/test1/cmake-build-debug /home/bee/Projects/computer-graphics-demos/test1/cmake-build-debug/CMakeFiles/test1.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test1.dir/depend
