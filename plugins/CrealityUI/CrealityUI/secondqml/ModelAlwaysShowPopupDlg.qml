import QtQuick 2.5
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.0

import ".."
import "../qml"

Popup
{
    id: idModelAlwaysDlg
    width: 298*screenScaleFactor
    height: 277*screenScaleFactor
    visible: false
    topInset: 10*screenScaleFactor
    bottomInset: -10*screenScaleFactor
    padding: 0
    //    background: BorderImage {
    //        source: Constants.modelAlwaysPopBg
    //    }
    //  closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    property var modelGroupMap: {0:0}
    signal sigModelAlwaysShowItemClick(var isTableShow, var modelGroupId, var btnNameText, var modelCount, var btnAuthorText, var btnAvtarImage, var modelTime)

    function setAlwaysShowItemData(strjson)
    {
        var componentButton = Qt.createComponent("../secondqml/ModelAlwaysShowItem.qml")
        if (componentButton.status === Component.Ready )
        {
            var objectArray = JSON.parse(strjson);
            if(objectArray.code === 0)
            {
                deleteModelGroupComponent()

                var objResult = objectArray.result.list;
                for( var key in objResult){
                    if(objResult[key].model == null)
                    {
                        continue
                    }
                    var objCovers = objResult[key].covers
                    var obj = componentButton.createObject(idModelLibraryList, {"btnNameText": objResult[key].model.groupName,
                                                               "btnModelImage": objResult[key].model.covers[0].url,
                                                               "modelGroupId": objResult[key].model.id,
                                                               "btnAuthorText": objResult[key].model.userInfo.nickName,
                                                               "btnAvtarImage": objResult[key].model.userInfo.avatar,
                                                               "modelCount": objResult[key].model.modelCount,
                                                               "modelTime": objResult[key].model.createTime})
                    modelGroupMap[objResult[key].model.id] = obj
                    obj.sigButtonClicked.connect(onSigButtonClicked)
                }
                {
                    var obj1 = componentButton.createObject(idModelLibraryList, {"isTableShow": true,
                                                                "modelGroupId": "-1"})
                    modelGroupMap["-1"] = obj1
                    obj1.sigButtonClicked.connect(onSigButtonClicked)
                }

            }
            else{}
        }
    }

    function onSigButtonClicked(moreBtn, id, name, count, author, avtar, ctime)
    {
        sigModelAlwaysShowItemClick(moreBtn, id, name, count, author, avtar, ctime)
    }

    function deleteModelGroupComponent()
    {
        for(var key in modelGroupMap)
        {
            var strkey = "-%1-".arg(key)
            if(strkey != "-0-")
            {
                modelGroupMap[key].destroy()
                delete modelGroupMap[key]
            }else{
                delete modelGroupMap[key]
            }
        }
    }
    function show(x, y)
    {
        idModelAlwaysDlg.x = x
        idModelAlwaysDlg.y = y
        idModelAlwaysDlg.visible = true
    }

    function hide()
    {
        idModelAlwaysDlg.visible = false
    }

    background: Rectangle{
        color: Constants.lpw_bgColor
    }

    CusPopViewTitle {
        id:idTitle
        width: parent.width - 40*screenScaleFactor
        height: 24*screenScaleFactor
        anchors.horizontalCenter: parent.horizontalCenter
        color: Constants.lpw_titleColor
        fontColor: Constants.themeTextColor
        borderColor : "transparent"
        closeBtnNColor:  "transparent"//color
        closeBtnWidth: 8*screenScaleFactor
        closeBtnHeight: 8*screenScaleFactor
        anchors.top: parent.top
        anchors.right: parent.right
        radius: 5
        leftTop: true
        rightTop: true
        clickedable: false
        maxBtnVis: false
        onCloseClicked:{
            idModelAlwaysDlg.visible = false
        }
    }

        Rectangle{
            anchors.top: idTitle.bottom
            anchors.left: idTitle.left
            width: parent.width
            height: parent.height-28*screenScaleFactor
            color: "transparent"
            Grid{
                x: 20*screenScaleFactor
                y: 20*screenScaleFactor
                id: idModelLibraryList
                width: parent.width - 40*screenScaleFactor
                height: parent.height - 40*screenScaleFactor
                spacing: 10
                columns: 3
                rows: 3
                // ModelAlwaysShowItem{}
                // ModelAlwaysShowItem{
                //     isTableShow: true
                // }
            }
        }
}
