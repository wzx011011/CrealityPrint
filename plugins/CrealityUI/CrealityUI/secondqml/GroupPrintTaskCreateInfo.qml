import QtQuick 2.10
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0 as QQC2
import CrealityUI 1.0
import "qrc:/CrealityUI"
import ".."
import "../qml"

Rectangle{
    property var clusterDeviceMap: {0: 0}
    property var clusterGcodeMap: {0: 0}
    property var addTaskSeriesIds: ""
    property var addTaskGcodeInfo: ""

    signal updateClusterDeviceList()
    signal updateClusterGcodeList(int page, int size)
    signal sigCreateClusterTask(var seriesIds, var gcodeInfo)
    signal sigUploadLocalGcodeFile()
    signal sigResetGcodeSliceType(var gcodeId, var sliceId)

    Timer{
        id: idUpdateDeviceBtnTimer
        interval: 10000
        repeat: false
        onTriggered:{
            idUpdateDeviceBtn.enabled = true
        }
    }
    Timer{
        id: idUpdateGcodeBtnTimer
        interval: 10000
        repeat: false
        onTriggered:{
            idUpdateGcodeBtn.enabled = true
        }
    }

    function setGcodeItemSliceTypeList(strjson)
    {
        idSliceTypeListModel.clear() 
        var objectArray = JSON.parse(strjson);
        if(objectArray.code === 0)
        {
            var objResult = objectArray.result.list;
            for ( var key in objResult ) {
                if(objResult[key].type === 1)
                {
                    idSliceTypeListModel.append({"key": objResult[key].id, "modelData": objResult[key].name})
                }
            }
        }
        else{}
    }

    function setAddTaskRetDlgShow(ret, str)
    {
        if(ret === true)
        {
            //idWarringCreateBtnClk.visible = false
            if(str === "success")
            {
                idWarringCreateBtnClk.msgText = qsTr("Print task created. You can print it on the Task List directly.")
            }
            else if(str === "failed")
            {
                idWarringCreateBtnClk.msgText = qsTr("Failed to create task.")
            }
            idWarringCreateBtnClk.visible = true 
        }
        else
        {
           // idWarringCreateBtnClk.msgText = qsTr("The print device and file is empty, please select again.")
            idWarringCreateBtnClk.visible = false 
        }

        //clear device and gcode list checked
        for(var se_id in clusterDeviceMap)
        {
            if(clusterDeviceMap[se_id].itemChecked)
            {
                clusterDeviceMap[se_id].itemChecked = false
            }
        }
        for(var gcodeId in clusterGcodeMap)
        {
            if(clusterGcodeMap[gcodeId].itemChecked)
            {
                clusterGcodeMap[gcodeId].itemChecked = false
            }
        }
    }

    function createClusterGcodeComponent(strjson)
    {
        var componentGcode = Qt.createComponent("../secondqml/GroupPrintDeviceItem.qml")
        if(componentGcode.status === Component.Ready)
        {   
            var objectArray = JSON.parse(strjson);
            if(objectArray.code === 0)
            {
                deleteClusterGcodeMap()
                var objResult = objectArray.result.list;
                for ( var key in objResult ) {

                    var modelDataStr = ""
                    var sliceSeriesId = ""
                    if(objResult[key].sliceDevice === undefined)
                    {
                        modelDataStr = "---"
                        sliceSeriesId = "---"
                    }
                    else{
                        modelDataStr = objResult[key].sliceDevice.name
                        sliceSeriesId = objResult[key].sliceDevice.seriesId
                    }
                    
                    var gcodeName = ""
                    if(objResult[key].name === "")
                    {
                        gcodeName = objResult[key].modelInfo.name
                    }
                    else{
                        gcodeName = objResult[key].name
                    }
                    var obj = componentGcode.createObject(idGcodeItemList, {"itemName": gcodeName, 
                                    "isGcodeItem": true,
                                    "sliceType": modelDataStr,
                                    "seriesId": sliceSeriesId,
                                    "gcodeId": objResult[key].id,  
                                    "gcodeLink": objResult[key].downloadLink,
                                    "deviceCount": 1})
                    obj.sigRedefineFileSliceType.connect(redefineFileSliceType)
                    clusterGcodeMap[objResult[key].id] = obj
                }
            }
        }
    }

    function redefineFileSliceType(id, name)
    {
        idRedefineSliceTypedlg.objId = id
        idRedefineSliceTypedlg.gcodeName = name
        idSliceTypeCombox.currentIndex = 0
        idRedefineSliceTypedlg.visible = true
    }

    function deleteClusterGcodeMap()
    {
        for(var key in clusterGcodeMap)
        {
            var strkey = "-%1-".arg(key)
			if(strkey != "-0-")
            {
                clusterGcodeMap[key].destroy()
                delete clusterGcodeMap[key]
            }
            else{
                 delete clusterGcodeMap[key]
            }
        }
    }

    function createClusterDeviceComponent()
    {
        var componentGcode = Qt.createComponent("../secondqml/GroupPrintDeviceItem.qml")
        if(componentGcode.status === Component.Ready)
        {
            deleteClusterDeviceMap()
            for(var i = 0; i < IotData.groupDeviceModelTypeArray.length; i++)
            {
                var obj = componentGcode.createObject(idDeviceItemList, {"itemName": IotData.groupDeviceModelTypeArray[i], 
                                    "allCount": IotData.groupDeviceCountMap[IotData.groupDeviceModelTypeArray[i]], 
                                    "deviceCount": IotData.groupPrintingCntMap[IotData.groupDeviceModelTypeArray[i]], 
                                    "seriesId": IotData.groupDeviceModelTypeArray[i]})
                
                clusterDeviceMap[IotData.groupDeviceModelTypeArray[i]] = obj
            }
        }
    }

    function deleteClusterDeviceMap()
    {
        for(var key in clusterDeviceMap)
        {
            var strkey = "-%1-".arg(key)
			if(strkey != "-0-")
            {
                clusterDeviceMap[key].destroy()
                delete clusterDeviceMap[key]
            }
            else{
                 delete clusterDeviceMap[key]
            }
        }
    }

    Column{
        x: 10
        y: 10
        width: parent.width
        height: parent.height
        spacing: 10
        Row{
            width: parent.width
            height: 592
            //spacing: 59
            Rectangle{
                width: 439+10
                height: parent.height
                border.width: 1
                border.color: "#AFAFAF"

                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 1
                    anchors.left: parent.left
                    anchors.leftMargin: 1
                    width: parent.width - 2
                    height: parent.height - 2
                    
                    Rectangle{
                        id: idDeviceTitle
                        width: parent.width
                        height: 29
                        color: "#D7D7D7"
                        StyledLabel{
                            anchors.left: parent.left
                            anchors.leftMargin:  19
                            width: parent.width - 50
                            height: parent.height  
                            font.pixelSize:18
                            verticalAlignment: Qt.AlignVCenter
                            color: "#333333" 
                            text: qsTr("Device")
                        }
                        CusSkinButton_Image{
                            id: idUpdateDeviceBtn
                            anchors{
                                //horizontalCenter: parent.horizontalCenter
                                verticalCenter: parent.verticalCenter
                            }
                            anchors.right: parent.right
                            anchors.rightMargin:  6
                            width: 20
                            height: 17
                            btnImgDisbaled: "qrc:/UI/photo/group_print_task_create_img_refresh.png"
                            btnImgNormal: "qrc:/UI/photo/group_print_task_create_img_refresh.png"
                            btnImgHovered: "qrc:/UI/photo/group_print_task_create_img_refresh_h.png"
                            btnImgPressed: "qrc:/UI/photo/group_print_task_create_img_refresh_h.png"
                            onClicked:
                            {
                                // update device list
                                updateClusterDeviceList()
                                idUpdateDeviceBtn.enabled = false
                                idUpdateDeviceBtnTimer.start()
                            }
                        }
                    }
                    Rectangle{
                        anchors.top: idDeviceTitle.bottom
                        width: parent.width
                        height: parent.height - 29
                        color: "#ECECEC"

                        Rectangle{
                            width: parent.width
                            height: 10
                            color: "transparent"
                        }
                        Rectangle{
                            anchors.top: parent.top
                            anchors.topMargin: 10
                            anchors.left: parent.left
                            anchors.leftMargin: 20
                            width: parent.width - 40
                            height: parent.height - 10
                            color: "transparent"
                            Column{
                                width: parent.width
                                height: parent.height
                                Row{
                                    width: parent.width
                                    height: 33
                                    Rectangle{
                                        width: 48
                                        height: parent.height
                                        color: "transparent"
                                    }
                                    StyledLabel{
                                        width: 130
                                        height: parent.height
                                        font.pixelSize:14
                                        color: "#333333"
                                        text: qsTr("Device Name")
                                        verticalAlignment: Text.AlignVCenter
                                        horizontalAlignment:Text.AlignHCenter
                                    }
                                    StyledLabel{
                                        width: 130
                                        height: parent.height
                                        font.pixelSize:14
                                        color: "#333333"
                                        text: qsTr("Device Count")
                                        verticalAlignment: Text.AlignVCenter
                                        horizontalAlignment:Text.AlignHCenter
                                    }
                                    StyledLabel{
                                        width: 99//parent.width - 58 - 130 - 130
                                        height: parent.height
                                        font.pixelSize:14
                                        color: "#333333"
                                        text: qsTr("Working")
                                        verticalAlignment: Text.AlignVCenter
                                        horizontalAlignment:Text.AlignHCenter
                                    }
                                }
                                Rectangle{
                                    width: parent.width
                                    height: parent.height - 33 - 20
                                    border.width: 1
                                    border.color: "#9C9C9C"
                                    BasicScrollView{
                                        x: 1
                                        y: 1
                                        width: parent.width - 2
                                        height: parent.height - 2
                                        hpolicy: ScrollBar.AlwaysOff
                                        vpolicy: ScrollBar.AsNeeded
                                        clip : true
                                        Column{
                                            id: idDeviceItemList
                                            width: parent.width
                                            //GroupPrintDeviceItem{}
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            Rectangle{
                width: 59
                height: parent.height
                color: "transparent"
                Image{
                    anchors{
                        horizontalCenter: parent.horizontalCenter
                        verticalCenter: parent.verticalCenter
                    }
                    width: 35
                    height: 35   
                    mipmap: true
                    smooth: true
                    cache: false
                    asynchronous: true
                    fillMode: Image.PreserveAspectFit
                    source: "qrc:/UI/photo/group_print_task_create_img_right_arrow.png"
                }
            }
            Rectangle{
                width: 439+10
                height: parent.height
                color: "#ECECEC"
                border.width: 1
                border.color: "#AFAFAF"

                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 1
                    anchors.left: parent.left
                    anchors.leftMargin: 1
                    width: parent.width - 2
                    height: parent.height - 2
                    
                    Rectangle{
                        id: idGcodeTitle
                        width: parent.width
                        height: 29
                        color: "#D7D7D7"
                        StyledLabel{
                            anchors.left: parent.left
                            anchors.leftMargin:  19
                            width: parent.width - 50
                            height: parent.height  
                            font.pixelSize:18
                            verticalAlignment: Qt.AlignVCenter
                            color: "#333333" 
                            text: qsTr("File")
                        }
                        CusSkinButton_Image{
                            id: idUpdateGcodeBtn
                            anchors{
                                //horizontalCenter: parent.horizontalCenter
                                verticalCenter: parent.verticalCenter
                            }
                            anchors.right: parent.right
                            anchors.rightMargin:  6
                            width: 20
                            height: 17
                            btnImgDisbaled: "qrc:/UI/photo/group_print_task_create_img_refresh.png"
                            btnImgNormal: "qrc:/UI/photo/group_print_task_create_img_refresh.png"
                            btnImgHovered: "qrc:/UI/photo/group_print_task_create_img_refresh_h.png"
                            btnImgPressed: "qrc:/UI/photo/group_print_task_create_img_refresh_h.png"
                            onClicked:
                            {
                                // update Gcode list
                                updateClusterGcodeList(1, 100)
                                idUpdateGcodeBtn.enabled = false
                                idUpdateGcodeBtnTimer.start()
                            }
                        }
                        CusSkinButton_Image{
                            id: idAddGcodeBtn
                            anchors{
                                //horizontalCenter: parent.horizontalCenter
                                verticalCenter: parent.verticalCenter
                            }
                            anchors.right: idUpdateGcodeBtn.right
                            anchors.rightMargin:  30
                            width: 20
                            height: 17
                            btnImgNormal: "qrc:/UI/photo/group_print_task_create_img_add.png"
                            btnImgHovered: "qrc:/UI/photo/group_print_task_create_img_add_h.png"
                            btnImgPressed: "qrc:/UI/photo/group_print_task_create_img_add_h.png"
                            onClicked:
                            {
                                // add Gcode list
                                sigUploadLocalGcodeFile()
                            }
                        }
                    }
                    Rectangle{
                        anchors.top: idGcodeTitle.bottom
                        width: parent.width
                        height: parent.height - 29
                        color: "#ECECEC"

                        Rectangle{
                            width: parent.width
                            height: 10
                            color: "transparent"
                        }
                        Rectangle{
                            anchors.top: parent.top
                            anchors.topMargin: 10
                            anchors.left: parent.left
                            anchors.leftMargin: 20
                            width: parent.width - 40
                            height: parent.height - 10
                            color: "transparent"
                            Column{
                                width: parent.width
                                height: parent.height
                                Row{
                                    width: parent.width
                                    height: 33
                                    Rectangle{
                                        width: 48
                                        height: parent.height
                                        color: "transparent"
                                    }
                                    StyledLabel{
                                        width: 130
                                        height: parent.height
                                        font.pixelSize:14
                                        color: "#333333"
                                        text: qsTr("File Name")
                                        verticalAlignment: Text.AlignVCenter
                                        horizontalAlignment:Text.AlignHCenter
                                    }
                                    StyledLabel{
                                        width: 130
                                        height: parent.height
                                        font.pixelSize:14
                                        color: "#333333"
                                        text: qsTr("Slice Type")
                                        verticalAlignment: Text.AlignVCenter
                                        horizontalAlignment:Text.AlignHCenter
                                    }
                                    StyledLabel{
                                        width: 99//parent.width - 58 - 130 - 130
                                        height: parent.height
                                        font.pixelSize:14
                                        color: "#333333"
                                        text: qsTr("Print Count")
                                        verticalAlignment: Text.AlignVCenter
                                        horizontalAlignment:Text.AlignHCenter
                                    }
                                }
                                Rectangle{
                                    width: parent.width
                                    height: parent.height - 33 - 20
                                    border.width: 1
                                    border.color: "#9C9C9C"
                                    BasicScrollView{
                                        x: 1
                                        y: 1
                                        width: parent.width - 2
                                        height: parent.height - 2
                                        hpolicy: ScrollBar.AlwaysOff
                                        vpolicy: ScrollBar.AsNeeded
                                        clip : true
                                        Column{
                                            id: idGcodeItemList
                                            width: parent.width
                                            //GroupPrintDeviceItem{isGcodeItem : true}
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        Rectangle{
            width: parent.width
            height: 48
            color: "transparent"
            BasicButton{
                anchors{
                    horizontalCenter: parent.horizontalCenter
                    verticalCenter: parent.verticalCenter
                }
                width: 125
                height: 28
                btnRadius: 3
                btnBorderW: 0
                pixSize: 12
                defaultBtnBgColor: "#D7D7D7"
                hoveredBtnBgColor: "#1E9BE2"
                btnTextColor: "#333333"
                text: qsTr("Create")
                onSigButtonClicked:
                {
                    addTaskSeriesIds = ""
                    for(var se_id in clusterDeviceMap)
                    {
                        if(clusterDeviceMap[se_id].itemChecked)
                        {
                            addTaskSeriesIds = addTaskSeriesIds + JSON.stringify(clusterDeviceMap[se_id].seriesId) + ","
                        }
                    }

                    //addTaskGcodeInfo 
                    addTaskGcodeInfo = ""
                    var info = {}
                    for(var gcodeId in clusterGcodeMap)
                    {
                        if(clusterGcodeMap[gcodeId].itemChecked)
                        {
                            info.gcodeId = gcodeId
                            info.seriesId = (clusterGcodeMap[gcodeId].seriesId === "---") ? null : clusterGcodeMap[gcodeId].seriesId
                            info.gcodeName = clusterGcodeMap[gcodeId].itemName
                            info.gcodeUrl = clusterGcodeMap[gcodeId].gcodeLink
                            info.copies = clusterGcodeMap[gcodeId].deviceCount
                            info.deviceModel = clusterGcodeMap[gcodeId].sliceType
                            addTaskGcodeInfo = addTaskGcodeInfo + JSON.stringify(info) + ","//JSON.parse(JSON.stringify(info))
                        }
                    }

                    if(addTaskSeriesIds != "" && addTaskGcodeInfo != "")
                    {
                        addTaskSeriesIds = addTaskSeriesIds.substr(0, addTaskSeriesIds.length - 1); 
                        addTaskSeriesIds = "[" + addTaskSeriesIds + "]"
                        addTaskGcodeInfo = addTaskGcodeInfo.substr(0, addTaskGcodeInfo.length - 1);
                        addTaskGcodeInfo = "[" + addTaskGcodeInfo + "]"

                        idWarringCreateBtnClk.msgText = qsTr("Print job is being created.")
                        idWarringCreateBtnClk.visible = true

                        sigCreateClusterTask(addTaskSeriesIds, addTaskGcodeInfo)
                    }
                    else if(addTaskSeriesIds === "" && addTaskGcodeInfo != "")
                    {
                        idWarringCreateBtnClk.msgText = qsTr("The print device is empty, please select again.")
                        idWarringCreateBtnClk.visible = true
                    }
                    else if(addTaskSeriesIds != "" && addTaskGcodeInfo === "")
                    {
                        idWarringCreateBtnClk.msgText = qsTr("The print file is empty, please select again.")
                        idWarringCreateBtnClk.visible = true 
                    }
                    else{
                        idWarringCreateBtnClk.msgText = qsTr("The print device and file is empty, please select again.")
                        idWarringCreateBtnClk.visible = true 
                    }
                }
            }
        }
    }


    UploadMessageDlg{
        id:idWarringCreateBtnClk
        //msgText: qsTr("Not logged in to Creality Cloud, Unable to upload, Do you want to log in?")
        visible: false
        cancelBtnVisible: false
        onSigOkButtonClicked:
        {
            idWarringCreateBtnClk.close()
        }
    }

    BasicDialog{
        id: idRedefineSliceTypedlg
        visible: false
        width: 500 + 10
        height: 211 + 10
        titleHeight : 30
        title: qsTr("Redefine Slice Type")

        property var objId: ""
        property var gcodeName: ""

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
                spacing: 10
                Row{
                    width: parent.width
                    height: 28

                    StyledLabel{
                        width: 120
                        height: parent.height
                        font.pixelSize:14
                        text: qsTr("File Name")
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment:Text.AlignLeft
                    }
                    StyledLabel{
                        width: parent.width - 120 - 30
                        height: parent.height
                        font.pixelSize:14
                        text: idRedefineSliceTypedlg.gcodeName
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment:Text.AlignLeft
                    }
                }
                Row{
                    width: parent.width
                    height: 28

                    StyledLabel{
                        width: 120
                        height: parent.height
                        font.pixelSize:14
                        text: qsTr("Slice Type")
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment:Text.AlignLeft
                    }
                    BasicCombobox{
                        id:idSliceTypeCombox
                        width: parent.width - 120 - 30
                        height: parent.height
                        font.pixelSize : 12
                        showCount: 3
                        //currentIndex: 0
                        model: ListModel {
                            id: idSliceTypeListModel
                        }
                        onComboBoxIndexChanged: {
                        }
                    }
                }
            }

            BasicButton{
                x: 170
                width: 125
                height: 28
                btnRadius: 3
                btnBorderW: 0
                pixSize: 12
                text: qsTr("OK")
                onSigButtonClicked:
                {
                    //
                    sigResetGcodeSliceType(idRedefineSliceTypedlg.objId, idSliceTypeListModel.get(idSliceTypeCombox.currentIndex).key)
                    idRedefineSliceTypedlg.visible = false
                }
            }
        }
    }
}
