file(GLOB SRCS *.h *.cpp)
set(CMAKE_CXX_STANDARD 17)

file(GLOB QRC_SRC *.qrc)
qt5_add_resources(RCC_SRC ${QRC_SRC})

set(LIBS Qt5::Core Qt5::Gui Qt5::Widgets)
set(INCS )

__add_real_target(plugin_log dll SOURCE ${SRCS} ${RCC_SRC} LIB ${LIBS} INC ${INCS} FOLDER plugin)
