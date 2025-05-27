TEMPLATE = lib
TARGET = CrealityUI
QT += qml quick
CONFIG += plugin c++11

TARGET = $$qtLibraryTarget($$TARGET)        #TARGET变量指定生成的目标库文件的名字，生成应用程序时即指定生成应用程序名
uri = CrealityUI


CONFIG (release, debug|release){
DESTDIR = $$_PRO_FILE_PWD_

}
CONFIG (debug, debug|release){
 DESTDIR =  $$_PRO_FILE_PWD_
 #OBJECTS_DIR = $$OUT_PWD /../../../binTest/Debug/
}


# Input
SOURCES +=         src/crealityui_plugin.cpp         src/crealityui.cpp

HEADERS +=         src/crealityui_plugin.h         src/crealityui.h

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) "$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)" "$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)"

    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

#qmldir.files = qmldir
unix {
    installPath = $$[QT_INSTALL_QML]/$$replace(uri, \., /)
    qmldir.path = $$installPath
    target.path = $$installPath
    INSTALLS += target qmldir
}

qmlsource.files += qml/*.qml
qmlsource.path =$$[QT_INSTALL_QML]/$${uri}/qml
installPath = $$[QT_INSTALL_QML]/$${uri}

win32 {
    installPath ~= s,/,\\,g
}
#qmldir.files = qmldir
qmldir.path = $$installPath
!android:INSTALLS += qmldir

!android:target.path = $$installPath
#INSTALLS += target qmlsource

RESOURCES += \
    qml.qrc



