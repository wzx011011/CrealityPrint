import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQml 2.13
import CrealityUI 1.0
import "qrc:/CrealityUI"

BasicDialog{
    //设备编号
    property string dn
    //传输进度
    property int progress: 0
    //当前传输文件路径
    property string curTransFilePath

    id: transProcessDialog
    width: 460
    height: 150
    titleHeight : 30
//    contentBackground : "#454545"
//    titleBackground : "#454545"
//    color : "#454545"
    title: ""
        Item{
            width: parent.width
            height: parent - titleHeight * 2
            visible: ! (transProcessDialog.progress === 100)
            anchors.centerIn: parent
            Column
            {
                spacing: 5
                width: parent.width
                height: __text.height + proBar.height + 5
                anchors.centerIn: parent
                Text{
                    id : __text
                    anchors.horizontalCenter: parent.horizontalCenter

                    color: "#ffffff"
                    text: "transmitting:"  + transProcessDialog.progress + "%"
                    font.pixelSize: 14
                    font.weight: Font.Medium

                }

                MyProgressBar{
                    id:proBar
                    width: 380
                    height: 2
                    txtVisible : false
                    anchors.horizontalCenter: parent.horizontalCenter
                    value:  transProcessDialog.progress
                    from: 0
                    to: 100
                }
            }


        }
        StyledLabel
        {
            text: qsTr("Translate Finished")
            anchors.centerIn: parent
            visible : transProcessDialog.progress === 100
        }




}
