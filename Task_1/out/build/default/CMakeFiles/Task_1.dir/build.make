# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_SOURCE_DIR = /home/darya/Документы/453502/ОАиП/LR8/Task_1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/darya/Документы/453502/ОАиП/LR8/Task_1/out/build/default

# Include any dependencies generated for this target.
include CMakeFiles/Task_1.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Task_1.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Task_1.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Task_1.dir/flags.make

CMakeFiles/Task_1.dir/main.cpp.o: CMakeFiles/Task_1.dir/flags.make
CMakeFiles/Task_1.dir/main.cpp.o: /home/darya/Документы/453502/ОАиП/LR8/Task_1/main.cpp
CMakeFiles/Task_1.dir/main.cpp.o: CMakeFiles/Task_1.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/darya/Документы/453502/ОАиП/LR8/Task_1/out/build/default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Task_1.dir/main.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Task_1.dir/main.cpp.o -MF CMakeFiles/Task_1.dir/main.cpp.o.d -o CMakeFiles/Task_1.dir/main.cpp.o -c /home/darya/Документы/453502/ОАиП/LR8/Task_1/main.cpp

CMakeFiles/Task_1.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Task_1.dir/main.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/darya/Документы/453502/ОАиП/LR8/Task_1/main.cpp > CMakeFiles/Task_1.dir/main.cpp.i

CMakeFiles/Task_1.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Task_1.dir/main.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/darya/Документы/453502/ОАиП/LR8/Task_1/main.cpp -o CMakeFiles/Task_1.dir/main.cpp.s

CMakeFiles/Task_1.dir/functions.cpp.o: CMakeFiles/Task_1.dir/flags.make
CMakeFiles/Task_1.dir/functions.cpp.o: /home/darya/Документы/453502/ОАиП/LR8/Task_1/functions.cpp
CMakeFiles/Task_1.dir/functions.cpp.o: CMakeFiles/Task_1.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/darya/Документы/453502/ОАиП/LR8/Task_1/out/build/default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Task_1.dir/functions.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Task_1.dir/functions.cpp.o -MF CMakeFiles/Task_1.dir/functions.cpp.o.d -o CMakeFiles/Task_1.dir/functions.cpp.o -c /home/darya/Документы/453502/ОАиП/LR8/Task_1/functions.cpp

CMakeFiles/Task_1.dir/functions.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Task_1.dir/functions.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/darya/Документы/453502/ОАиП/LR8/Task_1/functions.cpp > CMakeFiles/Task_1.dir/functions.cpp.i

CMakeFiles/Task_1.dir/functions.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Task_1.dir/functions.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/darya/Документы/453502/ОАиП/LR8/Task_1/functions.cpp -o CMakeFiles/Task_1.dir/functions.cpp.s

# Object files for target Task_1
Task_1_OBJECTS = \
"CMakeFiles/Task_1.dir/main.cpp.o" \
"CMakeFiles/Task_1.dir/functions.cpp.o"

# External object files for target Task_1
Task_1_EXTERNAL_OBJECTS =

Task_1: CMakeFiles/Task_1.dir/main.cpp.o
Task_1: CMakeFiles/Task_1.dir/functions.cpp.o
Task_1: CMakeFiles/Task_1.dir/build.make
Task_1: /usr/lib/x86_64-linux-gnu/libcurl.so
Task_1: CMakeFiles/Task_1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/darya/Документы/453502/ОАиП/LR8/Task_1/out/build/default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable Task_1"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Task_1.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Task_1.dir/build: Task_1
.PHONY : CMakeFiles/Task_1.dir/build

CMakeFiles/Task_1.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Task_1.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Task_1.dir/clean

CMakeFiles/Task_1.dir/depend:
	cd /home/darya/Документы/453502/ОАиП/LR8/Task_1/out/build/default && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/darya/Документы/453502/ОАиП/LR8/Task_1 /home/darya/Документы/453502/ОАиП/LR8/Task_1 /home/darya/Документы/453502/ОАиП/LR8/Task_1/out/build/default /home/darya/Документы/453502/ОАиП/LR8/Task_1/out/build/default /home/darya/Документы/453502/ОАиП/LR8/Task_1/out/build/default/CMakeFiles/Task_1.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/Task_1.dir/depend

