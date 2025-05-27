__cc_find(RapidJson)

__tree_add_current(SRCS)

file(GLOB QRC_SRC *.qrc)
qt5_add_resources(RCC_SRC ${QRC_SRC})
		 
set(DEFS ENGINE_WRAPPER_DLL)

set(LIBS Qt5::Core Qt5::Gui trimesh2 UserSettings BasicKernel qtuser_core qtuser_3d clipper mmesh)

__add_real_target(EngineWrapper dll SOURCE ${SRCS} ${RCC_SRC}
									DEF ${DEFS}
									INC ${INCS} ${RAPIDJSON_INCLUDE_DIRS}
									LIB ${LIBS}
									INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
									)



