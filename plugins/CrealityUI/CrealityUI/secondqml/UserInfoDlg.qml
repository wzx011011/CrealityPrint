import QtQuick 2.10
import QtQuick.Controls 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0 as QQC2
import CrealityUI 1.0
import "qrc:/CrealityUI"
import ".."
import "../qml"
BasicDialog
{
    signal quitClicked()
    signal skipWebCrealityCloud()
    signal getMyGCodeList(var page, var size)
    signal deleteCurrentGcodeItem(var gcodeId)
    signal getMyModelList(var page, var size)
    signal deleteCurrentModelItem(var modelId)
    signal importModelItem(var modelId, var modelcount)
    signal importGCodeItem(var gcodeid, var gcodename, var gcodedownlink)
    signal shareModelGroup(var modelsId)
    signal getMyFavoritedList(var page, var size)
    signal cancelFavoritedModel(var modelId)
    signal getMyPrinterList()
    //signal userPrinterItemClick(var name, var id, var tbid, var iotType)
    signal pauseUpdateDeviceTask()
    //signal printGcodeToControl(var id, var type)
    signal getCloudSlicingList(var page, var size)
    signal personalSpaceTutorial(var type)
    signal getPurchasedModelList(var page, var size)
    signal sigupdateDeviceStatus()
    signal sigupdatePrintCtrlInfo(var productName)

    property var gcodeItemMap: {0:0}
    property var modelItemMap: {0:0}
    property var favoritedItemMap: {0:0}
    property var currentGCodePage: 1
    property var nextGCodePage: 1
    property var currentModelPage: ""
    property var nextModelPage: ""
    property var currentModelId: 0
    property var currentGcodeId: 0
    property var currentFavoritedPage: ""
    property var nextFavoritedPage: ""
    property var currentPrinterPage: 1
    property var nextPrinterPage: 1
    property var currentCloudSlicePage: 1
    property var nextCloudSlicePage: 1    
    property var cloudSliceItemMap: {0:0}
    property var currentPurchasedModelPage: ""
    property var nextPurchasedModelPage: ""
    property var purchasedItemMap: {0:0}

    property var deviceOn: qsTr("Online")
    property var deviceOff: qsTr("Offline")
    property var tfCardOn: qsTr("Card Inserted")
    property var tfCardOff: qsTr("No Card Inserted")
    property var printerOn: qsTr("Connected")
    property var printerOff: qsTr("Disconnected")

    property var printerImageMap: {0: 0} //ori
    property var deviceMap: {0: 0}  //ori
    property var proLeftBtnStatus: ""

    //op begin

    onDialogClosed:
    {
        proLeftBtnStatus = ""
    }

    onUpdateBtnClicked:
    {
        getMyPrinterList()
    }

    function setUserInfoDlgShow(type)
    {
        if(type == "mymodel")
        {
            onLeftButtonClick("model")
            idDialog.visible = true
        }
        else if(type == "myuploads")
        {
            onLeftButtonClick("gcode")
            idDialog.visible = true
        }
        else if(type == "singleprint")
        {
            onLeftButtonClick("printer")
            idDialog.visible = true
        }
        else{
            onLeftButtonClick("gcode")
            idDialog.visible = true
        }
    }

    onVisibleChanged:
    {
       if(idDialog.visible == true)
        {
            //onLeftButtonClick("gcode")
        }
        else{
            onLeftButtonClick("")
        }
    }

    function copyModelLinkSuccess()
    {
        warringPrinterClicklg.msgText = qsTr("Copy link successfully!")
        warringPrinterClicklg.visible = true
    }

//    function syncDeviceList()
//    {
//        var componentGcode = Qt.createComponent("../secondqml/UseInfoPrinterItem.qml")
//    }

    function setSerDeviceData(strjson)
    {
        //save data
        var objectArray = JSON.parse(strjson);
        if(objectArray.code === 0)
        {
            IotData.serDeviceNameArray = []
            var objResult = objectArray.result.list;
            for(var key in objResult)
            {
                IotData.serDeviceNameArray.push(objResult[key].deviceName)
                IotData.serIotIdMap[objResult[key].deviceName] = objResult[key].iotId
                IotData.serProductkeyMap[objResult[key].deviceName] = objResult[key].productKey
                IotData.serNickNameMap[objResult[key].deviceName] = objResult[key].nickName
                IotData.serSyDidMap[objResult[key].deviceName] = objResult[key].syDid
                IotData.serSyLecenseMap[objResult[key].deviceName] = objResult[key].syLicense
                IotData.serTbIdMap[objResult[key].deviceName] = objResult[key].tbId
                IotData.serIotType[objResult[key].deviceName] = objResult[key].iotType
            }
        }

        //create obj
        var componentGcode = Qt.createComponent("../secondqml/UseInfoPrinterItem.qml")
        if(componentGcode.status === Component.Ready)
        {
            deleteDeviceMap()
            for(var i = 0; i < IotData.serDeviceNameArray.length; i++)
            {
                var obj = componentGcode.createObject(idPrinterManager, {"equipmentName": IotData.serNickNameMap[IotData.serDeviceNameArray[i]], 
                                        "equipmentID": IotData.serDeviceNameArray[i], 
                                        "printerName": IotData.serNickNameMap[IotData.serDeviceNameArray[i]]})
                obj.sigButtonClicked.connect(skipWebCrealityCloud)//useInfoPrinterItemBtnClick
                deviceMap[IotData.serDeviceNameArray[i]] = obj

                deviceMap[IotData.serDeviceNameArray[i]].equipmentStatus = deviceOff
                deviceMap[IotData.serDeviceNameArray[i]].tFCardStatus = tfCardOff
                deviceMap[IotData.serDeviceNameArray[i]].printerStatus = printerOff
                deviceMap[IotData.serDeviceNameArray[i]].printerStatusShow = printerOff
            }
        }

    }

    function updateDeviceStatus(strjson, productName)
    {
        var statusRet = JSON.parse(strjson)
        if(statusRet.Success){ 
            if(statusRet.Data.Status === "ONLINE")
            {
                IotData.iotDeviceStatusMap[productName] = "online"
            }
            else{
                IotData.iotDeviceStatusMap[productName] = "offline"
            }
        }

        sigupdateDeviceStatus()
        updateDeviceQmlData()
    }

    function updateDeviceInformation_NewIOT_Position(strjson, productName)
    {
        var dataRet = JSON.parse(strjson)
        if(dataRet.code === 0){
            var dataResult = dataRet.result
            IotData.iotCurPositionMap[productName] = dataResult.curPosition
        }
    }

    function updateDeviceInformation_NewIOT_RealTime(strjson, productName)
    {
        var dataRet = JSON.parse(strjson)
        if(dataRet.code === 0){
            var dataResult = dataRet.result
            IotData.iotPrintProgressMap[productName] = dataResult.printProgress[0].value

            IotData.iotNozzleTempMap[productName] = dataResult.nozzleTemp[0].value

            IotData.iotBedTempMap[productName] = dataResult.bedTemp[0].value

            IotData.iotCurFeedratePctMap[productName] = dataResult.curFeedratePct[0].value

            IotData.iotPrintedTimesMap[productName] = dataResult.printJobTime[0].value

            IotData.iotTimesLeftToPrintMap[productName] = dataResult.printLeftTime[0].value
        }
    }

    function updateDeviceInformation_NewIOT(strjson, productName)
    {
        var dataRet = JSON.parse(strjson)
        if(dataRet.code === 0){
            var dataArray = dataRet.result
            for(var i in dataArray){
                if(dataArray[i].key === "connect")//printer status
                {
                    if(dataArray[i].value === 1){
                        IotData.iotPrinterStatusMap[productName] = printerOn
                    }else{
                        IotData.iotPrinterStatusMap[productName] = printerOff
                    }
                }
                else if(dataArray[i].key === "model") //printer name
                {
                    IotData.iotPrinterNameMap[productName] = dataArray[i].value.trim()
                }
                else if(dataArray[i].key === "tfCard")  //tfCard status
                {
                    if(dataArray[i].value == 1){
                        IotData.iotTFCardStatusMap[productName] = tfCardOn
                    }else{
                        IotData.iotTFCardStatusMap[productName] = tfCardOff
                    }
                }
                else if(dataArray[i].key === "APILicense")  //shangYunAPILicense
                {
                    IotData.iotShangYunAPILicenseMap[productName] = dataArray[i].value
                }
                else if(dataArray[i].key === "DIDString")  //shangYunDIDString
                {
                    IotData.iotShangYunDIDStringMap[productName] = dataArray[i].value
                }
                else if(dataArray[i].key === "InitString")  //shangYunInitString
                {
                    IotData.iotShangYunInitStringMap[productName] = dataArray[i].value
                }
                else if(dataArray[i].key === "video") //video
                {
                    IotData.iotVideoStatusMap[productName] = dataArray[i].value
                }
                else if(dataArray[i].key === "state") //state
                {
                    IotData.iotPrintStateMap[productName] = dataArray[i].value
                }
                else if(dataArray[i].key === "active") //printProgress
                {
                    if(dataArray[i].value == true)
                    {
                        IotData.iotDeviceStatusMap[productName] = "online"
                    }
                    else{
                        IotData.iotDeviceStatusMap[productName] = "offline"
                    }
                }
                else if(dataArray[i].key === "nozzleTemp2") //nozzleTemp2
                {
                    IotData.iotNozzleTempTargetMap[productName] = dataArray[i].value
                }
                else if(dataArray[i].key === "bedTemp2") //bedTemp2
                {
                    IotData.iotBedTempTargetMap[productName] = dataArray[i].value
                }
                else if(dataArray[i].key === "curFeedratePct")//setFeedratePct
                {
                    IotData.iotCurFeedratePctTargetMap[productName] = dataArray[i].value
                }
                else if(dataArray[i].key === "layer") //layer
                {
                    IotData.iotLayerPrintMap[productName] = dataArray[i].value
                }
                else if(dataArray[i].key === "printId") //printId
                {
                    IotData.iotPrintIDMap[productName] = dataArray[i].value
                }
                else if(dataArray[i].key === "curPosition")
                {
                    IotData.iotCurPositionMap[productName] = dataArray[i].value
                }
                else if(dataArray[i].key === "fan")
                {
                    IotData.iotFanSwitchMap[productName] = dataArray[i].value
                }
                else if(dataArray[i].key === "netIP")
                {
                    IotData.iotNetIPMap[productName] = dataArray[i].value
                }
                else{}
            }
            sigupdatePrintCtrlInfo(productName)
            updateDeviceQmlData()
        }
        else
        {
            console.log("userinfodlg error")
        }
        
    }

    function updateDeviceInformation(strjson, productName)
    {
        var dataRet = JSON.parse(strjson)
        if(dataRet.Success){
            var dataArray = dataRet.Data.List.PropertyStatusInfo
            for(var key in dataArray){
                if(dataArray[key].Identifier === "connect")//printer status
                {
                    if(dataArray[key].Value === "1"){
                        IotData.iotPrinterStatusMap[productName] = printerOn
                    }else{
                        IotData.iotPrinterStatusMap[productName] = printerOff
                    }
                }
                else if(dataArray[key].Identifier === "model") //printer name
                {
                    IotData.iotPrinterNameMap[productName] = dataArray[key].Value.trim()
                }
                else if(dataArray[key].Identifier === "tfCard")  //tfCard status
                {
                    if(dataArray[key].Value === "1"){
                        IotData.iotTFCardStatusMap[productName] = tfCardOn
                    }else{
                        IotData.iotTFCardStatusMap[productName] = tfCardOff
                    }
                }
                else if(dataArray[key].Identifier === "APILicense")  //shangYunAPILicense
                {
                    IotData.iotShangYunAPILicenseMap[productName] = dataArray[key].Value
                }
                else if(dataArray[key].Identifier === "DIDString")  //shangYunDIDString
                {
                    IotData.iotShangYunDIDStringMap[productName] = dataArray[key].Value
                }
                else if(dataArray[key].Identifier === "InitString")  //shangYunInitString
                {
                    IotData.iotShangYunInitStringMap[productName] = dataArray[key].Value
                }
                else if(dataArray[key].Identifier === "video") //video
                {
                    IotData.iotVideoStatusMap[productName] = dataArray[key].Value
                }
                else if(dataArray[key].Identifier === "state") //state
                {
                    IotData.iotPrintStateMap[productName] = dataArray[key].Value
                }
                else if(dataArray[key].Identifier === "printProgress") //printProgress
                {
                    IotData.iotPrintProgressMap[productName] = dataArray[key].Value
                }
                else if(dataArray[key].Identifier === "nozzleTemp") //nozzleTemp
                {
                    IotData.iotNozzleTempMap[productName] = dataArray[key].Value
                }
                else if(dataArray[key].Identifier === "bedTemp") //bedTemp
                {
                    IotData.iotBedTempMap[productName] = dataArray[key].Value
                }
                else if(dataArray[key].Identifier === "nozzleTemp2") //nozzleTemp2
                {
                    IotData.iotNozzleTempTargetMap[productName] = dataArray[key].Value
                }
                else if(dataArray[key].Identifier === "bedTemp2") //bedTemp2
                {
                    IotData.iotBedTempTargetMap[productName] = dataArray[key].Value
                }
                else if(dataArray[key].Identifier === "curFeedratePct") //curFeedratePct
                {
                    IotData.iotCurFeedratePctMap[productName] = dataArray[key].Value
                }
                else if(dataArray[key].Identifier === "printJobTime") //printJobTime
                {
                    IotData.iotPrintedTimesMap[productName] = dataArray[key].Value
                }
                else if(dataArray[key].Identifier === "timesLeftToPrint") //timesLeftToPrint
                {
                    IotData.iotTimesLeftToPrintMap[productName] = ""//dataArray[key].DataType
                }
                else if(dataArray[key].Identifier === "curFeedratePct")//setFeedratePct
                {
                    IotData.iotCurFeedratePctTargetMap[productName] = dataArray[key].Value
                }
                else if(dataArray[key].Identifier === "layer") //layer
                {
                    IotData.iotLayerPrintMap[productName] = dataArray[key].Value
                }
                else if(dataArray[key].Identifier === "printId") //printId
                {
                    IotData.iotPrintIDMap[productName] = dataArray[key].Value
                }
                else if(dataArray[key].Identifier === "curPosition")
                {
                    IotData.iotCurPositionMap[productName] = dataArray[key].Value
                }
                else if(dataArray[key].Identifier === "fan")
                {
                    IotData.iotFanSwitchMap[productName] = dataArray[key].Value
                }
                else if(dataArray[key].Identifier === "netIP")
                {
                    IotData.iotNetIPMap[productName] = dataArray[key].Value
                }
                else{}
            }
        }
        sigupdatePrintCtrlInfo(productName)
        updateDeviceQmlData()
    }

    function updateDeviceQmlData()
    {
        for(var i = 0; i < IotData.serDeviceNameArray.length; i++)
        {
            deviceMap[IotData.serDeviceNameArray[i]].printerIp = IotData.iotNetIPMap[IotData.serDeviceNameArray[i]]
            if(IotData.iotDeviceStatusMap[IotData.serDeviceNameArray[i]] == "online")
            {
                deviceMap[IotData.serDeviceNameArray[i]].equipmentStatus = deviceOn
            }
            else{
                deviceMap[IotData.serDeviceNameArray[i]].equipmentStatus = deviceOff
            }

            if(deviceMap[IotData.serDeviceNameArray[i]].equipmentStatus == deviceOn)
            {
                if(IotData.iotPrintStateMap[IotData.serDeviceNameArray[i]] == 1)
                {
                    deviceMap[IotData.serDeviceNameArray[i]].printerStatusShow = qsTr("Printing")
                    deviceMap[IotData.serDeviceNameArray[i]].progressVisible =  true
                    deviceMap[IotData.serDeviceNameArray[i]].progressValue =  IotData.iotPrintProgressMap[IotData.serDeviceNameArray[i]]
                }
                else if(IotData.iotPrintStateMap[IotData.serDeviceNameArray[i]] == 5)
                {
                    deviceMap[IotData.serDeviceNameArray[i]].printerStatusShow = qsTr("Printing pause")
                    deviceMap[IotData.serDeviceNameArray[i]].progressVisible =  true
                    deviceMap[IotData.serDeviceNameArray[i]].progressValue =  IotData.iotPrintProgressMap[IotData.serDeviceNameArray[i]]
                }
                else{
                    deviceMap[IotData.serDeviceNameArray[i]].printerStatusShow = IotData.iotPrinterStatusMap[IotData.serDeviceNameArray[i]]
                    deviceMap[IotData.serDeviceNameArray[i]].progressVisible =  false
                    deviceMap[IotData.serDeviceNameArray[i]].progressValue =  0
                }

                deviceMap[IotData.serDeviceNameArray[i]].tFCardStatus = IotData.iotTFCardStatusMap[IotData.serDeviceNameArray[i]]
                deviceMap[IotData.serDeviceNameArray[i]].printerStatus = IotData.iotPrinterStatusMap[IotData.serDeviceNameArray[i]]
                //update device image
                deviceMap[IotData.serDeviceNameArray[i]].printerImage = printerImageMap[IotData.iotPrinterNameMap[IotData.serDeviceNameArray[i]]]
            }
            else{
                deviceMap[IotData.serDeviceNameArray[i]].progressVisible =  false
                deviceMap[IotData.serDeviceNameArray[i]].progressValue =  0
                deviceMap[IotData.serDeviceNameArray[i]].tFCardStatus = tfCardOff
                deviceMap[IotData.serDeviceNameArray[i]].printerStatus = printerOff
                deviceMap[IotData.serDeviceNameArray[i]].printerStatusShow = printerOff
            }

            if(deviceMap[IotData.serDeviceNameArray[i]].equipmentStatus == deviceOn && deviceMap[IotData.serDeviceNameArray[i]].tFCardStatus == tfCardOn && deviceMap[IotData.serDeviceNameArray[i]].printerStatus == printerOn)
            {
                deviceMap[IotData.serDeviceNameArray[i]].itemEnable = true
            }
            else{
                deviceMap[IotData.serDeviceNameArray[i]].itemEnable = false
            }
            
        }   
    }

//    function useInfoPrinterItemBtnClick(deviceName, deviceID)
//    {
//        if((deviceMap[deviceID].tFCardStatus == tfCardOn)&&(deviceMap[deviceID].printerStatus == printerOn)&&(deviceMap[deviceID].equipmentStatus == deviceOn))
//        {
//            userPrinterItemClick(deviceName, deviceID, IotData.serTbIdMap[deviceID], IotData.serIotType[deviceID])
//        }
//        else if((deviceMap[deviceID].tFCardStatus == tfCardOn)&&(deviceMap[deviceID].printerStatus == printerOn)&&(deviceMap[deviceID].equipmentStatus != deviceOn))
//        {
//            warringPrinterClicklg.msgText = qsTr("The device is offline. Printing Failure.")
//            warringPrinterClicklg.visible = true
//        }
//        else if((deviceMap[deviceID].tFCardStatus != tfCardOn)&&(deviceMap[deviceID].printerStatus == printerOn)&&(deviceMap[deviceID].equipmentStatus == deviceOn))
//        {
//            warringPrinterClicklg.msgText = qsTr("Unable to identify the TF card information. Printing Failure.")
//            warringPrinterClicklg.visible = true
//        }
//        else if((deviceMap[deviceID].tFCardStatus = tfCardOn)&&(deviceMap[deviceID].printerStatus != printerOn)&&(deviceMap[deviceID].equipmentStatus == deviceOn))
//        {
//            warringPrinterClicklg.msgText = qsTr("The device is not connected to the printer. Printing Failure.")
//            warringPrinterClicklg.visible = true
//        }
//        else{
//            warringPrinterClicklg.msgText = qsTr("The device is offline. Printing Failure.")
//            warringPrinterClicklg.visible = true
//        }
//    }

    function setPrinterImageMap(strjson)
    {
        var objectArray = JSON.parse(strjson);
        if(objectArray.code === 0)
        {
            var objResult = objectArray.result.list;
            for ( var key in objResult ) {
                printerImageMap[objResult[key].name] = objResult[key].thumbnail
            }
        }
        else{}
    }

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

    function setUserInfo(name, id, imageUrl, usedSpace, maxSpace)
    {
        idNickNameLabel.text = name
        idUserIdLabel.userid = id
        idAvtorImage.img_src = imageUrl
        progressBar.value = Math.ceil((usedSpace/maxSpace)*100)
        idSpceLabel.text = getSpaceUnit(usedSpace)+"/"+getSpaceUnit(maxSpace)
    }

    function getSpaceUnit(value)
    {
        if(value > 1024*1024*1024)
        {
            var num = Number(value/(1024*1024*1024)).toFixed(3)
            var index = num.indexOf('.')
            if (index !== -1) {
                num = num.substring(0, 2 + index + 1)
            }

            return num + "GB"//Math.ceil(value/(1024*1024*1024))+"GB"
        }
        else if(value > 1024*1024)
        {
            var num = Number(value/(1024*1024)).toFixed(3)
            var index = num.indexOf('.')
            if (index !== -1) {
                num = num.substring(0, 2 + index + 1)
            }

            return num + "MB"//Math.ceil(value/(1024*1024))+"MB"
        }
        else if(value > 1024)
        {
            var num = Number(value/1024).toFixed(3)
            var index = num.indexOf('.')
            if (index !== -1) {
                num = num.substring(0, 2 + index + 1)
            }

            return num + "KB"//Math.ceil(value/1024)+"KB"
        }
        else
        {
            return value+"B"
        }
    }

    function setMyGcodeList(strjson, page)
    {
        var componentGcode = Qt.createComponent("../secondqml/BasicUseInfoItem.qml")
        if(componentGcode.status === Component.Ready)
        {   
            var objectArray = JSON.parse(strjson);
            if(objectArray.code === 0)
            {
                idMyGcodeList.rows = Math.ceil((page*10)/2)
                var objResult = objectArray.result.list;
                currentGCodePage = page
                if(currentGCodePage == 1)
                {
                    deleteGcodeComponent()
                }
                if(objResult.length === 10)
                {
                    nextGCodePage = currentGCodePage + 1
                }
                for ( var key in objResult ) {
                    var obj = componentGcode.createObject(idMyGcodeList, {"baseName": objResult[key].name, 
                                                    "baseImage": objResult[key].thumbnail, 
                                                    "baseId": objResult[key].id, 
                                                    "downloadlink": objResult[key].downloadLink})
                    obj.deleteCurrentItem.connect(deleteGCodeCurrentGcodeItem)
                    obj.importCurrentItem.connect(importCurrentGCodeItem)
                    obj.sigGcodePrint.connect(skipWebCrealityCloud)//printCurrentGcode
                    gcodeItemMap[objResult[key].id] = obj
                }

                if(Object.keys(gcodeItemMap).length <= 0)
                {
                    idGcodeAddGcodeRow.visible = true
                    idMySlicesBtn.isImageBtnShow = false
                }
                else{
                    idGcodeAddGcodeRow.visible = false
                    idMySlicesBtn.isImageBtnShow = true
                }
                
            }
            else{}
        }
    }

    function setCloudSliceList(strjson, page)
    {
        var componentGcode = Qt.createComponent("../secondqml/BasicUseInfoItem.qml")
        if(componentGcode.status === Component.Ready)
        {   
            var objectArray = JSON.parse(strjson);
            if(objectArray.code === 0)
            {
                idCloudSliceList.rows = Math.ceil((page*10)/2)
                var objResult = objectArray.result.list;
                currentCloudSlicePage = page
                if(currentCloudSlicePage == 1)
                {
                    deleteCloudSliceComponent()
                }
                if(objResult.length === 10)
                {
                    nextCloudSlicePage = currentCloudSlicePage + 1
                }
                for ( var key in objResult ) {
                    if(objResult[key].state == 1)
                    {
                        var obj = componentGcode.createObject(idCloudSliceList, {"baseName": objResult[key].modelInfo.name, 
                                                    "baseImage": objResult[key].thumbnail, 
                                                    "baseId": objResult[key].id, 
                                                    "downloadlink": objResult[key].downloadLink})
                        obj.deleteCurrentItem.connect(deleteGCodeCurrentGcodeItem)
                        obj.importCurrentItem.connect(importCurrentGCodeItem)
                        obj.sigGcodePrint.connect(skipWebCrealityCloud)//printCurrentGcode
                        cloudSliceItemMap[objResult[key].id] = obj
                    }
                }
            }
            else{}
        }
    }

    function printCloudPrinting(id)
    {
        printGcodeToControl(id, "gcode")
    }

//    function printCurrentGcode(valueId)
//    {
//        printGcodeToControl(valueId, proLeftBtnStatus)
//    }

    function myModelDownloadSuccess()
    {
        idDialog.visible = false
        idUserInfoBtnList.enabled = true
        idCloudSliceList.enabled = true
        idMyGcodeList.enabled = true
        idMyModelList.enabled = true
        idPurchasedList.enabled = true

        if(modelItemMap[currentModelId] != undefined)
        {
            modelItemMap[currentModelId].setAnimatedImageStatus(false)
        }
        if(gcodeItemMap[currentGcodeId] != undefined)
        {
            gcodeItemMap[currentGcodeId].setAnimatedImageStatus(false)
        }
        if(cloudSliceItemMap[currentGcodeId] != undefined)
        {
            if(cloudSliceItemMap[currentGcodeId] != 0)
            {
                cloudSliceItemMap[currentGcodeId].setAnimatedImageStatus(false)
            }
            
        }
        if(purchasedItemMap[currentModelId] != undefined)
        {
            purchasedItemMap[currentModelId].setAnimatedImageStatus(false)
        }
        
    }

    function myModelDownloadFailed()
    {
        myModelDownloadSuccess()

        warringPrinterClicklg.msgText = qsTr("Failed to import model or gcode!")
        warringPrinterClicklg.visible = true
    }

    function importCurrentGCodeItem(id, name, downloadlink)
    {
        currentGcodeId = id
        
        importGCodeItem(id, name, downloadlink)

        idUserInfoBtnList.enabled = false
        idCloudSliceList.enabled = false
        idMyGcodeList.enabled = false
        idMyModelList.enabled = false
    }

    function deleteGCodeCurrentGcodeItem(id)
    {
        deleteCurrentGcodeItem(id)
    }
    function deleteGCodeResult(id)
    {
        if(gcodeItemMap[id] != undefined)
        {
            gcodeItemMap[id].destroy()
            delete gcodeItemMap[id]
        }
        
        if(cloudSliceItemMap[id] != undefined)
        {
            cloudSliceItemMap[id].destroy()
            delete cloudSliceItemMap[id]
        }
    }

    function deleteGcodeComponent()
    {
        for(var key in gcodeItemMap)
        {
            var strkey = "-%1-".arg(key)
			if(strkey != "-0-")
            {
                gcodeItemMap[key].destroy()
                delete gcodeItemMap[key]
            }
            else{
                 delete gcodeItemMap[key]
            }
        }
        
    }

    function setPurchasedList(strjson, page)
    {
        var componentGcode = Qt.createComponent("../secondqml/BasicUseInfoItem.qml")
        if(componentGcode.status === Component.Ready)
        {
            var objectArray = JSON.parse(strjson);
            if(objectArray.code === 0)
            {
                idPurchasedList.rows = Math.ceil((page*10)/2)
                var objResult = objectArray.result.list;
                currentPurchasedModelPage = page
                if(currentPurchasedModelPage == "")
                {
                    deletePurchasedComponent()
                }
                nextPurchasedModelPage =  objectArray.result.nextCursor
                for ( var key in objResult ) {
                    var objCovers = objResult[key].covers
                    var obj = componentGcode.createObject(idPurchasedList, {"baseName": objResult[key].groupName, 
                                                                        "baseImage": objResult[key].covers[0].url, 
                                                                        "baseId": objResult[key].modelId,
                                                                        "modelcount": objResult[key].modelCount,
                                                                        "baseType": 2,
                                                                        "isPreserveAspectCrop": true,
                                                                        "shareVisible": false,
                                                                        "deleteVisible": false,
                                                                        "printVisible": false})         
                    obj.importModel.connect(importCurrentModel)                                                                 
                    purchasedItemMap[objResult[key].modelId] = obj
                }
            }
            else{}
            
        }
    }

    function setMyModelList(strjson, page)
    {
        var componentGcode = Qt.createComponent("../secondqml/BasicUseInfoItem.qml")
        if(componentGcode.status === Component.Ready)
        {
            var objectArray = JSON.parse(strjson);
            if(objectArray.code === 0)
            {
                idMyModelList.rows = Math.ceil((page*10)/2)
                var objResult = objectArray.result.list;
                currentModelPage = page
                if(currentModelPage == "")
                {
                    deleteModelComponent()
                }
                nextModelPage =  objectArray.result.nextCursor
                for ( var key in objResult ) {
                    var objCovers = objResult[key].covers
                    var obj = componentGcode.createObject(idMyModelList, {"baseName": objResult[key].groupName, 
                                                                        "baseImage": objResult[key].covers[0].url, 
                                                                        "baseId": objResult[key].id,
                                                                        "modelcount": objResult[key].modelCount,
                                                                        "baseType": 2,
                                                                        "isPreserveAspectCrop": true,
                                                                        "printVisible": false})
                    obj.deleteCurrentItem.connect(deleteModelCurrentModelItem)            
                    obj.importModel.connect(importCurrentModel)              
                    obj.sigShareModel.connect(shareCurrentModel)                                                             
                    modelItemMap[objResult[key].id] = obj
                }
                if(Object.keys(modelItemMap).length <= 0)
                {
                    idModelAddModelRow.visible = true
                    idMyModelBtn.isImageBtnShow = false
                }
                else{
                    idModelAddModelRow.visible = false
                    idMyModelBtn.isImageBtnShow = true
                }
            }
            else{}
            
        }
    }

    function setFavoritedList(strjson, page)
    {
        var componentGcode = Qt.createComponent("../secondqml/BasicUseInfoItem.qml")
        if(componentGcode.status === Component.Ready)
        {
            var objectArray = JSON.parse(strjson);
            if(objectArray.code === 0)
            {
                idMyFavoritedList.rows = Math.ceil((page*10)/2)
                var objResult = objectArray.result.list;
                currentFavoritedPage = page
                if(currentFavoritedPage == "")
                {
                    deleteFavoritedComponent()
                }
                nextFavoritedPage =  objectArray.result.nextCursor
                for ( var key in objResult ) {
                    var objCovers = objResult[key].covers
                    var obj = componentGcode.createObject(idMyFavoritedList, {"baseName": objResult[key].groupName, 
                                                                        "baseImage": objResult[key].covers[0].url, 
                                                                        "baseId": objResult[key].id,
                                                                        "modelcount": objResult[key].modelCount,
                                                                        "baseType": 2,
                                                                        "useType": 3,
                                                                        "isPreserveAspectCrop": true,
                                                                        "printVisible": false})
                    obj.deleteCurrentItem.connect(cancelFavoritedItem)            
                    obj.importModel.connect(importCurrentModel)              
                    obj.sigShareModel.connect(shareCurrentModel)                                                              
                    favoritedItemMap[objResult[key].id] = obj
                }

                // if(Object.keys(favoritedItemMap).length <= 0)
                // {
                //     idFavoritedAddModelRow.visible = true
                //     idMyFavoritedBtn.isImageBtnShow = false
                // }
                // else{
                //     idFavoritedAddModelRow.visible = false
                //     idMyFavoritedBtn.isImageBtnShow = true
                // }
            }
            else{}
            
        }
    }

    function shareCurrentModel(id)
    {
        console.log("userinfo dlg shareCurrentModel id = " + id)
        shareModelGroup(id)
    }

    function deleteFavoritedComponent()
    {
        for(var key in favoritedItemMap)
        {
            var strkey = "-%1-".arg(key)
			if(strkey != "-0-")
            {
                favoritedItemMap[key].destroy()
                delete favoritedItemMap[key]
            }
            else{
                delete favoritedItemMap[key]
            }
        }
    }

    function deleteCloudSliceComponent()
    {
        for(var key in cloudSliceItemMap)
        {
            var strkey = "-%1-".arg(key)
			if(strkey != "-0-")
            {
                cloudSliceItemMap[key].destroy()
                delete cloudSliceItemMap[key]
            }
            else{
                 delete cloudSliceItemMap[key]
            }
        }
    }

    function deleteModelComponent()
    {
        for(var key in modelItemMap)
        {
            var strkey = "-%1-".arg(key)
			if(strkey != "-0-")
            {
                modelItemMap[key].destroy()
                delete modelItemMap[key]
            }
            else{
                delete modelItemMap[key]
            }
        }
    }

    function deletePurchasedComponent()
    {
        for(var key in purchasedItemMap)
        {
            var strkey = "-%1-".arg(key)
			if(strkey != "-0-")
            {
                purchasedItemMap[key].destroy()
                delete purchasedItemMap[key]
            }
            else{
                delete purchasedItemMap[key]
            }
        }
    }

    function cancelFavoritedItem(id)
    {
        cancelFavoritedModel(id)
    }

    function cancelFavoritedResult(id)
    {
        favoritedItemMap[id].destroy()
        delete favoritedItemMap[id]
    }

    function deleteModelCurrentModelItem(id)
    {
        deleteCurrentModelItem(id)
    }
    function deleteModelResult(id)
    {
        modelItemMap[id].destroy()
        delete modelItemMap[id]
    }

    function importCurrentModel(id, count)
    {
        currentModelId = id
        importModelItem(id, count)

        idUserInfoBtnList.enabled = false
        idMyGcodeList.enabled = false
        idMyModelList.enabled = false
        idPurchasedList.enabled = false
    }

    function onLeftButtonClick(button)
    {
        if(button == "printer")
        {
            idDialog.updateBtnVisible = true
        }
        else{
            idDialog.updateBtnVisible = false
        }
        if(button == "gcode" && proLeftBtnStatus != "gcode"){
            idMySlicesBtn.btnSelected = true
            idMyGcodeBtn.bottonSelected = true
            idCloudSliceBtn.bottonSelected = false
            idMyModelBtn.btnSelected = false
            idMyUploadModelBtn.bottonSelected = false
            idPrinterBtn.btnSelected = false
            idMyFavoritedBtn.bottonSelected = false
            idConfigureFileBtn.btnSelected = false

            idMyGcodeScrollView.visible = true
            idCloudSliceScrollView.visible = false
            idMyModeScrollView.visible = false
            idFavoritedScrollView.visible = false
            idPurchasedScrollView.visible = false
            idPrinterManagerScrollView.visible = false
            idConfigurationFileScrollView.visible = false
            currentGCodePage = 1
            if(currentGCodePage === 1){
                getMyGCodeList(currentGCodePage, 10)
            }

        }
        else if(button == "cloudslice" && proLeftBtnStatus != "cloudslice")
        {
            idMySlicesBtn.btnSelected = true
            idMyGcodeBtn.bottonSelected = false
            idCloudSliceBtn.bottonSelected = true
            idMyModelBtn.btnSelected = false
            idMyUploadModelBtn.bottonSelected = false
            idPrinterBtn.btnSelected = false
            idMyFavoritedBtn.bottonSelected = false
            idConfigureFileBtn.btnSelected = false

            idMyGcodeScrollView.visible = false
            idCloudSliceScrollView.visible = true
            idMyModeScrollView.visible = false
            idFavoritedScrollView.visible = false
            idPurchasedScrollView.visible = false
            idPrinterManagerScrollView.visible = false
            idConfigurationFileScrollView.visible = false
            currentCloudSlicePage = 1
            if(currentCloudSlicePage === 1)
            {
                getCloudSlicingList(currentCloudSlicePage, 10)
            }
        }
        else if(button == "model" && proLeftBtnStatus != "model"){
            idMySlicesBtn.btnSelected = false
            idMyGcodeBtn.bottonSelected = false
            idCloudSliceBtn.bottonSelected = false
            idMyModelBtn.btnSelected = true
            idMyUploadModelBtn.bottonSelected = true
            idPrinterBtn.btnSelected = false
            idMyFavoritedBtn.bottonSelected = false
            idMyPurchasedBtn.bottonSelected = false
            idConfigureFileBtn.btnSelected = false

            idMyGcodeScrollView.visible = false
            idCloudSliceScrollView.visible = false
            idMyModeScrollView.visible = true
            idFavoritedScrollView.visible = false
            idPurchasedScrollView.visible = false
            idPrinterManagerScrollView.visible = false
            idConfigurationFileScrollView.visible = false
            currentModelPage = ""
            if(currentModelPage === ""){
                getMyModelList(currentModelPage, 10)
            }

        }else if(button == "printer" && proLeftBtnStatus != "printer"){
            idMySlicesBtn.btnSelected = false
            idMyGcodeBtn.bottonSelected = false
            idCloudSliceBtn.bottonSelected = false
            idMyModelBtn.btnSelected = false
            idMyUploadModelBtn.bottonSelected = false
            idPrinterBtn.btnSelected = true
            idMyFavoritedBtn.bottonSelected = false
            idMyPurchasedBtn.bottonSelected = false
            idConfigureFileBtn.btnSelected = false

            idMyGcodeScrollView.visible = false
            idCloudSliceScrollView.visible = false
            idMyModeScrollView.visible = false
            idFavoritedScrollView.visible = false
            idPurchasedScrollView.visible = false
            idPrinterManagerScrollView.visible = true
            idConfigurationFileScrollView.visible = false

             updateDeviceQmlData()
        }else if(button == "favorited" && proLeftBtnStatus != "favorited"){
            idMySlicesBtn.btnSelected = false
            idMyGcodeBtn.bottonSelected = false
            idCloudSliceBtn.bottonSelected = false
            idMyModelBtn.btnSelected = true
            idMyUploadModelBtn.bottonSelected = false
            idPrinterBtn.btnSelected = false
            idMyFavoritedBtn.bottonSelected = true
            idMyPurchasedBtn.bottonSelected = false
            idConfigureFileBtn.btnSelected = false

            idMyGcodeScrollView.visible = false
            idCloudSliceScrollView.visible = false
            idMyModeScrollView.visible = false
            idFavoritedScrollView.visible = true
            idPurchasedScrollView.visible = false
            idPrinterManagerScrollView.visible = false
            idConfigurationFileScrollView.visible = false
            currentFavoritedPage = ""
            if(currentFavoritedPage === ""){
                getMyFavoritedList(currentFavoritedPage, 10)
            }


        }else if(button == "configurationfile" && proLeftBtnStatus != "configurationfile"){
            idMySlicesBtn.btnSelected = false
            idMyGcodeBtn.bottonSelected = false
            idCloudSliceBtn.bottonSelected = false
            idMyModelBtn.btnSelected = false
            idMyUploadModelBtn.bottonSelected = false
            idPrinterBtn.btnSelected = false
            idMyFavoritedBtn.bottonSelected = false
            idMyPurchasedBtn.bottonSelected = false
            idConfigureFileBtn.btnSelected = true

            idMyGcodeScrollView.visible = false
            idCloudSliceScrollView.visible = false
            idMyModeScrollView.visible = false
            idFavoritedScrollView.visible = false
            idPurchasedScrollView.visible = false
            idPrinterManagerScrollView.visible = false
            idConfigurationFileScrollView.visible = true

        }
        else if(button == "purchased" && proLeftBtnStatus != "purchased")
        {
            idMySlicesBtn.btnSelected = false
            idMyGcodeBtn.bottonSelected = false
            idCloudSliceBtn.bottonSelected = false
            idMyModelBtn.btnSelected = true
            idMyUploadModelBtn.bottonSelected = false
            idPrinterBtn.btnSelected = false
            idMyFavoritedBtn.bottonSelected = false
            idMyPurchasedBtn.bottonSelected = true
            idConfigureFileBtn.btnSelected = false

            idMyGcodeScrollView.visible = false
            idCloudSliceScrollView.visible = false
            idMyModeScrollView.visible = false
            idFavoritedScrollView.visible = false
            idPurchasedScrollView.visible = true
            idPrinterManagerScrollView.visible = false
            idConfigurationFileScrollView.visible = false
            currentPurchasedModelPage = ""
            if(currentPurchasedModelPage === ""){
                getPurchasedModelList(currentPurchasedModelPage, 10)
            }

        }

        proLeftBtnStatus = button
    }

    id: idDialog
    width: 730*screenScaleFactor
    height: 593*screenScaleFactor
    titleHeight : 30*screenScaleFactor
    title: qsTr("Personal Center")

    Column{
        anchors.top: parent.top
        anchors.topMargin: titleHeight+5+10
        anchors.left: parent.left
        anchors.leftMargin: 26*screenScaleFactor
        anchors.bottomMargin: 26*screenScaleFactor
        width: parent.width-31*screenScaleFactor
        height: parent.height-titleHeight - 41*screenScaleFactor
        spacing: 10

        RowLayout {
            width: parent.width - 21 * screenScaleFactor
//            width: parent.width-21*screenScaleFactor
//            spacing: 150

            BaseCircularImage{
                id: idAvtorImage

                Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                width: 70*screenScaleFactor
                height: 70*screenScaleFactor

                img_src: ""//"qrc:/UI/photo/crealityIcon.png"
            }

            ColumnLayout {
                Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                spacing: 5 * screenScaleFactor

                StyledLabel{
                    id: idNickNameLabel
                    Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                    width: 130 * screenScaleFactor
                    text: ""
                    elide: Text.ElideRight
                    font.family: Constants.labelFontFamily
                    font.pointSize: Constants.labelFontPointSize
                }

                StyledLabel{
                    id: idUserIdLabel
                    Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                    width: idNickNameLabel.width
                    property string userid: ""
                    text: qsTr("ID: ") + userid
                    font.family: Constants.labelFontFamily
                    font.pointSize: Constants.labelFontPointSize
                }
            }

            StyledLabel{
                id: idStorageSpace
                Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                text: qsTr("Remaining Space:")
                font.family: Constants.labelFontFamily
                font.pointSize: Constants.labelFontPointSize
            }

            ColumnLayout {
                Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft

                StyledLabel{
                    id: idSpceLabel
                    Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                    text:""
                    font.pointSize:Constants.labelFontPointSize
                }

                ProgressBar{
                    id: progressBar
                    Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                    from: 0
                    to:100
                    width: 170 * screenScaleFactor
                    height: 3 * screenScaleFactor
                    background: Rectangle {
                        implicitWidth: progressBar.width
                        implicitHeight: progressBar.height
                        color: Constants.progressBarBgColor
                    }
                    contentItem: Item {
                        Rectangle {
                            width: progressBar.visualPosition * progressBar.width
                            height: progressBar.height
                            color: "#1E9BE2"
                        }
                    }
                }
            }

            BasicButton{
                Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                Layout.minimumWidth: 140 * screenScaleFactor
                height: 36 * screenScaleFactor
                btnRadius: 3
                btnBorderW:0
                defaultBtnBgColor: Constants.profileBtnColor
                hoveredBtnBgColor: Constants.profileBtnHoverColor
                pointSize:Constants.labelFontPointSize
                text: qsTr("Quit")
                onSigButtonClicked:
                {
                    pauseUpdateDeviceTask()
                    quitClicked()
                }
            }
        }

        Item {
            width:parent.width - 21*screenScaleFactor
            height: 1
            BasicSeparator
            {
                anchors.fill: parent
            }
        }
        Rectangle{
            width: parent.width - 21*screenScaleFactor
            height: 1
            color: "transparent"
        }
        Row{
            spacing: 20
            BasicScrollView
            {
                hpolicy: ScrollBar.AlwaysOff
                vpolicy: ScrollBar.AsNeeded
                clip : true
                Column{
                    id: idUserInfoBtnList
                    spacing:10
                    Column{
                        width: 140*screenScaleFactor
                        height: idMySlicesBtn.btnSelected ? 94*screenScaleFactor : 36*screenScaleFactor
                        spacing: 1
                        UserInfoLeftButton{
                            id: idMySlicesBtn
                            width: 140*screenScaleFactor
                            height: 36*screenScaleFactor
                            btnRadius:3
                            btnBorderW: 0
                            btnSelected: true
                            pointSize:Constants.labelFontPointSize
                            defaultBtnBgColor: Constants.dialogContentBgColor
                            hoveredBtnBgColor: Constants.typeBtnHoveredColor
                            selectedBtnBgColor: "#1E9BE2"
                            text: qsTr("My Slices")
                            onSigButtonClicked:
                            {
                                onLeftButtonClick("gcode")
                            }
                            onSigImageBtnClicked:
                            {
                                personalSpaceTutorial("myslices")
                            }
                        }
                        BasicDialogButton
                        {
                            anchors{
                                horizontalCenter: parent.horizontalCenter
                                //verticalCenter: parent.verticalCenter
                            }
                            id: idMyGcodeBtn
                            width: 120*screenScaleFactor
                            height: 28*screenScaleFactor
                            btnRadius:3
                            btnBorderW: 0
                            pointSize:Constants.labelFontPointSize
                            bottonSelected: true
                            visible: idMySlicesBtn.btnSelected
                            enabledIconSource: "qrc:/UI/photo/user_info_upload_image.png"
                            pressedIconSource: "qrc:/UI/photo/user_info_upload_image.png"
                            disabledIconSource: "qrc:/UI/photo/user_info_upload_image.png"
                            defaultBtnBgColor: Constants.dialogContentBgColor
                            hoveredBtnBgColor: Constants.typeBtnHoveredColor
                            selectedBtnBgColor: "#548099"
                            text: qsTr("My Uploads")
                            onSigButtonClicked:
                            {
                                onLeftButtonClick("gcode")
                            }
                        }
                        BasicDialogButton
                        {
                            anchors{
                                horizontalCenter: parent.horizontalCenter
                                //verticalCenter: parent.verticalCenter
                            }
                            id: idCloudSliceBtn
                            width: 120*screenScaleFactor
                            height: 28*screenScaleFactor
                            btnRadius:3
                            btnBorderW: 0
                            pointSize:Constants.labelFontPointSize
                            visible: idMySlicesBtn.btnSelected
                            enabledIconSource: "qrc:/UI/photo/user_info_cloud_image.png"
                            pressedIconSource: "qrc:/UI/photo/user_info_cloud_image.png"
                            disabledIconSource: "qrc:/UI/photo/user_info_cloud_image.png"
                            defaultBtnBgColor: Constants.dialogContentBgColor
                            hoveredBtnBgColor: Constants.typeBtnHoveredColor
                            selectedBtnBgColor: "#548099"
                            text: qsTr("Cloud Slicing")
                            onSigButtonClicked:
                            {
                                onLeftButtonClick("cloudslice")
                            }
                        }
                    }
                    
                    Column{
                        width: 140*screenScaleFactor
                        height: idMyModelBtn.btnSelected ? 123*screenScaleFactor: 36*screenScaleFactor
                        spacing: 1
                        UserInfoLeftButton{
                            id: idMyModelBtn
                            width: 140*screenScaleFactor
                            height: 36*screenScaleFactor
                            btnRadius:3
                            btnBorderW: 0
                            pointSize:Constants.labelFontPointSize
                            defaultBtnBgColor: Constants.dialogContentBgColor
                            hoveredBtnBgColor: Constants.typeBtnHoveredColor
                            selectedBtnBgColor: "#1E9BE2"
                            text: qsTr("My Model")
                            onSigButtonClicked:
                            {
                                onLeftButtonClick("model")
                            }
                            onSigImageBtnClicked:
                            {
                                personalSpaceTutorial("mymodels")
                            }
                        }
                        BasicDialogButton{
                            anchors{
                                horizontalCenter: parent.horizontalCenter
                                //verticalCenter: parent.verticalCenter
                            }
                            id: idMyUploadModelBtn
                            width: 120*screenScaleFactor
                            height: 28*screenScaleFactor
                            btnRadius:3
                            btnBorderW: 0
                            pointSize:Constants.labelFontPointSize
                            visible: idMyModelBtn.btnSelected
                            enabledIconSource: "qrc:/UI/photo/user_info_cloud_image.png"
                            pressedIconSource: "qrc:/UI/photo/user_info_cloud_image.png"
                            disabledIconSource: "qrc:/UI/photo/user_info_cloud_image.png"
                            defaultBtnBgColor: Constants.dialogContentBgColor
                            hoveredBtnBgColor: Constants.typeBtnHoveredColor
                            selectedBtnBgColor: "#548099"
                            text: qsTr("My Uploads")
                            onSigButtonClicked:
                            {
                                onLeftButtonClick("model")
                            }
                        }
                        BasicDialogButton{
                            anchors{
                                horizontalCenter: parent.horizontalCenter
                                //verticalCenter: parent.verticalCenter
                            }
                            id: idMyFavoritedBtn
                            width: 120*screenScaleFactor
                            height: 28*screenScaleFactor
                            btnRadius:3
                            btnBorderW: 0
                            pointSize:Constants.labelFontPointSize
                            visible: idMyModelBtn.btnSelected
                            enabledIconSource: "qrc:/UI/photo/user_info_collect_image.png"
                            pressedIconSource: "qrc:/UI/photo/user_info_collect_image.png"
                            disabledIconSource: "qrc:/UI/photo/user_info_collect_image.png"
                            defaultBtnBgColor: Constants.dialogContentBgColor
                            hoveredBtnBgColor: Constants.typeBtnHoveredColor
                            selectedBtnBgColor: "#548099"
                            text: qsTr("My Favorited")
                            onSigButtonClicked:
                            {
                                onLeftButtonClick("favorited")
                            }
                        }
                        BasicDialogButton{
                            anchors{
                                horizontalCenter: parent.horizontalCenter
                                //verticalCenter: parent.verticalCenter
                            }
                            id: idMyPurchasedBtn
                            width: 120*screenScaleFactor
                            height: 28*screenScaleFactor
                            btnRadius:3
                            btnBorderW: 0
                            pointSize:Constants.labelFontPointSize
                            visible: idMyModelBtn.btnSelected
                            enabledIconSource: "qrc:/UI/photo/user_info_purchased_image.png"
                            pressedIconSource: "qrc:/UI/photo/user_info_purchased_image.png"
                            disabledIconSource: "qrc:/UI/photo/user_info_purchased_image.png"
                            defaultBtnBgColor: Constants.dialogContentBgColor
                            hoveredBtnBgColor: Constants.typeBtnHoveredColor
                            selectedBtnBgColor: "#548099"
                            text: qsTr("My Purchased")
                            onSigButtonClicked:
                            {
                                onLeftButtonClick("purchased")
                            }
                        }
                    }
                    
                    UserInfoLeftButton{
                        id: idPrinterBtn
                        width: 140*screenScaleFactor
                        height: 36*screenScaleFactor
                        btnSelected: false
                        btnRadius:3
                        btnBorderW: 0
                        pointSize:Constants.labelFontPointSize
                        defaultBtnBgColor: Constants.dialogContentBgColor
                        hoveredBtnBgColor: Constants.typeBtnHoveredColor
                        selectedBtnBgColor: "#1E9BE2"
                        text: qsTr("My Devices")
                        visible: true
                        onSigButtonClicked:
                        {
                            onLeftButtonClick("printer")
                        }
                        onSigImageBtnClicked:
                        {
                            personalSpaceTutorial("myprinter")
                        }
                    }
                    BasicButton{
                        id: idConfigureFileBtn
                        width: 140*screenScaleFactor
                        height: 36*screenScaleFactor
                        btnRadius:3
                        btnBorderW: 0
                        pointSize:Constants.labelFontPointSize
                        defaultBtnBgColor: Constants.dialogContentBgColor
                        hoveredBtnBgColor: Constants.typeBtnHoveredColor
                        selectedBtnBgColor: "#1E9BE2"
                        text: qsTr("My Profile")
                        visible: false
                        onSigButtonClicked:
                        {
                            onLeftButtonClick("configurationfile")
                        }
                    }
                }
            }
            Item {
                width:1*screenScaleFactor
                height:420*screenScaleFactor
                BasicSeparator
                {
                    anchors.fill: parent
                }
            }
            BasicScrollView
            {
                id: idPrinterManagerScrollView
                width: idDialog.width-215*screenScaleFactor
                height: 420*screenScaleFactor
                visible: true
                hpolicy: ScrollBar.AlwaysOff
                vpolicy: ScrollBar.AsNeeded
                clip : true
                Column{
                    id:idPrinterManager
                    visible: true
                    spacing: 10
                   // UseInfoPrinterItem{}
                   Row{
                       spacing: 10
                       id: idPrinterAddDeviceRow
                       visible: false
                       StyledLabel{
                           id: idPrinterLabel
                           width: idPrinterLabel.contentWidth
                           verticalAlignment: Qt.AlignVCenter
                           font.pointSize:Constants.labelFontPointSize
                           height: 19*screenScaleFactor
                           text: qsTr("How to add my devices?")
                       }
                       CusSkinButton_Image{
                            width: 18*screenScaleFactor
                            height: 19*screenScaleFactor
                            btnImgUrl: "qrc:/UI/photo/user_info_right_tip_image.png"
                            onClicked:
                            {	
                                personalSpaceTutorial("myprinter")
                            }
                       }
                   }
                }
            }
            Column{
                id: idMyGcodeScrollView
                visible: false
                BasicScrollView
                {
                    //id: idMyGcodeScrollView
                    width: idDialog.width-215*screenScaleFactor
                    height: 420*screenScaleFactor
                    hpolicy: ScrollBar.AlwaysOff
                    vpolicy: ScrollBar.AsNeeded
                    clip : true
                    Grid{
                        id:idMyGcodeList
                        width: parent.width
                        spacing: 16
                        columns: 2
                        rows: 5

                        Row{
                            spacing: 10
                            id: idGcodeAddGcodeRow
                            visible: false
                            StyledLabel{
                                id: idGcodeLabel
                                width: idGcodeLabel.contentWidth
                                verticalAlignment: Qt.AlignVCenter
                                font.pointSize:Constants.labelFontPointSize
                                height: 19*screenScaleFactor
                                text: qsTr("How to add my slices?")
                            }
                            CusSkinButton_Image{
                                width: 18*screenScaleFactor
                                height: 19*screenScaleFactor
                                btnImgUrl: "qrc:/UI/photo/user_info_right_tip_image.png"
                                onClicked:
                                {
                                    personalSpaceTutorial("myslices")
                                }
                            }
                        }
                    } 
                    onVPositionChanged:
                    {
                        if((vSize + vPosition) === 1){
                            if(currentGCodePage != nextGCodePage){
                                getMyGCodeList(nextGCodePage, 10)
                            }
                        }
                    }
                }
            }
            Column{
                id: idCloudSliceScrollView
                visible: false
                BasicScrollView
                {
                    width: idDialog.width-215*screenScaleFactor
                    height: 420*screenScaleFactor
                    hpolicy: ScrollBar.AlwaysOff
                    vpolicy: ScrollBar.AsNeeded
                    clip : true
                    Grid{
                        id:idCloudSliceList
                        width: parent.width
                        spacing: 16
                        columns: 2
                        rows: 5
                    } 
                    onVPositionChanged:
                    {
                        if((vSize + vPosition) === 1){
                            if(currentCloudSlicePage != nextCloudSlicePage){
                                getCloudSlicingList(nextCloudSlicePage, 10)
                            }
                        }
                    }
                }
            }
            Column{
                id: idMyModeScrollView
                visible: false
                BasicScrollView
                {
                    //id: idMyModeScrollView
                    width: idDialog.width-215*screenScaleFactor
                    height: 420*screenScaleFactor
                    hpolicy: ScrollBar.AlwaysOff
                    vpolicy: ScrollBar.AsNeeded
                    clip : true
                    
                    Grid{
                        id:idMyModelList
                        width: parent.width
                        spacing: 16
                        columns: 2
                        rows: 5

                        Row{
                            spacing: 10
                            id: idModelAddModelRow
                            visible: false
                            StyledLabel{
                                id: idModelLabel
                                width: idModelLabel.contentWidth
                                verticalAlignment: Qt.AlignVCenter
                                font.pointSize:Constants.labelFontPointSize
                                height: 19
                                text: qsTr("How to add my models?")
                            }
                            CusSkinButton_Image{
                                    width: 18*screenScaleFactor
                                    height: 19*screenScaleFactor
                                    btnImgUrl: "qrc:/UI/photo/user_info_right_tip_image.png"
                                    onClicked:
                                    {	
                                        personalSpaceTutorial("mymodels")
                                    }
                            }
                        }
                    }
                    onVPositionChanged:
                    {
                        if((vSize + vPosition) === 1){
                            if(currentModelPage != nextModelPage){
                                 if(nextModelPage != "")
                                    getMyModelList(nextModelPage, 10)
                            }
                        }
                    }
                }
            }
            Column{
                id: idFavoritedScrollView
                visible: false
                BasicScrollView
                {
                    width: idDialog.width-215*screenScaleFactor
                    height: 420*screenScaleFactor
                    hpolicy: ScrollBar.AlwaysOff
                    vpolicy: ScrollBar.AsNeeded
                    clip : true
                    
                    Grid{
                        id:idMyFavoritedList
                        width: parent.width
                        spacing: 16
                        columns: 2
                        rows: 5

                        Row{
                            spacing: 10
                            id: idFavoritedAddModelRow
                            visible: false
                            StyledLabel{
                                id: idFavoritedLabel
                                width: idFavoritedLabel.contentWidth
                                verticalAlignment: Qt.AlignVCenter
                                font.pointSize:Constants.labelFontPointSize
                                height: 19*screenScaleFactor
                                text: qsTr("How to collect models?")
                            }
                            CusSkinButton_Image{
                                    width: 18*screenScaleFactor
                                    height: 19*screenScaleFactor
                                    btnImgUrl: "qrc:/UI/photo/user_info_right_tip_image.png"
                                    onClicked:
                                    {	
                                        personalSpaceTutorial("myfavorited")
                                    }
                            }
                        }
                    }
                    onVPositionChanged:
                    {
                        if((vSize + vPosition) === 1){
                            if(currentFavoritedPage != nextFavoritedPage){
                                if(nextFavoritedPage != "")
                                    getMyFavoritedList(nextFavoritedPage, 10)
                            }
                        }
                    }
                }
            }
            Column{
                id: idPurchasedScrollView
                visible: false
                BasicScrollView
                {
                    width: idDialog.width-215*screenScaleFactor
                    height: 420*screenScaleFactor
                    hpolicy: ScrollBar.AlwaysOff
                    vpolicy: ScrollBar.AsNeeded
                    clip : true
                    Grid{
                        id:idPurchasedList
                        width: parent.width
                        spacing: 16
                        columns: 2
                        rows: 5
                    } 
                    onVPositionChanged:
                    {
                        if((vSize + vPosition) === 1){
                            if(currentPurchasedModelPage != nextPurchasedModelPage){
                                getPurchasedModelList(nextPurchasedModelPage, 10)
                            }
                        }
                    }
                }
            }
            BasicScrollView
            {
                id: idConfigurationFileScrollView
                width: idDialog.width-215*screenScaleFactor
                height: 420*screenScaleFactor
                visible: false
                hpolicy: ScrollBar.AlwaysOff
                vpolicy: ScrollBar.AsNeeded
                clip : true
                Column{
                    id:idConfigurationFileList
                    // StyledLabel{
                    //     width: 100
                    //     height:30
                    //     text: qsTr("file")
                    // }
                }
            } 
        }
    }

    UploadMessageDlg{
        id:warringPrinterClicklg
        //msgText: qsTr("Not logged in to Creality Cloud, Unable to upload, Do you want to log in?")
        visible: false
        cancelBtnVisible: false
        onSigOkButtonClicked:
        {
            warringPrinterClicklg.close()
        }
    }
}
