import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.13
import CrealityUI 1.0
import "qrc:/CrealityUI"

Rectangle{
    property real headerHeight: 30
    property var headTexts
//    property var headTexts: [qsTr("Printer"), qsTr("Importing Files"), qsTr("File Name"), qsTr("IP Address"), qsTr("Progress"), qsTr("State"), ""]
//    property var rowWidths: [width*0.1, width*0.1, width*0.15,
//        width*0.15, width*0.1, width*0.2, width*0.2]

    id:header
    width: parent.width
    height: headerHeight

    Row{
        anchors.fill: parent
        spacing: 0
        Repeater{
            model: headTexts
            delegate: Item{
                width: rowWidths[index]
                height: headerHeight
                CusText{
                    anchors.centerIn: parent
                    fontWidth: rowWidths[index]
                    fontText: modelData//headTexts[index]
                    fontColor: Constants.textColor//"#ffffff"
                    fontPixelSize: 12
                }
            }
        }
    }
}


