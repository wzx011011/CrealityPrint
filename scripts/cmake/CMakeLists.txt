option(SIGN_PACKAGE OFF)
option(BUILD_PACKAGE ON)

message(STATUS "Package Projects.")
if(BUILD_PACKAGE OR SIGN_PACKAGE)
    message(STATUS "Package Projects")
    add_custom_target(packaging ALL DEPENDS Creative3D COMMENT "Packaging Projects...")
    include(package/Package)
endif()

if(SIGN_PACKAGE)
    add_custom_target(signing ALL DEPENDS packaging COMMENT "Signing Package...")

    if(BUILD_OS_WINDOWS)
        include(signing/windows.cmake)
    elseif(BUILD_OS_OSX)
        include(signing/osx.cmake)
    elseif(BUILD_OS_LINUX)
        include(signing/linux.cmake)
    else()
        message(STATUS "Do not know how to sign the executable for the current platform! You will need to perform signing manually.")
    endif()
endif()

