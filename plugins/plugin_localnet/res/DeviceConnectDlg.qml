import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.13
import QtQml 2.13
import CrealityUI 1.0
import "qrc:/CrealityUI"
BasicDialog{
    QtObject{
        id:printInfoObj
        //打印信息
        property string printerType //打印机类型
        property string machinename
    }

    id: deviceDialog
    width: 850
    height: 480
    titleHeight : 30
    title: qsTr("Device Connect")
    //titleBackground : "#D7D7D7"
    //titleColor : "#333333"

    signal switchControlDlg()
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
            text: qsTr("Device List")
            //btnTextColor: "#ffffff"
            anchors.top: parent.top
            anchors.topMargin: 20
            anchors.left: parent.left
            anchors.leftMargin: 20
            btnRadius: 3
            btnBorderW : 0
            onSigButtonClicked:
            {
                //暂时关闭点击功能，等云打印做好之后开启
                __searchDevice.isChecked = true
                __manualConnect.isChecked = false

                //切换wifi数据源
                cwModel.changeDataSource(0)
                tableViewStack.currentIndex = 0
            }
        }

        BasicDialogButton{
            property bool isChecked: false
            id:__manualConnect
            defaultBtnBgColor: isChecked ? "#1E9BE2":Constants.profileBtnColor
        	hoveredBtnBgColor: Constants.profileBtnHoverColor
            width: 140
            height: 36
            text: qsTr("Manual Connect")
            //btnTextColor: "#ffffff"
            anchors.top: __searchDevice.bottom
            anchors.topMargin: 20
            anchors.left: __searchDevice.left
            anchors.leftMargin: 0
            btnRadius: 3
            btnBorderW : 0
            onSigButtonClicked:
            {
                //暂时关闭点击功能，等云打印做好之后开启
                __manualConnect.isChecked = true
                __searchDevice.isChecked = false

                //切换创想云数据源
                cwModel.changeDataSource(1)
                tableViewStack.currentIndex = 1
            }
        }

//        Image{
//            id:uploadToCloudImg
//            sourceSize.width: 88
//            sourceSize.height: 88
//            anchors.left: parent.left
//            anchors.leftMargin: 47
//            anchors.bottom: parent.bottom
//            anchors.bottomMargin: 50
//            source: "qrc:/UI/photo/uploadToCloud_normal.png"
//            visible: __manualConnect.visible
//            MouseArea{
//                hoverEnabled: true
//                anchors.fill: parent

//                onEntered: {
//                    uploadToCloudImg.source = "qrc:/UI/photo/uploadToCloud_normal.png"
//                }
//                onExited: {
//                    uploadToCloudImg.source = "qrc:/UI/photo/uploadToCloud_hover.png"
//                }

