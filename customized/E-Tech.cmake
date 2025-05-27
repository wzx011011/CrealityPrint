message(STATUS "build E-Tech project")

AppendCustomizedMarco(CUSTOM_MACHINE_LIST)
AppendCustomizedMarco(CUSTOM_KEY)
AppendCustomizedMarco(CUSTOM_UN_UPGRADEABLE)
AppendCustomizedMarco(CUSTOM_UN_FEEDBACKABLE)
AppendCustomizedMarco(CUSTOM_UN_TEACHABLE)
AppendCustomizedMarco(CUSTOM_HOLLOW_ENABLED)
AppendCustomizedMarco(CUSTOM_HOLLOW_FILL_ENABLED)
AppendCustomizedMarco(CUSTOM_SLICE_HEIGHT_SETTING_ENABLED)
AppendCustomizedMarco(CUSTOM_PARTITION_PRINT_ENABLED)

set(PROJECT_NAME "E-Tech")
set(BUNDLE_NAME "E-Tech Slicer")
set(myApp_ICON ${CMAKE_SOURCE_DIR}/customized/E-Tech/E-Tech.icns)
set(RES_FILES ${CMAKE_SOURCE_DIR}/customized/E-Tech/E-Tech.rc)
set(PACKAGE_CONF ${CMAKE_SOURCE_DIR}/customized/E-Tech/Package.cmake)
set(SPLASH_URL ${CMAKE_SOURCE_DIR}/customized/E-Tech/splash.png)
set(LOGO_URL ${CMAKE_SOURCE_DIR}/customized/E-Tech/logo.png)
set(MACHINE_LIST ${CMAKE_SOURCE_DIR}/customized/E-Tech/machineList_custom.json)

set(PROFILE_KEY ${CMAKE_SOURCE_DIR}/customized/E-Tech/keys_custom)
set(PROFILE_KEY_SHELL ${CMAKE_SOURCE_DIR}/customized/E-Tech/keys-shell_custom)
set(TS_FILES
  ${CMAKE_SOURCE_DIR}/locales/en.ts
  ${CMAKE_SOURCE_DIR}/locales/zh_CN.ts
  ${CMAKE_SOURCE_DIR}/locales/zh_TW.ts
)
