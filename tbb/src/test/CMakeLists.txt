# some excutables
file(GLOB tools *.cxx)
foreach(target ${tools})
	# message(${target})
	get_filename_component(execname ${target} NAME_WE)
	get_filename_component(srcfile ${target} NAME)

	add_executable(${execname} ${srcfile})

	set_target_properties(${execname} PROPERTIES LINKER_LANGUAGE CXX)
	target_link_libraries(${execname} PUBLIC pthread TBB::tbb)

endforeach()
