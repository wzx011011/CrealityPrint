import QtQuick 2.10
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.13
import "../secondqml"
BasicDialogNoWin_V4 {
    id : control_1
    color: Constants.itemBackgroundColor
    title: qsTr("Clone")
    width: 300* screenScaleFactor
    height: 168* screenScaleFactor
    readonly property int margin: 10
    property var control
    bdContentItem:Rectangle {
        color: Constants.lpw_bgColor
        Column{
            anchors.top: parent.top
            anchors.topMargin: 30* screenScaleFactor
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 30* screenScaleFactor
            RowLayout
            {
                spacing: 10* screenScaleFactor
                anchors.horizontalCenter: parent.horizontalCenter
                width: (parent.width - 40)
                StyledLabel
                {
                    id: cloneNum
                    Layout.preferredWidth: 60* screenScaleFactor
                    text: qsTr("Clone Number:")
                    color: Constants.textColor
                    font.pointSize: 9
                    Layout.alignment: Qt.AlignVCenter
                }

                Item{
                    Layout.fillWidth: true
                }

                BasicDialogTextField {
                    id: inputCloneNum
                    Layout.preferredWidth: 100* screenScaleFactor
                    Layout.preferredHeight: 28* screenScaleFactor
                    radius: 5
                    text: "1"
                    font.pointSize: 9
                    validator: IntValidator {bottom: 1; top: 9;}//by TCJ
                    Layout.alignment: Qt.AlignVCenter
                }
            }

            BasicButton {
                id: add_support
                text: qsTr("Add")
                btnRadius:14
                btnBorderW:1
                anchors.horizontalCenter: parent.horizontalCenter
                borderColor: Constants.lpw_BtnBorderColor
                borderHoverColor: Constants.lpw_BtnBorderHoverColor
                defaultBtnBgColor: Constants.lpw_BtnColor
                hoveredBtnBgColor: Constants.lpw_BtnHoverColor
                width: 258* screenScaleFactor
                height: 28* screenScaleFactor
                pointSize: 9
                onSigButtonClicked:
                {
                    if(control.isSelect())
                    {
                        var cnt = parseInt(inputCloneNum.text)
                        control.clone(cnt)
                    }else{
                        control.open()
                    }
                }
            }
        }
    }
}
