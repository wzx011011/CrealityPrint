import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.13
import "../qml"
CheckBox {
    id: control
    text: "CheckBox"
    checked: true
    property real indWidth: 28
    property real indHeight: 28

    indicator: Rectangle {
        implicitWidth: indWidth
        implicitHeight: indHeight
        x: control.leftPadding
        y: parent.height / 2 - height / 2
        color: Constants.themeColor_primary
        radius: 3
        border.color: control.down ? Constants.lpw_BtnBorderHoverColor : Constants.lpw_BtnBorderColor

        Image {
            id:idCheckboxImg
            //            anchors.fill: parent
            anchors.centerIn: parent
            source: Constants.checkBtnImg
            visible: control.checkState === Qt.Checked
        }
    }

    contentItem: Text {
        text: control.text
        font: control.font
        opacity: enabled ? 1.0 : 0.3
        color: Constants.themeTextColor
        verticalAlignment: Text.AlignVCenter
        leftPadding: control.indicator.width + control.spacing
    }
}
