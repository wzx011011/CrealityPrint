__cc_find(enchase)
__files_group(${CMAKE_CURRENT_SOURCE_DIR} SRCS)

find_package(Qt5 COMPONENTS Svg)

set(LIBS ${QtQmlQ3dLibs} trimesh2 ${qtuser} BasicKernel enchase Qt5::Svg)

__add_real_target(plugin_extend_mesh dll SOURCE ${SRCS} LIB ${LIBS} FOLDER plugin)