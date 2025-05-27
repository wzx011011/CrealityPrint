import QtQuick 2.0
import QtQuick.Controls 2.12
import "../qml"
GroupBox {
    id: control
    property color defaultBgColor: Constants.themeColor //"#061F3B"
    property color textColor: Constants.textColor
    property color borderColor: "#585C5E"
    property var borderWidth: 0//0.5
    property alias textY: idText.y
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            contentWidth + leftPadding + rightPadding,
                            implicitLabelWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             contentHeight + topPadding + bottomPadding)

    spacing: 0
    padding: 10
    font.family: Constants.labelFontFamily
    font.bold: true
    font.pixelSize: 14
    topPadding: padding + (implicitLabelWidth > 0 ? implicitLabelHeight + spacing : 0)
    label: Text {
        id : idText
        y : 0
        x: control.leftPadding
        width: control.availableWidth

        text: control.title
        font: control.font
        color: textColor
        elide: Text.ElideRight
        verticalAlignment: Text.AlignVCenter
        Rectangle
        {
            y: 4
            height:idText.height
            width: idText.contentWidth
            color:defaultBgColor
            z:-1
        }
    }

    background: Rectangle {
        y: control.topPadding - control.bottomPadding - idText.height
        width: parent.width
        height: parent.height - control.topPadding + control.bottomPadding + 10
        color: defaultBgColor
        border.color: borderColor
        border.width: borderWidth
    }


}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
