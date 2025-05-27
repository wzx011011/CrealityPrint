import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Window 2.12
import QtQuick.Layouts 1.13
import CrealityUI 1.0
import "qrc:/CrealityUI"
Item{

    //手动连接的表格
    property real headerHeight: 30
    property alias modelDataCount: tableView.count
    property alias model: tableView.model
    property real rowHeight: 30
    property real rowSpacing: 0
    property color backgroundColor: Constants.dialogContentBgColor//"#535353"
    property int rows: 10
    property int columns: 8
    property var stateObj: ({})

    property var connectStatusObj: ({})     //连接状态


    property color myFontColor: Constants.textColor//"white"

    property var rowWidths: [width*0.1, width*0.1, width*0.15,
        width*0.15, width*0.1, width*0.2, width*0.2]
    property var headTexts: [qsTr("Printer"), qsTr("Importing Files"), qsTr("File Name"),
        qsTr("IP Address"), qsTr("Progress"), qsTr("State"), ""]
//    signal viewPrintSig(int curIndex, string deviceIP, string deviceDN, string devceInteralName, int curState)
//    signal viewTransProcess(int curIndex, int progress, string deviceDN, string transFilePath)

    signal viewSwitchControl(int curIndex,string deviceDN, int curState)


     property var selectArray : []
    enum ConnectStatus
    {
        CONNECTED_STATE,   //连接的状态
        UNCONNECT_STATE      //未连接状态
    }

    enum TransAndPrintState
    {
        //初始状态
        INITIALICE_STATE =0,

        //传输过程
        //TRANS_PREPARED_STATE, //等待传输
        //TRANS_PAUSE_STATE, //传输暂停
        //TRANS_STOP_STATE, //传输停止
        //TRANSING_STATE, //传输中
//        TRANS_FINISHED_STATE, //传输完成
        //打印过程
        PRINTING_STATE, //打印中
        PRINT_FINISHED_STATE, //打印完成
        PRINT_FAILED_STATE,   //打印失败
        PRINT_STOP_STATE, //打印停止
        PRINT_PAUSE_STATE //打印暂停

    }

    id: tableViewItem
    CusTableViewHead{
        id:header
        width: parent.width
        height: headerHeight
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        color: Constants.dialogContentBgColor//"#535353"
        headTexts: tableViewItem.headTexts
    }

    ScrollView {
        width: parent.width
        height: parent.height - headerHeight - 10
        anchors.top: header.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
         clip: true
        Column {

             width: parent.width
             height: parent.height
            spacing: rowSpacing
            Repeater {
                id:tableView
                width: parent.width
                height:  parent.height
                clip: true





//    ListView{
//        id:tableView
//        width: parent.width
//        height: parent.height - headerHeight - 10
//        anchors.top: header.bottom
//        anchors.topMargin: 10
//        anchors.horizontalCenter: parent.horizontalCenter
//        clip: true
//        spacing: rowSpacing
        delegate: Rectangle{
            id:rowRec
            width: tableView.width
            height: rowHeight
            color:backgroundColor
            visible : isConnected
            Row{
                anchors.fill: parent
                spacing: 0
                Rectangle{
                    id:__selectRow
                    property bool isChecked: false
                    width: rowWidths[0]
                    height: parent.height
                    color:backgroundColor

                    StyleCheckBox
                    {
                        id: _checkBox
                        width: 18
                        height: 18
                        visible: (curState != CusControlTableView.TransAndPrintState.PRINTING_STATE && curState != CusControlTableView.TransAndPrintState.PRINT_PAUSE_STATE) //true
                        checked: __selectRow.isChecked
                        anchors.centerIn: parent
                        onCheckedChanged:
                        {
                            __selectRow.isChecked = checked

                            cwModel.refreshSelectStateByRow(deviceDN,checked)
                        }
                    }
                }

                Rectangle{
                    property string dName: deviceName
                    id:dNameRec
                    width: rowWidths[1]
                    height: parent.height
                    color: (_checkBox.visible && __selectRow.isChecked) ? "#1E9BE2" : backgroundColor
                    CusText{
                        id:nameText
                        fontWidth: parent.width
                        anchors.centerIn: parent
                        fontText: dNameRec.dName
                        fontColor: myFontColor
                        visible: (curTransProgress ===0)
                    }

                    Column{
                        width: parent.width
                        height: parent.height
                        visible: !(curTransProgress ===0)
                        Row{
                            width: parent.width
                            height: 20
                            StyledLabel{
                                width: parent.width - 40
                                height: 21
                                font.pixelSize:12
                                color: Constants.textColor//"white"
                                text: dNameRec.dName
                                elide: Text.ElideRight
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment:Text.AlignLeft
                            }
                            StyledLabel{
                                width: 40
                                height: 21
                                font.pixelSize:12
                                color: Constants.textColor//"white"
                                text: curTransProgress + " %"
                                elide: Text.ElideRight
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment:Text.AlignRight
                            }
                        }
                        ProgressBar{
                            id: __transprogressBar
                            from: 0
                            to:100
                            value: curTransProgress
                            width: parent.width
                            height: 4
                            visible: stateValues(curState)["progrssVisible"]

                            background: Rectangle {
                                implicitWidth: __transprogressBar.width
                                implicitHeight: __transprogressBar.height
                                color: "#303030"
                            }

                            contentItem: Item {
                                Rectangle {
                                    width: __transprogressBar.visualPosition * __transprogressBar.width
                                    height: __transprogressBar.height
                                    color:"#1E9BE2"
                                    /* {
                                        if(__transprogressBar.value == 0)
                                        {
                                            return "#ECECEC"
                                        }
                                        else if(__transprogressBar.value == 100)
                                        {
                                            return "#3ACE2F"
                                        }
                                        else{
                                            return "#1E9BE2"
                                        }
                                    }*/
                                }
                            }
                        }
                    }
                }

                Rectangle{
                    width: rowWidths[2]
                    height: parent.height
                    color: (_checkBox.visible && __selectRow.isChecked) ? "#1E9BE2" : backgroundColor
                    CusText{
                        fontWidth: parent.width
                        anchors.centerIn: parent
                        fontText: deviceIP
                        fontColor: myFontColor
                    }
                }



                Rectangle{
                    width: rowWidths[3]
                    height: parent.height
                    color: (_checkBox.visible && __selectRow.isChecked) ? "#1E9BE2" : backgroundColor
                    Row{
                        id : __stateRow
                        width: parent.width
                        height: 20
                        anchors.verticalCenter: parent.verticalCenter
                        visible: ! stateValues(curState)["progrssVisible"]
                        CusText{
                            fontWidth: parent.width
                            fontText: stateValues(curState)["stateText"]
                            fontColor: myFontColor
                        }
                    }

                    Column{
                        width: parent.width
                        height: 24
                        visible: ! __stateRow.visible
                        Row{
                            width: parent.width
                            height: 20
                            StyledLabel{
                                width: 100
                                height: 21
                                font.pixelSize:12
                                color: Constants.textColor//"white"
                                text: stateValues(curState)["stateText"]
                                //"printing..."
                                elide: Text.ElideRight
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment:Text.AlignLeft
                            }
                            StyledLabel{
                                width: parent.width - 100
                                height: 21
                                font.pixelSize:12
                                color: Constants.textColor//"white"
                                text: progressBar.value + " %"
                                elide: Text.ElideRight
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment:Text.AlignRight
                            }
                        }
                        ProgressBar{
                            id: progressBar
                            from: 0
                            to:100
                            value: curProgress
                            width: parent.width
                            height: 4

                            background: Rectangle {
                                implicitWidth: progressBar.width
                                implicitHeight: progressBar.height
                                color: "#303030"
                            }

                            contentItem: Item {
                                Rectangle {
                                    width: progressBar.visualPosition * progressBar.width
                                    height: progressBar.height
                                    color: {
                                        if(progressBar.value == 0)
                                        {
                                            return "#ECECEC"
                                        }
                                        else if(progressBar.value == 100)
                                        {
                                            return "#3ACE2F"
                                        }
                                        else{
                                            return "#1E9BE2"
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

//                Rectangle{
//                    width: rowWidths[3]
//                    height: parent.height
//                    color: __selectRow.isChecked ? "#1E9BE2" : backgroundColor
//                    CusText{
//                        fontWidth: parent.width
//                        anchors.centerIn: parent
//                        fontText: isConnected && isAllowTrans ? "isAllowTrans" : "Cannot Trans"
//                        fontColor: myFontColor
//                    }
//                }

                Rectangle{
                    width: rowWidths[4]
                    height: parent.height
                    color: backgroundColor
                    BasicDialogButton{
                        property bool isChecked: false
                        id:wifiBtn
                        x:12
                        width: parent.width -12
                        height: 28
                        text: curTransProgress===100 ? qsTr("Print") :  stateValues(curState)["btnText"]
                        anchors.centerIn: parent
                        btnRadius: 3
                        btnBorderW : 0
                        //defaultBtnBgColor :"#787878"
                        //hoveredBtnBgColor : "#1E9BE2"
                        defaultBtnBgColor: Constants.profileBtnColor
        	            hoveredBtnBgColor: Constants.profileBtnHoverColor
                        onSigButtonClicked:
                        {
                            curTransProgress = 0
                            viewSwitchControl(index,deviceDN, curState)
                        }
                    }
                }
            }
        }

      }
        }
    }

//    function choseFile(internalDeviceName, deviceDN)
//    {
//        if(fileDialog.dn === deviceDN){
//            let res = verifySlicer(internalDeviceName, fileDialog.file)
//            if(res)
//                cwModel.refreshFileName(deviceDN, fileDialog.file)
//        }
//    }

//    function verifySlicer(deviceType, filePath)
//    {
//        let res = cwModel.verifySlicer(deviceType, filePath);
//        if(!res){
//            //弹窗提示，设备型号不匹配
//            idMessageDlg.messageText = qsTr("The added model file does not match printer")
//            idMessageDlg.showSingleBtn()
//            idMessageDlg.show()
//            return false;
//        }else{
//            return true;
//        }
//    }

    function stateValues(curState)
    {
        switch(curState)
        {
            case CusControlTableView.TransAndPrintState.INITIALICE_STATE:
            case CusControlTableView.TransAndPrintState.PRINT_FINISHED_STATE:
            {
                //空闲
                stateObj["btnText"] = qsTr("View")
                stateObj["stateText"] = qsTr("Free")
                stateObj["progrssVisible"] = false
//                stateObj["btnText"] = qsTr("Connect")
    //            stateObj["btnColor"] = "#bebebe"
                break;
            }

//            case CusControlTableView.TransAndPrintState.TRANSING_STATE:
//            {
//                //传输中
//                stateObj["stateText"] = qsTr("Transferring")
//                stateObj["btnText"] = qsTr("View")
//                stateObj["progrssVisible"] = true
//                break;
//            }

            case CusControlTableView.TransAndPrintState.PRINTING_STATE:
            {
                //打印中
                stateObj["stateText"] = qsTr("Printing")
                stateObj["btnText"] = qsTr("View")
                stateObj["progrssVisible"] = true
                break;
            }
            case CusControlTableView.TransAndPrintState.PRINT_PAUSE_STATE:
            {
                stateObj["stateText"] = qsTr("Pause")
                stateObj["btnText"] = qsTr("View")
//                stateObj["btnColor"] = "#1e9be2"
                break;
            }

            case CusControlTableView.TransAndPrintState.PRINT_STOP_STATE:
            {
                stateObj["stateText"] = qsTr("Printing Stopped")
                stateObj["btnText"] = qsTr("View")
                stateObj["progrssVisible"] = false
                break;
            }
        }
        return stateObj;
    }

    Component.onCompleted:{
      //  cwModel.refreshWifiList()
    }
}


