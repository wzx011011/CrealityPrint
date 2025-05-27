set(AppDir ${CMAKE_INSTALL_PREFIX})

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/package/linux/default.desktop DESTINATION ${AppDir})
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/package/linux/default.png DESTINATION ${AppDir})
