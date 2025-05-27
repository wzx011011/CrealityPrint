import QtQuick 2.10
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0 as QQC2
import "../qml"
import "../secondqml"
BasicDialog{
    id: idPrintControlInfoDlg
    width: 860+60+ 50
    height: 520
    titleHeight : 30
    title: qsTr("Print Control")




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


    //begin  gcode to print
    property var gcodeToPrintFlag : false

    function initGcodeToPrint(gcodeId)
    {

    }

    function insertPrinterList()
    {

    }
    //end
    /////lsg

    enum PrintControlType
    {
        PRINT_START,
        PRINT_PAUSE,
        PRINT_CONTINUE,
        PRINT_STOP,
        CONTROL_MOVE_X,
        CONTROL_MOVE_Y,
        CONTROL_MOVE_Z,
        CONTROL_MOVE_XY0,
        CONTROL_MOVE_Z0,
        CONTROL_CMD_FUN,
        CONTROL_CMD_BEDTEMP,
        CONTROL_CMD_NOZZLETEMP,
        CONTROL_CMD_FEEDRATEPCT,
        CONTROL_CMD_AUTOHOME
    }

    property var curMac : ""
    property var curPrinterName: ""
    property var curPrintState: 0
    property var curFanSwitchState: 0
    property string curMyPosition
    property var curPrintJobTime : 0.0
    property var curNozzleTemp          //喷嘴当前温度
    property var curNozzleTempTarget       //喷嘴目标温度
    property var curBedTemp                //打印平台当前温度
    property var curBegTempTarget //打印平台目标温度
    property var curPrintSpeed          //打印速度
    property var curPrintProgress      //打印进度

    property var curSelectGCodePath : ""
    signal sendControlPrinterCmd(var strmac,var cmdType,var value)

    function initCurMac(mac)
    {
        curMac = mac
    }

    function initGcodeList(mapData)
    {
        idGcodeListModel.clear()
        for(var key in mapData)
        {
           var keyname = {"text": keyname}    //取key
            idGcodeListModel.append({"modelData": key,"modelFullData" : mapData[key]})

        }
        idGcodeCombox.currentIndex = 0

    }
    function updateMyConrtrolData(mapData,mac)
    {
        if(curMac !== mac)
        {
            return ;
        }
        for(var key in mapData)
        {
           var keyname = {"text": keyname}    //取key
            if(key === "printerName")
            {
                curPrinterName = mapData[key]
            }
            else if(key == "printState")
            {
                curPrintState = mapData[key]
            }
            else if (key =="fanSwitchState")
            {
                curFanSwitchState = mapData[key]
            }
            else if( key == "curPosition")
            {
                curMyPosition = mapData[key]
            }
            else if(key == "printJobTime")
            {
                curPrintJobTime = mapData[key]
            }
            else if(key == "nozzleTemp")
            {
                curNozzleTemp = mapData[key]
            }
            else if(key === "nozzleTempTarget")
            {
                curNozzleTempTarget = mapData[key]
            }
            else if(key == "bedTemp")
            {
                curBedTemp = mapData[key]
            }
            else if(key == "begTempTarget")
            {
                curBegTempTarget = mapData[key]
            }
            else if(key == "printSpeed")
            {
                curPrintSpeed = mapData[key]
            }
            else if(key == "printProgress")
            {
                curPrintProgress = mapData[key]
            }
            console.log("mapData[key] =" + mapData[key] + "-------kkkey =" + key)
        }
        updateQMLData()
    }

    ///
    function initPrintControlPage()
    {
        //gcode
        if(curPrintState == 0 || curPrintState == 2 || curPrintState == 3 || curPrintState == 4)
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
        else if(curPrintState == 1)//printing
        {
//            getPrintingPauseGcodeInfo(IotData.iotPrintIDMap[deviceId])

            idGcodeCombox.enabled = false
            idPrinterCombox.enabled = false

            idStartPrintBtn.visible = false
            idPausePrintBtn.visible = true
            idContinuePrintBtn.visible = false
            idStopPrintBtn.visible = true
        }
        else if(curPrintState== 5)//pause
        {

            idGcodeCombox.enabled = false
            idPrinterCombox.enabled = false

            idStartPrintBtn.visible = false
            idPausePrintBtn.visible = false
            idContinuePrintBtn.visible = true
            idStopPrintBtn.visible = true
        }

        updateQMLData()
    }
    function updateQMLData()
    {
        if(fanSwitchIsChanged == false)
        {
            curFanSwitch = curFanSwitchState
        }
        var str = curMyPosition //str = "X:112 Y:23 Z:0"
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

        //刷新打印时间
        if(curPrintState== 1 || curPrintState == 5)
        {
            var tmp = curPrintJobTime //IotData.iotPrintedTimesMap[deviceId]

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
            progressBar.value = curPrintProgress /*IotData.iotPrintProgressMap[deviceId]*/
        }
        else{
            idPrintedTimesLabel.text = ""
            progressBar.value = 0
        }

        idTimeLeftPrintLabel.text = "- - -"     //IotData.iotTimesLeftToPrintMap[deviceId]

        //喷嘴温度
        if(/*IotData.iotNozzleTempMap[deviceId]*/curNozzleTemp != undefined)
        {
            idNozzleTempText.text = /*IotData.iotNozzleTempMap[deviceId]*/curNozzleTemp + " ℃"
        }
        else{
            idNozzleTempText.text = 0 + " ℃"
        }
        //喷嘴目标温度
        if(nozzleTargetTempIsChanged == false)
        {
            idNozzleTempTargetLabel.realValue = /*IotData.iotNozzleTempTargetMap[deviceId]*/curNozzleTempTarget
        }
        else{
            nozzleTargetTempCount++
            if(nozzleTargetTempCount == 2)
            {
                nozzleTargetTempIsChanged = false
                nozzleTargetTempCount = 0
            }
        }
        //
        if(/*IotData.iotBedTempMap[deviceId] */curBedTemp!= undefined)
        {
            idBedTempText.text = curBedTemp/*IotData.iotBedTempMap[deviceId]*/ + " ℃"
        }
        else{
            idBedTempText.text =  0 + " ℃"
        }

        if(hotBedTargetTempIsChanged == false)
        {
            idBedtempTargetLabel.realValue =curBegTempTarget /*IotData.iotBedTempTargetMap[deviceId]*/
        }
        else{
            hotBedTargetTempCount++
            if(hotBedTargetTempCount == 2)
            {
                hotBedTargetTempIsChanged = false
                hotBedTargetTempCount = 0
            }
        }
        //打印速度
        if(printSpeedTargetTempIsChanged == false)
        {
            if(/*IotData.iotCurFeedratePctMap[deviceId]*/curPrintSpeed != undefined)
            {
                idCurFeedratePctTarget.realValue = curPrintSpeed/*IotData.iotCurFeedratePctMap[deviceId]*/
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

        if(/*IotData.iotCurFeedratePctMap[deviceId]*/curPrintSpeed == 0)
        {
            idCurFeedratePctLabel.text = "100" + " %"
        }
        else{
            idCurFeedratePctLabel.text = /*IotData.iotCurFeedratePctMap[deviceId]*/ curPrintSpeed + " %"
        }



        if(/*IotData.iotPrintStateMap[deviceId]*/curPrintState == 0) //initial
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
        else if(/*IotData.iotPrintStateMap[deviceId]*/curPrintState == 1)
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
        else if(/*IotData.iotPrintStateMap[deviceId]*/ curPrintState== 2) //print completed
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
        else if(/*IotData.iotPrintStateMap[deviceId]*/curPrintState == 3) //print failed
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
        else if(/*IotData.iotPrintStateMap[deviceId]*/curPrintState == 4) //print stop
        {
                idPrintState.text = qsTr("Idle")

                idStartPrintBtn.visible = true
                idPausePrintBtn.visible = false
                idContinuePrintBtn.visible = false
                idStopPrintBtn.visible = false
                idControllerBtn.visible = true
        }
        else if(/*IotData.iotPrintStateMap[deviceId]*/curPrintState== 5)
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

    Timer{
        id: idUpdateDeviceInfoTimer
        interval: 2000
        repeat: false
        onTriggered:{
            if(curMac != ""){
                //updateDeviceData(deviceId, IotData.serIotIdMap[deviceId], IotData.serProductkeyMap[deviceId], deviceTbId)
            }
        }
    }

    Timer{
        id:adjustmentTimer
        interval: 10000
        repeat: true
        triggeredOnStart: true
        onTriggered: {
            if(curMac != "")
            {
//                controlPrintTempSpeed(deviceId, IotData.serIotIdMap[deviceId], IotData.serProductkeyMap[deviceId], "ReqPrinterPara", 1, deviceTbId)
            }   
        }
    }

    Timer{
        id: idMoveXChangedTimer
        interval: 1000
        repeat: false
        onTriggered:{
            if(curMac != ""){
//                controlPrintTempSpeed(deviceId, IotData.serIotIdMap[deviceId], IotData.serProductkeyMap[deviceId], "gcodeCmdG0X", parseInt(moveCurAxisX*100), deviceTbId);
                sendControlPrinterCmd(curMac,PrintControlInfoDlg_Local.PrintControlType.CONTROL_MOVE_X, parseInt(moveCurAxisX*100) / 100)

                idUpdateDeviceInfoTimer.restart()
            }
        }
    }
    Timer{
        id: idMoveYChangedTimer
        interval: 1000
        repeat: false
        onTriggered:{
            if(curMac != ""){
//                controlPrintTempSpeed(deviceId, IotData.serIotIdMap[deviceId], IotData.serProductkeyMap[deviceId], "gcodeCmdG0Y", parseInt(moveCurAxisY*100), deviceTbId);
                sendControlPrinterCmd(curMac,PrintControlInfoDlg_Local.PrintControlType.CONTROL_MOVE_Y, parseInt(moveCurAxisY*100) / 100)

                idUpdateDeviceInfoTimer.restart()
            }
        }
    }
    Timer{
        id: idMoveZChangedTimer
        interval: 1000
        repeat: false
        onTriggered:{
            if(curMac != ""){
//               controlPrintTempSpeed(deviceId, IotData.serIotIdMap[deviceId], IotData.serProductkeyMap[deviceId], "gcodeCmdG0Z", parseInt(moveCurAxisZ*100), deviceTbId);
               sendControlPrinterCmd(curMac,PrintControlInfoDlg_Local.PrintControlType.CONTROL_MOVE_Z, parseInt(moveCurAxisZ*100) / 100)
                idUpdateDeviceInfoTimer.restart()
            }
        }
    }
    Timer{
        id: idMoveXYZeroTimer
        interval: 1000
        repeat: false
        onTriggered:{
            if(curMac != ""){
//               controlPrintTempSpeed(deviceId, IotData.serIotIdMap[deviceId], IotData.serProductkeyMap[deviceId], "gcodeCmdG28XY", 0, deviceTbId);
               sendControlPrinterCmd(curMac,PrintControlInfoDlg_Local.PrintControlType.CONTROL_MOVE_XY0,0)
                idUpdateDeviceInfoTimer.restart()
            }
        }
    }
    Timer{
        id: idMoveZZeroTimer
        interval: 1000
        repeat: false
        onTriggered:{
            if(curMac != ""){
//               controlPrintTempSpeed(deviceId, IotData.serIotIdMap[deviceId], IotData.serProductkeyMap[deviceId], "gcodeCmdG28Z", 0, deviceTbId);
                sendControlPrinterCmd(curMac,PrintControlInfoDlg_Local.PrintControlType.CONTROL_MOVE_Z0,0)

                idUpdateDeviceInfoTimer.restart()
            }
        }
    }
    Timer{
        id: idNozzleTempTimer
        interval: 1000
        repeat: false
        onTriggered:{
            //
            if(curMac != ""){
//               controlPrintTempSpeed(deviceId, IotData.serIotIdMap[deviceId], IotData.serProductkeyMap[deviceId], "nozzletargettemp", idNozzleTempTargetLabel.realValue, deviceTbId);
               sendControlPrinterCmd(curMac,PrintControlInfoDlg_Local.PrintControlType.CONTROL_CMD_NOZZLETEMP,idNozzleTempTargetLabel.realValue)
                idUpdateDeviceInfoTimer.restart()
            }
        }
    }
    Timer{
        id: idHotBedTempTimer
        interval: 1000
        repeat: false
        onTriggered:{
            if(curMac != ""){
//                controlPrintTempSpeed(deviceId, IotData.serIotIdMap[deviceId], IotData.serProductkeyMap[deviceId], "hotbedtargettemp", idBedtempTargetLabel.realValue, deviceTbId);
                sendControlPrinterCmd(curMac,PrintControlInfoDlg_Local.PrintControlType.CONTROL_CMD_BEDTEMP,idBedtempTargetLabel.realValue)
                idUpdateDeviceInfoTimer.restart()
            }
        }
    }
    Timer{
        id: idPrintSpeedTimer
        interval: 1000
        repeat: false
        onTriggered:{
            if(curMac != ""){
//                controlPrintTempSpeed(deviceId, IotData.serIotIdMap[deviceId], IotData.serProductkeyMap[deviceId], "printspeedtarget", idCurFeedratePctTarget.realValue, deviceTbId);
                sendControlPrinterCmd(curMac,PrintControlInfoDlg_Local.PrintControlType.CONTROL_CMD_FEEDRATEPCT,idCurFeedratePctTarget.realValue)
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
//            if(curproductName != "")
//            {
//                sigAddVideo(curproductName)
//            }
            
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
//        if(idPrintControlInfoDlg.visible == true)
//        {
//            changeIotPollTime("8000ms")
//        }
//        else{
//            changeIotPollTime("30000ms")
//        }
    }

    onDialogClosed:
    {
        gcodeFileChanged = false
        isClosed = true
        clearQMLData()
        adjustmentTimer.stop()
    }

    function setCurPrintingGcodeInfo(gcodeId)
    {
//        for(var i = 0; i < idGcodeListModel.count; i++)
//        {
//            if(gcodeId == idGcodeListModel.get(i).id)
//            {
//                idGcodeCombox.currentIndex = i
//            }
//        }

//        gcodeFileChanged = true
    }

    function selectCurPrinterInfo(name, id, tbid)
    {
//        deviceName = name
//        deviceId = id
//        deviceTbId = tbid

//        idPrinterCombox.enabled = false
//        idPrinterListModel.clear()
//        idPrinterCombox.currentIndex = 0
//        idRelatedPrinter.text = "  " + IotData.iotPrinterNameMap[deviceId]
//        idPrinterListModel.append({"key": IotData.iotPrinterNameMap[deviceId], "modelData": deviceName, "deviceValueID": deviceId})

//        updateQMLData()
    }

    function setPrinterModelInfo(strjson)
    {
//        var objectArray = JSON.parse(strjson);
//        if(objectArray.code === 0)
//        {
//            var objResult = objectArray.result.list;
//            for ( var key in objResult ) {
//                printerIdMap[objResult[key].name] = objResult[key].id
//            }
//        }
//        else{}
    }

    function setServerDeviceData(strjson)
    {
//        for(var i = 0; i < IotData.serDeviceNameArray.length; i++)
//        {
//            if(findMapBykey(IotData.serDeviceNameArray[i]))
//            {
//                continue
//            }
//            else
//            {
//                async_createVideoItem(IotData.serDeviceNameArray[i], IotData.serSyDidMap[IotData.serDeviceNameArray[i]], IotData.serSyLecenseMap[IotData.serDeviceNameArray[i]])
//            }
//        }

    }

    function findMapBykey(deviceName)
    {

    }


    function onUpdatePreview(productname)
    {
        updatePreviewImage(IotData.iotPrintIDMap[productname],productname)
    }


    function updateDeviceStatus()
    {
        if(curMac != "")
        {
            if(IotData.iotDeviceStatusMap[deviceId] == "offline" && idPrintControlInfoDlg.visible === true)
            {
                idDeviceOfflineDlg.visible = true
            }
        }
    }

    function updateDeviceInformation(productName)
    {

    }


    function insertMyGcodeList(strjson, page)
    {

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
                        visible: true
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
                                    currentIndex: 0
                                    model: ListModel {
                                        id: idPrinterListModel
                                    }

                                    onComboBoxIndexChanged: {

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
                                    text: curPrinterName
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
//                                            curGcodeUrl = idGcodeListModel.get(currentIndex).url
//                                            curGcodeId = idGcodeListModel.get(currentIndex).id

//                                            idDeviceType.text = "  " + idGcodeListModel.get(currentIndex).key

//                                            if(gcodeFileChanged == true)
//                                            {
//                                                idSelectGcodeWarringDlg.visible = true
//                                            }

                                            curSelectGCodePath = idGcodeListModel.get(currentIndex).modelFullData

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
                                    if(curMac != "")
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
                                    if(curMac != "")
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
                                    if(curMac != "")
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
                                    if(curMac != "")
                                    {
                                        fanSwitchIsChanged = true
                                        if(curFanSwitch == 0)
                                        {
                                            //controlPrintTempSpeed(deviceId, IotData.serIotIdMap[deviceId], IotData.serProductkeyMap[deviceId], "fanswitch", 1, deviceTbId);
                                            sendControlPrinterCmd(curMac,PrintControlInfoDlg_Local.PrintControlType.CONTROL_CMD_FUN, 1)

                                            curFanSwitch = 1
                                            idUpdateDeviceInfoTimer.restart()
                                        }
                                        else if(curFanSwitch == 1)
                                        {
                                           // controlPrintTempSpeed(deviceId, IotData.serIotIdMap[deviceId], IotData.serProductkeyMap[deviceId], "fanswitch", 0, deviceTbId);

                                            sendControlPrinterCmd(curMac,PrintControlInfoDlg_Local.PrintControlType.CONTROL_CMD_FUN, 0)

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
                                        if(curMac != "")
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
                                        if(curMac != "")
                                        {
                                            moveAxisZAddIsChanged = true
                                            if(idMove01mm.checked == true)
                                            {
                                                moveCurAxisZ = isReverseZAxisOperation(/*IotData.iotPrinterNameMap[deviceId]*/curPrinterName) ? ((parseFloat(moveCurAxisZ) - parseFloat(0.1)).toFixed(2)) : ((parseFloat(moveCurAxisZ) + parseFloat(0.1)).toFixed(2))
                                            }
                                            if(idMove1mm.checked == true)
                                            {
                                                moveCurAxisZ = isReverseZAxisOperation(/*IotData.iotPrinterNameMap[deviceId]*/curPrinterName) ? ((parseFloat(moveCurAxisZ) - parseFloat(1)).toFixed(2)) : ((parseFloat(moveCurAxisZ) + parseFloat(1)).toFixed(2))
                                            }
                                            if(idMove10mm.checked == true)
                                            {
                                                moveCurAxisZ = isReverseZAxisOperation(/*IotData.iotPrinterNameMap[deviceId]*/curPrinterName) ? ((parseFloat(moveCurAxisZ) - parseFloat(10)).toFixed(2)) : ((parseFloat(moveCurAxisZ) + parseFloat(10)).toFixed(2))
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
                                        if(curMac != "")
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
                                        if(curMac != "")
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
                                        if(curMac != "")
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
                                        if(curMac != "")
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
                                        if(curMac != "")
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
                                        if(curMac != "")
                                        {
                                            moveAxisZAddIsChanged = true
                                            if(idMove01mm.checked == true)
                                            {
                                                moveCurAxisZ = isReverseZAxisOperation(/*IotData.iotPrinterNameMap[deviceId]*/curPrinterName) ? ((parseFloat(moveCurAxisZ) + parseFloat(0.1)).toFixed(2)) : ((parseFloat(moveCurAxisZ) - parseFloat(0.1)).toFixed(2))
                                            }
                                            if(idMove1mm.checked == true)
                                            {
                                                moveCurAxisZ = isReverseZAxisOperation(/*IotData.iotPrinterNameMap[deviceId]*/curPrinterName) ? ((parseFloat(moveCurAxisZ) + parseFloat(1)).toFixed(2)) : ((parseFloat(moveCurAxisZ) - parseFloat(1)).toFixed(2))
                                            }
                                            if(idMove10mm.checked == true)
                                            {
                                                moveCurAxisZ = isReverseZAxisOperation(/*IotData.iotPrinterNameMap[deviceId]*/curPrinterName) ? ((parseFloat(moveCurAxisZ) + parseFloat(10)).toFixed(2)) : ((parseFloat(moveCurAxisZ) - parseFloat(10)).toFixed(2))
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
                            // StyledLabel{
                            //     width: 20
                            //     height: parent.height
                            //     text: qsTr("X: ")
                            // }
                            // StyledLabel{
                            //     width: 40
                            //     height: parent.height
                            //     color: "#1E9BE2"
                            //     text: moveCurAxisX
                            // }
                            // StyledLabel{
                            //     width: 10
                            //     height: parent.height
                            //     text: "/"
                            // }
                            // StyledLabel{
                            //     width: 40
                            //     height: parent.height
                            //     text: moveCurRealAxisX
                            // }
                            // Rectangle{
                            //     width: 20
                            //     height: parent.height
                            //     color: "transparent"
                            // }
                            // StyledLabel{
                            //     width: 20
                            //     height: parent.height
                            //     text: qsTr("Y: ")
                            // }
                            // StyledLabel{
                            //     width: 40
                            //     height: parent.height
                            //     color: "#1E9BE2"
                            //     text: moveCurAxisY
                            // }
                            // StyledLabel{
                            //     width: 10
                            //     height: parent.height
                            //     text: "/"
                            // }
                            // StyledLabel{
                            //     width: 40
                            //     height: parent.height
                            //     text: moveCurRealAxisY
                            // }
                            // Rectangle{
                            //     width: 20
                            //     height: parent.height
                            //     color: "transparent"
                            // }
                            // StyledLabel{
                            //     width: 20
                            //     height: parent.height
                            //     text: qsTr("Z: ")
                            // }
                            // StyledLabel{
                            //     width: 40
                            //     height: parent.height
                            //     color: "#1E9BE2"
                            //     text: moveCurAxisZ
                            // }
                            // StyledLabel{
                            //     width: 10
                            //     height: parent.height
                            //     text: "/"
                            // }
                            // StyledLabel{
                            //     width: 40
                            //     height: parent.height
                            //     text: moveCurRealAxisZ
                            // }
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
                        if(curMac != "")
                        {
//                            controlPrintGcode(deviceId, IotData.serIotIdMap[deviceId], IotData.serProductkeyMap[deviceId], "pause",deviceTbId);
                           sendControlPrinterCmd(curMac,PrintControlInfoDlg_Local.PrintControlType.PRINT_PAUSE, "")
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
                        if(curMac != "")
                        {
                           // controlPrintGcode(deviceId, IotData.serIotIdMap[deviceId], IotData.serProductkeyMap[deviceId], "continue",deviceTbId)

                            sendControlPrinterCmd(curMac,PrintControlInfoDlg_Local.PrintControlType.PRINT_CONTINUE, "")
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
            if(curMac != "")
            {
//                startPrintGcode(curGcodeUrl, deviceId, IotData.serIotIdMap[deviceId], IotData.serProductkeyMap[deviceId], curGcodeId, IotData.iotPrinterNameMap[deviceId], printerIdMap[IotData.iotPrinterNameMap[deviceId]], deviceTbId)

                sendControlPrinterCmd(curMac,PrintControlInfoDlg_Local.PrintControlType.PRINT_START, curSelectGCodePath)


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
            if(curMac != "")
            {

//                controlPrintGcode(deviceId, IotData.serIotIdMap[deviceId], IotData.serProductkeyMap[deviceId], "stop",deviceTbId);
                sendControlPrinterCmd(curMac,PrintControlInfoDlg_Local.PrintControlType.PRINT_STOP, "")

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
