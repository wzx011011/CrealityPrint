import QtQuick 2.0
import QtQuick.Layouts 1.13
import CrealityUI 1.0
import "qrc:/CrealityUI"

Rectangle {
    color: "transparent"
    width: 58 * screenScaleFactor
    height: 58 * screenScaleFactor
    property real dialogPos: 0 //0 right; 1 left

    CusImglButton {
        id: mouseButton
        enabledIconSource:"qrc:/UI/photo/mouseIcon.png"
        pressedIconSource:"qrc:/UI/photo/mouseIcon_d.png"
        borderWidth: 0
        //borderColor: "#3e4448"
        defaultBtnBgColor: "#4b4b4d"
        hoveredBtnBgColor: "#68686b"
        selectedBtnBgColor: "#1e9be2"
        width: parent.width
        height: parent.height
        anchors.centerIn: parent
        onClicked:{
            mouseOptDialog.visible = !mouseOptDialog.visible
        }
    }

    Rectangle{
        id: mouseOptDialog
        visible: false
        x: mouseButton.x + mouseButton.width + 10
        y: mouseButton.y - (height -  mouseButton.height )
        color: "#272a2d"
        radius: 5
        width: 257
        height: 203
        CusPopViewTitle {
            id:closeBtn
            width: 257
            height: 24
            color: "#56565c"
            borderColor : "#272a2d"
            closeBtnNColor:  "transparent"//color
            closeBtnWidth: 8
            closeBtnHeight: 8
            anchors.top: parent.top
            anchors.right: parent.right
            radius: 5
            leftTop: true
            rightTop: true
            clickedable: false
            maxBtnVis: false
            onCloseClicked:{
                mouseOptDialog.visible = false
            }
        }

        Column{
            spacing: 20
            //                anchors.centerIn: parent
            anchors.centerIn: parent
            Layout.margins: 32
            Repeater{
                model: ListModel{
                    ListElement{imgSource:"qrc:/UI/photo/mouseHoldRight.png"; textContent:qsTr("Rotate View : Press and Hold Right Button")}
                    ListElement{imgSource:"qrc:/UI/photo/mouseHoldLeft.png"; textContent:qsTr("Pan View : Press and Hold Wheel")}
                    ListElement{imgSource:"qrc:/UI/photo/mouseClickRight.png"; textContent:qsTr("Quick Popup ： Right Click")}
                    ListElement{imgSource:"qrc:/UI/photo/mouseScroll.png"; textContent:qsTr("Zoom View : Scroll Wheel")}
                }

                delegate: Item{
                    implicitWidth: mouseImg.width + mouseOptDetail.width + 5
                    implicitHeight: mouseImg.height
                    Image{
                        id:mouseImg
                        sourceSize.width: 20
                        sourceSize.height: 20
                        anchors.left: parent.left
                        source: imgSource
                    }

                    CusText{
                        id:mouseOptDetail
                        hAlign: 0
                        fontColor: "#ffffff"
                        fontPixelSize: 12
                        anchors.left: mouseImg.right
                        anchors.leftMargin: 5
                        fontText: textContent
                    }
                }
            }
        }
    }
}
