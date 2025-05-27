file(GLOB QRC_SRC *.qrc)
qt5_add_resources(RCC_SRC ${QRC_SRC})

file(GLOB SRCS *.h *.cpp)
set(LIBS ${QtQmlQ3dLibs} ${qtuser} ${CLibs} trimesh2 CreativeKernel BasicKernel)

__add_real_target(plugin_info dll SOURCE ${SRCS} ${RCC_SRC} LIB ${LIBS} INC ${INCS} FOLDER plugin)
