# Include the directory itself as a path to include directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# For convenience we define the sources as a variable. You can add 
# header files and cpp/c files and CMake will sort them out
file(GLOB SRCS *.cpp)
 
# Here we define the executable
add_executable(patchmatch ${SRCS})
