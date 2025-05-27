import QtQuick 2.10
import QtQuick.Controls 2.12
import CrealityUI 1.0
import "qrc:/CrealityUI"
import "qrc:/localNet"
Item
{

    width: 800
    height: 600
    anchors.centerIn: parent
    visible: true
    function showWifiDevice()
    {
        __cusControlDlg.show()
    }

    function showFirstWifiConnect()
    {
        __cusControlDlg.show()
        //发现设备
        cwModel.findDeviceList()
    }

    function showTransControlDlg(filePath,fileName)
    {
        console.log("showTransControlDlg =" + filePath)
        __cusControlDlg.filename = fileName
        __cusControlDlg.realFilePath = filePath
        __cusControlDlg.show()
        //发现设备
        cwModel.findDeviceList()
    }

    DeviceConnectDlg
    {
        id : __cusDlg
        onSwitchControlDlg:
        {
            __cusControlDlg.show()
        }
    }

    DeviceControlDlg
    {
        id : __cusControlDlg
        onDialogClosed:
        {
            cwModel.removeTmpFilePath(__cusControlDlg.realFilePath)
            __cusControlDlg.filename = ""
            __cusControlDlg.realFilePath = ""

        }
        onSwitchDeviceConnect:
        {
            __cusDlg.show()
        }
        onSwitchPrintControl:
        {
            cwModel.removeTmpFilePath(__cusControlDlg.realFilePath)
            __cusControlDlg.filename = ""
            __cusControlDlg.realFilePath = ""

            __printControlDlg.initCurMac(deviceDN, curState)
            __printControlDlg.show()
            __printControlDlg.initGcodeList(cwModel.getGCodeListData(deviceDN))
            cwModel.resetSomeStatus(deviceDN)
        }
    }
    MyPrintControlDlg
    {
        id: __printControlDlg
        objectName : "mylocalprintcontrolobj"

        onSendControlPrinterCmd:
        {
            var cmdTypeTmp = cmdType
            console.log( "cmdTypeTmp=====" + cmdTypeTmp)
            cwModel.sendUIControlCmdToDevice( strmac, cmdTypeTmp, value)
        }
    }

}
