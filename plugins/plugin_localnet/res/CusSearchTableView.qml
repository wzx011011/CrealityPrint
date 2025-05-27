import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.13
import CrealityUI 1.0
import "qrc:/CrealityUI"
Item{

    //搜索的设备列表，表格
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
    signal viewPrintSig(int curIndex, string deviceIP, string deviceDN, string devceInteralName, int curState)
    signal viewTransProcess(int curIndex, int progress, string deviceDN, string transFilePath)

    signal viewCreateConnect(int curIndex,string deviceDN)

    enum ConnectStatus
    {
        CONNECTED_STATE,   //连接的状态
        UNCONNECT_STATE      //未连接状态
    }

//    enum TransAndPrintState
//    {
//        //初始状态
//        INITIALICE_STATE =0,
//        //传输过程
//        TRANSPREPARED_STATE, //等待传输
//        TRANS_PAUSE_STATE, //传输暂停
//        TRANS_STOP_STATE, //传输停止
//        TRANSING_STATE, //传输中
//        TRANS_FINISHED_STATE, //传输完成
//        //打印过程
//        PRINTING_STATE, //打印中
//        PRINT_PAUSE_STATE, //打印暂停
//        PRINT_PAUSEING_STATE,//打印暂停中
//        PRINT_STOP_STATE, //打印停止
//        PRINT_STOPING_STATE, //打印停止中
//        PRINT_FINISHED_STATE, //打印完成
//        TOKEN_ERROR,         //token 错误

//        //传输过程错误码
//        TRANS_ERROR_1 = 21,
//        TRANS_ERROR_2,
//        TRANS_ERROR_3,
//        //打印过程错误码
//        PRINT_ERROR_1 = 31,
//        PRINT_ERROR_2,
//        PRINT_ERROR_3,
//        PRINT_ERROR_4,

//        STARTING_STATE = 100,   //开始中的状态
//        UNCONNECT_STATE      //未连接状态


//    }

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

    ListView{
        id:tableView
        width: parent.width
        height: parent.height - headerHeight - 10
        anchors.top: header.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        clip: true
        spacing: rowSpacing
        delegate: Rectangle{
            id:rowRec
            width: tableView.width
            height: rowHeight
            color:backgroundColor
            visible: connectType == 1
            Row{
                anchors.fill: parent
                spacing: 0
                Rectangle{
                    property string dName: deviceName
                    id:dNameRec
                    width: rowWidths[0]
                    height: parent.height
                    color:backgroundColor
                    CusText{
                        id:nameText
                        fontWidth: parent.width
                        anchors.centerIn: parent
                        fontText: dNameRec.dName
                        fontColor: myFontColor
                        visible: true
                    }
                }

                Rectangle{
                    width: rowWidths[1]
                    height: parent.height
                    color:backgroundColor
                    CusText{
                        fontWidth: parent.width
                        anchors.centerIn: parent
                        fontText: deviceIP
                        fontColor: myFontColor
                    }
                }



                Rectangle{
                    width: rowWidths[2]
                    height: parent.height
                    color: backgroundColor
                    Row{
                        width: parent.width
                        height: 20
                        anchors.verticalCenter: parent.verticalCenter
                        CusText{
                            fontWidth: parent.width - 20
                            fontText: connectStatus(isConnected)["stateText"]
                            fontColor: myFontColor
                        }
                        CusSkinButton_Image
                        {
                            width : 14
                            height : 14
                            visible : !(isConnected && isAllowTrans)
                            tipText: qsTr("Some connect error")
                            btnImgUrl: "qrc:/photo/wifiwarning.png"
                        }
                    }
                }

                Rectangle{
                    width: rowWidths[3]
                    height: parent.height
                    color:backgroundColor
//                    CusText{
//                        fontWidth: parent.width
//                        anchors.centerIn: parent
//                        fontText: isConnected && isAllowTrans ? "isAllowTrans" : "Cannot Trans"
//                        fontColor: myFontColor
//                    }
                }

                Rectangle{
                    width: rowWidths[4]
                    height: parent.height
                    color:backgroundColor
                    BasicDialogButton{
                        property bool isChecked: false
                        id:wifiBtn
                        visible: !isConnected
                        width: parent.width
                        height: 28
                        text: connectStatus(isConnected)["btnText"]
                        anchors.centerIn: parent
                        btnRadius: 3
                        btnBorderW : 0
                        //defaultBtnBgColor : "#787878"
                        //hoveredBtnBgColor : "#1E9BE2"
                        defaultBtnBgColor: Constants.profileBtnColor
        	            hoveredBtnBgColor: Constants.profileBtnHoverColor
                        onSigButtonClicked:
                        {
                            viewCreateConnect(index,deviceDN)
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
//        //        let stateObj = {};
//        let isError = false;
////        stateObj["statusImgVisible"] = false
//        switch(curState)
//        {
//            case CusWifiTableView.ConnectStatus.CONNECTED_STATE:
//            {
//                stateObj["stateText"] = qsTr("Connected")
////                stateObj["btnText"] = qsTr("Connect")
//    //            stateObj["btnColor"] = "#bebebe"
//                break;
//            }

//            case CusWifiTableView.ConnectStatus.UNCONNECT_STATE:
//            {
//                stateObj["stateText"] = qsTr("Disconnect")
//                stateObj["btnText"] = qsTr("Connect")
//                break;
//            }
//        }
//        return stateObj;
    }
    function connectStatus(isConnect)
    {

        if(isConnect)
        {
            connectStatusObj["stateText"] = qsTr("Connected")
        }
        else
        {
            connectStatusObj["stateText"] = qsTr("Disconnect")
            connectStatusObj["btnText"] = qsTr("Connect")
        }

        return connectStatusObj;
    }

    Component.onCompleted:{
      //  cwModel.refreshWifiList()
    }
}


