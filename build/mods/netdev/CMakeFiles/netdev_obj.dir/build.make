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
CMAKE_SOURCE_DIR = /home/reaur/Desktop/eiger_project/project1/tapdev_stack/lib_example1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/reaur/Desktop/eiger_project/project1/tapdev_stack/lib_example1/build

# Include any dependencies generated for this target.
include mods/netdev/CMakeFiles/netdev_obj.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include mods/netdev/CMakeFiles/netdev_obj.dir/compiler_depend.make

# Include the progress variables for this target.
include mods/netdev/CMakeFiles/netdev_obj.dir/progress.make

# Include the compile flags for this target's objects.
include mods/netdev/CMakeFiles/netdev_obj.dir/flags.make

mods/netdev/CMakeFiles/netdev_obj.dir/src/netdev.cc.o: mods/netdev/CMakeFiles/netdev_obj.dir/flags.make
mods/netdev/CMakeFiles/netdev_obj.dir/src/netdev.cc.o: ../mods/netdev/src/netdev.cc
mods/netdev/CMakeFiles/netdev_obj.dir/src/netdev.cc.o: mods/netdev/CMakeFiles/netdev_obj.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/reaur/Desktop/eiger_project/project1/tapdev_stack/lib_example1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object mods/netdev/CMakeFiles/netdev_obj.dir/src/netdev.cc.o"
	cd /home/reaur/Desktop/eiger_project/project1/tapdev_stack/lib_example1/build/mods/netdev && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT mods/netdev/CMakeFiles/netdev_obj.dir/src/netdev.cc.o -MF CMakeFiles/netdev_obj.dir/src/netdev.cc.o.d -o CMakeFiles/netdev_obj.dir/src/netdev.cc.o -c /home/reaur/Desktop/eiger_project/project1/tapdev_stack/lib_example1/mods/netdev/src/netdev.cc

mods/netdev/CMakeFiles/netdev_obj.dir/src/netdev.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/netdev_obj.dir/src/netdev.cc.i"
	cd /home/reaur/Desktop/eiger_project/project1/tapdev_stack/lib_example1/build/mods/netdev && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/reaur/Desktop/eiger_project/project1/tapdev_stack/lib_example1/mods/netdev/src/netdev.cc > CMakeFiles/netdev_obj.dir/src/netdev.cc.i

mods/netdev/CMakeFiles/netdev_obj.dir/src/netdev.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/netdev_obj.dir/src/netdev.cc.s"
	cd /home/reaur/Desktop/eiger_project/project1/tapdev_stack/lib_example1/build/mods/netdev && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/reaur/Desktop/eiger_project/project1/tapdev_stack/lib_example1/mods/netdev/src/netdev.cc -o CMakeFiles/netdev_obj.dir/src/netdev.cc.s

netdev_obj: mods/netdev/CMakeFiles/netdev_obj.dir/src/netdev.cc.o
netdev_obj: mods/netdev/CMakeFiles/netdev_obj.dir/build.make
.PHONY : netdev_obj

# Rule to build all files generated by this target.
mods/netdev/CMakeFiles/netdev_obj.dir/build: netdev_obj
.PHONY : mods/netdev/CMakeFiles/netdev_obj.dir/build

mods/netdev/CMakeFiles/netdev_obj.dir/clean:
	cd /home/reaur/Desktop/eiger_project/project1/tapdev_stack/lib_example1/build/mods/netdev && $(CMAKE_COMMAND) -P CMakeFiles/netdev_obj.dir/cmake_clean.cmake
.PHONY : mods/netdev/CMakeFiles/netdev_obj.dir/clean

mods/netdev/CMakeFiles/netdev_obj.dir/depend:
	cd /home/reaur/Desktop/eiger_project/project1/tapdev_stack/lib_example1/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/reaur/Desktop/eiger_project/project1/tapdev_stack/lib_example1 /home/reaur/Desktop/eiger_project/project1/tapdev_stack/lib_example1/mods/netdev /home/reaur/Desktop/eiger_project/project1/tapdev_stack/lib_example1/build /home/reaur/Desktop/eiger_project/project1/tapdev_stack/lib_example1/build/mods/netdev /home/reaur/Desktop/eiger_project/project1/tapdev_stack/lib_example1/build/mods/netdev/CMakeFiles/netdev_obj.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : mods/netdev/CMakeFiles/netdev_obj.dir/depend

