import QtQuick 2.0
import QtQuick.Controls 2.5
import QtGraphicalEffects 1.12
Button {
    id : propertyButton
    font.family: Constants.labelFontFamily
    font.weight: Constants.labelFontWeight
    font.pointSize: Constants.labelFontPointSize
    property string enabledIconSource
    property string highLightIconSource
    property string pressedIconSource
    property bool btnHightLight: false
    property bool bottonSelected:false
    property bool allowTooltip: true
    property color defaultBtnBgColor: Constants.leftBtnBgColor_normal
    property color hoveredBtnBgColor: Constants.leftBtnBgColor_hovered
    property color selectedBtnBgColor: Constants.leftBtnBgColor_selected

    property color  btnTextColor : Constants.leftTextColor
    property color  btnTextColor_d : Constants.leftTextColor_d
    property color  borderColor: Constants.rectBorderColor
    property var fontBold: false
    property var fontWeight: Font.bold
    property real borderWidth: 1
    property real sourceWidth: 12
    property real sourceHeight: 12

    property var btnRadius: 5
    property alias imgWidth: icon_image.width
    property alias imgHeight: icon_image.height
    property alias textWrapMode: idContentText.fontWrapMode
    property alias imgFillMode: icon_image.fillMode
    property alias textAlign: idContentText.hAlign
    opacity: enabled ? 1:0.3
    //tooltip
//    property real pos: 0 //0: 左 1：上 2：右 3：下
//    property var btnTipType : 0 // idTooptip.tipType
//    property var tipsource:"" // idTooptip.source
//    property var tipmaintext: "" //idTooptip.mainText
//    property var  tipDesc: "" // idTooptip.description
    property bool shadowEnabled: false
    state: "imgOnly"
    padding: 0
    states: [
        State {
            name: "wordsOnly"
            AnchorChanges {
                target: idContentText
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }
            PropertyChanges {
                target: icon_image
                visible: false
            }
            PropertyChanges {
                target: idContentText
                visible: true
            }
        },
        State {
            name: "imgOnly"
            PropertyChanges {
                target: icon_image
                visible: true
            }
            PropertyChanges {
                target: idContentText
                visible: false
            }
            AnchorChanges {
                target: icon_image
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }
        },
        State {
            name: "imgLeft"
            AnchorChanges {
                target: icon_image
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
            }
            PropertyChanges {
                target: icon_image
                anchors.leftMargin: 35 * screenScaleFactor
            }
            AnchorChanges {
                target: idContentText
                anchors.left: icon_image.right
                anchors.verticalCenter: parent.verticalCenter
            }
            PropertyChanges {
                target: idContentText
                anchors.leftMargin: 8 * screenScaleFactor
            }
        },
        State {
            name: "imgTop"
            AnchorChanges {
                target: icon_image
                anchors.top: parent.top
                anchors.horizontalCenter : parent.horizontalCenter
            }
            PropertyChanges {
                target: icon_image
                anchors.topMargin: (propertyButton.height - icon_image.height - idContentText.height - 4) / 2
            }
            AnchorChanges {
                target: idContentText
                anchors.top: icon_image.bottom
                anchors.horizontalCenter : parent.horizontalCenter
            }
            PropertyChanges {
                target: idContentText
                anchors.topMargin: 4
            }
        }
    ]

    contentItem: Item {
        clip: true
//        Item{
//            id:
//            implicitHeight:
//        }

        Image {
            id: icon_image
            source : if(propertyButton.pressed || bottonSelected)
                     {
                         return pressedIconSource
                     }
                     else if(btnHightLight)
                     {
                         return highLightIconSource
                     }
                     else
                     {

                         return enabledIconSource
                     }
            visible:
                (enabledIconSource.length > 0 || pressedIconSource.length > 0 || highLightIconSource.length > 0) ? true : false
            fillMode: Image.Pad
        }
        CusText {
            id: idContentText
            fontColor: {
                if(propertyButton.down || bottonSelected)
                {
                    return btnTextColor_d
                }
                return btnTextColor
            }

            fontText: propertyButton.text
            font_font: propertyButton.font
            fontWidth: propertyButton.width
            fontPointSize: Constants.imageButtomPointSize
            //fontPixelSize: 12
            fontBold: propertyButton.fontBold
            fontWeight: propertyButton.fontWeight
            fontSizeMode:Text.Fit
        }
    }

    background: Rectangle {
        radius: btnRadius
        opacity: enabled ? 1 : 0.3
        border.color: borderColor
        border.width: propertyButton.borderWidth
        color: {
            if(propertyButton.down || bottonSelected)
            {
                return selectedBtnBgColor
            }
            return propertyButton.hovered ?hoveredBtnBgColor:defaultBtnBgColor
        }

        layer.enabled: shadowEnabled
        layer.effect: DropShadow {
            verticalOffset: 3
            color:Constants.dropShadowColor
        }

    }
}

