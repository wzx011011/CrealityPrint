import QtQuick 2.10
import QtQuick.Controls 2.0

Rectangle {
    color: Constants.itemBackgroundColor
    width: 140
    height: 200
    radius: 5
    property  int buttonMargin: 5
    property var control
    Label {
        id: mirror_label
        width: 100
        height: 30
        color:Constants.textColor //"#00FFFF"
        Text{
            text: qsTr("Mirror");
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.verticalCenter: parent.verticalCenter;
            font.pixelSize: 18
        }
        anchors.horizontalCenter: parent.horizontalCenter
    }
    Button {
        id: x_mirror
        width: mirror_label.width
        height: mirror_label.height
        text: qsTr("X Mirror")
        font.pixelSize: mirror_label.font.pixelSize
        anchors.top: mirror_label.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: buttonMargin

        onClicked: {
            control.mirrorX()
        }
    }

    Button {
        id: y_mirror
        width: x_mirror.width
        height: x_mirror.height
        text: qsTr("Y Mirror")
        font.pixelSize: x_mirror.font.pixelSize
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: buttonMargin
        anchors.top: x_mirror.bottom

        onClicked: {
            control.mirrorY()
        }
    }

    Button {
        id: z_mirror
        width: x_mirror.width
        height: x_mirror.height
        text: qsTr("Z Mirror")
        font.pixelSize: x_mirror.font.pixelSize
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: buttonMargin
        anchors.top: y_mirror.bottom

        onClicked: {
            control.mirrorZ()
        }
    }

    Button {
        id: reset
        width: x_mirror.width
        height: x_mirror.height
        text:  "<font color='#000000'>" + qsTr("Reset") + "</font>"
        font.pixelSize: x_mirror.font.pixelSize
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: buttonMargin
        anchors.top: z_mirror.bottom
        onClicked: {
            control.reset()
        }
    }

}
