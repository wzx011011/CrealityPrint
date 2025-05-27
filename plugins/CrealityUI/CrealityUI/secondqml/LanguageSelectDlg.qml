import QtQuick 2.0
import QtQuick 2.0
import QtQuick.Controls 2.12
import "../qml"

BasicDialog{
    signal sigOkButtonClicked(var languageIndex)

    id: idDialog
    width: 500* screenScaleFactor
    height: 160* screenScaleFactor
    titleHeight : 30* screenScaleFactor
    title: qsTr("请选择语言种类：（Please select the language:）")
    btnVisible: false


    BasicCombobox 
    {
        id: idLanguageComb
        height: 30* screenScaleFactor
        width : 420* screenScaleFactor
        font.pointSize: Constants.labelLargeFontPointSize
        currentIndex : 0
        showCount:3
        backgroundColor: Constants.dialogItemRectBgColor
        y:60* screenScaleFactor
        x:(idDialog.width - idLanguageComb.width)/2
        model: ListModel {
            ListElement { text: qsTr("English");}
            ListElement { text: qsTr("简体中文");}
            ListElement { text: qsTr("繁體中文");}
        }
    }

    BasicButton{
        id: idNext
        width: 125* screenScaleFactor
        height: 30* screenScaleFactor
        btnRadius:3
        btnBorderW:0
        defaultBtnBgColor: Constants.profileBtnColor
        hoveredBtnBgColor: Constants.profileBtnHoverColor//"#8a8a8a"
        text: 
        {
            if(idLanguageComb.currentIndex == 1)
            {
                return "下一步"
            }
            else if(idLanguageComb.currentIndex == 2)
            {
                return "下一步"
            }
            else
            {
                return "Next"
            }
        }
        anchors.left: idLanguageComb.left
        anchors.top: idLanguageComb.bottom
        anchors.topMargin: 15
        anchors.leftMargin: (idLanguageComb.width - idNext.width)/2
        onSigButtonClicked:
        {
            sigOkButtonClicked(idLanguageComb.currentIndex)
        }
    }

}