# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

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
CMAKE_COMMAND = /home/anatoly/clion-2018.2.4/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/anatoly/clion-2018.2.4/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/anatoly/Industrial-projects/Dumps

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/anatoly/Industrial-projects/Dumps/cmake-build-debug

# Include any dependencies generated for this target.
include tests/googletest/googletest/CMakeFiles/gtest.dir/depend.make

# Include the progress variables for this target.
include tests/googletest/googletest/CMakeFiles/gtest.dir/progress.make

# Include the compile flags for this target's objects.
include tests/googletest/googletest/CMakeFiles/gtest.dir/flags.make

tests/googletest/googletest/CMakeFiles/gtest.dir/src/gtest-all.cc.o: tests/googletest/googletest/CMakeFiles/gtest.dir/flags.make
tests/googletest/googletest/CMakeFiles/gtest.dir/src/gtest-all.cc.o: ../tests/googletest/googletest/src/gtest-all.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/anatoly/Industrial-projects/Dumps/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/googletest/googletest/CMakeFiles/gtest.dir/src/gtest-all.cc.o"
	cd /home/anatoly/Industrial-projects/Dumps/cmake-build-debug/tests/googletest/googletest && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/gtest.dir/src/gtest-all.cc.o -c /home/anatoly/Industrial-projects/Dumps/tests/googletest/googletest/src/gtest-all.cc

tests/googletest/googletest/CMakeFiles/gtest.dir/src/gtest-all.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gtest.dir/src/gtest-all.cc.i"
	cd /home/anatoly/Industrial-projects/Dumps/cmake-build-debug/tests/googletest/googletest && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/anatoly/Industrial-projects/Dumps/tests/googletest/googletest/src/gtest-all.cc > CMakeFiles/gtest.dir/src/gtest-all.cc.i

tests/googletest/googletest/CMakeFiles/gtest.dir/src/gtest-all.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gtest.dir/src/gtest-all.cc.s"
	cd /home/anatoly/Industrial-projects/Dumps/cmake-build-debug/tests/googletest/googletest && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/anatoly/Industrial-projects/Dumps/tests/googletest/googletest/src/gtest-all.cc -o CMakeFiles/gtest.dir/src/gtest-all.cc.s

# Object files for target gtest
gtest_OBJECTS = \
"CMakeFiles/gtest.dir/src/gtest-all.cc.o"

# External object files for target gtest
gtest_EXTERNAL_OBJECTS =

lib/libgtestd.a: tests/googletest/googletest/CMakeFiles/gtest.dir/src/gtest-all.cc.o
lib/libgtestd.a: tests/googletest/googletest/CMakeFiles/gtest.dir/build.make
lib/libgtestd.a: tests/googletest/googletest/CMakeFiles/gtest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/anatoly/Industrial-projects/Dumps/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library ../../../lib/libgtestd.a"
	cd /home/anatoly/Industrial-projects/Dumps/cmake-build-debug/tests/googletest/googletest && $(CMAKE_COMMAND) -P CMakeFiles/gtest.dir/cmake_clean_target.cmake
	cd /home/anatoly/Industrial-projects/Dumps/cmake-build-debug/tests/googletest/googletest && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gtest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/googletest/googletest/CMakeFiles/gtest.dir/build: lib/libgtestd.a

.PHONY : tests/googletest/googletest/CMakeFiles/gtest.dir/build

tests/googletest/googletest/CMakeFiles/gtest.dir/clean:
	cd /home/anatoly/Industrial-projects/Dumps/cmake-build-debug/tests/googletest/googletest && $(CMAKE_COMMAND) -P CMakeFiles/gtest.dir/cmake_clean.cmake
.PHONY : tests/googletest/googletest/CMakeFiles/gtest.dir/clean

tests/googletest/googletest/CMakeFiles/gtest.dir/depend:
	cd /home/anatoly/Industrial-projects/Dumps/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/anatoly/Industrial-projects/Dumps /home/anatoly/Industrial-projects/Dumps/tests/googletest/googletest /home/anatoly/Industrial-projects/Dumps/cmake-build-debug /home/anatoly/Industrial-projects/Dumps/cmake-build-debug/tests/googletest/googletest /home/anatoly/Industrial-projects/Dumps/cmake-build-debug/tests/googletest/googletest/CMakeFiles/gtest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/googletest/googletest/CMakeFiles/gtest.dir/depend

