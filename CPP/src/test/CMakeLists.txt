# some excutables
file(GLOB tools *.c*)
foreach(target ${tools})
  get_filename_component(execname ${target} NAME_WE)
  get_filename_component(srcfile ${target} NAME)
  add_executable(${execname} ${srcfile})
  add_dependencies(${execname})
  set_target_properties(${execname} PROPERTIES LINKER_LANGUAGE CXX)
endforeach()
