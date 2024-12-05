# some excutables
find_package(Boost REQUIRED COMPONENTS graph)
message(STATUS "Boost Libaries: ${Boost_LIBRARIES} ${Boost_INCLUDE_DIRS}")

file(GLOB tools *.cxx)
foreach(target ${tools})
	# message(${target})
	get_filename_component(execname ${target} NAME_WE)
	get_filename_component(srcfile ${target} NAME)
	add_executable(${execname} ${srcfile})
	#add_dependencies(${execname})
	set_target_properties(${execname} PROPERTIES
		LINKER_LANGUAGE CXX
		CXX_STANDARD 20)
	target_link_libraries(${execname} PUBLIC pthread Boost::graph)
endforeach()

find_package(Boost REQUIRED COMPONENTS graph)
message(STATUS "Boost Libaries: ${Boost_LIBRARIES}")
add_executable(test_wcc test_wcc.cpp)
set_target_properties(test_wcc PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(test_wcc PUBLIC Boost::graph)


find_package(Eigen3 REQUIRED NO_MODULE)
message(STATUS "Eigen Libaries: ${Eigen3_LIBRARIES}")
add_executable(test_eigen test_eigen.cpp)
set_target_properties(test_eigen PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(test_eigen PUBLIC Eigen3::Eigen)