# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

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
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/aramsevagavarian/ArmBionics

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/aramsevagavarian/ArmBionics/build/Imported_Kit-Debug

# Utility rule file for ArmBionics_autogen.

# Include any custom commands dependencies for this target.
include CMakeFiles/ArmBionics_autogen.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/ArmBionics_autogen.dir/progress.make

CMakeFiles/ArmBionics_autogen:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/aramsevagavarian/ArmBionics/build/Imported_Kit-Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC and UIC for target ArmBionics"
	/Applications/CMake.app/Contents/bin/cmake -E cmake_autogen /Users/aramsevagavarian/ArmBionics/build/Imported_Kit-Debug/CMakeFiles/ArmBionics_autogen.dir/AutogenInfo.json Debug

ArmBionics_autogen: CMakeFiles/ArmBionics_autogen
ArmBionics_autogen: CMakeFiles/ArmBionics_autogen.dir/build.make
.PHONY : ArmBionics_autogen

# Rule to build all files generated by this target.
CMakeFiles/ArmBionics_autogen.dir/build: ArmBionics_autogen
.PHONY : CMakeFiles/ArmBionics_autogen.dir/build

CMakeFiles/ArmBionics_autogen.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ArmBionics_autogen.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ArmBionics_autogen.dir/clean

CMakeFiles/ArmBionics_autogen.dir/depend:
	cd /Users/aramsevagavarian/ArmBionics/build/Imported_Kit-Debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/aramsevagavarian/ArmBionics /Users/aramsevagavarian/ArmBionics /Users/aramsevagavarian/ArmBionics/build/Imported_Kit-Debug /Users/aramsevagavarian/ArmBionics/build/Imported_Kit-Debug /Users/aramsevagavarian/ArmBionics/build/Imported_Kit-Debug/CMakeFiles/ArmBionics_autogen.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ArmBionics_autogen.dir/depend

