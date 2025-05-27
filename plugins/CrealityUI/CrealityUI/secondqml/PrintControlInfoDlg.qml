import QtQuick 2.10
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0 as QQC2
import CrealityUI 1.0
import "qrc:/CrealityUI"
import ".."
import "../qml"
BasicDialog{
    id: idPrintControlInfoDlg
    width: 860+60+ 50
    height: 520
    titleHeight : 30
    title: qsTr("CrealityCloud Print")

    property var printerIdMap: {0: 0}

    property var deviceName: ""
    property var deviceId: ""
    property var deviceTbId:""
    property var deviceIotType: ""
    property var curGcodeUrl: ""
    property var curGcodeId: ""


    //move x y z 
    property var moveCurAxisX: 0
    property var moveCurAxisY: 0
    property var moveCurAxisZ: 0
    property var moveCurRealAxisX: 0
    property var moveCurRealAxisY: 0
    property var moveCurRealAxisZ: 0
    property var curFanSwitch: 0
    property var moveAxisXAddIsChanged: false
    property var moveAxisXAddCount: 0
    property var moveAxisYAddIsChanged: false
    property var moveAxisYAddCount: 0
    property var moveAxisZAddIsChanged: false
    property var moveAxisZAddCount: 0
    property var fanSwitchIsChanged: false
    property var fanSwitchCount: false

    //

    property var gcodeFileChanged: false

    property var nozzleTargetTempIsChanged: false
    property var nozzleTargetTempCount: 0
    property var hotBedTargetTempIsChanged: false
    property var hotBedTargetTempCount: 0
    property var printSpeedTargetTempIsChanged: false
    property var printSpeedTargetTempCount: 0

    property var serverPort: 30000
    property var urlStr: "rtsp://127.0.0.1:" + serverPort + "/ch0_0"
    property var isClosed: false

    signal initP2P(string apilicense, string strDid, string strinit, int portNum, string productName)
    signal startPrintGcode(var url, var productName, var iotId, var productKey, var gcodeId, var printerName, var printerId, var tbid, var iotType)
    signal controlPrintGcode(var productName, var iotId, var productKey, var control, var tbid, var iotType)
    signal updatePreviewImage(string printID, string deviceID)
    signal getPrintingPauseGcodeInfo(var printID)
    signal controlPrintTempSpeed(var productName, var iotId, var productKey, var targetInfo, var targetValue, var tbid, var iotType)
    signal initCameraLicense(var productName, var iotId, var productKey, var did, var license, var tbid, var iotType)
    signal updateDeviceData(var productName, var iotId, var productKey, var tbid, var iotType)
    signal changeIotPollTime(var time)
    signal sigAddVideo(var deviceID)
    signal sigPrintTaskCtrl(var deviceID, var state)

    //begin  gcode to print
    property var gcodeToPrintFlag : false
    function initGcodeToPrint(gcodeId)
    {
        gcodeToPrintFlag = true
        //set current gcode
        for(var i = 0; i < idGcodeListModel.count; i++)
        {
            if(gcodeId == idGcodeListModel.get(i).id)
            {
                idGcodeCombox.currentIndex = i
            }
        }

        idNozzleTempTargetLabel.realValue = "200.00"
        idNozzleTempText.text =" ℃"
        idBedtempTargetLabel.realValue = "60.00"
        idBedTempText.text = " ℃"
        idCurFeedratePctTarget.realValue = "100.00"
        idCurFeedratePctLabel.text = " %"

        for(var key in IotData.videoItemMap)
        {
            IotData.videoItemMap[key].visible = false
        }
    }

    function insertPrinterList()
    {
        idPrinterCombox.enabled = false
        idPrinterListModel.clear() 
        for(var i = 0; i < IotData.serDeviceNameArray.length; i++)
        {
            if(IotData.iotDeviceStatusMap[IotData.serDeviceNameArray[i]] == "online")
            {
                if(IotData.iotPrintStateMap[IotData.serDeviceNameArray[i]] == 0 || IotData.iotPrintStateMap[IotData.serDeviceNameArray[i]] == 2 || IotData.iotPrintStateMap[IotData.serDeviceNameArray[i]] == 3 || IotData.iotPrintStateMap[IotData.serDeviceNameArray[i]] == 4)
                {
                    idPrinterListModel.append({"key": IotData.iotPrinterNameMap[IotData.serDeviceNameArray[i]], "modelData": IotData.serNickNameMap[IotData.serDeviceNameArray[i]], "deviceValueID": IotData.serDeviceNameArray[i]})
                }
            } 
        }
        if(idPrinterListModel.count > 0)
        {
            idPrinterCombox.enabled = true
            idRelatedPrinter.text = qsTr(" Please select a printer")
        }
        else{
            idRelatedPrinter.text = qsTr(" No idle printer")
        }
    }

    //end

    Timer{
        id: idUpdateDeviceInfoTimer
        interval: 2000
        repeat: false
        onTriggered:{
            if(deviceId != ""){
                updateDeviceData(deviceId, IotData.serIotIdMap[deviceId], IotData.serProductkeyMap[deviceId], deviceTbId, deviceIotType)
            }
        }
    }

    Timer{
        id:adjustmentTimer
        interval: 10000
        repeat: true
        triggeredOnStart: true
        onTriggered: {
            if(deviceId != "")
            {
                //controlPrintTempSpeed(deviceId, IotData.serIotIdMap[deviceId], IotData.serProductkeyMap[deviceId], "ReqPrinterPara", 1, deviceTbId, deviceIotType)
            }   
        }
    }

    Timer{
        id: idMoveXChangedTimer
        interval: 1000
        repeat: false
        onTriggered:{
            if(deviceId != ""){
                controlPrintTempSpeed(deviceId, IotData.serIotIdMap[deviceId], IotData.serProductkeyMap[deviceId], "gcodeCmdG0X", parseInt(moveCurAxisX*100), deviceTbId, deviceIotType);
                idUpdateDeviceInfoTimer.restart()
            }
        }
    }
    Timer{
        id: idMoveYChangedTimer
        interval: 1000
        repeat: false
        onTriggered:{
            if(deviceId != ""){
                controlPrintTempSpeed(deviceId, IotData.serIotIdMap[deviceId], IotData.serProductkeyMap[deviceId], "gcodeCmdG0Y", parseInt(moveCurAxisY*100), deviceTbId, deviceIotType);
                idUpdateDeviceInfoTimer.restart()
            }
        }
    }
    Timer{
        id: idMoveZChangedTimer
        interval: 1000
        repeat: false
        onTriggered:{
            if(deviceId != ""){
               controlPrintTempSpeed(deviceId, IotData.serIotIdMap[deviceId], IotData.serProductkeyMap[deviceId], "gcodeCmdG0Z", parseInt(moveCurAxisZ*100), deviceTbId, deviceIotType);
               idUpdateDeviceInfoTimer.restart()
            }
        }
    }
    Timer{
        id: idMoveXYZeroTimer
        interval: 1000
        repeat: false
        onTriggered:{
            if(deviceId != ""){
               controlPrintTempSpeed(deviceId, IotData.serIotIdMap[deviceId], IotData.serProductkeyMap[deviceId], "gcodeCmdG28XY", 0, deviceTbId, deviceIotType);
               idUpdateDeviceInfoTimer.restart()
            }
        }
    }
    Timer{
        id: idMoveZZeroTimer
        interval: 1000
        repeat: false
        onTriggered:{
            if(deviceId != ""){
               controlPrintTempSpeed(deviceId, IotData.serIotIdMap[deviceId], IotData.serProductkeyMap[deviceId], "gcodeCmdG28Z", 0, deviceTbId, deviceIotType);
               idUpdateDeviceInfoTimer.restart()
            }
        }
    }
    Timer{
        id: idNozzleTempTimer
        interval: 1000
        repeat: false
        onTriggered:{
            if(deviceId != ""){
               controlPrintTempSpeed(deviceId, IotData.serIotIdMap[deviceId], IotData.serProductkeyMap[deviceId], "nozzletargettemp", idNozzleTempTargetLabel.realValue, deviceTbId, deviceIotType);
               idUpdateDeviceInfoTimer.restart()
            }
        }
    }
    Timer{
        id: idHotBedTempTimer
        interval: 1000
        repeat: false
        onTriggered:{
            if(deviceId != ""){
                controlPrintTempSpeed(deviceId, IotData.serIotIdMap[deviceId], IotData.serProductkeyMap[deviceId], "hotbedtargettemp", idBedtempTargetLabel.realValue, deviceTbId, deviceIotType);
                idUpdateDeviceInfoTimer.restart()
            }
        }
    }
    Timer{
        id: idPrintSpeedTimer
        interval: 1000
        repeat: false
        onTriggered:{
            if(deviceId != ""){
                controlPrintTempSpeed(deviceId, IotData.serIotIdMap[deviceId], IotData.serProductkeyMap[deviceId], "printspeedtarget", idCurFeedratePctTarget.realValue, deviceTbId, deviceIotType);
                idUpdateDeviceInfoTimer.restart()
            }
        }
    }

    Timer{
        id: idAddGroupVideoTimer
        property var curproductName:""
        interval: 2000
        repeat: false
        onTriggered:{
            if(curproductName != "")
            {
                sigAddVideo(curproductName) 
            }
            
        }
    }

    function isReverseZAxisOperation(printerName)
    {
        if(printerName == "Sermoon V1" || printerName == "CR-200A" || 
        printerName == "CR-200B" || printerName == "CT-300 Pro" || 
        printerName == "Ender-7" || printerName == "Sermoon D1" || printerName == "Sermoon V1 Pro")
        {
            return true
        }
        else{
            return false
        }
    }

    function clearQMLData()
    {
        deviceId = ""
        idPrinterListModel.clear()
        idRelatedPrinter.text = qsTr(" No idle printer")
        idGcodeListModel.clear()
        idDeviceType.text = qsTr("Please select a print file")
        idNozzleTempTargetLabel.realValue = ""
        idNozzleTempText.text =" ℃"
        idBedtempTargetLabel.realValue = ""
        idBedTempText.text = " ℃"
        idCurFeedratePctTarget.realValue = ""
        idCurFeedratePctLabel.text = " %"

        idPrintState.text = ""
        idPrintedTimesLabel.text = ""
        progressBar.value = 0

        idGcodeCombox.enabled = true
        idPrinterCombox.enabled = true
        idStartPrintBtn.visible = false
        idPausePrintBtn.visible = false
        idContinuePrintBtn.visible = false
        idStopPrintBtn.visible = false

        idPrinterCombox.videoIsInit = false

        moveAxisXAddIsChanged = false
        moveAxisYAddIsChanged = false
        moveAxisZAddIsChanged = false
    }

    onVisibleChanged:
    {
        if(idPrintControlInfoDlg.visible == true)
        {
            changeIotPollTime("8000ms")
        }
        else{
            changeIotPollTime("30000ms")
        }
    }

    onDialogClosed:
    {
        gcodeFileChanged = false
        isClosed = true
        clearQMLData()
        adjustmentTimer.stop()
    }

    function setPreviewImage(imageUrl,allLayers,deviceName)
    {
        if(IotData.iotPrintStateMap[deviceName] == 1 || IotData.iotPrintStateMap[deviceName] == 5)
        {
            IotData.videoItemMap[deviceName].setImage(imageUrl)
            IotData.videoItemMap[deviceName].setProgress(IotData.iotLayerPrintMap[deviceName] / allLayers)
        }
        else
        {
            IotData.videoItemMap[deviceName].setImage("")
        }
        
    }

    function initCameraInfo()
    {
        if(IotData.iotVideoStatusMap[deviceId] == "1")
        {
            initCameraLicense(deviceId, IotData.serIotIdMap[deviceId], IotData.serProductkeyMap[deviceId], IotData.serSyDidMap[deviceId], IotData.serSyLecenseMap[deviceId], deviceTbId, deviceIotType)
        }

        adjustmentTimer.start()
    }

    function stopAllVideo()
    {
        for(var key in IotData.videoItemMap)
        {
            console.log("stopAllVideo device:" + key)
            if(IotData.videoItemMap[key].hasVidew)
            {
                
                IotData.videoItemMap[key].stopVideo()
            }

            if(IotData.videoItemMap[key].isPreviewShow)
            {
                IotData.videoItemMap[key].endPreViewShow()
            }
        }
    }

    function restartVideoPlay(strDeviceID)
    {
        console.log("restartVideoPlay beg")
        IotData.videoItemMap[strDeviceID].restartVideo()
    }

    function startVideoPlay()
    {
        for(var key in IotData.videoItemMap)
        {
            if(key == deviceId)
            {
                IotData.videoItemMap[key].visible = true
                if(IotData.iotVideoStatusMap[deviceId] == "1")
                {
                     IotData.videoItemMap[key].selectMode = 1
                }
            }
            else
            {
                IotData.videoItemMap[key].visible = false
            }
        }
    }

    function initPrintControlPage()
    {
        //gcode
        if(IotData.iotPrintStateMap[deviceId] == 0 || IotData.iotPrintStateMap[deviceId] == 2 || IotData.iotPrintStateMap[deviceId] == 3 || IotData.iotPrintStateMap[deviceId] == 4)
        {
            idGcodeCombox.enabled = true
            idPrinterCombox.enabled = true
            idDeviceType.text = qsTr("Please select a print file")

            idStartPrintBtn.visible = true
            idPausePrintBtn.visible = false
            idContinuePrintBtn.visible = false
            idStopPrintBtn.visible = false

            gcodeFileChanged = true
        }
        else if(IotData.iotPrintStateMap[deviceId] == 1)//printing
        {
            getPrintingPauseGcodeInfo(IotData.iotPrintIDMap[deviceId])
            
            idGcodeCombox.enabled = false
            idPrinterCombox.enabled = false

            idStartPrintBtn.visible = false
            idPausePrintBtn.visible = true
            idContinuePrintBtn.visible = false
            idStopPrintBtn.visible = true
        }
        else if(IotData.iotPrintStateMap[deviceId] == 5)//pause
        {
            getPrintingPauseGcodeInfo(IotData.iotPrintIDMap[deviceId])

            idGcodeCombox.enabled = false
            idPrinterCombox.enabled = false

            idStartPrintBtn.visible = false
            idPausePrintBtn.visible = false
            idContinuePrintBtn.visible = true
            idStopPrintBtn.visible = true
        }

        updateQMLData()
    }

    function setCurPrintingGcodeInfo(gcodeId)
    {
        for(var i = 0; i < idGcodeListModel.count; i++)
        {
            if(gcodeId == idGcodeListModel.get(i).id)
            {
                idGcodeCombox.currentIndex = i
            }
        }

        gcodeFileChanged = true
    }

    function selectCurPrinterInfo(name, id, tbid, iotType)
    {
        deviceName = name
        deviceId = id
        deviceTbId = tbid
        deviceIotType = iotType

        idPrinterCombox.enabled = false
        idPrinterListModel.clear()
        idPrinterCombox.currentIndex = 0
        idRelatedPrinter.text = "  " + IotData.iotPrinterNameMap[deviceId]
        idPrinterListModel.append({"key": IotData.iotPrinterNameMap[deviceId], "modelData": deviceName, "deviceValueID": deviceId})

        updateQMLData()
    }

    function setPrinterModelInfo(strjson)
    {
        var objectArray = JSON.parse(strjson);
        if(objectArray.code === 0)
        {
            var objResult = objectArray.result.list;
            for ( var key in objResult ) {
                printerIdMap[objResult[key].name] = objResult[key].id
            }
        }
        else{}
    }

    function setServerDeviceData(strjson)
    {
        for(var i = 0; i < IotData.serDeviceNameArray.length; i++)
        {
            if(findMapBykey(IotData.serDeviceNameArray[i]))
            {
                continue
            }
            else
            {
                async_createVideoItem(IotData.serDeviceNameArray[i], IotData.serSyDidMap[IotData.serDeviceNameArray[i]], IotData.serSyLecenseMap[IotData.serDeviceNameArray[i]])
            }
        }

    }

    function findMapBykey(deviceName)
    {
        for(var key in IotData.videoItemMap)
        {
            if(deviceName == key)
            {
                return true
            }
        }
        return false
    }

    function async_createVideoItem(devicename, did, license)
	{
		var finishCreation = function()
        {
            if (componentVideoItem.status === Component.Ready)
            {
				var obj1 = componentVideoItem.incubateObject(idVideoItemRectangle, {
															"visible": false,
                                                            "deviceID": devicename,
															"xyStrDID": did,
															"xyStrLicense": license})
				if (obj1.status !== Component.Ready)
				{
					obj1.onStatusChanged = function(status)
					{
						if (status === Component.Ready)
						{
							obj1.object.updatePreview.connect(onUpdatePreview)
							obj1.object.maxBtnClicked.connect(videoMaxShow)
                            obj1.object.minBtnClicked.connect(videoMinShow)
							IotData.videoItemMap[devicename] = obj1.object
						}
					}
				}
				else
				{
					obj1.object.updatePreview.connect(onUpdatePreview)
                    obj1.object.maxBtnClicked.connect(videoMaxShow)
                    obj1.object.minBtnClicked.connect(videoMinShow)
					IotData.videoItemMap[devicename] = obj1.object
				}
            }
        }

		var componentVideoItem = Qt.createComponent("../secondqml/BasicVideoItem.qml")

        if (componentVideoItem.status === Component.Ready)
        {
            finishCreation();
        }
        else
        {
            componentVideoItem.statusChanged.connect(finishCreation);
        }
	}

    function onUpdatePreview(productname)
    {
        if((idPrintControlInfoDlg.visible == true) && (productname == deviceId))
        {
            updatePreviewImage(IotData.iotPrintIDMap[productname],productname)
        }
    }

    function videoMaxShow()
    {
        idControlBtn.visible = false
        idRectangleSpace5.visible = false
        idBorderVerRectangle.visible = false
        idControlBtnColumn.visible = false
        idPrinterDataInfoColumn.visible = false

        idMainLayoutColumn.x = -20
        idMainLayoutColumn.y = -20
        
        idVideoItemRectangle.width = idMainLayoutColumn.width + 40
        idVideoItemRectangle.height = idMainLayoutColumn.height + 40
    }

    function videoMinShow()
    {
        idControlBtn.visible = true
        idRectangleSpace5.visible = true
        idBorderVerRectangle.visible = true
        idControlBtnColumn.visible = true
        idPrinterDataInfoColumn.visible = true

        idMainLayoutColumn.x = 0
        idMainLayoutColumn.y = 0
        
        idVideoItemRectangle.width = 390
        idVideoItemRectangle.height = 300
    }

    function updateDeviceStatus()
    {
        if(deviceId != "")
        {
            if(IotData.iotDeviceStatusMap[deviceId] == "offline" && idPrintControlInfoDlg.visible === true)
            {
                idDeviceOfflineDlg.visible = true
            }
        }
    }

    function updateDeviceInformation(productName)
    {
        if(IotData.iotVideoStatusMap[productName] == "1" && IotData.iotDeviceStatusMap[productName] == "online" && IotData.videoItemMap[productName].hasVidew == false)
        {
            serverPort++
            initP2P(IotData.videoItemMap[productName].xyStrLicense,IotData.videoItemMap[productName].xyStrDID,"",serverPort,productName)
            console.log("video show productName:" + productName + " load url:"+ urlStr + " xyStrDID:" + IotData.videoItemMap[productName].xyStrDID + " xyStrLicense:" + IotData.videoItemMap[productName].xyStrLicense)
            IotData.videoItemMap[productName].loadVideoUrl(urlStr)
            IotData.videoItemMap[productName].hasVidew = true
            idAddGroupVideoTimer.curproductName =productName
            idAddGroupVideoTimer.start()
        }

        if((IotData.iotPrintStateMap[productName] == 1 || IotData.iotPrintStateMap[productName] == 5) && IotData.videoItemMap[productName].isPreviewShow ==false && IotData.iotDeviceStatusMap[productName] == "online")
        {
            IotData.videoItemMap[productName].initPreViewShow()
            IotData.videoItemMap[productName].isPreviewShow = true
        }

        if(IotData.iotPrintStateMap[productName] != 1 && IotData.iotPrintStateMap[productName] != 5)
        {
            IotData.videoItemMap[productName].endPreViewShow()
            IotData.videoItemMap[productName].setImage("")
            IotData.videoItemMap[productName].isPreviewShow = false
        }

        if(deviceId != "")
        {
            updateQMLData()
        }
    }

    function updateQMLData()
    {
        if(fanSwitchIsChanged == false)
        {
            if(IotData.iotFanSwitchMap[deviceId] == "0")
            {
                curFanSwitch = 0
            }
            else if(IotData.iotFanSwitchMap[deviceId] == "1")
            {
                curFanSwitch = 1
            }
        }
        else{
            fanSwitchCount++
            if(fanSwitchCount == 2)
            {
                fanSwitchCount = 0
                fanSwitchIsChanged = false
            }
        }

        var str = IotData.iotCurPositionMap[deviceId] //str = "X:112 Y:23 Z:0"
        console.log("IotData.iotCurPositionMap[deviceId]:" + IotData.iotCurPositionMap[deviceId])
        if(str)
        {
            var arr = str.split(" ");
            //moveCurAxisX = arr[0].split(":")[1]
            //moveCurAxisY = arr[1].split(":")[1]
            //moveCurAxisZ = arr[2].split(":")[1]

            if(arr[0].split(":")[1] != undefined)
            {
                if(moveAxisXAddIsChanged == false)
                {
                    moveCurAxisX = parseFloat(arr[0].split(":")[1]).toFixed(2)
                }
                moveCurRealAxisX = parseFloat(arr[0].split(":")[1]).toFixed(2)
            }

            if(arr[1].split(":")[1] != undefined)
            {
                if(moveAxisYAddIsChanged == false)
                {
                    moveCurAxisY = parseFloat(arr[1].split(":")[1]).toFixed(2)
                }
                moveCurRealAxisY = parseFloat(arr[1].split(":")[1]).toFixed(2)
            }

            if(arr[2].split(":")[1] != undefined)
            {
                if(moveAxisZAddIsChanged == false)
                {
                    moveCurAxisZ = parseFloat(arr[2].split(":")[1]).toFixed(2)
                }

                moveCurRealAxisZ = parseFloat(arr[2].split(":")[1]).toFixed(2)
            }
        }
        

        if(IotData.iotPrintStateMap[deviceId] == 1 || IotData.iotPrintStateMap[deviceId] == 5)
        {
            var tmp = IotData.iotPrintedTimesMap[deviceId]

            var timeStr = ""
            if(tmp == 0)
            {
                timeStr = ""
            }
            else if(tmp < 60)
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


            idPrintedTimesLabel.text = timeStr
            progressBar.value = IotData.iotPrintProgressMap[deviceId]
        }
        else{
            idPrintedTimesLabel.text = ""
            progressBar.value = 0
        }

        idTimeLeftPrintLabel.text = "- - -"//IotData.iotTimesLeftToPrintMap[deviceId]
        if(IotData.iotNozzleTempMap[deviceId] != undefined)
        {
            idNozzleTempText.text = IotData.iotNozzleTempMap[deviceId] + " ℃"
        }
        else{
            idNozzleTempText.text = 0 + " ℃"
        }
        
        if(nozzleTargetTempIsChanged == false)
        {
            idNozzleTempTargetLabel.realValue = IotData.iotNozzleTempTargetMap[deviceId]
        }
        else{
            nozzleTargetTempCount++
            if(nozzleTargetTempCount == 2)
            {
                nozzleTargetTempIsChanged = false
                nozzleTargetTempCount = 0
            }
        } 

        if(IotData.iotBedTempMap[deviceId] != undefined)
        {
            idBedTempText.text = IotData.iotBedTempMap[deviceId] + " ℃"
        }
        else{
            idBedTempText.text =  0 + " ℃"
        }
        
        if(printSpeedTargetTempIsChanged == false)
        {
            if(IotData.iotCurFeedratePctMap[deviceId] != undefined)
            {
                idCurFeedratePctTarget.realValue = IotData.iotCurFeedratePctMap[deviceId]
            }
            else{
                idCurFeedratePctTarget.realValue = 100
            }
            
            if(idCurFeedratePctTarget.realValue == 0)
            {
                idCurFeedratePctTarget.realValue = 100
            }
        }
        else{
            printSpeedTargetTempCount++
            if(printSpeedTargetTempCount == 2)
            {
                printSpeedTargetTempIsChanged = false
                printSpeedTargetTempCount = 0
            }
        }

        if(IotData.iotCurFeedratePctMap[deviceId] == 0)
        {
            idCurFeedratePctLabel.text = "100" + " %"
        }
        else{
            idCurFeedratePctLabel.text = IotData.iotCurFeedratePctMap[deviceId] + " %"
        }

        if(hotBedTargetTempIsChanged == false)
        {
            idBedtempTargetLabel.realValue = IotData.iotBedTempTargetMap[deviceId]
        }
        else{
            hotBedTargetTempCount++
            if(hotBedTargetTempCount == 2)
            {
                hotBedTargetTempIsChanged = false
                hotBedTargetTempCount = 0
            }
        }

        if(IotData.iotPrintStateMap[deviceId] == 0) //initial
        {
            idPrintState.text = qsTr("Idle")

            idGcodeCombox.enabled = true
            idPrinterCombox.enabled = true

            idStartPrintBtn.visible = true
            idPausePrintBtn.visible = false
            idContinuePrintBtn.visible = false
            idStopPrintBtn.visible = false
            idControllerBtn.visible = true
        }
        else if(IotData.iotPrintStateMap[deviceId] == 1)
        {
                idPrintState.text = qsTr("Printing") 

                idGcodeCombox.enabled = false
                idPrinterCombox.enabled = false

                idStartPrintBtn.visible = false
                idPausePrintBtn.visible = true
                idContinuePrintBtn.visible = false
                idStopPrintBtn.visible = true
                idControllerBtn.visible = false
        }
        else if(IotData.iotPrintStateMap[deviceId] == 2) //print completed
        {
                idPrintState.text = qsTr("Idle")

                idGcodeCombox.enabled = true
                idPrinterCombox.enabled = true

                idStartPrintBtn.visible = true
                idPausePrintBtn.visible = false
                idContinuePrintBtn.visible = false
                idStopPrintBtn.visible = false
                idControllerBtn.visible = true
        }
        else if(IotData.iotPrintStateMap[deviceId] == 3) //print failed
        {
                idPrintState.text = qsTr("Idle") 

                idGcodeCombox.enabled = true
                idPrinterCombox.enabled = true

                idStartPrintBtn.visible = true
                idPausePrintBtn.visible = false
                idContinuePrintBtn.visible = false
                idStopPrintBtn.visible = false
                idControllerBtn.visible = true
        }
        else if(IotData.iotPrintStateMap[deviceId] == 4) //print stop
        {
                idPrintState.text = qsTr("Idle") 

                idStartPrintBtn.visible = true
                idPausePrintBtn.visible = false
                idContinuePrintBtn.visible = false
                idStopPrintBtn.visible = false
                idControllerBtn.visible = true
        }
        else if(IotData.iotPrintStateMap[deviceId] == 5)
        {
                idPrintState.text = qsTr("Printing pause")

                idGcodeCombox.enabled = false
                idPrinterCombox.enabled = false

                idStartPrintBtn.visible = false
                idPausePrintBtn.visible = false
                idContinuePrintBtn.visible = true
                idStopPrintBtn.visible = true
                idControllerBtn.visible = false
        }
        else{}
        
    }

    function insertMyGcodeList(strjson)
    {
        idGcodeListModel.clear()
        var objectArray = JSON.parse(strjson);
        if(objectArray.code === 0)
        {
            var objResult = objectArray.result.list;
            for ( var key in objResult ) {
                var modelDataStr = ""
                if(objResult[key].sliceDevice === undefined)
                {
                    modelDataStr = qsTr("----")
                }
                else{
                    modelDataStr = objResult[key].sliceDevice.name
                }
                if(objResult[key].name == "")
                {
                    idGcodeListModel.append({"key": modelDataStr, "modelData": objResult[key].modelInfo.name, "url": objResult[key].downloadLink, "id": objResult[key].id})
                }
                else
                {
                    idGcodeListModel.append({"key": modelDataStr, "modelData": objResult[key].name, "url": objResult[key].downloadLink, "id": objResult[key].id})
                }
                
            }
        }
        else{}

        updateQMLData()
    }

    Item{
        anchors.top: parent.top
        anchors.topMargin: 30 + 5+ 20
        anchors.left: parent.left
        anchors.leftMargin: 20 + 5

        width: parent.width - 10 - 20*2
        height: parent.height - 30 - 10 - 20*2


        Column{
            id: idMainLayoutColumn
            width: parent.width
            height: parent.height
            spacing: 20
            
            Row{
                width: parent.width
                height: 365
                spacing: 20
                Column{
                    id: idControlBtnColumn
                    width: 140
                    height: parent.height

                    BasicButton{
                        id: idPrinterBtn
                        width: 140
                        height: 36
                        btnRadius: 0
                        btnBorderW: 0
                        pixSize: 12
                        hoveredBtnBgColor: Constants.typeBtnHoveredColor
                        defaultBtnBgColor: Constants.dialogContentBgColor
                        selectedBtnBgColor: "#1E9BE2"
                        btnSelected: true
                        text: qsTr("Print")
                        onSigButtonClicked:
                        {
                            idPrinterBtn.btnSelected = true
                            idControllerBtn.btnSelected = false
                            idPrinterInfo.visible = true
                            idGcodeInfo.visible = true
                            idRectangleSpace1.visible = true
                            idRectangleSpace2.visible = true
                            idPrintProgress.visible = true
                            idPrintTimeInfo.visible = true

                            idRectangleSpace3.visible = false
                            idFanInfo.visible = false
                            idRectangleSpace4.visible = false
                            idMovingAxisInfo.visible = false
                            idMoveSpcingInfo.visible = false
                            idMovePositionInfo.visible = false
                            idRectangleSpace5.visible = true
                            idControlBtn.visible = true
                        }
                    }
                    BasicButton{
                        id: idControllerBtn
                        width: 140
                        height: 36
                        btnRadius: 0
                        btnBorderW: 0
                        pixSize: 12
                        visible: false
                        hoveredBtnBgColor: Constants.typeBtnHoveredColor
                        defaultBtnBgColor: Constants.dialogContentBgColor
                        selectedBtnBgColor: "#1E9BE2"
                        btnSelected: false
                        text: qsTr("Adjustment")
                        onSigButtonClicked:
                        {
                            idPrinterBtn.btnSelected = false
                            idControllerBtn.btnSelected = true
                            idPrinterInfo.visible = false
                            idGcodeInfo.visible = false
                            idRectangleSpace1.visible = false
                            idRectangleSpace2.visible = false
                            idPrintProgress.visible = false
                            idPrintTimeInfo.visible = false

                            idRectangleSpace3.visible = true
                            idFanInfo.visible = true
                            idRectangleSpace4.visible = true
                            idMovingAxisInfo.visible = true
                            idMoveSpcingInfo.visible = true
                            idMovePositionInfo.visible = true
                            idRectangleSpace5.visible = false
                            idControlBtn.visible = false
                        }
                    }
                }
                Rectangle{
                    id: idBorderVerRectangle
                    width: 1
                    height: parent.height
                    border.color: Constants.splitLineColor
		            border.width: 1
		            color: "transparent"
                }
                Row{
                    width: parent.width - 140 - 20*3
                    height: parent.height
                    spacing: 20

                    Column{
                        id: idPrinterDataInfoColumn
                        width: parent.width - 390
                        height: parent.height
                        spacing: 10
                        Column{
                            id: idPrinterInfo
                            width: parent.width
                            height: 28*2 + 2
                            spacing: 2
                            Row{
                                width: parent.width
                                height: 28
                                spacing: 10
                                StyledLabel{
                                    width: 120
                                    height: parent.height
                                    font.pixelSize:12 
                                    verticalAlignment: Qt.AlignVCenter
                                    horizontalAlignment: Qt.AlignVCenter
                                    text: qsTr("Device Name:")
                                    elide: Text.ElideRight
                                }
                                BasicCombobox{
                                    property var videoIsInit: false
                                    id:idPrinterCombox
                                    width: parent.width - 120 - 10
                                    height: parent.height
                                    font.pixelSize : 12
                                    //currentIndex: 0
                                    model: ListModel {
                                        id: idPrinterListModel
                                    }

                                    onComboBoxIndexChanged: {
                                        if(idPrinterListModel.count > 0)
                                        {
                                            idPrintControlInfoDlg.deviceId = idPrinterListModel.get(currentIndex).deviceValueID
                                            idRelatedPrinter.text = "  " + idPrinterListModel.get(currentIndex).key

                                            moveAxisXAddIsChanged = false
                                            moveAxisYAddIsChanged = false
                                            moveAxisZAddIsChanged = false

                                            updateQMLData()
                                            initCameraInfo()

                                            for(var key in IotData.videoItemMap)
                                            {
                                                if(key == deviceId)
                                                {
                                                    IotData.videoItemMap[key].visible = true
                                                    if(IotData.iotVideoStatusMap[deviceId] == "1")
                                                    {
                                                        IotData.videoItemMap[key].selectMode = 1
                                                    }
                                                }
                                                else
                                                {
                                                    IotData.videoItemMap[key].visible = false
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            Row{
                                width: parent.width
                                height: 28
                                spacing: 10
                                StyledLabel{
                                    width: 120
                                    height: parent.height
                                    font.pixelSize:12 
                                    verticalAlignment: Qt.AlignVCenter
                                    horizontalAlignment: Qt.AlignVCenter
                                    text: qsTr("Related Printer:")
                                    elide: Text.ElideRight
                                }
                                StyledLabel{
                                    id: idRelatedPrinter
                                    width: parent.width - 120 - 10
                                    height: parent.height
                                    font.pixelSize:12 
                                    verticalAlignment: Qt.AlignVCenter
                                    horizontalAlignment: Qt.AlignVCenter
                                    borderColor: Constants.dialogItemRectBgBorderColor
                                    text: ""
                                    elide: Text.ElideRight
                                }
                            }
                        }
                        Column{
                            id: idGcodeInfo
                            width: parent.width
                            height: 28*2 + 2
                            spacing: 2
                            Row{
                                width: parent.width
                                height: 28
                                spacing: 10
                                StyledLabel{
                                    width: 120
                                    height: parent.height
                                    font.pixelSize:12 
                                    verticalAlignment: Qt.AlignVCenter
                                    horizontalAlignment: Qt.AlignVCenter
                                    text: qsTr("Printing File:")
                                    elide: Text.ElideRight
                                }
                                BasicCombobox{
                                    id:idGcodeCombox
                                    width: parent.width - 120 - 10
                                    height: parent.height
                                    font.pixelSize : 12
                                    //currentIndex: 0
                                    model: ListModel {
                                        id: idGcodeListModel
                                    }

                                    onComboBoxIndexChanged: {	
                                        if(idGcodeListModel.count > 0)
                                        {
                                            curGcodeUrl = idGcodeListModel.get(currentIndex).url
                                            curGcodeId = idGcodeListModel.get(currentIndex).id

                                            idDeviceType.text = "  " + idGcodeListModel.get(currentIndex).key

                                            if(gcodeFileChanged == true)
                                            {
                                                idSelectGcodeWarringDlg.visible = true
                                            }
                                        }
                                    }
                                }
                            }
                            Row{
                                width: parent.width
                                height: 28
                                spacing: 10
                                StyledLabel{
                                    width: 120
                                    height: parent.height
                                    font.pixelSize:12 
                                    verticalAlignment: Qt.AlignVCenter
                                    horizontalAlignment: Qt.AlignVCenter
                                    text: qsTr("Device Type:")
                                    elide: Text.ElideRight
                                }
                                StyledLabel{
                                    id: idDeviceType
                                    width: parent.width - 120 - 10
                                    height: parent.height
                                    font.pixelSize:12 
                                    verticalAlignment: Qt.AlignVCenter
                                    horizontalAlignment: Qt.AlignVCenter
                                    borderColor: Constants.dialogItemRectBgBorderColor
                                    text: ""
                                    elide: Text.ElideRight
                                }
                            }
                        }
                        Rectangle{
                            id: idRectangleSpace1
                            width: parent.width
                            height: 10
                            color: "transparent"
                        }
                        Row{
                            width: parent.width
                            height: 28
                            spacing: 10
                            StyledLabel{
                                width: 120
                                height: parent.height
                                font.pixelSize:12 
                                verticalAlignment: Qt.AlignVCenter
                                horizontalAlignment: Qt.AlignVCenter
                                text: qsTr("Nozzle Temperature:")
                                elide: Text.ElideRight
                            }
                            PrintControlSpinBox {
                                id: idNozzleTempTargetLabel
                                width: 92
                                height: parent.height
                                unitchar : ""
                                realStepSize:1
                                //wheelEnable: false
                                //decimals: 0
                                realFrom:0
                                realTo:260
                                realValue: 200
                                onValueEdited:{ 
                                    //set temp 
                                    if(deviceId != "")
                                    {
                                        nozzleTargetTempIsChanged = true
                                        idNozzleTempTimer.restart()
                                    }
                                }
                            }
                            StyledLabel{
                                id: idNozzleTempText
                                width: parent.width - 120 - 10 - 92
                                height: parent.height
                                font.pixelSize:12 
                                verticalAlignment: Qt.AlignVCenter
                                horizontalAlignment: Qt.AlignVCenter
                                text: "120" + " ℃"
                                elide: Text.ElideRight
                            }
                        }
                        Row{
                            width: parent.width
                            height: 28
                            spacing: 10
                            StyledLabel{
                                width: 120
                                height: parent.height
                                font.pixelSize:12 
                                verticalAlignment: Qt.AlignVCenter
                                horizontalAlignment: Qt.AlignVCenter
                                text: qsTr("Hot Bed Temperature:")
                                elide: Text.ElideRight
                            }
                            PrintControlSpinBox {
                                id: idBedtempTargetLabel
                                width: 92
                                height: parent.height
                                unitchar : ""
                                realStepSize:1
                                //wheelEnable: false
                                //decimals: 0
                                realFrom:0
                                realTo:200
                                realValue: 60
                                onValueEdited:{
                                    //set temp 
                                    if(deviceId != "")
                                    {
                                        hotBedTargetTempIsChanged = true
                                        idHotBedTempTimer.restart()
                                    }
                                }
                            }
                            StyledLabel{
                                id: idBedTempText
                                width: parent.width - 120 - 10 - 92
                                height: parent.height
                                font.pixelSize:12 
                                verticalAlignment: Qt.AlignVCenter
                                horizontalAlignment: Qt.AlignVCenter
                                text: "120" + " ℃"
                                elide: Text.ElideRight
                            }
                        }
                        Row{
                            width: parent.width
                            height: 28
                            spacing: 10
                            StyledLabel{
                                width: 120
                                height: parent.height
                                font.pixelSize:12 
                                verticalAlignment: Qt.AlignVCenter
                                horizontalAlignment: Qt.AlignVCenter
                                text: qsTr("Printing Speed:")
                                elide: Text.ElideRight
                            }
                            PrintControlSpinBox {
                                id: idCurFeedratePctTarget
                                width: 92
                                height: parent.height
                                unitchar : "%"
                                realStepSize:1
                                //wheelEnable: false
                                //decimals: 0
                                realFrom:0
                                realTo:150
                                realValue: 100
                                onValueEdited:{ 
                                    //set temp 
                                    if(deviceId != "")
                                    {
                                        printSpeedTargetTempIsChanged = true
                                        idPrintSpeedTimer.restart()
                                    }
                                    
                                }
                            }
                            StyledLabel{
                                id: idCurFeedratePctLabel
                                width: parent.width - 120 - 10 - 92
                                height: parent.height
                                font.pixelSize:12 
                                verticalAlignment: Qt.AlignVCenter
                                horizontalAlignment: Qt.AlignVCenter
                                text: "100" + " %"
                                elide: Text.ElideRight
                                visible: false
                            }
                        }
                        Rectangle{
                            id: idRectangleSpace2
                            width: parent.width
                            height: 1
                            color: "transparent"
                        }
                        Row{
                            id: idPrintProgress
                            width: parent.width
                            height: 28
                            spacing: 10
                            StyledLabel{
                                width: 120
                                height:parent.height
                                font.pixelSize:12 
                                verticalAlignment: Qt.AlignVCenter
                                horizontalAlignment: Qt.AlignVCenter
                                text: qsTr("Printing Progress:")
                                elide: Text.ElideRight
                            }
                            Column{
                                width: parent.width - 120 - 10
                                height: 25
                                Row{
                                    width: parent.width
                                    height: parent.height
                                    StyledLabel{
                                        id: idPrintState
                                        width: parent.width - 50
                                        height:parent.height
                                        font.pixelSize:12
                                        text: ""
                                        elide: Text.ElideRight
                                    }
                                    StyledLabel{
                                        id: idPrintValue
                                        width: 50
                                        height:parent.height
                                        font.pixelSize:12
                                        text: {
                                            if(progressBar.value == 0)
                                            {
                                                return ""
                                            }
                                            else{
                                                return progressBar.value + "%"
                                            }
                                        }
                                        elide: Text.ElideRight
                                    }
                                }
                                ProgressBar{
                                    id: progressBar
                                    from: 0
                                    to:100
                                    value: 0
                                    width: parent.width
                                    height: 3

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
                        }
                        Column{
                            id: idPrintTimeInfo
                            width: parent.width
                            height: 28*2 + 2
                            Row{
                                width: parent.width
                                height: 28
                                spacing: 10
                                StyledLabel{
                                    width: 120
                                    height: parent.height
                                    font.pixelSize:12 
                                    verticalAlignment: Qt.AlignVCenter
                                    horizontalAlignment: Qt.AlignVCenter
                                    text: qsTr("Printing Time:")
                                    elide: Text.ElideRight
                                }
                                StyledLabel{
                                    id: idPrintedTimesLabel
                                    width: parent.width - 120 - 10
                                    height: parent.height
                                    font.pixelSize:12 
                                    verticalAlignment: Qt.AlignVCenter
                                    horizontalAlignment: Qt.AlignVCenter
                                    text: "3h20min"
                                    elide: Text.ElideRight
                                }
                            }
                            Row{
                                width: parent.width
                                height: 28
                                spacing: 10
                                StyledLabel{
                                    width: 120
                                    height: parent.height
                                    visible: false
                                    font.pixelSize:12 
                                    verticalAlignment: Qt.AlignVCenter
                                    horizontalAlignment: Qt.AlignVCenter
                                    text: qsTr("Remaining Time:")
                                    elide: Text.ElideRight
                                }
                                StyledLabel{
                                    id: idTimeLeftPrintLabel
                                    width: parent.width - 120 - 10
                                    height: parent.height
                                    visible: false
                                    font.pixelSize:12 
                                    verticalAlignment: Qt.AlignVCenter
                                    horizontalAlignment: Qt.AlignVCenter
                                    text: "- - -"
                                    elide: Text.ElideRight
                                }
                            }
                        }
                        Rectangle{
                            id: idRectangleSpace3
                            width: parent.width
                            height: 5
                            color: "transparent"
                        }
                        Row{
                            id: idFanInfo
                            width: parent.width
                            height: 28
                            spacing: 10
                            visible: false
                            StyledLabel{
                                width: 120
                                height: parent.height
                                font.pixelSize:12 
                                verticalAlignment: Qt.AlignVCenter
                                horizontalAlignment: Qt.AlignVCenter
                                text: qsTr("Fan:")
                                elide: Text.ElideRight
                            }
                            CusSkinButton_Image{
                                width:40
                                height: 20
                                //btnImgNormal: "qrc:/UI/photo/print_fan_off.png"
                                //btnImgHovered: "qrc:/UI/photo/print_fan_off.png"
                                //btnImgPressed: "qrc:/UI/photo/print_fan_off.png"
                                btnImgUrl: {
                                    if(curFanSwitch == 0)
                                    {
                                        return Constants.printFanOffImg//"qrc:/UI/photo/print_fan_off.png"
                                    }
                                    else if(curFanSwitch == 1)
                                    {
                                        return Constants.printFanOnImg//"qrc:/UI/photo/print_fan_on.png"
                                    }
                                }//
                                onClicked:
                                {	
                                    if(deviceId != "")
                                    {
                                        fanSwitchIsChanged = true
                                        if(curFanSwitch == 0)
                                        {
                                            controlPrintTempSpeed(deviceId, IotData.serIotIdMap[deviceId], IotData.serProductkeyMap[deviceId], "fanswitch", 1, deviceTbId, deviceIotType);
                                            curFanSwitch = 1
                                            idUpdateDeviceInfoTimer.restart()
                                        }
                                        else if(curFanSwitch == 1)
                                        {
                                            controlPrintTempSpeed(deviceId, IotData.serIotIdMap[deviceId], IotData.serProductkeyMap[deviceId], "fanswitch", 0, deviceTbId, deviceIotType);
                                            curFanSwitch = 0
                                            idUpdateDeviceInfoTimer.restart()
                                        }
                                    }
                                }
                            }
                        }
                        Rectangle{
                            id: idRectangleSpace4
                            width: parent.width
                            height: 5
                            color: "transparent"
                        }
                        Column{
                            id: idMovingAxisInfo
                            width: parent.width
                            height: 145
                            spacing: 5
                            visible: false
                            Row{
                                Rectangle{
                                    width: 50
                                    height: 45
                                    color: "transparent"
                                }
                                CusSkinButton_Image{
                                    width:45
                                    height: 45
                                    btnImgNormal: Constants.printMoveAxisYUpImg//"qrc:/UI/photo/print_move_axis_y+.png"
                                    btnImgHovered: Constants.printMoveAxisYUp_HImg//"qrc:/UI/photo/print_move_axis_y+_h.png"
                                    btnImgPressed: Constants.printMoveAxisYUp_CImg//"qrc:/UI/photo/print_move_axis_y+_c.png"
                                    onClicked:
                                    {	
                                        if(deviceId != "")
                                        {
                                            moveAxisYAddIsChanged = true
                                            if(idMove01mm.checked == true)
                                            {
                                                moveCurAxisY = (parseFloat(moveCurAxisY) + parseFloat(0.1)).toFixed(2)
                                            }
                                            if(idMove1mm.checked == true)
                                            {
                                                moveCurAxisY = (parseFloat(moveCurAxisY) + parseFloat(1)).toFixed(2)
                                            }
                                            if(idMove10mm.checked == true)
                                            {
                                                moveCurAxisY = (parseFloat(moveCurAxisY) + parseFloat(10)).toFixed(2)
                                            }
                                            idMoveYChangedTimer.restart()
                                        }
                                    }
                                }
                                Rectangle{
                                    width: 69
                                    height: 45
                                    color: "transparent"
                                }
                                CusSkinButton_Image{
                                    width:45
                                    height: 45
                                    btnImgNormal: Constants.printMoveAxisZUpImg//"qrc:/UI/photo/print_move_axis_z+.png"
                                    btnImgHovered: Constants.printMoveAxisZUp_HImg//"qrc:/UI/photo/print_move_axis_z+_h.png"
                                    btnImgPressed: Constants.printMoveAxisZUp_CImg//"qrc:/UI/photo/print_move_axis_z+_c.png"
                                    btnImgDisbaled: Constants.printMoveAxisZUpImg
                                    enabled: {
                                        if(!isReverseZAxisOperation(printerNameMap[deviceId]))
                                        {
                                            return true
                                        }
                                        else
                                        {
                                             if(idMove01mm.checked == true)
                                            {
                                                if(moveCurAxisZ < 0.1)
                                                {
                                                    return false
                                                }
                                                else{
                                                    return true
                                                }
                                            }
                                            if(idMove1mm.checked == true)
                                            {
                                                if(moveCurAxisZ < 1)
                                                {
                                                    return false
                                                }
                                                else{
                                                    return true
                                                }
                                            }
                                            if(idMove10mm.checked == true)
                                            {
                                                if(moveCurAxisZ < 10)
                                                {
                                                    return false
                                                }
                                                else{
                                                    return true
                                                }
                                            }
                                        }
                                    }
                                    onClicked:
                                    {	
                                        if(deviceId != "")
                                        {
                                            moveAxisZAddIsChanged = true
                                            if(idMove01mm.checked == true)
                                            {
                                                moveCurAxisZ = isReverseZAxisOperation(IotData.iotPrinterNameMap[deviceId]) ? ((parseFloat(moveCurAxisZ) - parseFloat(0.1)).toFixed(2)) : ((parseFloat(moveCurAxisZ) + parseFloat(0.1)).toFixed(2))
                                            }
                                            if(idMove1mm.checked == true)
                                            {
                                                moveCurAxisZ = isReverseZAxisOperation(IotData.iotPrinterNameMap[deviceId]) ? ((parseFloat(moveCurAxisZ) - parseFloat(1)).toFixed(2)) : ((parseFloat(moveCurAxisZ) + parseFloat(1)).toFixed(2))
                                            }
                                            if(idMove10mm.checked == true)
                                            {
                                                moveCurAxisZ = isReverseZAxisOperation(IotData.iotPrinterNameMap[deviceId]) ? ((parseFloat(moveCurAxisZ) - parseFloat(10)).toFixed(2)) : ((parseFloat(moveCurAxisZ) + parseFloat(10)).toFixed(2))
                                            }
                                            idMoveZChangedTimer.restart()
                                        }
                                    }
                                }
                            }
                            Row{
                                spacing: 5
                                CusSkinButton_Image{
                                    width:45
                                    height: 45
                                    btnImgNormal: Constants.printMoveAxisXDownImg//"qrc:/UI/photo/print_move_axis_x-.png"
                                    btnImgHovered: Constants.printMoveAxisXDown_HImg//"qrc:/UI/photo/print_move_axis_x-_h.png"
                                    btnImgPressed: Constants.printMoveAxisXDown_CImg//"qrc:/UI/photo/print_move_axis_x-_c.png"
                                    btnImgDisbaled: Constants.printMoveAxisXDownImg//"qrc:/UI/photo/print_move_axis_x-.png"
                                    enabled: {
                                        if(idMove01mm.checked == true)
                                        {
                                            if(moveCurAxisX < 0.1)
                                            {
                                                return false
                                            }
                                            else{
                                                return true
                                            }
                                        }
                                        if(idMove1mm.checked == true)
                                        {
                                            if(moveCurAxisX < 1)
                                            {
                                                return false
                                            }
                                            else{
                                                return true
                                            }
                                        }
                                        if(idMove10mm.checked == true)
                                        {
                                            if(moveCurAxisX < 10)
                                            {
                                                return false
                                            }
                                            else{
                                                return true
                                            }
                                        }
                                    }
                                    onClicked:
                                    {	
                                        if(deviceId != "")
                                        {
                                            moveAxisXAddIsChanged = true
                                            if(idMove01mm.checked == true)
                                            {
                                                moveCurAxisX = (parseFloat(moveCurAxisX) - parseFloat(0.1)).toFixed(2)
                                            }
                                            if(idMove1mm.checked == true)
                                            {
                                                moveCurAxisX = (parseFloat(moveCurAxisX) - parseFloat(1)).toFixed(2)
                                            }
                                            if(idMove10mm.checked == true)
                                            {
                                                moveCurAxisX = (parseFloat(moveCurAxisX) - parseFloat(10)).toFixed(2)
                                            }
                                            idMoveXChangedTimer.restart()
                                        }
                                    }
                                }
                                CusSkinButton_Image{
                                    width:45
                                    height: 45
                                    btnImgNormal: Constants.printMoveAxisZeroImg//"qrc:/UI/photo/print_move_axis_zero.png"
                                    btnImgHovered: Constants.printMoveAxisZero_HImg//"qrc:/UI/photo/print_move_axis_zero_h.png"
                                    btnImgPressed: Constants.printMoveAxisZero_CImg//"qrc:/UI/photo/print_move_axis_zero_c.png"
                                    onClicked:
                                    {	
                                        if(deviceId != "")
                                        {
                                            moveAxisXAddIsChanged = true
                                            moveAxisYAddIsChanged = true

                                            moveCurAxisX = parseFloat(0).toFixed(2)
                                            moveCurAxisY = parseFloat(0).toFixed(2)

                                            idMoveXYZeroTimer.restart()
                                        }
                                    }
                                }
                                CusSkinButton_Image{
                                    width:45
                                    height: 45
                                    btnImgNormal: Constants.printMoveAxisXUpImg//"qrc:/UI/photo/print_move_axis_x+.png"
                                    btnImgHovered: Constants.printMoveAxisXUp_HImg//"qrc:/UI/photo/print_move_axis_x+_h.png"
                                    btnImgPressed: Constants.printMoveAxisXUp_CImg//"qrc:/UI/photo/print_move_axis_x+_c.png"
                                    onClicked:
                                    {	
                                        if(deviceId != "")
                                        {
                                            moveAxisXAddIsChanged = true
                                            if(idMove01mm.checked == true)
                                            {
                                                moveCurAxisX = (parseFloat(moveCurAxisX) + parseFloat(0.1)).toFixed(2)
                                            }
                                            if(idMove1mm.checked == true)
                                            {
                                                moveCurAxisX = (parseFloat(moveCurAxisX) + parseFloat(1)).toFixed(2)
                                            }
                                            if(idMove10mm.checked == true)
                                            {
                                                moveCurAxisX = (parseFloat(moveCurAxisX) + parseFloat(10)).toFixed(2)
                                            }
                                            idMoveXChangedTimer.restart()
                                        }
                                    }
                                }
                                Rectangle{
                                    width: 9
                                    height: 45
                                    color: "transparent"
                                }
                                CusSkinButton_Image{
                                    width:45
                                    height: 45
                                    btnImgNormal: Constants.printMoveAxisZeroImg//"qrc:/UI/photo/print_move_axis_zero.png"
                                    btnImgHovered: Constants.printMoveAxisZero_HImg//"qrc:/UI/photo/print_move_axis_zero_h.png"
                                    btnImgPressed: Constants.printMoveAxisZero_CImg//"qrc:/UI/photo/print_move_axis_zero_c.png"
                                    onClicked:
                                    {	
                                        if(deviceId != "")
                                        {
                                            moveAxisZAddIsChanged = true

                                            moveCurAxisZ = parseFloat(0).toFixed(2)
                                            
                                            idMoveZZeroTimer.restart()
                                        }
                                    }
                                }
                            }
                            Row{
                                Rectangle{
                                    width: 50
                                    height: 45
                                    color: "transparent"
                                }
                                CusSkinButton_Image{
                                    width:45
                                    height: 45
                                    btnImgNormal: Constants.printMoveAxisYDownImg//"qrc:/UI/photo/print_move_axis_y-.png"
                                    btnImgHovered: Constants.printMoveAxisYDown_HImg//"qrc:/UI/photo/print_move_axis_y-_h.png"
                                    btnImgPressed: Constants.printMoveAxisYDown_CImg//"qrc:/UI/photo/print_move_axis_y-_c.png"
                                    btnImgDisbaled: Constants.printMoveAxisYDownImg//"qrc:/UI/photo/print_move_axis_y-.png"
                                    enabled: {
                                        if(idMove01mm.checked == true)
                                        {
                                            if(moveCurAxisY < 0.1)
                                            {
                                                return false
                                            }
                                            else{
                                                return true
                                            }
                                        }
                                        if(idMove1mm.checked == true)
                                        {
                                            if(moveCurAxisY < 1)
                                            {
                                                return false
                                            }
                                            else{
                                                return true
                                            }
                                        }
                                        if(idMove10mm.checked == true)
                                        {
                                            if(moveCurAxisY < 10)
                                            {
                                                return false
                                            }
                                            else{
                                                return true
                                            }
                                        }
                                    }
                                    onClicked:
                                    {	
                                        if(deviceId != "")
                                        {
                                            moveAxisYAddIsChanged = true
                                            if(idMove01mm.checked == true)
                                            {
                                                moveCurAxisY = (parseFloat(moveCurAxisY) - parseFloat(0.1)).toFixed(2)
                                            }
                                            if(idMove1mm.checked == true)
                                            {
                                                moveCurAxisY = (parseFloat(moveCurAxisY) - parseFloat(1)).toFixed(2)
                                            }
                                            if(idMove10mm.checked == true)
                                            {
                                                moveCurAxisY = (parseFloat(moveCurAxisY) - parseFloat(10)).toFixed(2)
                                            }
                                            idMoveYChangedTimer.restart()
                                        }
                                    }
                                }
                                Rectangle{
                                    width: 69
                                    height: 45
                                    color: "transparent"
                                }
                                CusSkinButton_Image{
                                    width:45
                                    height: 45
                                    btnImgNormal: Constants.printMoveAxisZDownImg//"qrc:/UI/photo/print_move_axis_z-.png"
                                    btnImgHovered: Constants.printMoveAxisZDown_HImg//"qrc:/UI/photo/print_move_axis_z-_h.png"
                                    btnImgPressed: Constants.printMoveAxisZDown_CImg//"qrc:/UI/photo/print_move_axis_z-_c.png"
                                    btnImgDisbaled: Constants.printMoveAxisZDownImg//"qrc:/UI/photo/print_move_axis_z-.png"
                                    enabled: {
                                        if(isReverseZAxisOperation(printerNameMap[deviceId]))
                                        {
                                            return true
                                        }
                                        else
                                        {
                                             if(idMove01mm.checked == true)
                                            {
                                                if(moveCurAxisZ < 0.1)
                                                {
                                                    return false
                                                }
                                                else{
                                                    return true
                                                }
                                            }
                                            if(idMove1mm.checked == true)
                                            {
                                                if(moveCurAxisZ < 1)
                                                {
                                                    return false
                                                }
                                                else{
                                                    return true
                                                }
                                            }
                                            if(idMove10mm.checked == true)
                                            {
                                                if(moveCurAxisZ < 10)
                                                {
                                                    return false
                                                }
                                                else{
                                                    return true
                                                }
                                            }
                                        }
                                    }
                                    onClicked:
                                    {	
                                        if(deviceId != "")
                                        {
                                            moveAxisZAddIsChanged = true
                                            if(idMove01mm.checked == true)
                                            {
                                                moveCurAxisZ = isReverseZAxisOperation(IotData.iotPrinterNameMap[deviceId]) ? ((parseFloat(moveCurAxisZ) + parseFloat(0.1)).toFixed(2)) : ((parseFloat(moveCurAxisZ) - parseFloat(0.1)).toFixed(2))
                                            }
                                            if(idMove1mm.checked == true)
                                            {
                                                moveCurAxisZ = isReverseZAxisOperation(IotData.iotPrinterNameMap[deviceId]) ? ((parseFloat(moveCurAxisZ) + parseFloat(1)).toFixed(2)) : ((parseFloat(moveCurAxisZ) - parseFloat(1)).toFixed(2))
                                            }
                                            if(idMove10mm.checked == true)
                                            {
                                                moveCurAxisZ = isReverseZAxisOperation(IotData.iotPrinterNameMap[deviceId]) ? ((parseFloat(moveCurAxisZ) + parseFloat(10)).toFixed(2)) : ((parseFloat(moveCurAxisZ) - parseFloat(10)).toFixed(2))
                                            }
                                            idMoveZChangedTimer.restart()
                                        }
                                    }
                                }
                            }
                        }
                        Row{
                            id: idMovePositionInfo
                            width: parent.width
                            height: 28
                            visible: false
                            spacing: -1
                            StyledLabel{
                                width: 120
                                height: parent.height
                                font.pixelSize: 12
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment:Text.AlignHCenter
                                text: "X: " + "<font color='#FF0E3B'>" + moveCurAxisX +  "</font>" + "/" + moveCurRealAxisX
                                borderColor: Constants.dialogItemRectBgBorderColor
                                bgColor: Constants.textBackgroundColor
                            }
                            StyledLabel{
                                width: 120
                                height: parent.height
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment:Text.AlignHCenter
                                font.pixelSize: 12
                                text: "Y: " + "<font color='#00FF00'>" + moveCurAxisY +  "</font>" + "/" + moveCurRealAxisY
                                borderColor: Constants.dialogItemRectBgBorderColor//"#333333"
                                bgColor: Constants.textBackgroundColor//"#4B4B4B"
                            }
                            StyledLabel{
                                width: 120
                                height: parent.height
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment:Text.AlignHCenter
                                font.pixelSize: 12
                                text: "Z: " + "<font color='#3782FF'>" + moveCurAxisZ +  "</font>" + "/" + moveCurRealAxisZ
                                borderColor: Constants.dialogItemRectBgBorderColor
                                bgColor: Constants.textBackgroundColor
                            }
                        }
                        Row{
                            id:idMoveSpcingInfo
                            width:parent.width
                            height: 20
                            spacing:20
                            visible: false
                            ButtonGroup { id: idMoveSpcingGroup }
                            BasicRadioButton {
                                id: idMove01mm
                                text: "0.1mm"
                                checkRadiusColor: "#1E9BE2"
                                checked: true
                                ButtonGroup.group: idMoveSpcingGroup
                                onClicked:
                                {
                                }
                            }
                            BasicRadioButton {
                                id: idMove1mm
                                text: "1mm"
                                checkRadiusColor: "#1E9BE2"
                                ButtonGroup.group: idMoveSpcingGroup
                                onClicked:
                                {
                                }
                            }
                            BasicRadioButton {
                                id: idMove10mm
                                text: "10mm"
                                checkRadiusColor: "#1E9BE2"
                                ButtonGroup.group: idMoveSpcingGroup
                                checked: true
                                onClicked:
                                {
                                }
                            }
                        }
                    }

                    Rectangle{
                        id: idVideoItemRectangle
                        width: 390
                        height: 300
                        border.color: Constants.rectBorderColor
                        border.width: 1
                        color: "transparent"
                        
                    }
                }

            }
            Rectangle{
                id: idRectangleSpace5
                width: parent.width
                height: 1
                border.color: Constants.splitLineColor
		        border.width: 1
		        color: "transparent"
            }
            Row{
                id: idControlBtn
                width: parent.width
                height: 28
                spacing: 20

                Rectangle{
                    width: 125*2 + 50
                    height: parent.height
                    color: "transparent"
                }

                BasicDialogButton
                {
                    id: idStartPrintBtn
                    width: 125
                    height: 28
                    btnRadius: 3
                    btnBorderW: 0
                    visible: false
                    idAnimatedEnbale: true
                    isAnimatedImage:false
                    text: "  " + qsTr("Start")
                    enabled:{
                        if(idPrinterCombox.currentIndex >= 0 && idGcodeCombox.currentIndex >= 0){
                            return true
                        }
                        else{
                            return false
                        }
                    }
                    onSigButtonClicked:
                    {
                        idStartGcodeWarringDlg.visible = true
                    }
                    onVisibleChanged:
                    {
                        if(idStartPrintBtn.visible == false)
                        {
                            idStartPrintBtn.setAnimatedImageStatus(false)
                        }
                    }
                }
                BasicDialogButton
                {
                    id: idPausePrintBtn
                    width: 125
                    height: 28
                    btnRadius: 3
                    btnBorderW: 0
                    visible: false
                    isAnimatedImage:false
                    text: "  " + qsTr("Pause")
                    onSigButtonClicked:
                    {
                        if(deviceId != "")
                        {
                            controlPrintGcode(deviceId, IotData.serIotIdMap[deviceId], IotData.serProductkeyMap[deviceId], "pause",deviceTbId, deviceIotType);
                            idPausePrintBtn.setAnimatedImageStatus(true)
                            idUpdateDeviceInfoTimer.restart()
                        }
                    }
                    onVisibleChanged:
                    {
                        if(idPausePrintBtn.visible == false)
                        {
                            idPausePrintBtn.setAnimatedImageStatus(false)
                        }
                    }
                }
                BasicDialogButton
                {
                    id: idContinuePrintBtn
                    width: 125
                    height: 28
                    btnRadius: 3
                    btnBorderW: 0
                    visible: false
                    isAnimatedImage:false
                    text: "  " + qsTr("Continue")
                    onSigButtonClicked:
                    {
                        if(deviceId != "")
                        {
                            controlPrintGcode(deviceId, IotData.serIotIdMap[deviceId], IotData.serProductkeyMap[deviceId], "continue",deviceTbId, deviceIotType)
                            idContinuePrintBtn.setAnimatedImageStatus(true)
                            idUpdateDeviceInfoTimer.restart()
                        }
                    }
                    onVisibleChanged:
                    {
                        if(idContinuePrintBtn.visible == false)
                        {
                            idContinuePrintBtn.setAnimatedImageStatus(false)
                        }
                    }
                }
                BasicDialogButton
                {
                    id: idStopPrintBtn
                    width: 125
                    height: 28
                    btnRadius: 3
                    btnBorderW: 0
                    visible: false
                    isAnimatedImage:false
                    text: "  " + qsTr("Stop")
                    onSigButtonClicked:
                    {
                        idStopGcodeWarringDlg.visible = true
                    }
                    onVisibleChanged:
                    {
                        if(idStopPrintBtn.visible == false)
                        {
                            idStopPrintBtn.setAnimatedImageStatus(false)
                        }
                    }
                }
            }
        }
    }

    UploadMessageDlg{
        id:idSelectGcodeWarringDlg
        messageType:0
        msgText: qsTr("Please make sure that the device type you chose while slicing must match with the printing device, or it may lead you to a printing failure or even cause an irreversible printer damage. Do you want to start printing?")
        visible: false
        cancelBtnVisible: false
        onSigOkButtonClicked:
        {
            idSelectGcodeWarringDlg.close()
        }
    }

    UploadMessageDlg{
        id:idStartGcodeWarringDlg
        messageType:1
        msgText: qsTr("Please make sure to clear the HotBed before printing.( Or it wil causes irritrievable damage to the printer if there still has something on the HotBed)") 
        visible: false
        cancelBtnVisible: false
        onSigOkButtonClicked:
        {
            idStartGcodeWarringDlg.close()
            if(deviceId != "")
            {
                startPrintGcode(curGcodeUrl, deviceId, IotData.serIotIdMap[deviceId], IotData.serProductkeyMap[deviceId], curGcodeId, IotData.iotPrinterNameMap[deviceId], printerIdMap[IotData.iotPrinterNameMap[deviceId]], deviceTbId, deviceIotType)
                idStartPrintBtn.setAnimatedImageStatus(true)
                idUpdateDeviceInfoTimer.restart()
            }
        }
    }

    UploadMessageDlg{
        id:idStopGcodeWarringDlg
        messageType:1
        msgText: qsTr("Are you sure to stop printing?") 
        visible: false
        onSigOkButtonClicked:
        {
            idStopGcodeWarringDlg.close()
            if(deviceId != "")
            {
                controlPrintGcode(deviceId, IotData.serIotIdMap[deviceId], IotData.serProductkeyMap[deviceId], "stop",deviceTbId, deviceIotType);
                idStopPrintBtn.setAnimatedImageStatus(true)
                idUpdateDeviceInfoTimer.restart()
            }
        }
    }

    UploadMessageDlg{
        id: idDeviceOfflineDlg
        messageType:0
        msgText: qsTr("The device is offline, please check the device status!!!") 
        cancelBtnVisible: false
        visible: false
        onSigOkButtonClicked:
        {
            idDeviceOfflineDlg.close()
            idPrintControlInfoDlg.close()
        }
    }
}
