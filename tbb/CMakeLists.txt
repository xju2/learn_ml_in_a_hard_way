cmake_minimum_required (VERSION 3.10) ## a random version...

project (HardTBB)

# The version number.
set (HardTBB_VERSION_MAJOR 0)
set (HardTBB_VERSION_MINOR 1)

# Nicer structure for binary files
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(CMAKE_BUILD_TYPE MATCHES Debug)
  add_compile_options(-g)
else()
  add_compile_options(-O4)
endif()

# cpp compile options
add_compile_options(-Wfatal-errors -Wno-deprecated -std=c++17 -Wall -fPIC -pedantic)
# add_compile_options(-Wc++11-extensions)
# add_compile_options(-Wgnu-zero-variadic-macro-arguments)

# FIXME: if you are using the normal version of TBB, it has a config file so
#   remove cmake/FindTBB.cmake and add CONFIG to the next line.
find_package(TBB 2019.0.11007.2 REQUIRED)

add_subdirectory(src)