//                onClicked: {
//                    //动态创建文件选择的弹窗
//                    var newObject = Qt.createQmlObject(
//                                'import QtQuick 2.0; import Qt.labs.platform 1.1; FileDialog {}',
//                                deviceDialog,
//                                "dynamicSnippet1");
//                    newObject.nameFilters = ["Text files (*.cxdlp *.cxline)"]
//                    newObject.accepted.connect(
//                                function(){
//                                    cwModel.uploadCxdlp(newObject.file);
//                                    newObject.destroy()
//                                })
//                    newObject.open()
//                }
//            }
//        }

        BasicDialogButton{
            id : __refreash
            visible : __searchDevice.isChecked
            width: 120
            height: 28
            //            text: qsTr("刷新")
            //defaultBtnBgColor : "#4B4B4B"
            //hoveredBtnBgColor : "#1E9BE2"
            defaultBtnBgColor: Constants.profileBtnColor
        	hoveredBtnBgColor: Constants.profileBtnHoverColor
            btnTextColor: "#ffffff"
            enabledIconSource : Constants.wifiRefreshImg
            disabledIconSource : Constants.wifiRefreshImg
            pressedIconSource : Constants.wifiRefreshImg
            anchors.top: rect.top
            anchors.topMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 20
            btnRadius: 3
            btnBorderW : 0
            z: wifiTableView.z + 1
            onSigButtonClicked:
            {
                refreshTableList()
            }
        }


        Rectangle{
            id: rect
            width: 1
            height: 331 + 38
            color:Constants.splitLineColor
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
            visible: wifiTableView.modelDataCount ===0 && __searchDevice.isChecked
            StyledLabel{
                text: qsTr("No device found, try refreshing to get it again.")
                anchors.centerIn: parent

            }
        }

        Item {
            width: 650
            height: 28
            anchors.left: rect.right
            anchors.leftMargin: 10
            anchors.top: tableViewStack.bottom
            anchors.topMargin: 30
            visible: !__empty.visible //(wifiTableView.modelDataCount !==0)
            Row
            {
                anchors.centerIn: parent
                spacing: 10
                StyledLabel{
                    width : 71
                    text: qsTr("Input Ip")
                    anchors.verticalCenter: parent.verticalCenter
                    visible : __manualConnect.isChecked
                }

                BasicDialogTextField {
                    id: __iptext
                    height: 28
                   // text: machine_height
                    font.pixelSize: 14
                    width : 276
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Qt.AlignVCenter
                    placeholderText : qsTr("Input Ip")
                    visible : __manualConnect.isChecked

                    validator: RegExpValidator {
                              regExp: /^((2(5[0-5]|[0-4]\d))|[0-1]?\d{1,2})(\.((2(5[0-5]|[0-4]\d))|[0-1]?\d{1,2})){3}$/
                              ///^(?!(0[0-9]{0,}$))[0-9]{1,}[.]{0,}[0-9]{0,}$/
                            }

                }

                BasicDialogButton{
                    width: 120
                    height : 28
                    text: qsTr("Connect")
                    //btnTextColor: "#ffffff"
                    btnRadius: 3
                    btnBorderW : 0
                    defaultBtnBgColor: Constants.profileBtnColor
        			hoveredBtnBgColor: Constants.profileBtnHoverColor
                    visible : __manualConnect.isChecked
                    onSigButtonClicked:
                    {
                        if(__iptext.text === "")
                        {
                            return ;
                        }

                        if(cwModel.isConnected(__iptext.text))
                        {                            
                            __cusConnectMsgDlg.msgText = __iptext.text + " " + qsTr("device already exist") + "..."
                            __cusConnectMsgDlg.show()
                            return;
                        }

                        cwModel.createConnect(__iptext.text)

                        __cusConnectMsgDlg.msgText = __iptext.text + " " + qsTr("connecting") + "..."
                        __cusConnectMsgDlg.show()

                    }
                }


                BasicDialogButton{
                    width: 120
                    height : 28
                    text: qsTr("Device Control")
                    //btnTextColor: "#ffffff"
                    btnRadius: 3
                    btnBorderW : 0
                    defaultBtnBgColor: Constants.profileBtnColor
        			hoveredBtnBgColor: Constants.profileBtnHoverColor
                    visible:false
                    onSigButtonClicked:
                    {
                        close()
                        switchControlDlg()
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

            CusSearchTableView{
                id:wifiTableView
                rowSpacing : 18
                rowHeight: 28
                width: 640
                model: cwModel

//                rowWidths: [width*0.2, width*0.25, width*0.15,width*0.2, width*0.18]
                rowWidths: [width*0.2, width*0.3, width*0.2,width*0.10, width*0.18]
                headTexts: [qsTr("Name"), qsTr("IP Address"), qsTr("State"), "",""]

                onViewCreateConnect:
                {
                    cwModel.reconnectIp(deviceDN)
                }

            }

            CusManualTableView{
                id:cloudTableView
                rowSpacing : 18
                rowHeight: 28
                width: 640
                model: cwModel

//                rowWidths: [width*0.2, width*0.25, width*0.15,width*0.2, width*0.18]
                rowWidths: [width*0.2, width*0.3, width*0.2,width*0.10, width*0.18]
                headTexts: [qsTr("Name"), qsTr("IP Address"), qsTr("State"), "",""]

                onViewCreateConnect:
                {
                    cwModel.createConnect(deviceDN)
                }
            }
        }
    }


    //    CusViewPrint{
    //        property int curIndex: 0
    //        id: viewPrintDialog
    //        width: 723
    //        height: 423
    //    }

    CusConnectMessageDlg
    {
        id : __cusConnectMsgDlg
    }


    Connections
    {
        target: cwModel
        onSigConnectedIpSuccessed:
        {
            __cusConnectMsgDlg.msgText = qsTr("connect success")
            __cusConnectMsgDlg.close()
        }
        onSigConnectedIpFailed:
        {
            __cusConnectMsgDlg.msgText = qsTr("connect failed")
        }
    }
    function refreshTableList()
    {

        cwModel.refreshWifiList()
    }

    function showTableList()
    {
        //        cwModel.showAllTableData()
    }
    function exportSlicer(filePath){
        //        cwModel.refreshFileName(filePath);
    }


}
