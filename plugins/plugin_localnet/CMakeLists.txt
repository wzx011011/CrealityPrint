file(GLOB QRC_SRC res/*.qrc)
file(GLOB PLUGIN_SRC res/plugin/*.qrc)
qt5_add_resources(RCC_SRC ${QRC_SRC})

file(GLOB SRCS *.h *.cpp)
set(LIBS ${QtQmlQ3dLibs} ${qtuser} ${CLibs} trimesh2 CreativeKernel BasicKernel cxmdns curl)

__add_real_target(plugin_localnet dll SOURCE ${SRCS} ${RCC_SRC} LIB ${LIBS} INC ${INCS} FOLDER plugin)
