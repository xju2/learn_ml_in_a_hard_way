# find_package(Python3 REQUIRED COMPONENTS Development)
find_package(pybind11 REQUIRED)

add_library(my_graph_module MODULE pybind11.cxx)
target_link_libraries(my_graph_module PRIVATE pybind11::module)
target_include_directories(my_graph_module PRIVATE ${Python3_INCLUDE_DIRS})
set_target_properties(my_graph_module PROPERTIES PREFIX "" SUFFIX ".so")
