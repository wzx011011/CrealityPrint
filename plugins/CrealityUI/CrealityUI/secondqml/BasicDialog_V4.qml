import QtQuick 2.0
import QtQuick.Controls 2.12
//import QtQuick.Layouts 1.3
import QtQuick.Window 2.0
import QtQml 2.3
import QtGraphicalEffects 1.12
import "../qml"
import "../secondqml"

Window {
    id: basic_Dialog
    property real borderWidth: 1
    property real shadowWidth: 3
    property real titleHeight: 30
    property string title: ""
    property string borderColor: "#262626"
    property string shadowColor: Constants.dropShadowColor
    property string titleBgColor: "#6E6E73"
    property string titleFtColor: "#FFFFFF"
    property string backgroundColor: "#4B4B4D"
    property Component bdContentItem
    property alias cloader: contentLoader
    property alias moveEnabled: mouseControler.enabled

    width: 300
    height: 200
    flags: Qt.FramelessWindowHint | Qt.Dialog

    modality: Qt.ApplicationModal
    color: "transparent"
    signal closing()
    //signal afterClose()

    Rectangle{
        id: bgRect
        anchors.fill: parent
        anchors.margins: shadowWidth
        border.color: borderColor
        border.width: borderWidth
        color: backgroundColor
        radius: 5
        //标题栏
        CusPopViewTitle_V4 {
            id: titleCom
            x: borderWidth
            y: borderWidth
            color: titleBgColor
            width: parent.width - borderWidth*2
            height: titleHeight - borderWidth
            leftTop: true
            rightTop: true
            fontColor: titleFtColor
            clickedable: false
            title: basic_Dialog.title

            MouseArea{
                id: mouseControler
                property point clickPos: "0,0"
                anchors.fill: parent
                anchors.right: parent.right
                anchors.rightMargin: 40

                onPressed: {
                    clickPos = Qt.point(mouse.x,mouse.y)
                }

                onPositionChanged: {
                    //鼠标偏移量motai
                    var delta = Qt.point(mouse.x-clickPos.x, mouse.y-clickPos.y)
                    //如果mainwindow继承自QWidget,用setPos
                    basic_Dialog.setX(basic_Dialog.x+delta.x)
                    basic_Dialog.setY(basic_Dialog.y+delta.y)
                }
            }
            onCloseClicked:{
                closing()
                close()
            }
        }

        Loader{
            id: contentLoader
            anchors.top: titleCom.bottom
            anchors.horizontalCenter: titleCom.horizontalCenter
            width: parent.width
            height: parent.height - titleHeight
            sourceComponent: bdContentItem
        }
    }

    DropShadow {
        anchors.fill: bgRect
        radius: 8
        spread: 0.2
        samples: 17
        source: bgRect
        color: shadowColor
    }
}
