import QtQuick 2.0
import QtQuick.Controls 2.12
import CrealityUI 1.0
import "qrc:/CrealityUI"
BasicDialog
{
    id: idDialog
    width: 400
    height: 150//200
    titleHeight : 30
    title: qsTr("Message")

    signal sigNumber(var clonenum)
    StyledLabel {
        id: idLabel
        x:60//30
        y:20 + titleHeight
        width:80//120
        height:30
        text: qsTr("Clone Number:")
        font.pixelSize: 16
        color:Constants.textColor
    }

    SizeTextField/*StyledTextInput*/ {
        id: idInput
        anchors.left: idLabel.right
        anchors.leftMargin: 30//10
        anchors.top: idLabel.top
        anchors.topMargin: -6//-8
        width: 150
        height: 30
        color: Constants.textColor
        text: "1"
        validator: IntValidator {bottom: 1; top: 9;}
        unitchar:""
        font.pixelSize: 16
        horizontalAlignment: Qt.LeftToRight
        onTextEdited:
        {}
        Rectangle
        {
            anchors.fill: parent
            color: Constants.dialogItemRectBgColor
            z:-1
            //x:-5
            border.width: 1
            border.color: Constants.dialogItemRectBgBorderColor
        }
    }


    BasicButton {
        id: basicComButton
        width: 100
        height: 28
        text: qsTr("Cancel")
        btnRadius:3
        btnBorderW:0
        defaultBtnBgColor: Constants.profileBtnColor
        hoveredBtnBgColor: Constants.profileBtnHoverColor
        anchors.top: idLabel.bottom
        anchors.topMargin: 20
        anchors.left: parent.horizontalCenter
        anchors.leftMargin: 5
        onSigButtonClicked:
        {
            idDialog.close();
        }
    }

    BasicButton {
        id: basicComButton2
        width: 100
        height: 28
        text: qsTr("Yes")
        btnRadius:3
        btnBorderW:0
        defaultBtnBgColor: Constants.profileBtnColor
        hoveredBtnBgColor: Constants.profileBtnHoverColor
        anchors.top: basicComButton.top
        anchors.right: parent.horizontalCenter
        anchors.rightMargin: 5
        onSigButtonClicked:
        {
            idDialog.close();
            sigNumber(idInput.text)
        }
    }

}
