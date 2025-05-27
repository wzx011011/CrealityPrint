cmake_minimum_required (VERSION 3.1)


project(Creative3D LANGUAGES CXX C)
set(BUNDLE_NAME "Creality Print")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/scripts/cmake/")
include(BuildPlatformDetection) 
set(APP_TYPE 7)  
set(BUILD_OS "xxxx")
if(BUILD_OS_WIN64)
	set(BUILD_OS "win64")
elseif(BUILD_OS_WIN32)
	set(BUILD_OS "win32")
elseif(BUILD_OS_LINUX)
	set(BUILD_OS "linux")
elseif(BUILD_OS_OSX)
	set(BUILD_OS "macx")
endif()
include(ConfigureAllModules)  

set(CC_GLOBAL_PACKAGE_INSTALL 1)  
if(BUILD_OS_WIN64 OR BUILD_OS_WIN32)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
	set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
endif()

set(CMAKE_CXX_STANDARD 14)
if (WIN32)
	add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
	add_definitions(-DWIN32DLL)
else()
    set(CMAKE_CXX_FLAGS "-fPIC")
    set(CMAKE_C_FLAGS "-fPIC")
endif()

if(NOT ("${PROJECT_VERSION_EXTRA}" STREQUAL "Alpha"))
	add_definitions(-DMACHINE_FILE_SUBNAME)
endif()
	add_definitions(-DCLOUD_BETA_URL)

set(QML_ENTRY_DIR ${CMAKE_SOURCE_DIR}/kernel/CreativeKernel/res/qml/)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

__build_info_header()
__enable_cxdef()
__enable_qt5()
__enable_spdlog()
__enable_openmp()

__cc_find(QhullWrapper)
__cc_find(Clipper)
__cc_find(Clipper3r)
# __cc_find(Qcxffmpeg)

__cc_find(OssWrapper)
__cc_find(AliyunOss)

__cc_find(tbb)

__cc_find(CXbin)
__cc_find(NestPlacer)
__cc_find(Quazip)
__cc_find(ppcs)
__cc_find(sensors_analytics_sdk)
__cc_find(SplitSlot)
__cc_find(enchase)
__cc_find(libqrencode)
__cc_find(cpr)

option(GEN_TRANSLATIONS_TS "create TS file" OFF)

if(CC_BC_MAC)
     add_definitions(-DNOUSE_IGL)
endif()

add_subdirectory(mmesh)
add_subdirectory(qtuser)

set(qtuser qtuser_core qtuser_3d qtuser_qml)

set(CXSS_CURA_ENGINE "cloud")
add_subdirectory(cxutil)
add_subdirectory(cxss)

add_subdirectory(cxmdns)
add_subdirectory(cxnet)
add_subdirectory(Qcxffmpeg)

set(CLibs ${QtQmlQ3dLibs} ${qtuser} trimesh2 UserSettings mmesh enchase)

set(NPLACER_SUPPORT_LIBS clipperz libnest2d nestplacer nlopt)

add_subdirectory(kernel)
add_subdirectory(cserialport)
set(Creative3D_plugins  CrealityUI 
					    plugin_cx3d_exporter 
					    plugin_info
					    plugin_letter
					    plugin_log
					    plugin_meshio_common
					    plugin_pick_bottom
						plugin_simple_support 
						plugin_split
                        plugin_repairnew
						plugin_extend_mesh
						plugin_slice_fdm
						plugin_laser
						plugin_localnet
						)
						
add_subdirectory(plugins)

add_subdirectory(Creative3D)			
add_subdirectory(scripts/cmake/)

