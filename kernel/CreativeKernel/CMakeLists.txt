file(GLOB QRC_SRC res/*.qrc)
file(GLOB PLUGIN_SRC res/plugin/*.qrc)
qt5_add_resources(RCC_SRC ${QRC_SRC} ${PLUGIN_SRC})

__files_group(${CMAKE_CURRENT_SOURCE_DIR}/external/ EXTERNAL_SRCS)
__files_group_2(${CMAKE_CURRENT_SOURCE_DIR}/internal/ internal INTERNAL_SRCS)

__cc_find(RapidJson)

set(SRCS ${EXTERNAL_SRCS} ${INTERNAL_SRCS} ${RCC_SRC})
set(DEFS CREATIVE_KERNEL_DLL Z_PREFIX)
set(INCS ${CMAKE_CURRENT_SOURCE_DIR}/external/ ${RAPIDJSON_INCLUDE_DIRS})

set(LIBS ${CLibs} quazip zlib ppcs cxmdns cxnet libqrencode curl cpr cxss)
	
__add_real_target(CreativeKernel dll SOURCE ${SRCS} 
						LIB ${LIBS} ${qhull} BasicKernel
						DEF ${DEFS} INC ${INCS})
set_property(TARGET CreativeKernel PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/external/)

