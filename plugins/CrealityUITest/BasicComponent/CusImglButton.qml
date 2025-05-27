import QtQuick 2.0
import QtQuick.Controls 2.5
import QtGraphicalEffects 1.12
Button {
    id : propertyButton
    width: parent.width
    height: parent.height
    font.family: Constants.labelFontFamily
    font.weight: Constants.labelFontWeight

    property var pixSize: 12
    font.pixelSize: pixSize
    property string enabledIconSource
    property string highLightIconSource
    property string pressedIconSource
    property bool btnHightLight: false
    property bool bottonSelected:false
    property bool allowTooltip: true
    property color defaultBtnBgColor: "transparent" //Constants.buttonColor
    property color hoveredBtnBgColor: "#435865" //Constants.hoveredColor
    property color selectedBtnBgColor: "#009CFF" //Constants.selectionColor

    property color btnTextColor: "#C3C3C3" //Constants.textColor
    property color btnTextColor_d: "white"
    property color borderColor: Constants.rectBorderColor
    property real borderWidth: 1

    property var btnRadius: 5
    property var strTooptip: ""
    property alias imgWidth: icon_image.width
    property alias imgHeight: icon_image.height
    property alias textWrapMode: idContentText.fontWrapMode
    property alias imgFillMode: icon_image.fillMode
    opacity: enabled ? 1:0.3
    //tooltip
    property real pos: 0 //0: 左 1：上 2：右 3：下
    property var btnTipType : 0 // idTooptip.tipType
    property var tipsource:"" // idTooptip.source
    property var tipmaintext: "" //idTooptip.mainText
    property var  tipDesc: "" // idTooptip.description

    property bool shadowEnabled: false

    states: [
            State {
                when: pos == 2
                name: "right"
                PropertyChanges {
                    target: idTooptip
                    x: propertyButton.width + 10
                    y: 0
                    width: contentWidth
                    height: contentHeight
                }
            },
            State {
                when: pos == 3
                name: "bottom"
                PropertyChanges {
                    target: idTooptip
                    x: 0
                    y: propertyButton.height + 10
                    width: contentWidth
                    height: contentHeight
                }
            },
            State {
                when: btnTipType == 0
                name: "wordsOnly"
                PropertyChanges {
                    target: idTooptip
                    width: contentWidth
                    height: contentHeight
                }
            }
        ]

    contentItem: Item {
        anchors.fill: parent
        clip: true
        Image {
            id: icon_image
            anchors.centerIn: parent
            anchors.verticalCenterOffset:idContentText.fontText ===""? 0 : 6- (parent.height  - icon_image.height )/2  // -pixSize /2
            width: sourceSize.width
            height: sourceSize.height
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
                return propertyButton.hovered ? btnTextColor_d :btnTextColor
            }
            y: icon_image.height + 6 + 2
            x: 0
            //  Distance between img and text  is 4px
            fontText: propertyButton.text
            font_font: propertyButton.font
            fontWidth: propertyButton.width
            fontSizeMode:Text.Fit
        }
    }

    background: Rectangle {
        //        implicitWidth: parent.width
        //        implicitHeight: parent.height
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
              color: "#19191B"
          }

    }
}

