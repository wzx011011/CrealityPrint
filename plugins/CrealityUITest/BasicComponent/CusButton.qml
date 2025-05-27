import QtQuick 2.9
import QtQuick.Controls 2.2


CusRoundedBg{
    id:rec
    property alias txtContent: txt.text
    property alias txtFamily: txt.font.family
    property alias txtWeight: txt.font.weight
    property alias txtPixelSize: txt.font.pixelSize
    property color txtColor: "#ffffff"
    property bool isHovered: false
    property bool isPressed: false
    property color normalColor: "#616165"
    property color hoveredColor: "#009cff"
    property color pressedColor: "#009cff"

    property color normalBdColor: "#616165"
    property color hoveredBdColor: "#009cff"
    property color pressedBdColor: "#009cff"
    allRadius: true
    clickedable: false
    color: isHovered ? (isPressed ? pressedColor : hoveredColor) : normalColor
    opacity : enabled ? 1 : 0.3
    borderColor: isHovered ? (isPressed ? pressedBdColor : hoveredBdColor) : normalBdColor

    Text {
        id: txt
        text: ""
        color: txtColor
        anchors.centerIn: parent
    }

    MouseArea{
        anchors.fill: parent
        hoverEnabled: true
        onEntered: {
            console.log("onEntered")
            isHovered = true
        }
        onExited: {
            isHovered = false
        }
        onPressed: {
            isPressed = true
        }
        onReleased: {
            isPressed = false
            rec.clicked()
        }
    }
}


