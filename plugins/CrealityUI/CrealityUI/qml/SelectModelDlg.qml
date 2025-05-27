import QtQuick 2.0
import QtQuick.Controls 2.0

Rectangle {
    id: informationDlg
    width: 262
    height: 152
    radius : 5
    color: Constants.itemBackgroundColor

    property var imageSource :"qrc:/UI/images/warning.png"
    property var informationText :qsTr("No model is selected") // qsTr("当前未选择操作模型。")
    property var control
    Image {
        id: imageInformation
        x: 96
        y: 30
        width: 60
        height: 50
        source: imageSource
        fillMode: Image.PreserveAspectFit       //拉伸是缩放 总是显示完整图片
    }
    StyledLabel {
       id: panel_name
       y: 90
       verticalAlignment: Label.AlignTop
       horizontalAlignment: Text.AlignHCenter
       text: informationText
       anchors.horizontalCenter: parent.horizontalCenter
       wrapMode: Label.WordWrap
       color: Constants.textColor
       font.pixelSize:Constants.labelFontPixelSize  // panelFontSize
    }

//    BasicButton
//    {
//        id: add_support
//        text: qsTr("OK")
//        btnTextColor : "#E3EBEE"
//        hoveredBtnBgColor:  Constants.hoveredColor
//        defaultBtnBgColor : Constants.buttonColor
//        width: parent.width -20
//        height: 32
//        anchors.left: parent.left
//        anchors.top: panel_name.bottom
//        anchors.topMargin: 10
//        anchors.leftMargin: 10
//        onSigButtonClicked:
//        {
//            parent.visible = false
//        }
//    }
}
