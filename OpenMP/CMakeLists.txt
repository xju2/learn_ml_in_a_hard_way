cmake_minimum_required (VERSION 2.8)

project (HardOpenMP)

# The version number.
set (HardOpenMP_VERSION_MAJOR 0)
set (HardOpenMP_VERSION_MINOR 1)

# Nicer structure for binary files
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_MACOSX_RPATH 1)

#include modules
#list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmakes")

# Source code
add_subdirectory(src)
