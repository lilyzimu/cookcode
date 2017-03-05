# Project name is not mandatory, but you should use it
project(patchmatch)

# States that CMake required version must be >= 2.6
cmake_minimum_required(VERSION 2.6)

# set to include custom modules
set(CMAKE_MODULE_PATH ${patchmatch_SOURCE_DIR}/../../lib/cmake/Modules ${CMAKE_MODULE_PATH})

# glab
find_package(GlabLib REQUIRED)
include_directories(${GLABLIB_INCLUDE_DIRS})
link_libraries(${GLABLIB_LIBRARIES})
add_definitions(${GLABLIB_DEFINITIONS})

# opencv

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIR})
link_libraries(${OpenCV_LIBS})

# Add subdirs
add_subdirectory(src)
