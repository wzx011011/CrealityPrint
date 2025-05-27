import QtQuick 2.0
import QtQuick 2.0
import QtQuick.Controls 2.12
import "../qml"

BasicDialog{
    signal sigOkButtonClicked(var serverIndex)
    signal sigBackButtonClicked()
    property var serverTypeSelect: 0

    function setServerType(idx)
    {
        idServer.currentIndex = idx
    }

    id: idDialog
    width: 500 * screenScaleFactor
    height: 160 * screenScaleFactor
    titleHeight : 30 * screenScaleFactor
    title: qsTr("Server Setting")
    btnVisible: false

    onVisibleChanged:{
        if(this.visible)
        {
            if(serverTypeSelect == 0)
            {
                idServer.displayText = qsTr("China")
            }
            else
            {
                idServer.displayText = qsTr("International")
            }
        }
	}


    BasicCombobox 
    {
        id: idServer
        height: 30 * screenScaleFactor
        width : 420 * screenScaleFactor
        font.pointSize: Constants.labelFontPointSize
        currentIndex : 0
        showCount:2
        y: 60 * screenScaleFactor
        x: (idDialog.width - idServer.width)/2
        backgroundColor: Constants.dialogItemRectBgColor
        model: ListModel {
            id: model
            ListElement { text: qsTr("China");}
            ListElement { text: qsTr("International");}
        }
        onCurrentIndexChanged: {
            serverTypeSelect = currentIndex
            if(serverTypeSelect == 0)
            {
                idServer.displayText = qsTr("China")
            }
            else
            {
                idServer.displayText = qsTr("International")
            }

        }
    }

    BasicButton{
        id: idBack
        width: 125 * screenScaleFactor
        height: 30 * screenScaleFactor
        btnRadius: 5
        btnBorderW:0
        defaultBtnBgColor: Constants.profileBtnColor
        hoveredBtnBgColor: Constants.profileBtnHoverColor
        text: qsTr("Back")
        anchors.left: idServer.left
        anchors.top: idServer.bottom
        anchors.topMargin: 15 * screenScaleFactor
        anchors.leftMargin: (idServer.width - (idBack.width + idNext.width))/2
        onSigButtonClicked:
        {
            sigBackButtonClicked()
        }
    }

    BasicButton{
        id: idNext
        width: 125 * screenScaleFactor
        height: 30 * screenScaleFactor
        btnRadius: 5
        btnBorderW:0
        defaultBtnBgColor: Constants.profileBtnColor
        hoveredBtnBgColor: Constants.profileBtnHoverColor
        text: qsTr("Next")
        anchors.left: idBack.right
        anchors.top: idServer.bottom
        anchors.topMargin: 15 * screenScaleFactor
        anchors.leftMargin: 20 * screenScaleFactor
        onSigButtonClicked:
        {
            sigOkButtonClicked(idServer.currentIndex)
        }
    }
    

}
