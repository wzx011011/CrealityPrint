set(INCS)
set(LIBS Qt5::Core)

foreach(plugin ${Creative3D_plugins})
	add_subdirectory(${plugin})
endforeach()
