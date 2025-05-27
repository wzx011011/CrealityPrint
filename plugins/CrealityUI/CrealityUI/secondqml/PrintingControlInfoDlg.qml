import QtQuick 2.10
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0 as QQC2
import CrealityUI 1.0
import "qrc:/CrealityUI"
import ".."
import "../qml"
BasicDialog{
    id: idPrintingControlInfoDlg
    width: 679 + 10
    height: 341
    titleHeight : 30
    title: qsTr("Print Control")


    property var deviceMap: {0: 0}

    signal sigViewPrintingInfomation(var deviceName, var deviceID, var deviceTBId, var iotType)
    signal sigPrintingStatus(var status)

    function deleteDeviceMap()
    {
        for(var key in deviceMap)
        {
            var strkey = "-%1-".arg(key)
			if(strkey != "-0-")
            {
                deviceMap[key].destroy()
                delete deviceMap[key]
            }
            else{
                 delete deviceMap[key]
            }
        }
    }

    function updateDeviceStatus()
    {

        updateDeviceQmlData()
    }

    function setServerPrintingDeviceData(strjson)
    {

        //create obj
        var componentGcode = Qt.createComponent("../secondqml/PrintingControlItem.qml")
        if(componentGcode.status === Component.Ready)
        {
            deleteDeviceMap()
            for(var i = 0; i < IotData.serDeviceNameArray.length; i++)
            {
                var obj = componentGcode.createObject(idPrinterManager, {"deviceName": IotData.serNickNameMap[IotData.serDeviceNameArray[i]], 
                                                                         "visible": true,
                                                                         "deviceID": IotData.serDeviceNameArray[i],
                                                                         "deviceTBId": IotData.serTbIdMap[IotData.serDeviceNameArray[i]]})
                obj.sigViewPrintBtnClicked.connect(viewPrintingInfo)
                deviceMap[IotData.serDeviceNameArray[i]] = obj
            }
        }
    }

    function viewPrintingInfo(name, id, tbId, type)
    {
        sigViewPrintingInfomation(name, id, tbId, type)
    }


    function updateDeviceInformation(productName)
    {
        updateDeviceQmlData()
    }

    function getPrintingStatus()
    {
        for(var i = 0; i < IotData.serDeviceNameArray.length; i++)
        {
            if(IotData.iotDeviceStatusMap[IotData.serDeviceNameArray[i]] == "online" && (IotData.iotPrintStateMap[IotData.serDeviceNameArray[i]] == 1 || IotData.iotPrintStateMap[IotData.serDeviceNameArray[i]] == 5))
            {
                sigPrintingStatus(true)
                return 
            }
            else{}
        }
        sigPrintingStatus(false)
        return 
    }

    function updateDeviceQmlData()
    {
         for(var i = 0; i < IotData.serDeviceNameArray.length; i++)
         {
            if(IotData.iotDeviceStatusMap[IotData.serDeviceNameArray[i]] == "online" && (IotData.iotPrintStateMap[IotData.serDeviceNameArray[i]] == 1 || IotData.iotPrintStateMap[IotData.serDeviceNameArray[i]] == 5))
            {
                deviceMap[IotData.serDeviceNameArray[i]].visible = true
            }
            else{
                deviceMap[IotData.serDeviceNameArray[i]].visible = false
            }

            if(IotData.iotVideoStatusMap[IotData.serDeviceNameArray[i]] == "1")
            {
                deviceMap[IotData.serDeviceNameArray[i]].isCameraShow = true
            }
            else{
                deviceMap[IotData.serDeviceNameArray[i]].isCameraShow = false
            }
            deviceMap[IotData.serDeviceNameArray[i]].printProgress = IotData.iotPrintProgressMap[IotData.serDeviceNameArray[i]]

            //time
            {
                var tmp = IotData.iotPrintedTimesMap[IotData.serDeviceNameArray[i]]
                
                var timeStr = ""
                if(tmp < 60)
                {
                    timeStr = tmp + "s"
                }
                else if(tmp > 60 && tmp < 60 * 60)
                {
                    timeStr = Math.floor(tmp/60) + "min" + (tmp - Math.floor(tmp/60) * 60) + "s"
                }
                else if(tmp > 60 * 60)
                {
                    timeStr = Math.floor(tmp/(60 * 60)) + "h" + Math.floor((tmp - Math.floor(tmp/(60 * 60)) * 3600)/60) + "min"
                }

                deviceMap[IotData.serDeviceNameArray[i]].printTime = timeStr
            }
         }

         getPrintingStatus()
    }
    

    Item{
        anchors.top: parent.top
        anchors.topMargin: 30 + 5+ 20
        anchors.left: parent.left
        anchors.leftMargin: 20 + 5

        width: parent.width - 10 - 20
        height: parent.height - 30 - 10 - 20 * 2

        Column{
            width: parent.width
            height: parent.height
            Row{
                width: parent.width
                height: 23
                spacing: 20
                StyledLabel{
                    width: 120
                    height: parent.height
                    verticalAlignment: Text.AlignTop
                    horizontalAlignment:Text.AlignHCenter
                    text: qsTr("Device Model")
                }
                StyledLabel{
                    width: 40
                    height: parent.height
                    verticalAlignment: Text.AlignTop
                    horizontalAlignment:Text.AlignHCenter
                    text: qsTr("Camera")
                }
                StyledLabel{
                    width: 120
                    height: parent.height
                    verticalAlignment: Text.AlignTop
                    horizontalAlignment:Text.AlignHCenter
                    text: qsTr("Printing Time")
                }
                StyledLabel{
                    width: 120
                    height: parent.height
                    verticalAlignment: Text.AlignTop
                    horizontalAlignment:Text.AlignHCenter
                    text: qsTr("Printing Progress")
                }
            }
            BasicScrollView
            {
                width: parent.width - 5
                height: parent.height-23
                visible: true
                hpolicy: ScrollBar.AlwaysOff
                vpolicy: ScrollBar.AsNeeded
                clip : true
                Column{
                    id:idPrinterManager
                    visible: true
                    spacing: 2
                }
            }
        }

    }
}