find_package(Qt5 COMPONENTS Core SerialPort LinguistTools REQUIRED)

if(WIN32)
	#message(STATUS "Using windres")
	set(RES_FILES "Creative3D.rc")
	ENABLE_LANGUAGE(RC)
	if(NOT MSVC)
		SET(CMAKE_RC_COMPILER_INIT windres)
		SET(CMAKE_RC_COMPILE_OBJECT
			"<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>"
			)
	endif()
endif(WIN32)

set(SRCS src/main.cpp
		 src/scence3dapp.h
		 src/scence3dapp.cpp
		 src/qmlworkflow.h
		 src/qmlworkflow.cpp
		 src/MiniDump.h
		 src/MiniDump.cpp
		 src/creativeplugincenter.h
		 src/creativeplugincenter.cpp
		 )
		 
if (WIN32)
    list(APPEND SRCS ${RES_FILES})
endif(WIN32)

SET(TS_FILES
../locales/en.ts
../locales/zh_CN.ts
../locales/zh_TW.ts
)

qt5_add_resources(QT_QRC scence3d.qrc)

find_package(Qt5QuickCompiler)
qtquick_compiler_add_resources(RESOURCES scence3d.qrc)

if(GEN_TRANSLATIONS_TS)
    message(STATUS "GEN_TRANSLATIONS_TS")
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR}/plugins ${CMAKE_SOURCE_DIR}/kernel ${CMAKE_SOURCE_DIR}/Creative3D ${TS_FILES})
else()
    qt5_add_translation(QM_FILES ${TS_FILES})
endif()

if(APPLE)
    set(MACOSX_BUNDLE_ICON_FILE Creative3D.icns)
    set(myApp_ICON ${CMAKE_CURRENT_SOURCE_DIR}/Creative3D.icns)
    set_source_files_properties(${myApp_ICON} PROPERTIES
           MACOSX_PACKAGE_LOCATION "Resources")
    list(APPEND SRCS ${myApp_ICON})
endif()

configure_file(translations.qrc ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
qt5_add_resources(MAIN_QRC ${CMAKE_CURRENT_BINARY_DIR}/translations.qrc)

if(CC_BC_LINUX)
	set(AppDir ${CMAKE_INSTALL_PREFIX})
	
	install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/default.desktop DESTINATION ${AppDir})
	install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/default.png DESTINATION ${AppDir})
    install(DIRECTORY "${CMAKE_SOURCE_DIR}/resources" DESTINATION ${AppDir})
endif()


set(LIBS ${QtQmlQ3dLibs} ${qtuser} CreativeKernel  BasicKernel UserSettings EngineWrapper qcxffmpeg trimesh2 Qt5::SerialPort)
__add_real_target(${PROJECT_NAME} exe SOURCE ${SRCS} ${MAIN_QRC} ${QT_QRC}
											  LIB ${LIBS} 
											  INC ${PROJECT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src
											  MAC_OUTPUT_NAME ${PROJECT_NAME}
											  MAC_GUI_IDENTIFIER com.creality.creative3d
											  QML_PLUGINS CrealityUI
											  DEPLOYQT
											  )

list(APPEND QML_IMPORT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../plugins")
set(QML_IMPORT_PATH ${QML_IMPORT_PATH}
    CACHE STRING "Qt Creator extra qml import paths"
    FORCE
)

	
if(CC_BC_WIN)	
	target_sources(${PROJECT_NAME} PRIVATE dpi-aware.manifest)
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		TARGET_COMPILE_DEFINITIONS(${PROJECT_NAME}
             PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
	endif()
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    			COMMAND ${CMAKE_COMMAND} -E copy_directory
    			${CMAKE_SOURCE_DIR}/resources/
    			"$<TARGET_FILE_DIR:${PROJECT_NAME}>/resources/"
    			)
elseif(CC_BC_MAC)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/resources/
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/../Resources/resources/"
            )
endif()

foreach(plugin ${Creative3D_plugins})
	add_dependencies(${PROJECT_NAME} ${plugin})
endforeach()
