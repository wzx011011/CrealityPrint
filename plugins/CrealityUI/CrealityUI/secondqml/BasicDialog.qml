import QtQuick 2.0
import QtQuick.Controls 2.12
//import QtQuick.Layouts 1.3
import QtQuick.Window 2.0
import QtGraphicalEffects 1.12
import ".."
import "../qml"
Window {
    id: eo_askDialog
    width: 300
    height: 200
    property string title: "baisc dialog"
    property var titleIcon: ""//"qrc:/UI/photo/LOGO.png"
    property var closeIcon: closeButton.hovered ? "qrc:/UI/photo/closeBtn_d.png" : "qrc:/UI/photo/closeBtn.png"//"qrc:/UI/photo/closeBtn.png"
    // property int titleIconWidth: 30     //defalut 30
    property string titleBackground:  Constants.dialogTitleColor //"#061F3B"     //
    property string contentBackground: Constants.themeColor
    property string titleColor: Constants.dialogTitleTextColor
    property var updateBtnVisible: false
    property var shadowEnabled: true
    property var titleHeight: 30* screenScaleFactor

    flags: Qt.FramelessWindowHint | Qt.Dialog
    // A model window prevents other windows from receiving input events. Possible values are Qt.NonModal (the default), Qt.WindowModal, and Qt.ApplicationModal.
    modality: Qt.ApplicationModal
    color:"transparent" //Constants.themeColor

    property alias btnEnabled: closeButton.enabled
    property alias btnVisible: closeButton.visible

    signal dialogClosed()
    signal updateBtnClicked()

    Timer{
        id: idUpdateBtnTimer
        interval: 30000
        repeat: false
        onTriggered:{
            updateButton.enabled = true
        }
    }

    Rectangle{
        x:5
        y:5
        width: mainLayout.width
        implicitHeight: 30* screenScaleFactor
        color: titleBackground
        z:mainLayout.z + 1
        border.color: Constants.dialogBorderColor
        border.width: 1
        //标题栏
        Rectangle{
            border.width: 0
            anchors.fill: parent
            anchors.leftMargin: 1
            anchors.topMargin: 1
            anchors.rightMargin: 1
            anchors.bottomMargin: 0

            color: titleBackground
            layer.enabled: shadowEnabled
            layer.effect:
                DropShadow {
                verticalOffset: 2
                //        samples: 5
                color:Constants.dropShadowColor
            }
            id: titleBar
            //1.实现标题栏
            //        border.color: "cyan"
            //        border.width: 1
            MouseArea{
                id: mouseControler
                property point clickPos: "0,0"
                property var dialogX: "0"
                property var dialogY: "0"
                height: parent.height
                width: parent.width
                //title
                Image {
                    id: idTitleImg
                    anchors.left: parent.left
                    anchors.leftMargin: 5
                    source: titleIcon
                    anchors.verticalCenter: parent.verticalCenter
                }
                Text{
                    text: title
                    font.family: Constants.labelFontFamily
                    font.weight: Constants.labelFontWeight
                    font.pointSize: Constants.labelFontPointSize
                    anchors.left: parent.left
                    anchors.leftMargin: 10 + idTitleImg.width
                    anchors.verticalCenter: parent.verticalCenter
                    color: titleColor//"white"
                }

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
            Button{
                id: updateButton
                height: parent.height
                implicitWidth: 30
                anchors.right: closeButton.right
                anchors.rightMargin: 30
                visible: updateBtnVisible
                onClicked: {
                    console.log("update button clicked.");
                    updateBtnClicked()
                    updateButton.enabled = false
                    idUpdateBtnTimer.start()
                }
                contentItem: Item
                {
                    anchors.fill: parent
                    Image
                    {
                        anchors.centerIn: parent
                        width: 20
                        height: 17
                        source:  "qrc:/UI/photo/updateBtnImage.png"
                    }
                }

                background: Rectangle
                {
                    anchors.fill: parent
                    color: updateButton.hovered? "#ACACAC":"transparent"

                }
            }
            //close button
            Button{
                id: closeButton
                height: parent.height
                width: height
                anchors.right: parent.right
                // anchors.rightMargin: 10
                onClicked: {
                    console.log("close button clicked.");
                    eo_askDialog.visible = false;
                    dialogClosed()
                }
                contentItem: Item
                {
                    anchors.fill: parent
                    Image
                    {
                        anchors.centerIn: parent
                        width: 10
                        height: 10
                        source:  closeIcon
                    }
                }

                background: Rectangle
                {
                    anchors.fill: parent
                    color: closeButton.hovered? "red"/*"#3968E9"*/:"transparent"
                    //opacity: enabled ? 1 : 0.3

                }
            }
        }
    }
    

    Rectangle
    {
        id: mainLayout
        // 一个填满窗口的容器，Page、Rectangle 都可以
        anchors.fill: parent
        // 当窗口全屏时，设置边距为 0，则不显示阴影，窗口化时设置边距为 10 就可以看到阴影了
        //radius: 5
        anchors.margins: 5
        color: contentBackground//Constants.themeColor //"transparent"
        opacity: 1
        border.color: Constants.dialogBorderColor//"#061F3B"
        border.width: 1
    }

    DropShadow {
        anchors.fill: mainLayout
        radius: 8
        samples: 17
        source: mainLayout
        color:  Constants.dropShadowColor
    }

}
