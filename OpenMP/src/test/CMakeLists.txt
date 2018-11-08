find_package(OpenMP COMPONENTS CXX)
if(NOT OpenMP_CXX_FOUND)
	set(OpenMP_CXX_FLAGS "-Xpreprocessor -fopenmp -I/usr/local/opt/libomp/include")
	set(OpenMP_CXX_LIB_NAMES "omp")
	set(OpenMP_omp_LIBRARY "/usr/local/opt/libomp/lib/libomp.dylib")
	find_package(OpenMP COMPONENTS CXX)
endif()

# some excutables
file(GLOB tools *.cxx)
foreach(target ${tools})
	# message(${target})
	get_filename_component(execname ${target} NAME_WE)
	get_filename_component(srcfile ${target} NAME)
	add_executable(${execname} ${srcfile})

	if(OpenMP_CXX_FOUND)
		target_link_libraries(${execname} PRIVATE OpenMP::OpenMP_CXX)
	endif()

	#add_dependencies(${execname})
	#set_target_properties(${execname} PROPERTIES LINKER_LANGUAGE CXX)
	
endforeach()
