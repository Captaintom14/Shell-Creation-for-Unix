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
CMAKE_COMMAND = /Users/thomasmejia/Downloads/cmake-3.28.1/bin/cmake

# The command to remove a file.
RM = /Users/thomasmejia/Downloads/cmake-3.28.1/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/thomasmejia/Documents/GITHUB DOCUMENTS/Assignment_1_346/assignment_1/unix-shell"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/thomasmejia/Documents/GITHUB DOCUMENTS/Assignment_1_346/assignment_1/unix-shell/build"

# Include any dependencies generated for this target.
include CMakeFiles/unix_shell.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/unix_shell.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/unix_shell.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/unix_shell.dir/flags.make

CMakeFiles/unix_shell.dir/main.c.o: CMakeFiles/unix_shell.dir/flags.make
CMakeFiles/unix_shell.dir/main.c.o: /Users/thomasmejia/Documents/GITHUB\ DOCUMENTS/Assignment_1_346/assignment_1/unix-shell/main.c
CMakeFiles/unix_shell.dir/main.c.o: CMakeFiles/unix_shell.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/thomasmejia/Documents/GITHUB DOCUMENTS/Assignment_1_346/assignment_1/unix-shell/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/unix_shell.dir/main.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/unix_shell.dir/main.c.o -MF CMakeFiles/unix_shell.dir/main.c.o.d -o CMakeFiles/unix_shell.dir/main.c.o -c "/Users/thomasmejia/Documents/GITHUB DOCUMENTS/Assignment_1_346/assignment_1/unix-shell/main.c"

CMakeFiles/unix_shell.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/unix_shell.dir/main.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/Users/thomasmejia/Documents/GITHUB DOCUMENTS/Assignment_1_346/assignment_1/unix-shell/main.c" > CMakeFiles/unix_shell.dir/main.c.i

CMakeFiles/unix_shell.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/unix_shell.dir/main.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/Users/thomasmejia/Documents/GITHUB DOCUMENTS/Assignment_1_346/assignment_1/unix-shell/main.c" -o CMakeFiles/unix_shell.dir/main.c.s

CMakeFiles/unix_shell.dir/parser.c.o: CMakeFiles/unix_shell.dir/flags.make
CMakeFiles/unix_shell.dir/parser.c.o: /Users/thomasmejia/Documents/GITHUB\ DOCUMENTS/Assignment_1_346/assignment_1/unix-shell/parser.c
CMakeFiles/unix_shell.dir/parser.c.o: CMakeFiles/unix_shell.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/thomasmejia/Documents/GITHUB DOCUMENTS/Assignment_1_346/assignment_1/unix-shell/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/unix_shell.dir/parser.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/unix_shell.dir/parser.c.o -MF CMakeFiles/unix_shell.dir/parser.c.o.d -o CMakeFiles/unix_shell.dir/parser.c.o -c "/Users/thomasmejia/Documents/GITHUB DOCUMENTS/Assignment_1_346/assignment_1/unix-shell/parser.c"

CMakeFiles/unix_shell.dir/parser.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/unix_shell.dir/parser.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/Users/thomasmejia/Documents/GITHUB DOCUMENTS/Assignment_1_346/assignment_1/unix-shell/parser.c" > CMakeFiles/unix_shell.dir/parser.c.i

CMakeFiles/unix_shell.dir/parser.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/unix_shell.dir/parser.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/Users/thomasmejia/Documents/GITHUB DOCUMENTS/Assignment_1_346/assignment_1/unix-shell/parser.c" -o CMakeFiles/unix_shell.dir/parser.c.s

CMakeFiles/unix_shell.dir/utils.c.o: CMakeFiles/unix_shell.dir/flags.make
CMakeFiles/unix_shell.dir/utils.c.o: /Users/thomasmejia/Documents/GITHUB\ DOCUMENTS/Assignment_1_346/assignment_1/unix-shell/utils.c
CMakeFiles/unix_shell.dir/utils.c.o: CMakeFiles/unix_shell.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/thomasmejia/Documents/GITHUB DOCUMENTS/Assignment_1_346/assignment_1/unix-shell/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/unix_shell.dir/utils.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/unix_shell.dir/utils.c.o -MF CMakeFiles/unix_shell.dir/utils.c.o.d -o CMakeFiles/unix_shell.dir/utils.c.o -c "/Users/thomasmejia/Documents/GITHUB DOCUMENTS/Assignment_1_346/assignment_1/unix-shell/utils.c"

CMakeFiles/unix_shell.dir/utils.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/unix_shell.dir/utils.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/Users/thomasmejia/Documents/GITHUB DOCUMENTS/Assignment_1_346/assignment_1/unix-shell/utils.c" > CMakeFiles/unix_shell.dir/utils.c.i

CMakeFiles/unix_shell.dir/utils.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/unix_shell.dir/utils.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/Users/thomasmejia/Documents/GITHUB DOCUMENTS/Assignment_1_346/assignment_1/unix-shell/utils.c" -o CMakeFiles/unix_shell.dir/utils.c.s

# Object files for target unix_shell
unix_shell_OBJECTS = \
"CMakeFiles/unix_shell.dir/main.c.o" \
"CMakeFiles/unix_shell.dir/parser.c.o" \
"CMakeFiles/unix_shell.dir/utils.c.o"

# External object files for target unix_shell
unix_shell_EXTERNAL_OBJECTS =

unix_shell: CMakeFiles/unix_shell.dir/main.c.o
unix_shell: CMakeFiles/unix_shell.dir/parser.c.o
unix_shell: CMakeFiles/unix_shell.dir/utils.c.o
unix_shell: CMakeFiles/unix_shell.dir/build.make
unix_shell: CMakeFiles/unix_shell.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir="/Users/thomasmejia/Documents/GITHUB DOCUMENTS/Assignment_1_346/assignment_1/unix-shell/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable unix_shell"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/unix_shell.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/unix_shell.dir/build: unix_shell
.PHONY : CMakeFiles/unix_shell.dir/build

CMakeFiles/unix_shell.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/unix_shell.dir/cmake_clean.cmake
.PHONY : CMakeFiles/unix_shell.dir/clean

CMakeFiles/unix_shell.dir/depend:
	cd "/Users/thomasmejia/Documents/GITHUB DOCUMENTS/Assignment_1_346/assignment_1/unix-shell/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/thomasmejia/Documents/GITHUB DOCUMENTS/Assignment_1_346/assignment_1/unix-shell" "/Users/thomasmejia/Documents/GITHUB DOCUMENTS/Assignment_1_346/assignment_1/unix-shell" "/Users/thomasmejia/Documents/GITHUB DOCUMENTS/Assignment_1_346/assignment_1/unix-shell/build" "/Users/thomasmejia/Documents/GITHUB DOCUMENTS/Assignment_1_346/assignment_1/unix-shell/build" "/Users/thomasmejia/Documents/GITHUB DOCUMENTS/Assignment_1_346/assignment_1/unix-shell/build/CMakeFiles/unix_shell.dir/DependInfo.cmake" "--color=$(COLOR)"
.PHONY : CMakeFiles/unix_shell.dir/depend

