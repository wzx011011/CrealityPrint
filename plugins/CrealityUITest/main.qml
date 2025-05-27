import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQml 2.3
//import CrealityUI 1.0
//import "qrc:/CrealityUI"

ApplicationWindow {
    id: standaloneWindow
    visible: true
    width: 1024//Constants.width
    height: 768//Constants.height
    title: qsTr("Creality Slicer UI Test Project!")

    HalotBoxTotalRightUIPanel{
        anchors.centerIn: parent
    }

    Component.onCompleted:
    {
    }
}
