import QtQuick 2.9
import QtQuick.Controls 2.2


CusRoundedBg{
    id:rec
    property alias txtContent: txt.text
    property alias txtFamily: txt.font.family
    property alias txtWeight: txt.font.weight
    property alias txtBold: txt.font.bold
    property alias txtPixelSize: txt.font.pixelSize
    property alias txtPointSize: txt.font.pointSize
    property color txtColor: Constants.textColor
    property bool isHovered: false
    property bool isChecked: false
    property color normalColor: Constants.profileBtnColor
    property color hoveredColor: Constants.profileBtnHoverColor
    property color pressedColor: Constants.profileBtnHoverColor

    property color normalBdColor: "transparent"
    property color hoveredBdColor: "transparent"
    property color pressedBdColor: "transparent"
    property bool checkable: false
    property bool showToolTip: false

    allRadius: true
    clickedable: false
    color: isChecked ? pressedColor : (isHovered ? hoveredColor : normalColor)
    opacity : enabled ? 1 : 0.3
    borderColor: isHovered ? (isChecked ? pressedBdColor : hoveredBdColor) : normalBdColor

    Text {
        id: txt
        text: ""
        color: txtColor
        anchors.centerIn: parent
        font.weight: Constants.labelFontWeight
        font.family: Constants.labelFontFamily
        font.pointSize : Constants.labelFontPointSize

    }

    MouseArea{
        anchors.fill: parent
        hoverEnabled: true
        onEntered: {
            isHovered = true
        }
        onExited: {
            isHovered = false
        }
        onPressed: {
        }
        onReleased: {
//            if(checkable)
//                isChecked = !isChecked
            rec.clicked()
        }
    }
}


