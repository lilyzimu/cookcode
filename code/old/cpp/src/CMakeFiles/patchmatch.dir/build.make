# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.3

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
CMAKE_SOURCE_DIR = /scratch/leeh/mycpp/code/cpp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /scratch/leeh/mycpp/code/cpp

# Include any dependencies generated for this target.
include src/CMakeFiles/patchmatch.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/patchmatch.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/patchmatch.dir/flags.make

src/CMakeFiles/patchmatch.dir/patchmatch.cpp.o: src/CMakeFiles/patchmatch.dir/flags.make
src/CMakeFiles/patchmatch.dir/patchmatch.cpp.o: src/patchmatch.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/scratch/leeh/mycpp/code/cpp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/patchmatch.dir/patchmatch.cpp.o"
	cd /scratch/leeh/mycpp/code/cpp/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/patchmatch.dir/patchmatch.cpp.o -c /scratch/leeh/mycpp/code/cpp/src/patchmatch.cpp

src/CMakeFiles/patchmatch.dir/patchmatch.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/patchmatch.dir/patchmatch.cpp.i"
	cd /scratch/leeh/mycpp/code/cpp/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /scratch/leeh/mycpp/code/cpp/src/patchmatch.cpp > CMakeFiles/patchmatch.dir/patchmatch.cpp.i

src/CMakeFiles/patchmatch.dir/patchmatch.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/patchmatch.dir/patchmatch.cpp.s"
	cd /scratch/leeh/mycpp/code/cpp/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /scratch/leeh/mycpp/code/cpp/src/patchmatch.cpp -o CMakeFiles/patchmatch.dir/patchmatch.cpp.s

src/CMakeFiles/patchmatch.dir/patchmatch.cpp.o.requires:

.PHONY : src/CMakeFiles/patchmatch.dir/patchmatch.cpp.o.requires

src/CMakeFiles/patchmatch.dir/patchmatch.cpp.o.provides: src/CMakeFiles/patchmatch.dir/patchmatch.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/patchmatch.dir/build.make src/CMakeFiles/patchmatch.dir/patchmatch.cpp.o.provides.build
.PHONY : src/CMakeFiles/patchmatch.dir/patchmatch.cpp.o.provides

src/CMakeFiles/patchmatch.dir/patchmatch.cpp.o.provides.build: src/CMakeFiles/patchmatch.dir/patchmatch.cpp.o


# Object files for target patchmatch
patchmatch_OBJECTS = \
"CMakeFiles/patchmatch.dir/patchmatch.cpp.o"

# External object files for target patchmatch
patchmatch_EXTERNAL_OBJECTS =

src/patchmatch: src/CMakeFiles/patchmatch.dir/patchmatch.cpp.o
src/patchmatch: src/CMakeFiles/patchmatch.dir/build.make
src/patchmatch: /usr/lib64/libImath.so
src/patchmatch: /usr/lib64/libIlmImf.so
src/patchmatch: /usr/lib64/libIex.so
src/patchmatch: /usr/lib64/libHalf.so
src/patchmatch: /usr/lib64/libz.so
src/patchmatch: /usr/lib64/libpng.so
src/patchmatch: /usr/lib64/libz.so
src/patchmatch: /usr/lib64/libjpeg.so
src/patchmatch: /usr/lib64/libopencv_videostab.so.3.1.0
src/patchmatch: /usr/lib64/libopencv_superres.so.3.1.0
src/patchmatch: /usr/lib64/libopencv_stitching.so.3.1.0
src/patchmatch: /usr/lib64/libopencv_shape.so.3.1.0
src/patchmatch: /usr/lib64/libopencv_photo.so.3.1.0
src/patchmatch: /usr/lib64/libopencv_face.so.3.1.0
src/patchmatch: /usr/lib64/libopencv_calib3d.so.3.1.0
src/patchmatch: /usr/lib64/libpng.so
src/patchmatch: /usr/lib64/libjpeg.so
src/patchmatch: /usr/lib64/libopencv_features2d.so.3.1.0
src/patchmatch: /usr/lib64/libopencv_flann.so.3.1.0
src/patchmatch: /usr/lib64/libopencv_video.so.3.1.0
src/patchmatch: /usr/lib64/libopencv_objdetect.so.3.1.0
src/patchmatch: /usr/lib64/libopencv_ml.so.3.1.0
src/patchmatch: /usr/lib64/libopencv_highgui.so.3.1.0
src/patchmatch: /usr/lib64/libopencv_videoio.so.3.1.0
src/patchmatch: /usr/lib64/libopencv_imgcodecs.so.3.1.0
src/patchmatch: /usr/lib64/libopencv_imgproc.so.3.1.0
src/patchmatch: /usr/lib64/libopencv_core.so.3.1.0
src/patchmatch: src/CMakeFiles/patchmatch.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/scratch/leeh/mycpp/code/cpp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable patchmatch"
	cd /scratch/leeh/mycpp/code/cpp/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/patchmatch.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/patchmatch.dir/build: src/patchmatch

.PHONY : src/CMakeFiles/patchmatch.dir/build

src/CMakeFiles/patchmatch.dir/requires: src/CMakeFiles/patchmatch.dir/patchmatch.cpp.o.requires

.PHONY : src/CMakeFiles/patchmatch.dir/requires

src/CMakeFiles/patchmatch.dir/clean:
	cd /scratch/leeh/mycpp/code/cpp/src && $(CMAKE_COMMAND) -P CMakeFiles/patchmatch.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/patchmatch.dir/clean

src/CMakeFiles/patchmatch.dir/depend:
	cd /scratch/leeh/mycpp/code/cpp && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /scratch/leeh/mycpp/code/cpp /scratch/leeh/mycpp/code/cpp/src /scratch/leeh/mycpp/code/cpp /scratch/leeh/mycpp/code/cpp/src /scratch/leeh/mycpp/code/cpp/src/CMakeFiles/patchmatch.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/patchmatch.dir/depend

