set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if(NOT TS_FILES)
	SET(TS_FILES
    ../locales/en.ts
    ../locales/zh_CN.ts
    ../locales/zh_TW.ts
	)
endif()

qt5_add_translation(QM_FILES ${TS_FILES})


configure_file(resource.qrc ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(main.qml ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)


#add_definitions(-DCUS_SOURCE_DIR=${CMAKE_SOURCE_DIR})
set(SRCS
  main.cpp
  unittesttool.h
  unittesttool.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/resource.qrc
  ../plugins/CrealityUI/qml.qrc
)

set(DEFS
  BOOST_ALL_NO_LIB
)

set(LIBS Qt5::Svg Qt5::Core Qt5::Quick Qt5::Qml)



__add_real_target(unittesttool exe
  SOURCE ${SRCS}
  LIB ${LIBS}
  DEF ${DEFS}
  DISABLE_VLD
)

if(CXX_VLD_ENABLED)
	message(STATUS "CXX_VLD_ENABLED --------> : ${CMAKE_CURRENT_SOURCE_DIR}")
	message(STATUS "CXX_VLD_ENABLED BIN_OUTPUT_DIR --------> : ${BIN_OUTPUT_DIR}")
	add_custom_command(TARGET unittesttool PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E make_directory ${BIN_OUTPUT_DIR}/Debug/
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/vld_config/vld_parse.py ${BIN_OUTPUT_DIR}/Debug/
		)
endif()
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/uinttool.in
               ${CMAKE_BINARY_DIR}/unittool.h)
