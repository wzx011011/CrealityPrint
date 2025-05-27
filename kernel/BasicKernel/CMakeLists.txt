
__files_group(${CMAKE_CURRENT_SOURCE_DIR}/external/ EXTERNAL_SRCS)
__files_group_2(${CMAKE_CURRENT_SOURCE_DIR}/internal/ internal INTERNAL_SRCS)
set(SRCS ${EXTERNAL_SRCS} ${INTERNAL_SRCS})
set(LIBS qhullWrapper ${QtQmlQ3dLibs} ${qtuser} trimesh2 UserSettings mmesh
		cmesh fmesh quazip zlib libqrencode libnest2d clipper3r nestplacer 
		nlopt clipper splitslot OpenMP::OpenMP_CXX 
		sensors_analytics_sdk
		curl cxmdns cxnet cpr imageproc
		)

set(DEFS BASIC_KERNEL_DLL)
set(INCS ${CMAKE_CURRENT_SOURCE_DIR}/external/)
# __enable_cxx17()
__cc_find(RapidJson)
 
__add_real_target(BasicKernel dll 	SOURCE ${SRCS} 
									LIB ${LIBS}
									DEF ${DEFS}
									INC ${INCS} ${BOOST_INCLUDE_DIRS} ${RAPIDJSON_INCLUDE_DIRS}
									INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/external/
									)
if(CC_BC_LINUX)
	target_link_libraries(BasicKernel PRIVATE stdc++ stdc++fs curl zlib pthread)
endif()



