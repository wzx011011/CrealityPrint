import QtQuick 2.10
import QtQuick.Controls 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0 as QQC2
import CrealityUI 1.0
import "qrc:/CrealityUI"
import ".."
import "../qml"
Rectangle
{
    id: idGroupRect
    width:977
    height:742
    color: Constants.dialogContentBgColor//"#535353"
    property var clickcnt:1
    property var groupVideoMap: {0: 0}

    function addVideoItem(deviceid)
    {
        if(findMapBykey(deviceid))
        {
            return
        }
        else
        {
            async_createVideoItem(deviceid)
        }
    }

    function findMapBykey(deviceid)
    {
        for(var key in groupVideoMap)
        {
            if(deviceid == key)
            {
                return true
            }
        }
        return false
    }

    function updateDeviceInformation(deviceid)
    {
        if(findMapBykey(deviceid))
        {
            console.log("VideoGroupPanel  updateDeviceInformation")
            if(IotData.iotPrintStateMap[deviceid] == 1)
            {
                groupVideoMap[deviceid].printState = qsTr("Printing")
                groupVideoMap[deviceid].devicename = IotData.serNickNameMap[deviceid]
                groupVideoMap[deviceid].progressValue =  IotData.iotPrintProgressMap[deviceid]
                groupVideoMap[deviceid].gcodeFile = IotData.taskListGcodeNameByDn[deviceid]
                if(groupVideoMap[deviceid].visible == false)
                {
                     console.log("IotData.videoItemMap[deviceid].playUrl :" + IotData.videoItemMap[deviceid].playUrl)
                    groupVideoMap[deviceid].loadVideoUrl(IotData.videoItemMap[deviceid].playUrl)
                    groupVideoMap[deviceid].visible = true
                }
            }
            else if(IotData.iotPrintStateMap[deviceid] == 5)
            {
                groupVideoMap[deviceid].printState = qsTr("Print Pause")
                groupVideoMap[deviceid].progressValue =  IotData.iotPrintProgressMap[deviceid]

                if(groupVideoMap[deviceid].visible == false)
                {
                    groupVideoMap[deviceid].loadVideoUrl(IotData.videoItemMap[deviceid].playUrl)
                    groupVideoMap[deviceid].visible = true
                }
            }
            else{
                groupVideoMap[deviceid].progressValue =  0

                if(groupVideoMap[deviceid].visible == true)
                {
                    groupVideoMap[deviceid].stopVideo()
                    groupVideoMap[deviceid].visible = false
                }
            }   
        }
        
    }


    function async_createVideoItem(deviceid)
	{
        console.log("VideoGroupPanel  async_createVideoItem")
		var finishCreation = function()
        {
            if (componentVideoItem.status === Component.Ready)
            {
				var obj1 = componentVideoItem.incubateObject(videoItemlList, {
															"visible": false,
                                                            "deviceID": deviceid})
				if (obj1.status !== Component.Ready)
				{
					obj1.onStatusChanged = function(status)
					{
						if (status === Component.Ready)
						{
							groupVideoMap[deviceid] = obj1.object
						}
					}
				}
				else
				{
					groupVideoMap[deviceid] = obj1.object
				}
            }
        }

		var componentVideoItem = Qt.createComponent("../secondqml/VideoGroupItem.qml")

        if (componentVideoItem.status === Component.Ready)
        {
            finishCreation();
        }
        else
        {
            componentVideoItem.statusChanged.connect(finishCreation);
        }
	}

    BasicScrollView
    {
        id: idParamScrollView
        x:15
        y:15
        width: 977
        height: 680
        hpolicy: ScrollBar.AlwaysOff
        vpolicy: ScrollBar.AsNeeded
        clip : true
        Grid
        {
            id: videoItemlList
            columns: 2
            rows: Math.ceil(Object.keys(groupVideoMap).length / 4)
            rowSpacing: 20
            columnSpacing: 15

        }
    }
}
