file(GLOB QRC_SRC *.qrc)
qt5_add_resources(RCC_SRC ${QRC_SRC})
__files_group(${CMAKE_CURRENT_SOURCE_DIR} SRCS)

find_package(Qt5 COMPONENTS SerialPort Concurrent  REQUIRED)
set(LIBS ${CLibs} Qt5::Xml BasicKernel CreativeKernel EngineWrapper ossWrapper CuraEngine cxutil cxss clipper quazip zlib cxnet thumbnail cxlog Qt5::SerialPort Qt5::Concurrent OpenMP::OpenMP_CXX curl cpr cserialport)
		
set(DEFS NOMINMAX Z_PREFIX)

__add_real_target(plugin_slice_fdm dll SOURCE ${SRCS} ${RCC_SRC} LIB ${LIBS} DEF ${DEFS} FOLDER plugin)
