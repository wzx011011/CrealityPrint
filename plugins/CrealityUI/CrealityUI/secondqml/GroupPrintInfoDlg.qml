import QtQuick 2.10
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0 as QQC2
import CrealityUI 1.0
import "qrc:/CrealityUI"
import ".."
import "../qml"
BasicDialog{

    property var isUpdateClusterDeviceListArray: true
    property var isUpdateClusterTaskListArray: true
    property var isFirstGetClusterGcodeList: true
    property var groupChildTaskIdArray: {0, 0}

    signal sigUpdateClusterDeviceList()
    signal sigGetClusterGcodeList(int page, int pageSize)
    signal createClusterTask(var ids, var info)
    signal sigClusterConformPrinterStatus(var value, var status)
    signal sigClusterControlChildTaskStatus(var dnName, var operation)
    signal uploadLocalGcodeFile()
    signal resetGcodeSliceType(var gcodeId, var sliceId)

    function setClusterGcodeSliteTypeList(strjson)
    {
        idGroupPrintTaskCreateInfo.setGcodeItemSliceTypeList(strjson)
    }

    function addClusterPrintTaskRet(ret)
    {
        if(idTaskCreateBtn.btnSelected === true)
        {
            if(ret === true)
            {
                idGroupPrintTaskCreateInfo.setAddTaskRetDlgShow(true, "success")
            }
            else
            {
                idGroupPrintTaskCreateInfo.setAddTaskRetDlgShow(true, "failed")
            }
        }
        else
        {
            idGroupPrintTaskCreateInfo.setAddTaskRetDlgShow(false, "failed")
        }
    }

    function setTaskPrintReportData(strjson)
    {
        var dataRet = JSON.parse(strjson)
        if(dataRet.code === 0){
            var objResult = dataRet.result

            //
            idTaskReportDlg.printDevice = (objResult.printName == null) ? "" : objResult.printName
            idTaskReportDlg.printFile = (objResult.gcodeName == null) ? "" : objResult.gcodeName
            idTaskReportDlg.printTime = (objResult.printTime == null) ? "" : objResult.printTime

            idTaskReportDlg.visible = true
        }
    }

    function setClusterTaskListData(strjson)
    {
        var dataRet = JSON.parse(strjson)
        if(dataRet.code === 0){
            var objResult = dataRet.result
            if(isUpdateClusterTaskListArray)
            {
                IotData.groupTaskDeviceModelTypeArray = []
                for(var key in objResult)
                {
                    IotData.groupTaskDeviceModelTypeArray.push(objResult[key].deviceModel)
                }
            }
            for(var key in objResult)
            {
                IotData.groupTaskSeriesIdMap[objResult[key].deviceModel] = objResult[key].seriesId
                IotData.groupTaskSeriesNameMap[objResult[key].deviceModel] = objResult[key].seriesName
                IotData.groupTaskDeviceNumMap[objResult[key].deviceModel] = objResult[key].deviceNum
                IotData.groupTaskPrintingCntMap[objResult[key].deviceModel] = objResult[key].printingCnt
                IotData.groupTaskWaitingCntMap[objResult[key].deviceModel] = objResult[key].waitingCnt
                IotData.groupTaskFinishCntMap[objResult[key].deviceModel] = objResult[key].finishCnt
                IotData.groupVideoMap[objResult[key].deviceModel] = objResult[key].video
                var objChildResult = objResult[key].childTasks
                groupChildTaskIdArray = []
                for(var Childkey in objChildResult)
                {
                    if(objChildResult[Childkey].id == null)
                    {
                        objChildResult[Childkey].id = objChildResult[Childkey].deviceName//
                    } 
                    groupChildTaskIdArray.push(objChildResult[Childkey].id)
                    IotData.groupChildSeriedIdMap[objChildResult[Childkey].id] = objChildResult[Childkey].seriesId
                    IotData.groupChildPrintIdMap[objChildResult[Childkey].id] = (objChildResult[Childkey].printId == null) ? "" : objChildResult[Childkey].printId
                    IotData.groupChildTaskPrintNameMap[objChildResult[Childkey].id] = (objChildResult[Childkey].printName == null) ? "" : objChildResult[Childkey].printName
                    IotData.groupChildTaskGcodeIdMap[objChildResult[Childkey].id] = (objChildResult[Childkey].gcodeId == null) ? "" : objChildResult[Childkey].gcodeId
                    IotData.groupChildTaskGcodeNameMap[objChildResult[Childkey].id] = (objChildResult[Childkey].gcodeName == null) ? "" : objChildResult[Childkey].gcodeName
                    IotData.groupChildTaskPrintPictureMap[objChildResult[Childkey].id] = objChildResult[Childkey].printPicture
                    IotData.groupChildTaskStatusMap[objChildResult[Childkey].id] = objChildResult[Childkey].status
                    IotData.groupChildTaskPrintProgressMap[objChildResult[Childkey].id] = (objChildResult[Childkey].printProgress == null) ? 0 : objChildResult[Childkey].printProgress
                    IotData.groupChildTaskPrintTimeMap[objChildResult[Childkey].id] = (objChildResult[Childkey].printTime == null) ? 0 : objChildResult[Childkey].printTime
                    IotData.groupChildTaskPrintLeftTimeMap[objChildResult[Childkey].id] = (objChildResult[Childkey].printLeftTime == null) ? 0 : objChildResult[Childkey].printLeftTime
                    IotData.groupChildDeviceNameMap[objChildResult[Childkey].id] = (objChildResult[Childkey].deviceName == null) ? "" : objChildResult[Childkey].deviceName
                    IotData.groupChildTaskTbIdMap[objChildResult[Childkey].id] = objChildResult[Childkey].tbId
                    IotData.groupChildTaskVideoMap[objChildResult[Childkey].id] = objChildResult[Childkey].video
                    IotData.groupChildTaskSyDidMap[objChildResult[Childkey].id] = objChildResult[Childkey].syDid
                    IotData.groupChildTaskSyInitMap[objChildResult[Childkey].id] = objChildResult[Childkey].syInit
                    IotData.groupChildTaskSyLicenseMap[objChildResult[Childkey].id] = objChildResult[Childkey].syLicense
                }
                IotData.groupDeviceModelTypeTaskMap[objResult[key].deviceModel] = groupChildTaskIdArray
            }
        }

        //create panel obj
        if(isUpdateClusterTaskListArray)
        {
            idGroupPrintTaskListInfo.createTaskFoldingPanelComponent()
        }

        isUpdateClusterTaskListArray = false

        //update child task id
        idGroupPrintTaskListInfo.updateChildTaskInfo()
        idGroupPrintTaskListInfo.statisticsDeviceReport()
    }

    function setClusterGcodeListData(strjson, page)
    {
        idGroupPrintTaskCreateInfo.createClusterGcodeComponent(strjson)
    }

    function setClusterDeviceListData(strjson)
    {
        var dataRet = JSON.parse(strjson)
        if(dataRet.code === 0){
            var objResult = dataRet.result
            if(isUpdateClusterDeviceListArray)
            {
                IotData.groupDeviceModelTypeArray = []
                for(var key in objResult)
                {
                    IotData.groupDeviceModelTypeArray.push(objResult[key].deviceModel)
                }
            }

            var tempCnt = 0
            for(var key in objResult)
            {
                tempCnt++
                IotData.groupSeriesIdMap[objResult[key].deviceModel] = (objResult[key].seriesId == null) ? "" : objResult[key].seriesId
                IotData.groupSeriesNameMap[objResult[key].deviceModel] = (objResult[key].seriesName == null) ? "" : objResult[key].seriesName
                IotData.groupDeviceCountMap[objResult[key].deviceModel] = objResult[key].deviceCount
                IotData.groupPrintingCntMap[objResult[key].deviceModel] = objResult[key].printingCnt
            }

            //when device cnt change, update 
            if(tempCnt != IotData.groupDeviceModelTypeArray.length)
            {
                idGroupPrintTaskCreateInfo.updateClusterDeviceList()
                return 
            }
        }

        //create obj
        if(isUpdateClusterDeviceListArray)
        {
            idGroupPrintTaskCreateInfo.createClusterDeviceComponent()
        }

        isUpdateClusterDeviceListArray = false
    }

    function onLeftButtonClick(type)
    {
        if(type === "taskcreate")
        {
            idTaskCreateBtn.btnSelected = true
            idTaskListBtn.btnSelected = false
            idTaskPrintingBtn.btnSelected = false
            idTaskWaitingBtn.btnSelected = false
            idTaskCompleteBtn.btnSelected = false
            idPrintMonitoringBtn.btnSelected = false
            idTeamBtn.btnSelected = false
            idPrintReportBtn.btnSelected = false

            idTaskCreateRtl.visible = true
            idTaskPrintingRtl.visible = false
            idTaskWaitingRtl.visible = false
            idTaskCompleteRtl.visible = false
            idPrintMonitoringRtl.visible = false
            idTeamRtl.visible = false
            idPrintReportRtl.visible = false

            if(isFirstGetClusterGcodeList)
            {
                sigGetClusterGcodeList(1, 100)
                isFirstGetClusterGcodeList = false
            }
            
        }
        else if(type === "tasklist")
        {
            idTaskCreateBtn.btnSelected = false
            idTaskListBtn.btnSelected = true
            idTaskPrintingBtn.btnSelected = true
            idTaskWaitingBtn.btnSelected = false
            idTaskCompleteBtn.btnSelected = false
            idPrintMonitoringBtn.btnSelected = false
            idTeamBtn.btnSelected = false
            idPrintReportBtn.btnSelected = false

            idTaskCreateRtl.visible = false
            idTaskPrintingRtl.visible = true
            idTaskWaitingRtl.visible = false
            idTaskCompleteRtl.visible = false
            idPrintMonitoringRtl.visible = false
            idTeamRtl.visible = false
            idPrintReportRtl.visible = false

            IotData.taskListBtnStatus = "printing"
        }
        else if(type === "taskprinting")
        {
            idTaskCreateBtn.btnSelected = false
            idTaskListBtn.btnSelected = true
            idTaskPrintingBtn.btnSelected = true
            idTaskWaitingBtn.btnSelected = false
            idTaskCompleteBtn.btnSelected = false
            idPrintMonitoringBtn.btnSelected = false
            idTeamBtn.btnSelected = false
            idPrintReportBtn.btnSelected = false

            idTaskCreateRtl.visible = false
            idTaskPrintingRtl.visible = true
            idTaskWaitingRtl.visible = false
            idTaskCompleteRtl.visible = false
            idPrintMonitoringRtl.visible = false
            idTeamRtl.visible = false
            idPrintReportRtl.visible = false

            IotData.taskListBtnStatus = "printing"
        }
        else if(type === "taskwaiting")
        {
            idTaskCreateBtn.btnSelected = false
            idTaskListBtn.btnSelected = true
            idTaskPrintingBtn.btnSelected = false
            idTaskWaitingBtn.btnSelected = true
            idTaskCompleteBtn.btnSelected = false
            idPrintMonitoringBtn.btnSelected = false
            idTeamBtn.btnSelected = false
            idPrintReportBtn.btnSelected = false

            idTaskCreateRtl.visible = false
            idTaskPrintingRtl.visible = true
            idTaskWaitingRtl.visible = false
            idTaskCompleteRtl.visible = false
            idPrintMonitoringRtl.visible = false
            idTeamRtl.visible = false
            idPrintReportRtl.visible = false

            IotData.taskListBtnStatus = "waiting"
        }
        else if(type === "taskallocating")
        {
            idTaskCreateBtn.btnSelected = false
            idTaskListBtn.btnSelected = true
            idTaskPrintingBtn.btnSelected = false
            idTaskWaitingBtn.btnSelected = false
            idTaskCompleteBtn.btnSelected = true
            idPrintMonitoringBtn.btnSelected = false
            idTeamBtn.btnSelected = false
            idPrintReportBtn.btnSelected = false

            idTaskCreateRtl.visible = false
            idTaskPrintingRtl.visible = true
            idTaskWaitingRtl.visible = false
            idTaskCompleteRtl.visible = false
            idPrintMonitoringRtl.visible = false
            idTeamRtl.visible = false
            idPrintReportRtl.visible = false

            IotData.taskListBtnStatus = "idling"
        }
        else if(type === "printmonitoring")
        {
            idTaskCreateBtn.btnSelected = false
            idTaskListBtn.btnSelected = false
            idTaskPrintingBtn.btnSelected = false
            idTaskWaitingBtn.btnSelected = false
            idTaskCompleteBtn.btnSelected = false
            idPrintMonitoringBtn.btnSelected = true
            idTeamBtn.btnSelected = false
            idPrintReportBtn.btnSelected = false

            idTaskCreateRtl.visible = false
            idTaskPrintingRtl.visible = false
            idTaskWaitingRtl.visible = false
            idTaskCompleteRtl.visible = false
            idPrintMonitoringRtl.visible = true
            idTeamRtl.visible = false
            idPrintReportRtl.visible = false
        }
        else if(type === "team")
        {
            idTaskCreateBtn.btnSelected = false
            idTaskListBtn.btnSelected = false
            idTaskPrintingBtn.btnSelected = false
            idTaskWaitingBtn.btnSelected = false
            idTaskCompleteBtn.btnSelected = false
            idPrintMonitoringBtn.btnSelected = false
            idTeamBtn.btnSelected = true
            idPrintReportBtn.btnSelected = false

            idTaskCreateRtl.visible = false
            idTaskPrintingRtl.visible = false
            idTaskWaitingRtl.visible = false
            idTaskCompleteRtl.visible = false
            idPrintMonitoringRtl.visible = false
            idTeamRtl.visible = true
            idPrintReportRtl.visible = false
        }
        else if(type === "printreport")
        {
            idTaskCreateBtn.btnSelected = false
            idTaskListBtn.btnSelected = false
            idTaskPrintingBtn.btnSelected = false
            idTaskWaitingBtn.btnSelected = false
            idTaskCompleteBtn.btnSelected = false
            idPrintMonitoringBtn.btnSelected = false
            idTeamBtn.btnSelected = false
            idPrintReportBtn.btnSelected = true

            idTaskCreateRtl.visible = false
            idTaskPrintingRtl.visible = false
            idTaskWaitingRtl.visible = false
            idTaskCompleteRtl.visible = false
            idPrintMonitoringRtl.visible = false
            idTeamRtl.visible = false
            idPrintReportRtl.visible = true
        }
    }

    function updateDeviceInformation(deviceID)
    {
        idvideoPanel.updateDeviceInformation(deviceID)
    }

    function addVideoItem(deviceID)
    {
        idvideoPanel.addVideoItem(deviceID)
    }

    id: idGroupPrintInfoDlg
    width: 1200 + 10
    height: 742 + 10
    titleHeight : 30
    title: qsTr("Cluster Printing")

    Row{
        id: idGroupPrintInfoContent
        anchors.top: parent.top
        anchors.topMargin: 30+5+20
        anchors.left: parent.left
        anchors.leftMargin: 5+21
        width: parent.width-10 - 21
        height: parent.height-titleHeight-10 - 20 - 21
        spacing: 20

        Column{
            width: 140
            height: parent.height
            spacing: 10
            BasicButton{
                id: idTaskCreateBtn
                width: 140
                height: 36
                btnRadius: 3
                btnBorderW: 0
                pixSize: 12
                btnSelected: false
                defaultBtnBgColor:Constants.dialogContentBgColor
                hoveredBtnBgColor:Constants.typeBtnHoveredColor
                selectedBtnBgColor: "#1E9BE2"
                text: qsTr("Task Create")
                onSigButtonClicked:
                {
                    onLeftButtonClick("taskcreate")
                }
            }
            Column
            {
                spacing: 1
                BasicButton{
                    id: idTaskListBtn
                    width: 140
                    height: 36
                    btnRadius: 3
                    btnBorderW: 0
                    pixSize: 12
                    btnSelected: false
                    defaultBtnBgColor:Constants.dialogContentBgColor
                    hoveredBtnBgColor:Constants.typeBtnHoveredColor
                    selectedBtnBgColor: "#1E9BE2"
                    text: qsTr("Task List")
                    onSigButtonClicked:
                    {
                        onLeftButtonClick("tasklist")
                    }
                }
                BasicButton{
                    anchors{
                        horizontalCenter: parent.horizontalCenter
                        //verticalCenter: parent.verticalCenter
                    }
                    id: idTaskPrintingBtn
                    width: 120
                    height: 26
                    btnRadius: 3
                    btnBorderW: 0
                    pixSize: 12
                    visible: idTaskListBtn.btnSelected
                    btnSelected: false
                    defaultBtnBgColor:Constants.dialogContentBgColor
                    selectedBtnBgColor: Constants.secondBtnSelectionColor
                    hoveredBtnBgColor: Constants.secondBtnHoverdColor
                    text:{
                        if(IotData.clusterTaskPrintingCnt === 0)
                        {
                            return qsTr("Printing")
                        }
                        else{
                            return qsTr("Printing") + " ( " + IotData.clusterTaskPrintingCnt + " )"
                        }
                    } 
                    onSigButtonClicked:
                    {
                        onLeftButtonClick("taskprinting")
                    }
                }
                BasicButton{
                    anchors{
                        horizontalCenter: parent.horizontalCenter
                        //verticalCenter: parent.verticalCenter
                    }
                    id: idTaskWaitingBtn
                    width: 120
                    height: 26
                    btnRadius: 3
                    btnBorderW: 0
                    pixSize: 12
                    visible: idTaskListBtn.btnSelected
                    btnSelected: false
                    defaultBtnBgColor:Constants.dialogContentBgColor
                    selectedBtnBgColor: Constants.secondBtnSelectionColor
                    hoveredBtnBgColor: Constants.secondBtnHoverdColor
                    text: {
                        if(IotData.clusterTaskWaitingCnt === 0){
                            return qsTr("Waiting") 
                        }
                        else{
                            return qsTr("Waiting") + " ( " + IotData.clusterTaskWaitingCnt + " )"
                        }
                    }
                    onSigButtonClicked:
                    {
                        onLeftButtonClick("taskwaiting")
                    }
                }
                BasicButton{
                    anchors{
                        horizontalCenter: parent.horizontalCenter
                        //verticalCenter: parent.verticalCenter
                    }
                    id: idTaskCompleteBtn
                    width: 120
                    height: 26
                    btnRadius: 3
                    btnBorderW: 0
                    pixSize: 12
                    visible: idTaskListBtn.btnSelected
                    btnSelected: false
                    defaultBtnBgColor:Constants.dialogContentBgColor
                    selectedBtnBgColor: Constants.secondBtnSelectionColor
                    hoveredBtnBgColor: Constants.secondBtnHoverdColor
                    text: {
                        if((IotData.clusterTaskAllocatingCnt) === 0)
                        {
                            return qsTr("Allocating") 
                        }
                        else{
                            return qsTr("Allocating") + " ( " + (IotData.clusterTaskAllocatingCnt) + " )"
                        }
                    }
                    onSigButtonClicked:
                    {
                        onLeftButtonClick("taskallocating")
                    }
                }
            }
            
            BasicButton{
                id: idPrintMonitoringBtn
                width: 140
                height: 36
                btnRadius: 3
                btnBorderW: 0
                pixSize: 12
                btnSelected: false
                defaultBtnBgColor:Constants.dialogContentBgColor
                hoveredBtnBgColor:Constants.typeBtnHoveredColor
                selectedBtnBgColor: "#1E9BE2"
                text: qsTr("Print Monitoring")
                onSigButtonClicked:
                {
                    onLeftButtonClick("printmonitoring")
                }
            }
            BasicButton{
                id: idTeamBtn
                width: 140
                height: 36
                btnRadius: 3
                btnBorderW: 0
                pixSize: 12
                btnSelected: false
                defaultBtnBgColor:Constants.dialogContentBgColor
                hoveredBtnBgColor:Constants.typeBtnHoveredColor
                visible: false
                selectedBtnBgColor: "#1E9BE2"
                text: qsTr("Team")
                onSigButtonClicked:
                {
                    onLeftButtonClick("team")
                }
            }
            BasicButton{
                id: idPrintReportBtn
                width: 140
                height: 36
                btnRadius: 3
                btnBorderW: 0
                pixSize: 12
                btnSelected: false
                visible: false
                defaultBtnBgColor:Constants.dialogContentBgColor
                hoveredBtnBgColor:Constants.typeBtnHoveredColor
                selectedBtnBgColor: "#1E9BE2"
                text: qsTr("Print Report")
                onSigButtonClicked:
                {
                    onLeftButtonClick("printreport")
                }
            }
        }
        Rectangle{
            width: 1
            height: parent.height
            border.color: Constants.splitLineColor
            border.width: 1
            color: "transparent"
        }
        Rectangle{
            width: parent.width - 140 - 40 - 22
            height: parent.height
            color: "#FFFFFF"

            Rectangle{
                id: idTaskCreateRtl
                width: parent.width
                height: parent.height
                visible: false
                GroupPrintTaskCreateInfo{
                    id: idGroupPrintTaskCreateInfo
                    width: parent.width
                    height: parent.height

                    onUpdateClusterDeviceList:
                    {
                        sigUpdateClusterDeviceList()
                        isUpdateClusterDeviceListArray = true
                        isUpdateClusterTaskListArray = true
                    }
                    onUpdateClusterGcodeList:
                    {
                        sigGetClusterGcodeList(page, size)
                    }
                    onSigCreateClusterTask:
                    {
                        createClusterTask(seriesIds, gcodeInfo)
                    }
                    onSigUploadLocalGcodeFile:
                    {
                        uploadLocalGcodeFile()
                    }
                    onSigResetGcodeSliceType:
                    {
                        resetGcodeSliceType(gcodeId, sliceId)
                    }
                }
            }
            Rectangle{
                id: idTaskPrintingRtl
                width: parent.width
                height: parent.height
                visible: false
                GroupPrintTaskListInfo{
                    id: idGroupPrintTaskListInfo
                    width: parent.width
                    height: parent.height

                    onSigTaskItemConformStatus:
                    {
                        sigClusterConformPrinterStatus(data, status)
                    }
                    onSigTaskItemControlChildTaskStatus:
                    {
                        sigClusterControlChildTaskStatus(dnName, operation)
                    }
                    onSigReportOperation:
                    {
                        if(operation === "addmoretask")
                        {
                            onLeftButtonClick("taskcreate")
                        }
                        else if(operation === "addmorefile")
                        {
                            //upload gcode file
                            uploadLocalGcodeFile()
                        }
                    }
                    onSigFoldPanelVidelBtnClick:
                    {
                        onLeftButtonClick("printmonitoring")
                    }
                }
            }
            Rectangle{
                id: idTaskWaitingRtl
                width: parent.width
                height: parent.height
                visible: false
                StyledLabel{
                    text: "task waiting"
                }
            }
            Rectangle{
                id: idTaskCompleteRtl
                width: parent.width
                height: parent.height
                visible: false
                StyledLabel{
                    text: "task complete"
                }
            }
            Rectangle{
                id: idPrintMonitoringRtl
                width: parent.width
                height: parent.height
                visible: false
                VideoGroupPanel
                {
                    id: idvideoPanel
                    width: parent.width
                    height: parent.height
                }
            }
            Rectangle{
                id: idTeamRtl
                width: parent.width
                height: parent.height
                visible: false
                StyledLabel{
                    text: "print team"
                }
            }
            Rectangle{
                id: idPrintReportRtl
                width: parent.width
                height: parent.height
                visible: false
                StyledLabel{
                    text: "print report"
                }
            }
        }
    }

    BasicDialog{
        id: idTaskReportDlg
        property var printDevice: "Ender-6"
        property var printFile: "Ender-3_S1_Tower.gcode"
        property var printTime: 2336
        visible: false
        
        width: 500 + 10
        height: 211 + 10
        titleHeight : 30
        title: qsTr("Print Report")

        function getTimeShowFromTime(tmp)
        {
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

            return timeStr
        }

        Column{
            anchors.top: parent.top
            anchors.topMargin: 30+5+30
            anchors.left: parent.left
            anchors.leftMargin: 5+21
            width: parent.width-10 - 21
            height: parent.height-titleHeight-10 - 30 - 31
            spacing: 30

            Column{
                width: parent.width
                height: parent.height - 28 - 30
                Row{
                    width: parent.width
                    height: parent.height/3
                    StyledLabel{
                        width: 120
                        height: parent.height
                        font.pixelSize:14
                        text: qsTr("Print Device") + ": "
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment:Text.AlignLeft
                    }
                    StyledLabel{
                        width: parent.width - 120 - 30
                        height: parent.height
                        font.pixelSize:14
                        text: idTaskReportDlg.printDevice
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment:Text.AlignLeft
                    }
                }
                Row{
                    width: parent.width
                    height: parent.height/3
                    StyledLabel{
                        width: 120
                        height: parent.height
                        font.pixelSize:14
                        text: qsTr("Print File") + ": "
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment:Text.AlignLeft
                    }
                    StyledLabel{
                        width: parent.width - 120 - 30
                        height: parent.height
                        font.pixelSize:14
                        text: idTaskReportDlg.printFile
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment:Text.AlignLeft
                    }
                }
                Row{
                    width: parent.width
                    height: parent.height/3
                    StyledLabel{
                        width: 120
                        height: parent.height
                        font.pixelSize:14
                        text: qsTr("Print Time") + ": "
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment:Text.AlignLeft
                    }
                    StyledLabel{
                        width: parent.width - 120 - 30
                        height: parent.height
                        font.pixelSize:14
                        text: idTaskReportDlg.getTimeShowFromTime(idTaskReportDlg.printTime)
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment:Text.AlignLeft
                    }
                }
            }

            BasicButton{
                x: 150
                width: 125
                height: 28
                btnRadius: 3
                btnBorderW: 0
                pixSize: 12
                text: qsTr("Close")
                onSigButtonClicked:
                {
                    idTaskReportDlg.visible = false
                }
            }
        }
    }
}
