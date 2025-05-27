import QtQuick 2.15
import QtQuick.Controls 2.5
import "../qml"
import "../secondqml"
import "qrc:/CrealityUI/components"

BasicDialogV4{
    property var fileList
    property var isAllDelete
    id: noExistFilesDialog
    width: 600 * screenScaleFactor
    height: 400 * screenScaleFactor
    title: cxTr("Tips")
    maxBtnVis: false

    signal okClicked()
    signal cancelClicked()

    bdContentItem: Item{
        Text{
            id: title1Txt
            width: parent.width - 40
            wrapMode: Text.WordWrap
            text: cxTr("The exported file contains deleted files. This operation can only export non-deleted files. Do you want to continue?")
            font.weight: Font.Normal
            font.family: Constants.mySystemFont.name
            font.pointSize: Constants.labelFontPointSize_12
            color: sourceTheme.text_normal_color
            anchors.top: parent.top
            anchors.topMargin: 20 * screenScaleFactor
            anchors.left: parent.left
            anchors.leftMargin: 20 * screenScaleFactor
        }

        Text{
            id: title2Txt
            width: parent.width - 40
            wrapMode: Text.WordWrap
            text: cxTr("Deleted files list")+":"
            font.weight: Font.Normal
            font.family: Constants.mySystemFont.name
            font.pointSize: Constants.labelFontPointSize_10
            color: sourceTheme.text_normal_color
            anchors.top: title1Txt.bottom
            anchors.topMargin: 5 * screenScaleFactor
            anchors.left: parent.left
            anchors.leftMargin: 20 * screenScaleFactor
        }

        Rectangle{
            id: listRec
            anchors.top: title2Txt.bottom
            anchors.topMargin: 10 * screenScaleFactor

            anchors.bottom: btnsRow.top
            anchors.bottomMargin: 10 * screenScaleFactor + noExistFilesDialog.isAllDelete ? 30 * screenScaleFactor : 0

            anchors.left: parent.left
            anchors.leftMargin: 20 * screenScaleFactor
            anchors.right: parent.right
            anchors.rightMargin: 20 * screenScaleFactor

            border.width: 1 * screenScaleFactor
            border.color: Constants.dialogTitleColor
            radius: 5 * screenScaleFactor
            color:"transparent"
            ListView{
                clip: true
                anchors.fill: parent
                anchors.margins: 20 * screenScaleFactor
                model: noExistFilesDialog.fileList
                delegate: Text{
                    text: modelData
                    font.weight: Font.Normal
                    font.family: Constants.mySystemFont.name
                    font.pointSize: Constants.labelFontPointSize_9
                    color: sourceTheme.text_normal_color
                }
            }
        }

        Text{
            id: notifyTxt
            visible: isAllDelete
            width: parent.width - 40
            wrapMode: Text.WordWrap
            text: cxTr("All selected files have been deleted")
            font.weight: Font.Normal
            font.family: Constants.mySystemFont.name
            font.pointSize: Constants.labelFontPointSize_10
            color: "red"
            anchors.top: listRec.bottom
            anchors.topMargin: 5 * screenScaleFactor
            anchors.left: listRec.left
            anchors.leftMargin: 0
        }

        Row{
            id: btnsRow
            spacing: 20 * screenScaleFactor
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 20 * screenScaleFactor
            CusButton{
                width: 100 * screenScaleFactor
                height: 30 * screenScaleFactor
                txtContent: cxTr("OK")
                txtColor: sourceTheme.text_normal_color
                enabled: !noExistFilesDialog.isAllDelete
                radius: 15 * screenScaleFactor
                onClicked: {
                    okClicked()
                    close()
                }
            }

            CusButton{
                width: 100 * screenScaleFactor
                height: 30 * screenScaleFactor
                txtContent: cxTr("Cancel")
                txtColor: sourceTheme.text_normal_color
                radius: 15 * screenScaleFactor
                onClicked: {
                    cancelClicked();
                    close()
                }
            }
        }
    }
}
