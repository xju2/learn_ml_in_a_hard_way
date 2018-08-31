cmake_minimum_required (VERSION 2.8)

project (HardMPI)

# The version number.
set (HardMPI_VERSION_MAJOR 0)
set (HardMPI_VERSION_MINOR 1)

# Nicer structure for binary files
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_MACOSX_RPATH 1)

#if(CMAKE_BUILD_TYPE MATCHES Debug)
#  add_compile_options(-g)
#else()
#  add_compile_options(-O4)
#endif()

# cpp compile options
add_compile_options(-Wfatal-errors)
add_compile_options(-Wno-deprecated)
add_compile_options(-std=c++11)
#add_compile_options(-Wc++11-extensions)
add_compile_options(-Wall)
add_compile_options(-fPIC)

add_subdirectory(test)
