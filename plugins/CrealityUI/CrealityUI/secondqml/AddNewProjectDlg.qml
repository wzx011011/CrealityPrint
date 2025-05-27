import QtQuick 2.0
import QtQuick.Controls 2.12
import CrealityUI 1.0
import "qrc:/CrealityUI"
BasicDialog
{
    id: idDialog
    width: 400
    height: 150
    titleHeight : 30
    property int spacing: 5
    title: qsTr("New Project")
    property alias projectName: idProjectInput.text
    signal sigProject(var name)
    StyledLabel {//by TCJ
        id: idLabel
        x:30
        y:20 + titleHeight
        width:120
        height:30
        text: qsTr("Project Name:")
        font.pixelSize: 16
        color:"white"//"black"
		verticalAlignment: Qt.AlignVCenter
		horizontalAlignment: Qt.AlignRight
    }

    BasicDialogTextField {
        id: idProjectInput
        anchors.left: idLabel.right
        anchors.leftMargin: 10
        anchors.top: idLabel.top
        //anchors.topMargin: -8
        width: 200
        height: 30
        color: "white"
        text: "Untitled"
        font.pixelSize: 16
        verticalAlignment: Qt.AlignVCenter
        onTextEdited:
        {}
        //Rectangle
        //{
        //    anchors.fill: parent
        //    color: "white"
        //    z:-1
        //    x:-5
        //    border.width: 1
        //    border.color: "#707070"
        //}
    }



    BasicDialogButton {
        id: basicComButton
        width: 100
        height: 32
        text: qsTr("Cancel")
        anchors.top: idLabel.bottom
        anchors.topMargin: 20
        anchors.left: parent.horizontalCenter
        anchors.leftMargin: 15
        //btnRadius:6
        onSigButtonClicked:
        {
            idDialog.close();
        }
    }
    BasicDialogButton {
        id: basicComButton2
        width: 100
        height: 32
        text: qsTr("Yes")
        anchors.top: basicComButton.top
        anchors.right: parent.horizontalCenter
        anchors.rightMargin: 15
        //btnRadius:6
        onSigButtonClicked:
        {
            idDialog.close();
            sigProject(idProjectInput.text)
        }
    }
}
