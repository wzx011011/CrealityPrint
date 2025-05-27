IF (CMAKE_BUILD_TYPE MATCHES "Release")
	set(DESKTOP_LINK_NAME "Creality Print")
    string(REPLACE " " "_" CPACK_FILE_NAME_NO_SPACES "${DESKTOP_LINK_NAME}")
	macro(prepareNSIS_Link linkName appName params) 
		#prepare start menu links 
		LIST(APPEND CPACK_NSIS_CREATE_ICONS_EXTRA " CreateShortCut '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\${linkName}.lnk' '$INSTDIR\\\\${appName}.exe' '${params}'") 
		LIST(APPEND CPACK_NSIS_DELETE_ICONS_EXTRA " Delete '$SMPROGRAMS\\\\$START_MENU\\\\${linkName}.lnk'") 
	
		#prepare desktop links 
		LIST(APPEND CPACK_NSIS_CREATE_ICONS_EXTRA " CreateShortCut '$DESKTOP\\\\${linkName}.lnk' '$INSTDIR\\\\${appName}.exe' '${params}'") 
		LIST(APPEND CPACK_NSIS_DELETE_ICONS_EXTRA " Delete '$DESKTOP\\\\${linkName}.lnk'") 

        SET(CPACK_SOFEWARE_PROCESS_NAME "${appName}.exe")
		LIST(APPEND CPACK_ONINIT_DELETE_DESTOP_LINK "Delete '$DESKTOP\\\\${linkName}.lnk'") 
	endmacro() 

    #option(PROJECT_VERSION_MAJOR "option for version major" 0)
    #option(PROJECT_VERSION_MINOR "option for version minor" 0)
    #option(PROJECT_VERSION_PATCH "option for patch" 1)
	#option(PROJECT_BUILD_ID "option for build" 1)
	#option(PROJECT_VERSION_EXTRA "rc beta alpha" "alpha")
    
    SET(CMAKE_INSTALL_PREFIX ${RUNTIME_OUTPUT_DIRECTORY_RELEASE})
    
    SET(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}.${PROJECT_BUILD_ID}")
    message(STATUS ${PROJECT_VERSION_MAJOR})
    SET(CPACK_GENERATOR "TBZ2")
    SET(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
    SET(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
    SET(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
    SET(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
    SET(CPACK_PACKAGE_VENDOR "${PROJECT_VENDOR}")
    SET(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/${README_FILE}")
    SET(PROJECT_ICONS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/package/icon")
    set(LICENSE_FILE "license.rtf")
    set(LICENSE_FILE_EN "license_en.rtf")
    set(LICENSE_FILE_TD "license.rtf")

    IF(CC_BC_WIN)
		#INSTALL(DIRECTORY "${BIN_OUTPUT_DIR}/Release/fonts" DESTINATION .)
		INSTALL(DIRECTORY "${CMAKE_SOURCE_DIR}/resources" DESTINATION .)
		if(CMAKE_CL_64)
            #SET(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-v${PROJECT_VERSION}-win64-x64-${PROJECT_VERSION_EXTRA}")
            set(CPACK_SYSTEM_NAME "win64")
            #FIND_PROGRAM(VCREDIST_X64 vcredist_x64.exe PATH runtime/x64)
			FILE(GLOB RUNTIME_DLL ${CMAKE_CURRENT_SOURCE_DIR}/package/runtime/x64/*.dll ${CMAKE_CURRENT_SOURCE_DIR}/package/runtime/x64/*.exe)
            INSTALL(FILES ${RUNTIME_DLL} DESTINATION .)
            #IF(VCREDIST_X64) 
            LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "ExecWait '\\\"$INSTDIR\\\\vcredist_x64.exe\\\" /q'")
            #ENDIF(VCREDIST_X64)
        else()
            #SET(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}-win32-x86-${PROJECT_VERSION_EXTRA}")
            set(CPACK_SYSTEM_NAME "win32")
            #FIND_PROGRAM(VCREDIST_X86 vcredist_x86.exe PATH runtime/x86)
            #IF(VCREDIST_X86)
			FILE(GLOB RUNTIME_DLL ${CMAKE_CURRENT_SOURCE_DIR}/package/runtime/x86/*.dll ${CMAKE_CURRENT_SOURCE_DIR}/package/runtime/x86/*.exe)
            INSTALL(FILES  ${RUNTIME_DLL} DESTINATION .)
            LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "ExecWait '\\\"$INSTDIR\\\\vcredist_x86.exe\\\" /q'")
            #ENDIF(VCREDIST_X86)
        endif()
        SET(CPACK_GENERATOR "NSIS")
		set(CPACK_NSIS_PACKAGE_NAME "${DESKTOP_LINK_NAME}")
		set(CPACK_NSIS_DISPLAY_NAME "${DESKTOP_LINK_NAME}")
        SET(CPACK_PACKAGE_EXECUTABLES "${PROJECT_NAME}" "${PROJECT_NAME}")
        SET(CPACK_PACKAGE_INSTALL_DIRECTORY "${DESKTOP_LINK_NAME}")

        SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/${LICENSE_FILE}")
        SET(CPACK_RESOURCE_FILE_LICENSE_EN "${CMAKE_SOURCE_DIR}/${LICENSE_FILE_EN}")
        SET(CPACK_RESOURCE_FILE_LICENSE_TD "${CMAKE_SOURCE_DIR}/${LICENSE_FILE_TD}")
        SET(CPACK_NSIS_EXECUTABLES_DIRECTORY ".")
        SET(CPACK_NSIS_MUI_ICON "${PROJECT_ICONS_DIRECTORY}/NSIS.ico")
        SET(CPACK_PACKAGE_ICON "${PROJECT_ICONS_DIRECTORY}\\\\NSISHeader.ico")
        SET(CPACK_NSIS_URL_INFO_ABOUT "http://${PROJECT_DOMAIN}")
        SET(CPACK_NSIS_INSTALLED_ICON_NAME "${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}")
        SET(CPACK_NSIS_MENU_LINKS "${LICENSE_FILE}" "License")
        SET(CPACK_NSIS_MUI_FINISHPAGE_RUN "${CPACK_NSIS_INSTALLED_ICON_NAME}")
		set(CPACK_NSIS_MUI_WELCOMEFINISHPAGE_BITMAP ${PROJECT_ICONS_DIRECTORY}\\\\creality_banner_nsis.bmp)    # note: fails with forward '/'
		set(CPACK_NSIS_MUI_UNWELCOMEFINISHPAGE_BITMAP ${PROJECT_ICONS_DIRECTORY}\\\\creality_banner_nsis.bmp)
	    set(PRODUCT_DIR_REGKEY "Software\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\App Paths\\\\dpinst-x86.exe")
		set(PROGRAM_CMD "$INSTDIR\\\\${PROJECT_NAME}.exe -e %1")
		
		#include 
		
		SET(CPACK_NSIS_ADDITIONAL_SCRIPT "!include 'nsProcess.nsh'")
		SET(CPACK_PACKAGE_UNICODE "Unicode True") #Multilingual garbled
		#SET(CPACK_NSIS_DEFINES "!include 'nsProcess.nsh'")

		
		#install
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKLM '${PRODUCT_DIR_REGKEY}' '' '$INSTDIR\\\\${PROJECT_NAME}.exe'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR '.stl' '' 'stl_cxsw'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS " WriteRegStr HKCR '.obj' '' 'obj_cxsw'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS " WriteRegStr HKCR '.gcode' '' 'gcode_cxsw'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS " WriteRegStr HKCR '.cxprj' '' 'cx3d_cxsw'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS " WriteRegStr HKCR '.bmp' '' 'bmp_cxsw'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS " WriteRegStr HKCR '.jpg' '' 'jpg_cxsw'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS " WriteRegStr HKCR '.jpeg' '' 'jpeg_cxsw'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS " WriteRegStr HKCR '.png' '' 'png_cxsw'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS " WriteRegStr HKCR '.gtl' '' 'gtl_cxsw'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS " WriteRegStr HKCR '.gbl' '' 'gbl_cxsw'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS " WriteRegStr HKCR '.gbo' '' 'gbo_cxsw'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS " WriteRegStr HKCR '.gbp' '' 'gbp_cxsw'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS " WriteRegStr HKCR '.gbs' '' 'gbs_cxsw'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS " WriteRegStr HKCR '.gko' '' 'gko_cxsw'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS " WriteRegStr HKCR '.gml' '' 'gml_cxsw'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS " WriteRegStr HKCR '.gpb' '' 'gpb_cxsw'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS " WriteRegStr HKCR '.gpt' '' 'gpt_cxsw'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS " WriteRegStr HKCR '.gto' '' 'gto_cxsw'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS " WriteRegStr HKCR '.gtp' '' 'gtp_cxsw'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS " WriteRegStr HKCR '.gts' '' 'gts_cxsw'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS " WriteRegStr HKCR '.stp' '' 'stp_cxsw'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS " WriteRegStr HKCR '.step' '' 'step_cxsw'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS " WriteRegStr HKCR '.dae' '' 'dae_cxsw'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS " WriteRegStr HKCR '.3mf' '' '3mf_cxsw'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS " WriteRegStr HKCR '.3ds' '' '3ds_cxsw'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS " WriteRegStr HKCR '.iges' '' 'iges_cxsw'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS " WriteRegStr HKCR '.wrl' '' 'wrl_cxsw'")
		
		##add stl
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'stl_cxsw' '' 'model (.stl)'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'stl_cxsw\\\\DefaultIcon' '' '$INSTDIR\\\\${PROJECT_NAME}.exe,1'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'stl_cxsw\\\\shell\\\\open\\\\command' '' '${PROGRAM_CMD}'")
		
		##add obj
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'obj_cxsw' '' 'model (.obj)'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'obj_cxsw\\\\DefaultIcon' '' '$INSTDIR\\\\${PROJECT_NAME}.exe,2'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'obj_cxsw\\\\shell\\\\open\\\\command' '' '${PROGRAM_CMD}'")
		
		##add cx3d
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'cx3d_cxsw' '' 'project (.cxprj)'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'cx3d_cxsw\\\\DefaultIcon' '' '$INSTDIR\\\\${PROJECT_NAME}.exe,3'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'cx3d_cxsw\\\\shell\\\\open\\\\command' '' '${PROGRAM_CMD}'")
		
		##add gcode
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'gcode_cxsw' '' 'G-gcode'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'gcode_cxsw\\\\DefaultIcon' '' '$INSTDIR\\\\${PROJECT_NAME}.exe,4'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'gcode_cxsw\\\\shell\\\\open\\\\command' '' '${PROGRAM_CMD}'")

        ##add bmp
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'bmp_cxsw' '' 'model (.bmp)'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'bmp_cxsw\\\\DefaultIcon' '' '$INSTDIR\\\\${PROJECT_NAME}.exe,5'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'bmp_cxsw\\\\shell\\\\open\\\\command' '' '${PROGRAM_CMD}'")

        ##add jpg
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'jpg_cxsw' '' 'model (.jpg)'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'jpg_cxsw\\\\DefaultIcon' '' '$INSTDIR\\\\${PROJECT_NAME}.exe,6'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'jpg_cxsw\\\\shell\\\\open\\\\command' '' '${PROGRAM_CMD}'")

        ##add jpeg
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'jpeg_cxsw' '' 'model (.jpeg)'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'jpeg_cxsw\\\\DefaultIcon' '' '$INSTDIR\\\\${PROJECT_NAME}.exe,7'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'jpeg_cxsw\\\\shell\\\\open\\\\command' '' '${PROGRAM_CMD}'")

        ##add png
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'png_cxsw' '' 'model (.png)'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'png_cxsw\\\\DefaultIcon' '' '$INSTDIR\\\\${PROJECT_NAME}.exe,8'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'png_cxsw\\\\shell\\\\open\\\\command' '' '${PROGRAM_CMD}'")

        ##add gtl
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'gtl_cxsw' '' 'model (.gtl)'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'gtl_cxsw\\\\DefaultIcon' '' '$INSTDIR\\\\${PROJECT_NAME}.exe,9'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'gtl_cxsw\\\\shell\\\\open\\\\command' '' '${PROGRAM_CMD}'")

        ##add gbl
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'gbl_cxsw' '' 'model (.gbl)'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'gbl_cxsw\\\\DefaultIcon' '' '$INSTDIR\\\\${PROJECT_NAME}.exe,10'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'gbl_cxsw\\\\shell\\\\open\\\\command' '' '${PROGRAM_CMD}'")

        ##add gbo
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'gbo_cxsw' '' 'model (.gbo)'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'gbo_cxsw\\\\DefaultIcon' '' '$INSTDIR\\\\${PROJECT_NAME}.exe,11'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'gbo_cxsw\\\\shell\\\\open\\\\command' '' '${PROGRAM_CMD}'")

        ##add gbp
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'gbp_cxsw' '' 'model (.gbp)'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'gbp_cxsw\\\\DefaultIcon' '' '$INSTDIR\\\\${PROJECT_NAME}.exe,12'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'gbp_cxsw\\\\shell\\\\open\\\\command' '' '${PROGRAM_CMD}'")

        ##add gbs
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'gbs_cxsw' '' 'model (.gbs)'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'gbs_cxsw\\\\DefaultIcon' '' '$INSTDIR\\\\${PROJECT_NAME}.exe,13'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'gbs_cxsw\\\\shell\\\\open\\\\command' '' '${PROGRAM_CMD}'")

        ##add gko
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'gko_cxsw' '' 'model (.gko)'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'gko_cxsw\\\\DefaultIcon' '' '$INSTDIR\\\\${PROJECT_NAME}.exe,14'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'gko_cxsw\\\\shell\\\\open\\\\command' '' '${PROGRAM_CMD}'")

        ##add gml
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'gml_cxsw' '' 'model (.gml)'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'gml_cxsw\\\\DefaultIcon' '' '$INSTDIR\\\\${PROJECT_NAME}.exe,15'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'gml_cxsw\\\\shell\\\\open\\\\command' '' '${PROGRAM_CMD}'")

        ##add gpb
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'gpb_cxsw' '' 'model (.gpb)'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'gpb_cxsw\\\\DefaultIcon' '' '$INSTDIR\\\\${PROJECT_NAME}.exe,16'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'gpb_cxsw\\\\shell\\\\open\\\\command' '' '${PROGRAM_CMD}'")

        ##add gpt
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'gpt_cxsw' '' 'model (.gpt)'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'gpt_cxsw\\\\DefaultIcon' '' '$INSTDIR\\\\${PROJECT_NAME}.exe,17'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'gpt_cxsw\\\\shell\\\\open\\\\command' '' '${PROGRAM_CMD}'")

        ##add gto
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'gto_cxsw' '' 'model (.gto)'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'gto_cxsw\\\\DefaultIcon' '' '$INSTDIR\\\\${PROJECT_NAME}.exe,18'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'gto_cxsw\\\\shell\\\\open\\\\command' '' '${PROGRAM_CMD}'")

        ##add gtp
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'gtp_cxsw' '' 'model (.gtp)'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'gtp_cxsw\\\\DefaultIcon' '' '$INSTDIR\\\\${PROJECT_NAME}.exe,18'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'gtp_cxsw\\\\shell\\\\open\\\\command' '' '${PROGRAM_CMD}'")

        ##add gts
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'gts_cxsw' '' 'model (.gts)'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'gts_cxsw\\\\DefaultIcon' '' '$INSTDIR\\\\${PROJECT_NAME}.exe,19'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'gts_cxsw\\\\shell\\\\open\\\\command' '' '${PROGRAM_CMD}'")

        ##add stp
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'stp_cxsw' '' 'model (.stp)'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'stp_cxsw\\\\DefaultIcon' '' '$INSTDIR\\\\${PROJECT_NAME}.exe,20'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'stp_cxsw\\\\shell\\\\open\\\\command' '' '${PROGRAM_CMD}'")

        ##add step
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'step_cxsw' '' 'model (.step)'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'step_cxsw\\\\DefaultIcon' '' '$INSTDIR\\\\${PROJECT_NAME}.exe,21'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'step_cxsw\\\\shell\\\\open\\\\command' '' '${PROGRAM_CMD}'")

        ##add dae
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'dae_cxsw' '' 'model (.dae)'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'dae_cxsw\\\\DefaultIcon' '' '$INSTDIR\\\\${PROJECT_NAME}.exe,22'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'dae_cxsw\\\\shell\\\\open\\\\command' '' '${PROGRAM_CMD}'")

        ##add 3mf
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR '3mf_cxsw' '' 'model (.3mf)'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR '3mf_cxsw\\\\DefaultIcon' '' '$INSTDIR\\\\${PROJECT_NAME}.exe,23'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR '3mf_cxsw\\\\shell\\\\open\\\\command' '' '${PROGRAM_CMD}'")

        ##add 3ds
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR '3ds_cxsw' '' 'model (.3ds)'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR '3ds_cxsw\\\\DefaultIcon' '' '$INSTDIR\\\\${PROJECT_NAME}.exe,24'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR '3ds_cxsw\\\\shell\\\\open\\\\command' '' '${PROGRAM_CMD}'")

        ##add iges
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'iges_cxsw' '' 'model (.iges)'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'iges_cxsw\\\\DefaultIcon' '' '$INSTDIR\\\\${PROJECT_NAME}.exe,25'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'iges_cxsw\\\\shell\\\\open\\\\command' '' '${PROGRAM_CMD}'")

        ##add wrl
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'wrl_cxsw' '' 'model (.wrl)'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'wrl_cxsw\\\\DefaultIcon' '' '$INSTDIR\\\\${PROJECT_NAME}.exe,26'")
		LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKCR 'wrl_cxsw\\\\shell\\\\open\\\\command' '' '${PROGRAM_CMD}'")

        LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "CreateDirectory $APPDATA\\\\Creality")
        LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "CreateDirectory $APPDATA\\\\Creality\\\\${PROJECT_NAME}")
        LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "CreateDirectory $APPDATA\\\\Creality\\\\${PROJECT_NAME}\\\\resources")
        LIST(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "CopyFiles '$INSTDIR\\\\resources\\\\' '$APPDATA\\\\${PROJECT_NAME}\\\\resources'")
		prepareNSIS_Link("${DESKTOP_LINK_NAME}" "${PROJECT_NAME}" "")
		STRING (REPLACE ";" "\n" CPACK_NSIS_CREATE_ICONS_EXTRA "${CPACK_NSIS_CREATE_ICONS_EXTRA}") 
		STRING (REPLACE ";" "\n" CPACK_NSIS_DELETE_ICONS_EXTRA "${CPACK_NSIS_DELETE_ICONS_EXTRA}") 
        STRING (REPLACE ";" "\n" CPACK_NSIS_EXTRA_INSTALL_COMMANDS "${CPACK_NSIS_EXTRA_INSTALL_COMMANDS}")
		
		#unstall
		 #LIST(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "\${nsProcess::KillProcess} 'Creative3D.exe' $R0")
		 #LIST(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "Sleep 3000")
		 LIST(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "SetShellVarContext current")
		 LIST(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "DeleteRegKey HKCU 'Software\\Creality\\Creative3D'")

		 LIST(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "DeleteRegKey HKCR '.stl'")
		 LIST(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "DeleteRegKey HKCR '.obj'")
		 LIST(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "DeleteRegKey HKCR '.cxprj'")
		 LIST(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "DeleteRegKey HKCR '.gcode'")
		 LIST(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "DeleteRegKey HKCR '.bmp'")
		 LIST(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "DeleteRegKey HKCR '.jpg'")
		 LIST(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "DeleteRegKey HKCR '.png'")
		 LIST(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "DeleteRegKey HKCR '.gtl'")
		 LIST(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "DeleteRegKey HKCR '.gbl'")
		 LIST(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "DeleteRegKey HKCR '.gbo'")
		 LIST(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "DeleteRegKey HKCR '.gbp'")
		 LIST(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "DeleteRegKey HKCR '.gbs'")
		 LIST(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "DeleteRegKey HKCR '.gko'")
		 LIST(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "DeleteRegKey HKCR '.gml'")
		 LIST(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "DeleteRegKey HKCR '.gpb'")
		 LIST(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "DeleteRegKey HKCR '.gpt'")
		 LIST(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "DeleteRegKey HKCR '.gto'")
		 LIST(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "DeleteRegKey HKCR '.gtp'")
		 LIST(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "DeleteRegKey HKCR '.gts'")
		 LIST(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "DeleteRegKey HKCR '.stp'")
		 LIST(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "DeleteRegKey HKCR '.step'")
		 LIST(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "DeleteRegKey HKCR '.dae'")
		 LIST(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "DeleteRegKey HKCR '.3mf'")
		 LIST(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "DeleteRegKey HKCR '.3ds'")
		 LIST(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "DeleteRegKey HKCR '.iges'")
		 LIST(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "DeleteRegKey HKCR '.wrl'")

		 LIST(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "RMDir /r '$APPDATA\\\\Creality\\\\Creative3D'")
		 LIST(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "RMDir /r '$LOCALAPPDATA\\\\Creality\\\\Creative3D'")
         LIST(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "RMDir /r '$INSTDIR\\resources'")
		 LIST(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "SetShellVarContext all")
		 LIST(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "Delete '$INSTDIR\\\\*.csv'")
		 LIST(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "Delete '$INSTDIR\\\\*_show'")
		 STRING (REPLACE ";" "\n" CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "${CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS}")
		 set(CPACK_NSIS_INSTALLER_MUI_FINISHPAGE_RUN_CODE "!define MUI_FINISHPAGE_RUN \\\"$WINDIR\\\\explorer.exe\\\"\n!define MUI_FINISHPAGE_RUN_PARAMETERS \\\"$INSTDIR\\\\${PROJECT_NAME}.exe\\\"")	
    ELSEIF(CC_BC_MAC)
		include(package/macx) 
    ELSEIF(CC_BC_LINUX)
    ENDIF()

    # install MSVC redistributables and files listed in CMAKE_INSTALL_DEBUG_LIBRARIES
    set(CPACK_PACKAGE_FILE_NAME "${CPACK_FILE_NAME_NO_SPACES}-v${PROJECT_VERSION}-${CPACK_SYSTEM_NAME}-${PROJECT_VERSION_EXTRA}")
	set(SIGN_CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_FILE_NAME})
    INCLUDE(CPack)
ENDIF()
