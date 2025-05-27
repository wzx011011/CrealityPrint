import QtQuick 2.10
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0 as QQC2
import CrealityUI 1.0
import Qt.labs.platform 1.1

import "qrc:/CrealityUI"
import ".."
import "../qml"
BasicDialog{
     property var buttonMap: {0:0}
     property var modelGroupMap: {0:0}
     property var modelSearchMap: {0:0}
     property var imageMap: {0:0}
     property var modelMap: {0:0}
     property var currentModelLibraryPage: ""
     property var nextModelLibraryPage: ""
     property var currentModelSearchPage: 1
     property var nextModelSearchPage: 2
     property var currentBtnType: 0
     property var modelCurrentGroupId: 0
     property var modelCurrentGroupCount: 0
     property var preSearchKey: ""
     property var currentModelId: 0
     property var curModelDetailImage: 0
     property var modelGroupArray:[]
     property var modelGroupSearchArray: []
     property var curModelName: ""

     property var deleteModelGroupComponentStatus: false

     signal getPageModelLibraryList(var typeId, var pageIndex, var pageSize)
     signal importModelLibraryItem(var modelGroupId, var modelCount)
     signal getModelGroupDetailInfo(var groupId, var modelCount)
     signal getSearchModelList(var keyword, var pageIndex, var pageSize)
     signal importModelDetailItem(var id, var name, var link)
     signal getModelGroupInfo(var id)
     signal shareModelGroup(var id)
     signal collectModelStatus(var id, var status)
     signal downLoadModelGroup(var type)
     signal downModelToLocall(var id, var count, var name, var path)

    function openDownFolderDlg(type)
    {
        idFolderDownDlg.receiveType = type
        idFolderDownDlg.open()
    }

    //model alway show to model library
    function justShowRecommendModel()
    { 
        if(idModelLibraryContent.visible == true)
        {
            if(idBtnResultScrollView.visible == true)
            {
                idSearch.text = ""
                idSearchButton.sigButtonClicked()
            }
        }
        if(idModelLibraryDetail.visible == true)
        {
            idModelLibraryDetail.visible = false
            idModelLibraryContent.visible = true

            if(idBtnResultScrollView.visible == true)
            {
                idSearch.text = ""
                idSearchButton.sigButtonClicked()
            }
        }

        if(currentBtnType == "recommendmodellist")
        {

        }
        else{
            onClassTypeButtonClicked("recommendmodellist")
        }
    }
    //end

    function setModelTypeListBtn(strjson)
    {
        var componentButton = Qt.createComponent("../qml/BasicButton.qml")
        if (componentButton.status === Component.Ready )
        {
            var objectArray = JSON.parse(strjson);
            if(objectArray.code === 0){
                deleteButtonComponent()

                //add recommend
                {
                    var recommendObj = componentButton.createObject(idModelTypeListBtn,
                                                                    {"width": 145,
                                                                    "height" : 36,
                                                                    "btnRadius": 3,
                                                                    "btnBorderW": 0,
                                                                    "pointSize": Constants.labelFontPointSize,
															        "defaultBtnBgColor": Constants.dialogContentBgColor,
															        "hoveredBtnBgColor": Constants.typeModelBtnListHoveredColor,
                                                                    "selectedBtnBgColor": "#1E9BE2",
                                                                    "keyStr": "recommendmodellist",
                                                                    "text": qsTr("Recommend")})
                    recommendObj.sigButtonClickedWithKey.connect(onClassTypeButtonClicked)
                    buttonMap["recommendmodellist"] = recommendObj
                }

                var objResult = objectArray.result.list;
                for( var key in objResult)
                {
                    var obj = componentButton.createObject(idModelTypeListBtn, 
                                                                {"width": 145*screenScaleFactor,
                                                                "height" : 36*screenScaleFactor,
                                                                "btnRadius": 3, 
                                                                "btnBorderW": 0,
                                                                "pointSize": Constants.labelFontPointSize,
															    "defaultBtnBgColor": Constants.dialogContentBgColor,
															    "hoveredBtnBgColor": Constants.typeModelBtnListHoveredColor,
                                                                "selectedBtnBgColor": "#1E9BE2",
                                                                "keyStr": objResult[key].id,
                                                                "text": objResult[key].name})
                    obj.sigButtonClickedWithKey.connect(onClassTypeButtonClicked)
                    buttonMap[objResult[key].id] = obj
                }
                if(currentBtnType === 0){
                    //getPageModelLibraryList(objResult[0].id, 1, 16)
                    //currentBtnType = objResult[0].id
                    getPageModelLibraryList("recommendmodellist", 1, 16)
                    currentBtnType = "recommendmodellist"
                }
                buttonMap[currentBtnType].btnSelected = true
            }
            else{}
        }
    }

    function setModelLibraryList(strjson, page)
    {
        var componentButton = Qt.createComponent("../secondqml/ModelLibraryItem.qml")
        if (componentButton.status === Component.Ready )
        {
            var objectArray = JSON.parse(strjson);
            if(objectArray.code === 0)
            {
                if(deleteModelGroupComponentStatus)
                {
                    deleteModelGroupComponent()
                    deleteModelGroupComponentStatus = false
                }

                idModelLibraryList.rows = Math.ceil((page*16)/4)
                var objResult = objectArray.result.list;
                currentModelLibraryPage = page
                nextModelLibraryPage =  objectArray.result.nextCursor
                for( var key in objResult){
                    var objCovers = objResult[key].covers
                    var obj = componentButton.createObject(idModelLibraryList, {"btnNameText": objResult[key].groupName, 
                                                                        "btnModelImage": objResult[key].covers[0].url, 
                                                                        "modelGroupId": objResult[key].id,
                                                                        "btnAuthorText": objResult[key].userInfo.nickName, 
                                                                        "btnAvtarImage": objResult[key].userInfo.avatar,
                                                                        "modelCount": objResult[key].modelCount,
                                                                        "modelTime": objResult[key].createTime})  
                    obj.sigButtonDownClicked.connect(onSigButtonDownClicked) 
                    obj.sigButtonClicked.connect(onSigButtonClicked)
                    obj.crealityModelImage()
                    modelGroupMap[objResult[key].id] = obj
                    modelGroupArray.push(objResult[key].id)
                }
            }
            else{}
            
        }
    }

    function setModelRecommendLibraryList(strjson, page)
    {
        var componentButton = Qt.createComponent("ModelLibraryItem.qml")
        if (componentButton.status === Component.Ready )
        {
            var objectArray = JSON.parse(strjson);
            if(objectArray.code === 0)
            {
                if(deleteModelGroupComponentStatus)
                {
                    deleteModelGroupComponent()
                    deleteModelGroupComponentStatus = false
                }
                idModelLibraryList.rows = Math.ceil(((page/16+1)*16)/4)
                var objResult = objectArray.result.list;
                currentModelLibraryPage = page
                nextModelLibraryPage =  objectArray.result.nextCursor
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
                    obj.sigButtonDownClicked.connect(onSigButtonDownClicked)
                    obj.sigButtonClicked.connect(onSigButtonClicked)
                    obj.crealityModelImage()
                    modelGroupMap[objResult[key].model.id] = obj
                    modelGroupArray.push(objResult[key].model.id)
                }
            }
            else{}

        }
    }

    function setModelSearchList(strjson, page)
    {
        var componentButton = Qt.createComponent("../secondqml/ModelLibraryItem.qml")
        if (componentButton.status === Component.Ready )
        {
            var objectArray = JSON.parse(strjson);
            if(objectArray.code === 0)
            {
                idModelSearchList.rows = Math.ceil((page*16)/4)
                var objResult = objectArray.result.list;
                currentModelSearchPage = page
                if(objResult.length === 16)
                {
                    nextModelSearchPage = currentModelSearchPage + 1
                }
                for( var key in objResult){
                    var objCovers = objResult[key].covers
                    var obj = componentButton.createObject(idModelSearchList, {"btnNameText": objResult[key].groupName, 
                                                                        "btnModelImage": objResult[key].covers[0].url, 
                                                                        "modelGroupId": objResult[key].id,
                                                                        "btnAuthorText": objResult[key].userInfo.nickName, 
                                                                        "btnAvtarImage": objResult[key].userInfo.avatar,
                                                                        "modelCount": objResult[key].modelCount,
                                                                        "modelTime": objResult[key].createTime})  
                    obj.sigButtonDownClicked.connect(onSigButtonDownClicked) 
                    obj.sigButtonClicked.connect(onSigButtonClicked)
                    obj.crealityModelImage()
                    modelSearchMap[objResult[key].id] = obj
                    modelGroupSearchArray.push(objResult[key].id)
                }
            }
            else{}
            
        }
    }

    function setModelGroupInfo(strjson, strBaseLink)
    {
        var objectArray = JSON.parse(strjson);
        if(objectArray.code === 0)
        {
            var objResult = objectArray.result.groupItem
            
            idShareModelPopup.modelId = objResult.id
            idShareLink.text = "  " + strBaseLink + objResult.id

            if(objResult.license == "CC BY")
            {

                idLcenseIconImg.source = "qrc:/UI/photo/license_by.png"
                idLcenseIconImg.visible = true
            }
            else if(objResult.license == "CC BY-SA")
            {

                idLcenseIconImg.source = "qrc:/UI/photo/license_by_sa.png"
                idLcenseIconImg.visible = true
            }
            else if(objResult.license == "CC BY-NC")
            {

                idLcenseIconImg.source = "qrc:/UI/photo/license_by_nc.png"
                idLcenseIconImg.visible = true
            }
            else if(objResult.license == "CC BY-NC-SA")
            {

                idLcenseIconImg.source = "qrc:/UI/photo/license_by_nc_sa.png"
                idLcenseIconImg.visible = true
            }
            else if(objResult.license == "'CC BY-ND")
            {

                idLcenseIconImg.source = "qrc:/UI/photo/license_by_nd.png"
                idLcenseIconImg.visible = true
            }
            else if(objResult.license == "CC BY-NC-ND")
            {

                idLcenseIconImg.source = "qrc:/UI/photo/license_by_nc_nd.png"
                idLcenseIconImg.visible = true
            }
            else if(objResult.license == "CC0")
            {

                idLcenseIconImg.source = "qrc:/UI/photo/license_cc0.png"
                idLcenseIconImg.visible = true
            }
            else{
                idLcenseIconImg.visible = false
            }

            if(objResult.isCollection == true){
                idCollectModelBtn.collectStatus = 1
                idCollectModelBtn.pressedIconSource = "qrc:/UI/photo/model_library_collect.png"
            }
            else{
                idCollectModelBtn.collectStatus = 0
                idCollectModelBtn.pressedIconSource = "qrc:/UI/photo/model_library_uncollect.png"
            }
        }
        else{}
    }

    function setModelDetailInfo(strjson)
    {
        var componentButton = Qt.createComponent("../secondqml/BasicImageButton.qml")
        var componentModelItem = Qt.createComponent("../secondqml/ModelLibraryDetailIem.qml")
        if (componentButton.status === Component.Ready )
        {
            deleteImageMapComponent()
            deleteModelMapComponent()
            var objectArray = JSON.parse(strjson);
            if(objectArray.code === 0)
            {
                var objResult = objectArray.result.list;
                for( var key in objResult){
                    var value = Number(key) +1
                    var obj = componentButton.createObject(idModelDetailListImage, {
                                                                        "visible": false,  
                                                                        "keystr": value,
                                                                        "id": objResult[key].id,
                                                                        "btnImgUrl": objResult[key].coverUrl})  
                    obj.sigBtnClicked.connect(onSigBtnClicked)
                    imageMap[value] = obj

                    var obj1 = componentModelItem.createObject(idModelListItem, {"modelname": objResult[key].fileName, 
                                                                        "modeSize": objResult[key].fileSize, 
                                                                        "modelid": objResult[key].id,  
                                                                        "keystr": value})
                    obj1.sigBtnDetailClicked.connect(onSigBtnDetailItemClicked)
                    obj1.sigBtnDownLoadDetailModel.connect(onSigBtnDownLoadDetailModel)
                    modelMap[objResult[key].id] = obj1
                }
                modelMap[objResult[0].id].btnIsSelected = true
            }
            else{}
            idDetailImage.source = objResult[0].coverUrl 

            curModelDetailImage = 1
            letModelDetailImageListShow(curModelDetailImage, modelCurrentGroupCount)
        }
    }

    function onSigBtnDownLoadDetailModel(itemid, itemName, itemLink)
    {
        currentModelId = itemid
        curModelName = itemName
        if(itemLink == "import")
        {
            importModelDetailItem(currentModelId, itemName, itemLink)
        }
        else if(itemLink == "download")
        {
            downLoadModelGroup("downloadModelItem")
        }
    }

    function startDownloadDetailModel()
    {
        if(modelGroupMap[modelCurrentGroupId] != undefined){
            modelGroupMap[modelCurrentGroupId].setAnimatedImageStatus(true)
        }
        if(modelSearchMap[modelCurrentGroupId] != undefined){
            modelSearchMap[modelCurrentGroupId].setAnimatedImageStatus(true)
        }
        idModelLibraryList.enabled = false
        idModelSearchList.enabled = false
        idModelTypeListBtn.enabled = false
        idModelListItem.enabled = false
        idSearch.enabled = false
        modelMap[currentModelId].setAnimatedImageStatus(true)
        idModelLibraryDetailLoad.enabled = false
        idDetailDownModelLocatalBtn.enabled = false
    }

    function onSigBtnDetailItemClicked(keyValue)
    {
        for(var key in modelMap)
		{
			var strkey = "-%1-".arg(key)
			if(strkey != "-0-")
			{
				modelMap[key].btnIsSelected = false
			}else{}
		}
        modelMap[keyValue].btnIsSelected = true

        curModelDetailImage = modelMap[keyValue].keystr
        letModelDetailImageListShow(curModelDetailImage, modelCurrentGroupCount)
        idDetailImage.source = imageMap[curModelDetailImage].btnImgUrl
    }

    function letModelDetailImageListShow(curValue, count)
    {
        //cur-3 < 1 ==== 1 
        //cur+1 < 5 ==== 5
        var min = (curValue - 3) < 1 ? 1 : (curValue - 3)
        var max = 0
        if((curValue + 1) < 5){
            max = 5
        }else if((curValue+1) > count)
        {
            max = count
            min = count - 4
        }
        else{
            max = curValue+1
        }
        for(var key in imageMap)
        {
            var strKey = "-%1-".arg(key)
            if(strKey != "-0-"){
                if(imageMap[key].btnSelect = true){
                    imageMap[key].btnSelect = false
                }

                if(key >= min && key <= max){
                    imageMap[key].visible = true
                }
                else{
                    imageMap[key].visible = false
                }
            }
        }
        imageMap[curValue].btnSelect = true

        if(curValue === 1)
        {
            idDetailImageLeft.enabled = false
        }else{
            idDetailImageLeft.enabled = true
        }
        if(curValue === count)
        {
            idDetailImageRight.enabled = false
        }else{
            idDetailImageRight.enabled = true
        }
    }

    function onSigBtnClicked(key)
    {
        curModelDetailImage = key
        letModelDetailImageListShow(curModelDetailImage, modelCurrentGroupCount)
        onSigBtnDetailItemClicked(imageMap[curModelDetailImage].id)
        idDetailImage.source = imageMap[key].btnImgUrl
    }

    function onSigButtonClicked(id, name, count, author, avtar, ctime)
    {
        idModelLibraryContent.visible = false
        idModelLibraryDetail.visible = true
        idModelNameLabel.text = name
        var t = new Date(Number(ctime))
        //var y = t.getFullYear();
        var m = t.getMonth()+1;
        var d = t.getDate();
        var h = t.getHours();
        var mm = t.getMinutes();
        m = m < 10 ? ('0' + m) : m
        d = d < 10 ? ('0' + d) : d
        h = h < 10 ? ('0' + h) : h
        mm = mm < 10 ? ('0' + mm) : mm
        idModelUploadTimeLabel.text = qsTr("Uploaded ") + m + "-" + d + " "+ h +":"+ mm
        idAvtarImage.img_src = avtar
        idAuthorName.text = author
        modelCurrentGroupId = id
        modelCurrentGroupCount = count
        getModelGroupInfo(id)
        getModelGroupDetailInfo(id, count)
    }

    function modelLibraryDownLoadSuccess()
    {
        idModelLibraryDlg.visible = false
        idModelLibraryList.enabled = true
        idModelSearchList.enabled = true
        idModelTypeListBtn.enabled = true
        idModelListItem.enabled = true
        idSearch.enabled = true
        //modelGroupMap[modelCurrentGroupId].setAnimatedImageStatus(false)
        if(modelGroupMap[modelCurrentGroupId] != undefined){
            modelGroupMap[modelCurrentGroupId].setAnimatedImageStatus(false)
        }
        if(modelSearchMap[modelCurrentGroupId] != undefined){
            modelSearchMap[modelCurrentGroupId].setAnimatedImageStatus(false)
        }
        idModelLibraryDetailLoad.setAnimatedImageStatus(false)
        idModelLibraryDetailLoad.enabled = true
        idDetailDownModelLocatalBtn.setAnimatedImageStatus(false)
        idDetailDownModelLocatalBtn.enabled = true
        if(modelMap[currentModelId] != undefined)
        {
            if(modelMap[currentModelId] != 0)
            {
                modelMap[currentModelId].setAnimatedImageStatus(false)
            }
        }
    }

    function modelLibraryDownLoadFailed()
    {
        modelLibraryDownLoadSuccess()
        idDownModelFailedDlg.visible = true
    }

    function onSigButtonDownClicked(modelid, count)
    {
        modelCurrentGroupId = modelid
        //modelCurrentGroupCount = count
        importModelLibraryItem(modelid, count)
    }

    function startDownLoadModel(type)
    {
        if(modelGroupMap[modelCurrentGroupId] != undefined){
            modelGroupMap[modelCurrentGroupId].setAnimatedImageStatus(true)
        }
        if(modelSearchMap[modelCurrentGroupId] != undefined){
            modelSearchMap[modelCurrentGroupId].setAnimatedImageStatus(true)
        }
        if(type == "downimport")
        {
            idModelLibraryDetailLoad.setAnimatedImageStatus(true)
            idDetailDownModelLocatalBtn.enabled = false
        }
            
        if(type == "downlocall")
        {
            idDetailDownModelLocatalBtn.setAnimatedImageStatus(true)
            idModelLibraryDetailLoad.enabled = false
        }

        idModelLibraryList.enabled = false
        idModelSearchList.enabled = false
        idModelTypeListBtn.enabled = false
        idModelListItem.enabled = false
        idSearch.enabled = false
    }

    function onClassTypeButtonClicked(keystr)
    {
        for(var key in buttonMap)
		{
			if(key != 0)
			{
				if(key == keystr)
                {
                    buttonMap[key].btnSelected = true
                    if(currentBtnType != keystr)
                    {
                        currentBtnType = keystr
                        //deleteModelGroupComponent()
                        deleteModelGroupComponentStatus = true
                        idModelScrollView.vPosition = 0
                        currentModelLibraryPage = ""
                        
                        getPageModelLibraryList(key, currentModelLibraryPage, 16);
                     
                    }
                    else{}
                }
                else
                {
                    buttonMap[key].btnSelected = false
                }
			}	
		}
    }

    function deleteButtonComponent()
    {
        for(var key in buttonMap)
		{
			var strkey = "-%1-".arg(key)
			if(strkey != "-0-")
			{
				buttonMap[key].destroy()
				delete buttonMap[key]
			}else{
                delete buttonMap[key]
            }
		}
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
        modelGroupArray = []
    }

    function deleteSearchModelComponent()
    {
        for(var key in modelSearchMap)
		{
            var strkey = "-%1-".arg(key)
			if(strkey != "-0-")
			{
				modelSearchMap[key].destroy()
				delete modelSearchMap[key]
			}else{
                delete modelSearchMap[key]
            }
		}
        modelGroupSearchArray = []
    }

     function deleteModelMapComponent()
    {
        for(var key in modelMap)
		{
			var strkey = "-%1-".arg(key)
			if(strkey != "-0-")
			{
				modelMap[key].destroy()
				delete modelMap[key]
			}else{
                delete modelMap[key]
            }
		}
    }

    function deleteImageMapComponent()
    {
        for(var key in imageMap)
		{
			var strkey = "-%1-".arg(key)
			if(strkey != "-0-")
			{
				imageMap[key].destroy()
				delete imageMap[key]
			}else
            {
                delete imageMap[key]
            }
		}
    }

    function controlModelImageShow(value, modelCount, type)
    {
        var showCount = 12
        var min = value - showCount
        if(min < 0)
        {
            min = 0
        }
        var max = value + showCount
        if(max > modelCount)
        {
            max = modelCount
        }
        for(var i = 0; i < modelCount; i++)
        {
            if((i < min) || ( i>max ))
            {
                if(type == "modelgroup")
                {
                    modelGroupMap[modelGroupArray[i]].destroyModelImage()
                }
                else if(type == "modelsearchgroup")
                {
                    modelSearchMap[modelGroupSearchArray[i]].destroyModelImage()
                }
                
            }
            else{
                if(type == "modelgroup")
                {
                    modelGroupMap[modelGroupArray[i]].crealityModelImage()
                }
                else if(type == "modelsearchgroup")
                {
                    modelSearchMap[modelGroupSearchArray[i]].crealityModelImage()
                }
                
            }
        }
    }

    id: idModelLibraryDlg
    width: 1210*screenScaleFactor
    height: 752*screenScaleFactor
    titleHeight : 30*screenScaleFactor
    title: qsTr("Model Library")

    Column{
        id: idModelLibraryContent
        anchors.top: parent.top
        anchors.topMargin: 35 * screenScaleFactor
        anchors.left: parent.left
        anchors.leftMargin: 26 * screenScaleFactor
        width: parent.width-10 - 21
        height: parent.height-titleHeight-10
        Row{
            width: parent.width
            height: 68 * screenScaleFactor
            spacing:10 
            CusSkinButton_Image
            {
                anchors{
                    verticalCenter: parent.verticalCenter
                }
                id: clearSearchBtn
                width: 12 * screenScaleFactor
                height: 18 * screenScaleFactor
                btnImgNormal: "qrc:/UI/photo/model_library_detail_back.png"
                btnImgHovered: "qrc:/UI/photo/model_library_detail_back_h.png"
                btnImgPressed: "qrc:/UI/photo/model_library_detail_back_h.png"
                visible: idSearch.text != ""
                onPressed:
                {
                    idSearch.text = ""
                    idBtnScrollView.visible = true
                    idBtnResultScrollView.visible = false
                    idModelScrollView.visible = true
                    idModelSearchScrollView.visible = false

                }
            }

            BasicLoginTextEdit
            {
                anchors{
                    verticalCenter: parent.verticalCenter
                }
                id: idSearch
                placeholderText: qsTr("Search")
                height : 28 * screenScaleFactor
                width : 300 * screenScaleFactor
                font.pointSize:Constants.labelFontPointSize
                headImageSrc:hovered ? Constants.sourchBtnImg_d : Constants.sourchBtnImg
                tailImageSrc: hovered && idSearch.text != "" ? Constants.clearBtnImg : ""
                hoveredTailImageSrc: Constants.clearBtnImg_d
                radius : 14
                text: ""
                onEditingFinished:
                {
                    console.log("model library search onEditingFinished : " + idSearch.text)
                    idSearchResultBtn.forceActiveFocus();
                    if(idSearch.text != "")
                    {
                        idBtnScrollView.visible = false
                        idBtnResultScrollView.visible = true
                        idModelScrollView.visible = false
                        idModelSearchScrollView.visible = true
                        
                        if(idSearch.text != preSearchKey)
                        {
                            preSearchKey = idSearch.text
                            deleteSearchModelComponent()
                            idModelSearchScrollView.vPosition = 0
                            currentModelSearchPage = 1
                            getSearchModelList(idSearch.text, currentModelSearchPage, 16)
                        }
                    }
                    else{
                        idBtnScrollView.visible = true
                        idBtnResultScrollView.visible = false
                        idModelScrollView.visible = true
                        idModelSearchScrollView.visible = false
                    }
                }
                onTailBtnClicked:
                {
                    idSearch.text = ""
                    idSearchButton.sigButtonClicked()
                }
            }
            BasicButton {
                id: idSearchButton
                anchors{
                    verticalCenter: parent.verticalCenter
                }
                width: 70 * screenScaleFactor
                height: 30 * screenScaleFactor
                btnRadius:13
                btnBorderW:enabled ? 0 : 1
                pointSize: Constants.labelLargeFontPointSize
                enabled: idSearch.text != "" ? true : false
                defaultBtnBgColor: enabled ?  Constants.searchBtnHoveredColor : Constants.searchBtnDisableColor
                hoveredBtnBgColor: Constants.typeBtnHoveredColor
                text: qsTr("Search")
                onSigButtonClicked:
                {
                    console.log("model library search onSigButtonClicked : " + idSearch.text)
                    idSearchResultBtn.forceActiveFocus();
                    if(idSearch.text != "")
                    {
                        idBtnScrollView.visible = false
                        idBtnResultScrollView.visible = true
                        idModelScrollView.visible = false
                        idModelSearchScrollView.visible = true
                        
                        if(idSearch.text != preSearchKey)
                        {
                            preSearchKey = idSearch.text
                            deleteSearchModelComponent()
                            idModelSearchScrollView.vPosition = 0
                            currentModelSearchPage = 1
                            getSearchModelList(idSearch.text, currentModelSearchPage, 16)
                        }
                    }
                    else{
                        idBtnScrollView.visible = true
                        idBtnResultScrollView.visible = false
                        idModelScrollView.visible = true
                        idModelSearchScrollView.visible = false
                    }
                }
            }
        }
        Row{
            width: parent.width
            height: parent.height - 68
            spacing: 5
            
            BasicScrollView
            {
                id: idBtnScrollView
                width: 163 * screenScaleFactor
                height: parent.height - 31
                visible: true
                hpolicy: ScrollBar.AlwaysOff
                vpolicy: ScrollBar.AsNeeded
                clip : true
                Column{
                    id: idModelTypeListBtn
                    spacing: 1
                }
            }
            BasicScrollView
            {
                id: idBtnResultScrollView
                visible: false
                width: 163 * screenScaleFactor
                height: parent.height - 31
                hpolicy: ScrollBar.AlwaysOff
                vpolicy: ScrollBar.AsNeeded
                clip : true
                Column{
                    spacing: 5
                    BasicButton{
                        id: idSearchResultBtn
                        width: 140 * screenScaleFactor
                        height: 36 * screenScaleFactor
                        btnRadius: 3
                        btnBorderW: 0
                        pointSize: Constants.labelFontPointSize
                        hoveredBtnBgColor: "#1E9BE2"
                        defaultBtnBgColor: "#1E9BE2"
                        text: qsTr("Search Result")
                    }
                }
            }
            // Item {
            //     width: 1
            //     height: parent.height - 31
            //     BasicSeparator
            //     {
            //         anchors.fill: parent
            //     }
            // }
            Rectangle{
                width: 1
                height: parent.height - 31
                border.color: Constants.splitLineColor
                border.width: 1
                color: "transparent"
            }
            Rectangle{
                width: 5
                height: parent.height - 31
                color: "transparent"
            }
            BasicScrollView
            {
                id: idModelScrollView
                width: parent.width - 140 - 40 - 3 - 8
                height: parent.height - 41
                visible: true
                hpolicy: ScrollBar.AlwaysOff
                vpolicy: ScrollBar.AsNeeded
                clip : true
                Grid{
                    id: idModelLibraryList
                    width: parent.width
                    height: parent.height
                    spacing: 10
                    columns: 4
                    rows: 2
                }
                onVPositionChanged:
                {
                    if((vSize + vPosition) === 1){
                        if(currentModelLibraryPage != nextModelLibraryPage){
                            if(nextModelLibraryPage != "")
                                getPageModelLibraryList(currentBtnType, nextModelLibraryPage, 16)
                        }
                    }

                    //
                    var count = modelGroupArray.length
                    var value = Math.ceil((vSize + vPosition) * count)
                    controlModelImageShow(value, count, "modelgroup")
                }

                onVSizeChanged:
                {
                    var count = modelGroupArray.length
                    var value = Math.ceil((vSize + vPosition) * count)
                    controlModelImageShow(value, count, "modelgroup")
                }
            }
            BasicScrollView
            {
                id: idModelSearchScrollView
                width: parent.width - 140 - 40 - 3 - 8
                height: parent.height - 41
                visible: false
                hpolicy: ScrollBar.AlwaysOff
                vpolicy: ScrollBar.AsNeeded
                clip : true
                Grid{
                    id: idModelSearchList
                    width: parent.width
                    height: parent.height
                    spacing: 10
                    columns: 4
                    rows: 2
                }
                onVPositionChanged:
                {
                    if((vSize + vPosition) === 1){
                        console.log("getSearchModelList currentModelSearchPage = " + currentModelSearchPage + ", nextModelSearchPage = " +nextModelSearchPage)
                        if(currentModelSearchPage != nextModelSearchPage){
                            getSearchModelList(idSearch.text, nextModelSearchPage, 16)
                        }
                    }

                    var count = modelGroupSearchArray.length
                    var value = Math.ceil((vSize + vPosition) * count)
                    controlModelImageShow(value, count, "modelsearchgroup")
                }

                onVSizeChanged:
                {
                    var count = modelGroupSearchArray.length
                    var value = Math.ceil((vSize + vPosition) * count)
                    controlModelImageShow(value, count, "modelsearchgroup")
                }
            }
        }
    }
    Column{
        id: idModelLibraryDetail
        anchors.top: parent.top
        anchors.topMargin: 30+5
        anchors.left: parent.left
        anchors.leftMargin: 5+21
        width: parent.width-10 - 21
        height: parent.height-titleHeight-10
        visible: false

        Row{
            width: parent.width
            height: 58
            CusSkinButton_Image
            {
                anchors{
                    verticalCenter: parent.verticalCenter
                }
                width: 12
                height: 18
                btnImgNormal: "qrc:/UI/photo/model_library_detail_back.png"
                btnImgHovered: "qrc:/UI/photo/model_library_detail_back_h.png"
                btnImgPressed: "qrc:/UI/photo/model_library_detail_back_h.png"
                onClicked:
                {
                    idModelLibraryDetail.visible = false
                    idModelLibraryContent.visible = true
                }
            }
        }
        Column{
            width: parent.width
            height: parent.height
            spacing: 10
            Row{
                spacing: 10
                width: parent.width
                height: 500* screenScaleFactor
                Column{
                    width: 442 * screenScaleFactor
                    height: 500 * screenScaleFactor
                    Rectangle {
                        width: 442 * screenScaleFactor
                        height: 399 * screenScaleFactor
                        opacity: 1
                        color: "#E9E9E9"

                        Image{
                            id: idDetailImage
                            width: 442 * screenScaleFactor
                            height: 399 * screenScaleFactor  
                            mipmap: true
                            smooth: true
                            cache: false
                            asynchronous: true
                            fillMode: Image.PreserveAspectFit
                            source: ""
                        }
                    }
                    Rectangle{
                        width: 442 * screenScaleFactor
                        height: 101 * screenScaleFactor
                        color: "#FFFFFF"
                        Row{
                            anchors{
                                horizontalCenter: parent.horizontalCenter
                                verticalCenter: parent.verticalCenter
                            }
                            spacing: 20
                            CusSkinButton_Image{
                                id: idDetailImageLeft
                                anchors{
                                    verticalCenter: parent.verticalCenter
                                }
                                width: 11*screenScaleFactor
                                height: 16*screenScaleFactor
                                btnImgDisbaled: "qrc:/UI/photo/model_library_detail_left.png"
                                btnImgNormal: "qrc:/UI/photo/model_library_detail_left.png"
                                btnImgHovered: "qrc:/UI/photo/model_library_detail_left_h.png"
                                btnImgPressed: "qrc:/UI/photo/model_library_detail_left_h.png"
                                onClicked:
                                {
                                    curModelDetailImage = curModelDetailImage-1
                                    letModelDetailImageListShow(curModelDetailImage, modelCurrentGroupCount)
                                    onSigBtnDetailItemClicked(imageMap[curModelDetailImage].id)
                                    idDetailImage.source = imageMap[curModelDetailImage].btnImgUrl
                                }
                            }
                            Row{
                                id: idModelDetailListImage
                                height: 60 * screenScaleFactor
                                width: 340 * screenScaleFactor
                                spacing: 10
                            }
                            CusSkinButton_Image{
                                id: idDetailImageRight
                                anchors{
                                    verticalCenter: parent.verticalCenter
                                }
                                width: 11 * screenScaleFactor
                                height: 16 * screenScaleFactor
                                btnImgDisbaled: "qrc:/UI/photo/model_library_detail_right.png"
                                btnImgNormal: "qrc:/UI/photo/model_library_detail_right.png"
                                btnImgHovered: "qrc:/UI/photo/model_library_detail_right_h.png"
                                btnImgPressed: "qrc:/UI/photo/model_library_detail_right_h.png"
                                onClicked:
                                {
                                    curModelDetailImage = curModelDetailImage+1
                                    letModelDetailImageListShow(curModelDetailImage, modelCurrentGroupCount)
                                    onSigBtnDetailItemClicked(imageMap[curModelDetailImage].id)
                                    idDetailImage.source = imageMap[curModelDetailImage].btnImgUrl
                                }
                            }
                        }
                    }
                }
                Rectangle{
                    width: 706 * screenScaleFactor
                    height: 500 * screenScaleFactor
                    color: "#FFFFFF"
                    Column{
                        x: 20 * screenScaleFactor
                        y: 20 * screenScaleFactor
                        spacing: 6
                        width: parent.width-40
                        height: parent.height-40
                        StyledLabel{
                            id: idModelNameLabel
                            width: parent.width-20
                            height: 20
                            //wrapMode: Text.WordWrap
                            elide: Text.ElideRight
                            text: ""
                            font.pointSize:14
                            font.weight: "Bold"
                            color: "#333333"
                            verticalAlignment: Qt.AlignVCenter
                        }
                        StyledLabel{
                            id: idModelUploadTimeLabel
                            width: idModelUploadTimeLabel.contentWidth
                            height: 12
                            text: ""
                            font.pointSize:Constants.labelFontPointSize
                            color: "#666666"
                        }
                        Row{
                            width: parent.width
                            height: 85 * screenScaleFactor
                            spacing: 9
                            BaseCircularImage{
                                id: idAvtarImage
                                anchors{
                                    //horizontalCenter: parent.horizontalCenter
                                    verticalCenter: parent.verticalCenter
                                }
                                width: 60 * screenScaleFactor
                                height: 60 * screenScaleFactor
                                img_src: ""
                            }
                            StyledLabel{
                                id: idAuthorName
                                anchors{
                                    //horizontalCenter: parent.horizontalCenter
                                    verticalCenter: parent.verticalCenter
                                }
                                width: 203 * screenScaleFactor
                                height: 60 * screenScaleFactor
                                elide: Text.ElideRight
                                font.pointSize:14
                                color: "#333333"
                                text: ""
                                verticalAlignment: Qt.AlignVCenter
                            }
                            BasicDialogButton{
                                id: idCollectModelBtn
                                property var collectStatus: 0
                                width: 36 * screenScaleFactor
                                height: 36 * screenScaleFactor
                                btnRadius: 3
                                btnBorderW: 1
                                borderColor: "#E7E7E7"
                                pressedIconSource: "qrc:/UI/photo/model_library_uncollect.png"
                                defaultBtnBgColor: "#FFFFFF"
                                hoveredBtnBgColor: "#FFFFFF"
                                strTooptip: {
                                   if(collectStatus == 1){
                                       return qsTr("UnCollect")
                                   }else{
                                       return qsTr("Collect")
                                   }
                                }
                                anchors{
                                    verticalCenter: parent.verticalCenter
                                }
                                onSigButtonClicked:
                                {
                                    if(collectStatus == 1){
                                        collectModelStatus(modelCurrentGroupId, true)
                                    }else{
                                        collectModelStatus(modelCurrentGroupId, false)
                                    }
                                }
                            }
                            BasicDialogButton{
                                id: idShareModelBtn
                                width: 36 * screenScaleFactor
                                height: 36 * screenScaleFactor
                                btnRadius: 3
                                btnBorderW: 1
                                borderColor: "#E7E7E7"
                                pressedIconSource: idShareModelBtn.hovered ? "qrc:/UI/photo/model_library_share_h.png" : "qrc:/UI/photo/model_library_share.png"
                                defaultBtnBgColor: "#FFFFFF"
                                hoveredBtnBgColor: "#FFFFFF"
                                strTooptip: qsTr("Share")
                                anchors{
                                    verticalCenter: parent.verticalCenter
                                }
                                onSigButtonClicked:
                                {
                                    idShareModelPopup.visible = true
                                    idShareModelPopup.x = idModelLibraryDlg.x + 970 - 208
                                    idShareModelPopup.y = idModelLibraryDlg.y + 218
                                }
                            }
                            BasicDialogButton{
                                id: idModelLibraryDetailLoad
                                anchors{
                                    //horizontalCenter: parent.horizontalCenter
                                    verticalCenter: parent.verticalCenter
                                }
                                width: 140 * screenScaleFactor
                                height: 36 * screenScaleFactor
                                btnRadius: 3
                                btnBorderW: 0
                                pointSize: Constants.labelLargeFontPointSize
                                isAnimatedImage:false
                                btnTextColor:{
                                    if(idModelLibraryDetailLoad.enabled){
                                        return Constants.textColor
                                    }else{
                                        return "#666666"
                                    }
                                }
                                pressedIconSource: "qrc:/UI/photo/model_library_detail_import.png"
                                defaultBtnBgColor: "#1E9BE2"
                                hoveredBtnBgColor: "#1EB6E2"
                                text: qsTr("Import Model")
                                onSigButtonClicked:
                                {
                                    importModelLibraryItem(modelCurrentGroupId, idModelNameLabel.text)
                                }
                            }

                            BasicDialogButton{
                                id: idDetailDownModelLocatalBtn
                                anchors{
                                    //horizontalCenter: parent.horizontalCenter
                                    verticalCenter: parent.verticalCenter
                                }
                                width: 140 * screenScaleFactor
                                height: 36 * screenScaleFactor
                                btnRadius: 3
                                btnBorderW: 1
                                borderColor: "#E7E7E7"
                                pointSize: Constants.labelLargeFontPointSize
                                isAnimatedImage:false
                                btnTextColor:{
                                    if(idDetailDownModelLocatalBtn.enabled){
                                        return btnHovred ? "#FFFFFF" : "#ACACAC"
                                    }else{
                                        return "#666666"
                                    }
                                }
                                pressedIconSource: btnHovred ? "qrc:/UI/photo/model_library_detail_download.png":"qrc:/UI/photo/model_library_detail_item_down.png"
                                defaultBtnBgColor: "#FFFFFF"
                                hoveredBtnBgColor: "#1E9BE2"
                                text: qsTr("DownLoad Model")
                                onSigButtonClicked:
                                {
                                    downLoadModelGroup("downloadModelGroup")
                                }
                            }
                        }
                        StyledLabel{
                            width: parent.width
                            height: 16 * screenScaleFactor
                            font.pointSize:12
                            color: "#333333"
                            text: qsTr("Model List")
                        }
                        Rectangle{
                            width: 10
                            height: 1
                            color: "transparent"
                        }
                        BasicScrollView{
                            width: parent.width
                            height: 252 * screenScaleFactor
                            hpolicy: ScrollBar.AlwaysOff
                            vpolicy: ScrollBar.AsNeeded
                            clip : true
                            Column
                            {
                                id: idModelListItem
                                width: parent.width
                            }
                        }
                    }

                }
            }
            Rectangle{
                width: 1158 * screenScaleFactor
                height: 123 * screenScaleFactor
                color: "#FFFFFF"
                Row{
                    x: 20
                    y: 20
                    width: parent.width
                    height: parent.height
                    Column{
                        spacing: 10
                        Row{
                            spacing: 10
                            StyledLabel{
                                id: idLicenseLabel
                                width: idLicenseLabel.contentWidth
                                height: 18 * screenScaleFactor
                                font.pointSize:14 
                                font.weight: "Bold"
                                color: "#333333"
                                text: qsTr("Creative Commons License")
                            }
                            CusSkinButton_Image{
                                anchors{
                                    verticalCenter: parent.verticalCenter
                                }
                                width: 16* screenScaleFactor
                                height: 16* screenScaleFactor
                                btnImgNormal: "qrc:/UI/photo/model_library_license.png"
                                btnImgHovered: "qrc:/UI/photo/model_library_license_h.png"
                                btnImgPressed: "qrc:/UI/photo/model_library_license_h.png"

                                onClicked:
                                {	
                                    idLicenseDesDlg.visible = true
                                }
                            }
                        }
                        StyledLabel{
                            id: idLicenseDisLabel
                            width: idLicenseDisLabel.contentWidth
                            height: 15* screenScaleFactor
                            font.pointSize:Constants.labelLargeFontPointSize
                            color: "#666666"
                            text: qsTr("Please check the copyright information in the description.")
                        }
                    }
                    Rectangle{
                        width: 941* screenScaleFactor-idLicenseDisLabel.contentWidth
                        height: 60* screenScaleFactor
                        color: "transparent"
                    }
                    Image{
                        id: idLcenseIconImg
                        width: 172* screenScaleFactor
                        height: 60 * screenScaleFactor
                        mipmap: true
                        smooth: true
                        cache: false
                        asynchronous: true
                        fillMode: Image.PreserveAspectFit
                        source: "qrc:/UI/photo/license_by_nc.png"
                    }
                }
                
            }
        }
    }

    LicenseDescriptionDlg{
        id:idLicenseDesDlg
        visible:false
    }

    ModelLibraryShareDialog{
        id: idShareModelPopup
        visible: false
        property var modelId: ""

        Column{
            x: 21 + 10
            y: 36
            spacing: 10
            Row{
                spacing: 9
                StyledLabel{
                    width: 37* screenScaleFactor
                    height: 30* screenScaleFactor
                    font.pointSize:Constants.labelFontPointSize
                    text: qsTr("Link: ")
                    color: "#333333"
                    verticalAlignment: Text.AlignVCenter
                }
                StyledLabel{
                    id: idShareLink
                    width: 304* screenScaleFactor
                    height: 30* screenScaleFactor
                    font.pointSize:Constants.labelFontPointSize
                    text: ""
                    color: "#333333"
                    elide: Text.ElideRight
                    verticalAlignment: Text.AlignVCenter
                    Rectangle{
                        anchors.fill: parent
                        color: "transparent"
                        border.width: 1
                        border.color: "#E7E7E7"
                    }
                }
            }
            Row{
                x: 45
                spacing: 24
                BasicButton{
                    width: 140* screenScaleFactor
                    height: 36* screenScaleFactor
                    text: qsTr("Copy Link")
                    btnRadius: 3
                    btnBorderW: 0
                    pointSize: Constants.labelFontPointSize
                    // defaultBtnBgColor: "#BBBBBB"
                    // hoveredBtnBgColor: "#1E9BE2"
                    btnTextColor: "white"
                    defaultBtnBgColor : "#1E9BE2"
                    hoveredBtnBgColor: "#1EB6E2"
                    onSigButtonClicked:
                    {
                        shareModelGroup(idShareModelPopup.modelId)
                        idShareModelPopup.visible = false
                    }
                }
                BasicButton{
                    width: 140* screenScaleFactor
                    height: 36* screenScaleFactor
                    text: qsTr("Cancel")
                    btnRadius: 3
                    btnBorderW: 0
                    pointSize: Constants.labelFontPointSize
                    btnTextColor: "white"
                    defaultBtnBgColor: "#BBBBBB"
                    hoveredBtnBgColor: "#1E9BE2"

                    onSigButtonClicked:
                    {
                        idShareModelPopup.visible = false
                    }
                }
            }
        }
    }

    FolderDialog{
        id: idFolderDownDlg
        property var receiveType: ""
        onAccepted:
        {
            if(receiveType == "downloadModelGroup")
            {
                downModelToLocall(modelCurrentGroupId, modelCurrentGroupCount, idModelNameLabel.text, currentFolder)
            }
            else if(receiveType == "downloadModelItem")
            {
                downModelToLocall(currentModelId, 0, curModelName, currentFolder)
            }
        }
    }

    UploadMessageDlg{
        id: idDownModelFailedDlg
        msgText: qsTr("Failed to import or download model!") 
        cancelBtnVisible: false
        visible: false
        onSigOkButtonClicked:
        {
            idDownModelFailedDlg.close()
        }
    }
}
