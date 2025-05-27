#file(GLOB QRC_SRC sliceconfig/*.qrc)
#qt5_add_resources(RCC_SRC ${QRC_SRC})

__files_group(${CMAKE_CURRENT_SOURCE_DIR} SRCS)
__cc_find(RapidJson)

list(APPEND SRCS ${RCC_SRC} ${QRC_SRC})
set(LIBS Qt5::Core qtuser_core ${openssl} ssl crypto)

if( WIN32 AND NOT CYGWIN )
	set(LIBS ${LIBS} ws2_32 crypt32)
endif()

set(DEFS USERSETTINGS_DLL)
set(INCS ${RAPIDJSON_INCLUDE_DIRS})

__add_real_target(UserSettings dll  SOURCE ${SRCS}
								    LIB ${LIBS}
									DEF ${DEFS} 
									INC ${INCS}
									INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
									)