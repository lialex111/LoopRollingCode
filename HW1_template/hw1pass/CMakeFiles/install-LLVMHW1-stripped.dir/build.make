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

# Utility rule file for install-LLVMHW1-stripped.

# Include any custom commands dependencies for this target.
include hw1pass/CMakeFiles/install-LLVMHW1-stripped.dir/compiler_depend.make

# Include the progress variables for this target.
include hw1pass/CMakeFiles/install-LLVMHW1-stripped.dir/progress.make

hw1pass/CMakeFiles/install-LLVMHW1-stripped:
	cd /home/weijie/LoopRollingCode/HW1_template/hw1pass && /usr/bin/cmake -DCMAKE_INSTALL_COMPONENT="LLVMHW1" -DCMAKE_INSTALL_DO_STRIP=1 -P /home/weijie/LoopRollingCode/HW1_template/cmake_install.cmake

install-LLVMHW1-stripped: hw1pass/CMakeFiles/install-LLVMHW1-stripped
install-LLVMHW1-stripped: hw1pass/CMakeFiles/install-LLVMHW1-stripped.dir/build.make
.PHONY : install-LLVMHW1-stripped

# Rule to build all files generated by this target.
hw1pass/CMakeFiles/install-LLVMHW1-stripped.dir/build: install-LLVMHW1-stripped
.PHONY : hw1pass/CMakeFiles/install-LLVMHW1-stripped.dir/build

hw1pass/CMakeFiles/install-LLVMHW1-stripped.dir/clean:
	cd /home/weijie/LoopRollingCode/HW1_template/hw1pass && $(CMAKE_COMMAND) -P CMakeFiles/install-LLVMHW1-stripped.dir/cmake_clean.cmake
.PHONY : hw1pass/CMakeFiles/install-LLVMHW1-stripped.dir/clean

hw1pass/CMakeFiles/install-LLVMHW1-stripped.dir/depend:
	cd /home/weijie/LoopRollingCode/HW1_template && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/weijie/LoopRollingCode/HW1_template /home/weijie/LoopRollingCode/HW1_template/hw1pass /home/weijie/LoopRollingCode/HW1_template /home/weijie/LoopRollingCode/HW1_template/hw1pass /home/weijie/LoopRollingCode/HW1_template/hw1pass/CMakeFiles/install-LLVMHW1-stripped.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : hw1pass/CMakeFiles/install-LLVMHW1-stripped.dir/depend
