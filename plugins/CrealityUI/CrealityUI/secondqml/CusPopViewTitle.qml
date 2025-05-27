import QtQuick 2.12
import "../qml"

CusRoundedBg{
    id: crb
    property alias title: ct.fontText
    property alias fontColor: ct.fontColor
    property real closeBtnWidth: 8
    property real closeBtnHeight: 8
    property color closeBtnNColor: "#56565c"
    property string closeIcon: "qrc:/UI/photo/close_normal.png"
    property string closeIcon_d: "qrc:/UI/photo/close_press.png"
    property bool isMaxed: false
    property bool maxBtnVis: true
    property bool closeBtnVis: true
    property var moveItem
    signal closeClicked()
    signal normalClicked()
    signal maxClicked()
    clickedable: false
    radius: 5
    borderColor: "#0a0a0a"

    CusText{
        id: ct
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.verticalCenter: parent.verticalCenter
        fontPointSize: 9
        fontColor: "#ffffff"
        hAlign: 0
    }

    MouseArea{
        id: mouseControler
        property point clickPos: "0,0"
        anchors.fill: parent

        onPressed: {
            clickPos = Qt.point(mouse.x,mouse.y)
        }

        onPositionChanged: {
            //鼠标偏移量motai
            var delta = Qt.point(mouse.x-clickPos.x, mouse.y-clickPos.y)
            //如果mainwindow继承自QWidget,用setPos
            moveItem.setX(eo_askDialog.x+delta.x)
            moveItem.setY(eo_askDialog.y+delta.y)
        }
    }

    TitleBarBtn{
        id:maxBtn
        visible: maxBtnVis
        width: parent.height - 4
        height: parent.height - 4
        anchors.top: parent.top
        anchors.topMargin: 2
        anchors.right: parent.right
        anchors.rightMargin: 35
        normalIconSource: crb.isMaxed ?"qrc:/UI/photo/normalBtn.png" : "qrc:/UI/photo/maxBtn.png" //"qrc:/UI/images/maximized.png"
        onClicked: {
            if(crb.isMaxed)
            {
                crb.isMaxed=false
                moveItem.showNormal()
                normalIconSource =  "qrc:/UI/photo/maxBtn.png"
            }
            else{
                crb.isMaxed=true
                moveItem.showMaximized()
                normalIconSource =  "qrc:/UI/photo/normalBtn.png"
            }
        }

    }

    CusButton{
        id:closeBtn
        visible: closeBtnVis
        width: parent.height - 2
        height: parent.height - 2
        rightTop: true
        allRadius: false
        radius: 5
        anchors.top: parent.top
        anchors.topMargin: 1
        anchors.right: parent.right
        anchors.rightMargin: 1
        normalColor:  closeBtnNColor
        hoveredColor: "#ff3c28"
        pressedColor: "#ff3c28"
        borderWidth: 0
        Image{
            anchors.centerIn: parent
            sourceSize.width: closeBtnWidth
            sourceSize.height: closeBtnHeight
            source: closeBtn.isHovered ? closeIcon_d : closeIcon
        }

        onClicked: {
            crb.closeClicked()
        }
    }

    Rectangle{
        width: parent.width
        height: 1
        border.width: 0
        color: Constants.lpw_BtnBorderColor
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
