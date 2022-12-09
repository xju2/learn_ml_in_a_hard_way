cmake_minimum_required (VERSION 3.16)

project (HardCPP)

# The version number.
set (HardCPP_VERSION_MAJOR 0)
set (HardCPP_VERSION_MINOR 1)

# Nicer structure for binary files
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# set(CMAKE_MACOSX_RPATH 1)
# Source code
add_subdirectory(src)
