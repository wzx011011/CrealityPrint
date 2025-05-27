
import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.13
import QtQml 2.13
import CrealityUI 1.0
import "qrc:/CrealityUI"
BasicDialog{

    id: deviceDialog
    width: 460
    height: 150
    titleHeight : 30
    title: ""
    titleBackground : "#454545"
    color : "#454545"

    property string msgText : ""


    StyledLabel{
        text: msgText
        anchors.centerIn : parent
//        visible : __manualConnect.isChecked
    }

//    titleColor : "#333333"

}
