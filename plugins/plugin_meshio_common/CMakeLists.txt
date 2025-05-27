set(SRCS commonmeshioplugin.h
		 commonmeshioplugin.cpp
		 meshioimpl.h
		 meshioimpl.cpp
		 )
set(LIBS trimesh2 cxbin stringutil ${QtGuiLibs} ${qtuser} BasicKernel)

__add_real_target(plugin_meshio_common dll  SOURCE ${SRCS}
											LIB ${LIBS}
											FOLDER plugin
											)
