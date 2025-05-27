import QtQuick 2.12
import "../qml"

CusRoundedBg{
    signal closeClicked()
    property alias title: title_Text.fontText
    property alias fontColor: title_Text.fontColor
    //property color closeBtnNColor: "#6E6E73"
    clickedable: false
    radius: 5

    CusText{
        id: title_Text
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.verticalCenter: parent.verticalCenter
        hAlign: 0
    }

    CusButton{
        property var titleHeight: parent.height
        width: titleHeight - titleHeight*0.2
        height: titleHeight - titleHeight*0.2
        //rightTop: true
        radius: 5
        anchors.top: parent.top
        anchors.topMargin: titleHeight*0.1
        anchors.right: parent.right
        anchors.rightMargin: titleHeight*0.1
        normalColor: parent.color
        allRadius: true
        hoveredColor: "#ff3c28"
        pressedColor: "#ff3c28"
        borderColor: "transparent"

        Image{
            id: closeImage
            anchors.centerIn: parent
            source: "qrc:/UI/photo/closeBtn_V4.png"
        }

        onClicked: {
            isHovered = false
            parent.closeClicked()
        }
    }
}
