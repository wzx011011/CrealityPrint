import QtQuick 2.0
import QtQuick 2.0
import QtQuick.Controls 2.12
import "../qml"

BasicDialog{

    property var msgText: "message contect"
    property var otherMsg: ""
    property var cancelBtnVisible: true
    property var ignoreBtnVisible: false
    property var okBtnText: qsTr("Ok")
    property var ignoreBtnText: qsTr("Ignore")
    property var cancelBtnText: qsTr("Cancel")
    property var messageType: 0
    property var isInfo: false

    signal sigOkButtonClicked()
    signal sigIgnoreButtonClicked()
    signal sigCancelButtonClicked()

    id: idDialog
    width: 500*screenScaleFactor
    height: 160*screenScaleFactor
    titleHeight : 30*screenScaleFactor
    title: qsTr("Message")

    Row{
        x: (idDialog.width - idUploadImage.width - idMsgLabel.contentWidth)/2
        y: 50*screenScaleFactor
        spacing: 10
        Image{
            id: idUploadImage
            height:sourceSize.height
            width: sourceSize.width
            source: messageType == 0 || isInfo ? "qrc:/UI/photo/UploadSuccess.png" : (messageType == 1 ? "qrc:/UI/photo/upload_msg.png" : "qrc:/UI/photo/upload_success_image.png")
        }
        StyledLabel{
            id: idMsgLabel
            width: idDialog.width - 130*screenScaleFactor - idUploadImage.sourceSize.width
            height: idUploadImage.sourceSize.height
            wrapMode: Text.WordWrap
            text: msgText
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment:Text.AlignLeft
        }
    }
    Row{
        x: ignoreBtnVisible ? (idDialog.width - 395*screenScaleFactor)/2 : (cancelBtnVisible ? (idDialog.width - 260*screenScaleFactor)/2 : (idDialog.width - 135*screenScaleFactor)/2)
        y: 75*screenScaleFactor + idMsgLabel.height 
        spacing: 10
        BasicButton{
            width: 125*screenScaleFactor
            height: 28*screenScaleFactor
            btnRadius:3
            btnBorderW:0
            defaultBtnBgColor: Constants.profileBtnColor
        hoveredBtnBgColor: Constants.profileBtnHoverColor
            text: okBtnText
            onSigButtonClicked:
            {
                sigOkButtonClicked()
            }
        }

        BasicButton{
            width: 125*screenScaleFactor
            height: 28*screenScaleFactor
            btnRadius: 3
            btnBorderW: 0
            defaultBtnBgColor: Constants.profileBtnColor
            hoveredBtnBgColor: Constants.profileBtnHoverColor
	        visible: ignoreBtnVisible
            text: ignoreBtnText
            onSigButtonClicked:
            {
                sigIgnoreButtonClicked()
            }
        }

        BasicButton{
            width: 125*screenScaleFactor
            height: 28*screenScaleFactor
            btnRadius: 3
            btnBorderW: 0
            defaultBtnBgColor: Constants.profileBtnColor
            hoveredBtnBgColor: Constants.profileBtnHoverColor
	        visible: cancelBtnVisible
            text: cancelBtnText
            onSigButtonClicked:
            {
                sigCancelButtonClicked()
                idDialog.close()
            }
        }
    }

}
