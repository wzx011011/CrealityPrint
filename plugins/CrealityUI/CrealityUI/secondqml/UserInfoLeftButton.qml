import QtQuick 2.0
import QtQuick.Controls 2.5
import "qrc:/CrealityUI"
import "../qml"
Item {
    id: basicButton
    implicitWidth: 32
    implicitHeight: 32
    property alias text: propertyButton.text
    //property alias tooltip: propertyButton.tooltip
    property bool btnEnabled:true
    property bool btnSelected:false
    property color defaultBtnBgColor: Constants.buttonColor
//    property color pressBtnBgColor : "transparent"
    property color hoveredBtnBgColor: Constants.hoveredColor
    property color selectedBtnBgColor: Constants.selectionColor//"transparent"

    property color btnTextColor:Constants.textColor//"#E3EBEE"
    property alias btnText: btnTxt
    property bool isImageBtnShow: false

    property var btnRadius: 14
    property var btnBorderW: 1
    property var pointSize: Constants.labelFontPointSize
    property var fontFamily: Constants.labelFontFamily

    property alias hovered: propertyButton.hovered
    property alias down: propertyButton.down
    signal sigButtonClicked()
    signal sigImageBtnClicked()

    Button {
        id : propertyButton
        width: parent.width
        height: parent.height
        font.family: fontFamily
        font.weight: Constants.labelFontWeight
        font.pointSize: pointSize
        contentItem: Item {
            Text {
                  id: btnTxt
                  color:  btnTextColor
                  anchors.centerIn: parent
                  elide: Text.ElideRight
                  text: propertyButton.text
                  font: propertyButton.font
            }
            CusSkinButton_Image{
                anchors.centerIn: parent
                anchors.horizontalCenterOffset:btnTxt.contentWidth
                width: 18
                height: 19
                visible: (isImageBtnShow && propertyButton.hovered && btnSelected)
                btnImgUrl: "qrc:/UI/photo/user_info_left_btn_tip_image.png"

                onClicked:
                {	
                    sigImageBtnClicked()
                }
            }
        }

        background: Rectangle {
            implicitWidth: parent.width
            implicitHeight: parent.height
            radius: btnRadius
            opacity: enabled ? 1 : 0.3
            color: {
                if(btnSelected)
                {
                  return selectedBtnBgColor
                }
            // return propertyButton.down ? selectedBtnBgColor : propertyButton.hovered ?hoveredBtnBgColor : defaultBtnBgColor;
               return propertyButton.hovered ?hoveredBtnBgColor:defaultBtnBgColor
            }
            border.width: btnBorderW/*1*/
            border.color: propertyButton.hovered? hoveredBtnBgColor : Constants.rectBorderColor
        }
        onClicked:
        {
            sigButtonClicked()
        }
    }
}

