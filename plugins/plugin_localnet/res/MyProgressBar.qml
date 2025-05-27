import QtQuick 2.0
import QtQuick.Controls 2.5
ProgressBar{
    property color normalColor: "#e6e6e6"
    property color finishedColor: "#17a81a"
    property bool txtVisible: true
    id: control
    from : 0
    to : 100
    //    property var value: 10
    padding: 2
    background: Rectangle {
        anchors.fill: parent
        color: normalColor
        radius: 3
    }

    contentItem: Item{
        anchors.fill: parent
        Rectangle {
            width: control.visualPosition * parent.width
            height: parent.height
            radius: 2
            color: finishedColor
        }

        Text {
            id: text1
            color: "blue"
            visible: txtVisible
            text: control.value + "%" //Math.floor(control.value * 100) + "%"
            z: 2
            anchors.centerIn: parent
        }
    }
}
