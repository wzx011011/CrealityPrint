import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15

import CrealityUI 1.0

import "qrc:/CrealityUI"

Window {
    id: root

    x: (Screen.width - root.width) / 2
    y: (Screen.height - root.height) / 2
    width: 860 * screenScaleFactor
    height: 480 * screenScaleFactor

    modality: Qt.ApplicationModal
    flags: Qt.SplashScreen
    color: "transparent"

    Image {
        id: image
        anchors.fill: parent
        sourceSize.width: width
        sourceSize.height: height
        source: "qrc:/scence3d/res/splash.png"
    }

}
