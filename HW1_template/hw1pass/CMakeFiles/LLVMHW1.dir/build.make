# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/weijie/LoopRollingCode/HW1_template

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/weijie/LoopRollingCode/HW1_template

# Include any dependencies generated for this target.
include hw1pass/CMakeFiles/LLVMHW1.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include hw1pass/CMakeFiles/LLVMHW1.dir/compiler_depend.make

# Include the progress variables for this target.
include hw1pass/CMakeFiles/LLVMHW1.dir/progress.make

# Include the compile flags for this target's objects.
include hw1pass/CMakeFiles/LLVMHW1.dir/flags.make

hw1pass/CMakeFiles/LLVMHW1.dir/hw1pass.cpp.o: hw1pass/CMakeFiles/LLVMHW1.dir/flags.make
hw1pass/CMakeFiles/LLVMHW1.dir/hw1pass.cpp.o: hw1pass/hw1pass.cpp
hw1pass/CMakeFiles/LLVMHW1.dir/hw1pass.cpp.o: hw1pass/CMakeFiles/LLVMHW1.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/weijie/LoopRollingCode/HW1_template/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object hw1pass/CMakeFiles/LLVMHW1.dir/hw1pass.cpp.o"
	cd /home/weijie/LoopRollingCode/HW1_template/hw1pass && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT hw1pass/CMakeFiles/LLVMHW1.dir/hw1pass.cpp.o -MF CMakeFiles/LLVMHW1.dir/hw1pass.cpp.o.d -o CMakeFiles/LLVMHW1.dir/hw1pass.cpp.o -c /home/weijie/LoopRollingCode/HW1_template/hw1pass/hw1pass.cpp

hw1pass/CMakeFiles/LLVMHW1.dir/hw1pass.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LLVMHW1.dir/hw1pass.cpp.i"
	cd /home/weijie/LoopRollingCode/HW1_template/hw1pass && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/weijie/LoopRollingCode/HW1_template/hw1pass/hw1pass.cpp > CMakeFiles/LLVMHW1.dir/hw1pass.cpp.i

hw1pass/CMakeFiles/LLVMHW1.dir/hw1pass.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LLVMHW1.dir/hw1pass.cpp.s"
	cd /home/weijie/LoopRollingCode/HW1_template/hw1pass && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/weijie/LoopRollingCode/HW1_template/hw1pass/hw1pass.cpp -o CMakeFiles/LLVMHW1.dir/hw1pass.cpp.s

# Object files for target LLVMHW1
LLVMHW1_OBJECTS = \
"CMakeFiles/LLVMHW1.dir/hw1pass.cpp.o"

# External object files for target LLVMHW1
LLVMHW1_EXTERNAL_OBJECTS =

hw1pass/LLVMHW1.so: hw1pass/CMakeFiles/LLVMHW1.dir/hw1pass.cpp.o
hw1pass/LLVMHW1.so: hw1pass/CMakeFiles/LLVMHW1.dir/build.make
hw1pass/LLVMHW1.so: hw1pass/CMakeFiles/LLVMHW1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/weijie/LoopRollingCode/HW1_template/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared module LLVMHW1.so"
	cd /home/weijie/LoopRollingCode/HW1_template/hw1pass && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/LLVMHW1.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
hw1pass/CMakeFiles/LLVMHW1.dir/build: hw1pass/LLVMHW1.so
.PHONY : hw1pass/CMakeFiles/LLVMHW1.dir/build

hw1pass/CMakeFiles/LLVMHW1.dir/clean:
	cd /home/weijie/LoopRollingCode/HW1_template/hw1pass && $(CMAKE_COMMAND) -P CMakeFiles/LLVMHW1.dir/cmake_clean.cmake
.PHONY : hw1pass/CMakeFiles/LLVMHW1.dir/clean

hw1pass/CMakeFiles/LLVMHW1.dir/depend:
	cd /home/weijie/LoopRollingCode/HW1_template && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/weijie/LoopRollingCode/HW1_template /home/weijie/LoopRollingCode/HW1_template/hw1pass /home/weijie/LoopRollingCode/HW1_template /home/weijie/LoopRollingCode/HW1_template/hw1pass /home/weijie/LoopRollingCode/HW1_template/hw1pass/CMakeFiles/LLVMHW1.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : hw1pass/CMakeFiles/LLVMHW1.dir/depend

