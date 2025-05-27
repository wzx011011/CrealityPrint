import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.13
import QtQml 2.13
import CrealityUI 1.0
import "qrc:/CrealityUI"
BasicDialog{
    id: deviceDialog
    width: 850
    height: 480
    titleHeight : 30
    title: qsTr("Device Control")
    //titleBackground : "#D7D7D7"
    //titleColor : "#333333"
    signal switchDeviceConnect();
    signal switchPrintControl(int indexRow,string deviceDN, int curState)
    property string filename: ""
    property string realFilePath: ""


    Rectangle{
        x:10
        y : titleHeight
        width: parent.width - 20
        height: 440//parent.height - 30
        color:Constants.dialogContentBgColor

        BasicDialogButton{
            property bool isChecked: true
            id:__searchDevice
            defaultBtnBgColor: isChecked ? "#1E9BE2":Constants.profileBtnColor
        	hoveredBtnBgColor: Constants.profileBtnHoverColor
            width: 140
            height: 36
            text: qsTr("Edit Group1")
            //btnTextColor: "#ffffff"
            anchors.top: parent.top
            anchors.topMargin: 20
            anchors.left: parent.left
            anchors.leftMargin: 20
            btnRadius: 3
            btnBorderW : 0
            onSigButtonClicked:
            {

            }
        }

        Rectangle{
            id: rect
            width: 1
            height: 331 + 38
            color: Constants.splitLineColor//"#666666"
            anchors.top: __searchDevice.top
            anchors.left: __searchDevice.right
            anchors.leftMargin: 20
        }

        Item {
            id: __empty
            width: 650
            height: 330 + 38
            anchors.left: rect.right
            anchors.leftMargin: 10
            anchors.top: rect.top
            anchors.topMargin: 0
//            color:  "red"
            visible: wifiTableView.modelDataCount ===0
            StyledLabel{
                text: qsTr("No device found, try refreshing to get it again.")
                anchors.centerIn: parent

            }
        }

        Item {
            width: 650
            height: 48
            anchors.left: rect.right
            anchors.leftMargin: 10
            anchors.top: tableViewStack.bottom
            anchors.topMargin: 30
            StyledLabel{
                height : 20

                text: qsTr("Transferred file : ") + filename
                visible : filename !== ""
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Row
            {
                y: 20

//                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 10
                height : 28
//                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                width : 250
                BasicDialogButton{
                    width: 120
                    height : 28
                    text: qsTr("Connect Device")
                    //btnTextColor: "#ffffff"
                    btnRadius: 3
                    btnBorderW : 0
                    //defaultBtnBgColor : "#787878"
                    //hoveredBtnBgColor : "#1E9BE2"
                    defaultBtnBgColor: Constants.profileBtnColor
        			hoveredBtnBgColor: Constants.profileBtnHoverColor
                    onSigButtonClicked:
                    {
                        switchDeviceConnect()
                        close()

                    }
                }


                BasicDialogButton{
                    width: 120
                    height : 28
                    text: qsTr("Send Gcode")
                    //btnTextColor: "#ffffff"
                    btnRadius: 3
                    btnBorderW : 0
                    //defaultBtnBgColor : "#787878"
                    //hoveredBtnBgColor : "#1E9BE2"
                    defaultBtnBgColor: Constants.profileBtnColor
        			hoveredBtnBgColor: Constants.profileBtnHoverColor
                    onSigButtonClicked:
                    {
                        if(realFilePath !== "")
                        {
                            cwModel.sendFileToPrinters(realFilePath);
                        }
                        else
                        {
                            //动态创建文件选择的弹窗
                            var newObject = Qt.createQmlObject(
                                        'import QtQuick 2.0; import Qt.labs.platform 1.1; FileDialog {}',
                                        deviceDialog,
                                        "dynamicSnippet1");
                            newObject.nameFilters = ["Text files (*.gcode)"]
                            newObject.accepted.connect(
                                        function(){
                                           console.log("newObject.file =" + newObject.file)
                                            cwModel.sendFileToPrinters(newObject.file.toString());
                                            newObject.destroy()


                                            //弹出传输界面


                                        })
                            newObject.open()
                        }
                    }
                }

            }
        }



        StackLayout{
            id:tableViewStack
            width: 640
            height: 315
            anchors.left: rect.right
            anchors.leftMargin: 10
            anchors.top: rect.top
            anchors.topMargin: 0

            CusControlTableView{
                id:wifiTableView
                rowSpacing : 18
                rowHeight: 28
                width: 640
                model: cwModel

                rowWidths: [width * 0.1 ,width*0.2, width*0.25, width*0.25, width*0.18]
                headTexts: ["",qsTr("Name"), qsTr("IP Address"), qsTr("State"), ""]

                onViewSwitchControl:
                {
//                    cwModel.reconnectIp(deviceDN)
                    switchPrintControl(curIndex,deviceDN,curState)

                }

            }
        }
    }

    CusConnectMessageDlg
    {
        id : __cusConnectMsgDlg

    }
    
    CusConnectMessageDlg
    {
        id : __cusTransMsgDlg
    }

    CusTransFileMessageDlg
    {
        id : __cusTransDlg

    }
    Connections
    {
        target: cwModel
//         onSigFileTransProgress:
//         {
// //            __cusTransDlg.visible = true
//             console.log("onSigFileTransProgress = " + progressVal)
//             __cusTransDlg.progress = progressVal
//         }

        onSigFileTransMessage:
        {
            __cusTransMsgDlg.msgText = errMsg
            __cusTransMsgDlg.show()
        }
    }


}
