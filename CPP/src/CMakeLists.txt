if(CMAKE_BUILD_TYPE MATCHES Debug)
  add_compile_options(-g)
else()
  add_compile_options(-O4)
endif()

# cpp compile options
add_compile_options(-Wfatal-errors)
add_compile_options(-Wno-deprecated)
add_compile_options(-std=c++11)
add_compile_options(-Wc++11-extensions)
add_compile_options(-Wall)
add_compile_options(-fPIC)

add_subdirectory(test)
