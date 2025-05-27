import QtQuick 2.9
import QtQuick.Controls 2.2
import "../secondqml"
import "../qml"

Rectangle {
    id: rec
    property bool isHovered: false
    property bool isChecked: false
    property bool isEnabled: true
    property bool isPressed: false
    property bool bottonSelected: false

    property color normalColor: Constants.profileBtnColor
    property color hoveredColor: Constants.profileBtnHoverColor
    property color pressedColor: Constants.profileBtnHoverColor
    property color disabledColor: Constants.profileBtnHoverColor

    property color normalBdColor: "transparent"
    property color hoveredBdColor: "transparent"
    property color pressedBdColor: "transparent"
    property bool checkable: false
    property bool showToolTip: false

    property alias toolTipText: idTooptip.text
    property alias toolTipPosition: idTooptip.position

    property alias btnImg: img

    signal clicked()
    signal doubleClicked()
    color: ((isPressed || bottonSelected) && isEnabled)? pressedColor : (isHovered && isEnabled ? hoveredColor : normalColor)
//    border.color: isHovered ? (isChecked ? pressedBdColor : hoveredBdColor) : normalBdColor

    opacity: isEnabled ? 1 : 0.6
    BasicTooltip {
        id: idTooptip
        visible: isHovered
    }

    Image {
        id: img
        anchors.centerIn: parent
    }

    MouseArea {
        hoverEnabled: true
        anchors.fill: parent
        onEntered: rec.isHovered = true
        onExited: rec.isHovered = false
        onPressed: {
            isPressed = true
        }
        onDoubleClicked: {
            if(isEnabled){
                rec.doubleClicked()
            }
        }
        onReleased: {
            isPressed = false
            if(isEnabled){
                rec.clicked()
            }
        }
    }
}
