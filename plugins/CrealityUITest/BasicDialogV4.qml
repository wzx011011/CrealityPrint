import QtQuick 2.0
import QtQuick.Controls 2.12
//import QtQuick.Layouts 1.3
import QtQuick.Window 2.0
import QtGraphicalEffects 1.12
import QtQml 2.3

Window {
    id: eo_askDialog
    property real shadowWidth: 3
    property string title: "baisc dialog"
    property var titleIcon: ""//"qrc:/UI/photo/LOGO.png"
    property var closeIcon: "qrc:/UI/photo/close.png"
    property var closeIcon_d: "qrc:/UI/photo/close_d.png"
    property string titleBackground:  Constants.dialogTitleColor
    property alias cloader: contentLoader
    property color titleColor: "black"
    property var titleHeight: 30
    property Component bdContentItem

    width: 300
    height: 200
    flags: Qt.FramelessWindowHint | Qt.Dialog

    modality: Qt.ApplicationModal
    color: "transparent"
    signal closing()
    signal afterClose()

    Rectangle{
        id: bgRec
        anchors.fill: parent
        anchors.margins: shadowWidth
        color: "#3f3f43"
        radius: 10
        //标题栏
        CusPopViewTitle{
            id: titleCom
            color: "#56565c"
            width: parent.width
            height: titleHeight
            leftTop: true
            rightTop: true
            fontColor: "#ffffff"
            clickedable: false
            title: eo_askDialog.title

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
                    eo_askDialog.setX(eo_askDialog.x+delta.x)
                    eo_askDialog.setY(eo_askDialog.y+delta.y)
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
        anchors.fill: bgRec
        radius: 8
        spread: 0.2
        samples: 17
        source: bgRec
        color:  "#000000"//"#cecece"
    }
}
