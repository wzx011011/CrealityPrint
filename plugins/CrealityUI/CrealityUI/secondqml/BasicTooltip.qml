import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.impl 2.12
import QtQuick.Templates 2.12 as T
import CrealityUI 1.0
import "qrc:/CrealityUI"
T.ToolTip {
    id: control

    x: parent ? (parent.width - implicitWidth) / 2 : 0
    y: -implicitHeight - 3
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            contentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             contentHeight + topPadding + bottomPadding)
    font.family: Constants.labelFontFamily
    font.weight: Constants.labelFontWeight
    font.pixelSize: 12
//    margins: 6
//    padding: 6
    delay: 100
    timeout: 2000
    closePolicy: T.Popup.CloseOnEscape | T.Popup.CloseOnPressOutsideParent | T.Popup.CloseOnReleaseOutsideParent

    contentItem: Text {
        height: control.height
        text: control.text
        font: control.font
        color: control.palette.toolTipText
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }
    background: Rectangle {
        border.color: control.palette.dark
        color: control.palette.toolTipBase
    }
}
