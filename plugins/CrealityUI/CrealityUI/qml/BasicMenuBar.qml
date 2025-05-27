import QtQuick 2.0
import QtQuick.Controls 2.13
import QtQuick.Templates 2.12 as T
import QtQuick.Window 2.2
import ".."
import "../secondqml"

Rectangle {
    id: idHeader
    width: parent.width
    height: 48 *  screenScaleFactor
    color: Constants.headerBackgroundColor

    property var myLoad
    property var mainWindow
    property var imageUrl: ""
    property var nickName: ""
    property var userID: ""
    property var usedStorageSpace: 0
    property var maxStorageSpace: 0
    property var loginSRC:"menu"  //gcodeUpload:gcode上传时登录   modelUpload:模型上传时登录
    property var winPos
    property var curLagnuage: 0
    property var isModelLibraryDlgShowed: false
    property var isGroupPrintDlgShowed: false

    property bool isMaxed: true
    property bool isLogin: false

    property alias myMenubar: idMenuBar
    property alias imageurl: idLoginBtn.btnImgUrl

    signal closeWindow()
    signal machineAdded(string name)
    signal machineManager()
    signal selectmachineChanged(string name)
    signal showAddPrinterDlg()
    signal skipUrlCrealityCloud()
    signal changeLanguage(int type)

    signal login()
    signal logout()
    signal dlgClose()
    signal loginByAccont(string accont,string password,string logintype,bool autoLogin,string phoneAreaCode)
    signal quickloginByAccont(string phoneNumber,string phoneAreaCode, string verifyCode,bool autoLogin)
    signal qrcodeImgUpdate()
    signal getVerificationCode(string phoneNum)
    signal sigAutoLogin(string strToken, string strUserId)
    signal showGcodeUpload(string type)
    signal showModelUpload(string type)
    signal getGCodeList(int pageIndex, int pageSize)
    signal deleteGCode(string id)
    signal getModelList(string pageIndex, int pageSize)
    signal deleteModel(string id)
    signal importModel(string id, string name, int modelType) //1: model library; 2: my model
    signal importGCode(string id, string name, string loadLink)
    signal getCategoryList(int type)
    signal getModelLibraryList(string id, string page, int size)
    signal getModelGroupDetail(string id, int count)
    signal getModelGroupInfomation(string id)
    signal searchModelList(string key, int page, int size)
    signal importDetailItem(string modelGroupId, string modelName, string modelLink)
    signal shareModel(string id)
    signal getFavoritedList(string pageIndex, int pageSize)
    signal cancelFavoriteModel(string id)
    signal favoriteModel(string id, bool status)
    signal sigDownLoadModelType(string status)
    signal sigDownLoadModelToLocall(string modelId, int modelCount, string modelName, string filePath)
    signal changeServer(int type)
    signal initP2PPipelie(string apilicense, string strDid, string strinit, int portNum, string productName)
    signal getPrinterList(int pageIndex, int pageSize)
    signal getGCodeListPrintControl(int pageIndex, int pageSize, string type)
    signal sigStartPrintGcode(string urlStr, string productNameStr, string iotIdStr, string productKeyStr, string gcodeIdStr, string printerNameStr, string printerIdStr, string tbid, int iotType)
    signal sigControlPrintGcode(string productNameStr, string iotIdStr, string productKeyStr, string controlStr, string tbid, int iotType)
    signal sigControlPrintTempSpeed(string productNameStr, string iotIdStr, string productKeyStr, string targetInfo, int target, string tbid, int iotType)
    signal sigUpdatePreviewImage(string printID,string deviceID)
    signal sigGetPrintingGcodeInfo(string printID)
    signal pauseGetDeviceTask(string pause)
    signal sigInitCameraLicense(string productNameStr, string iotIdStr, string productKeyStr, string syDid, string syLicense, string tbid, int iotType)
    signal sigUpdateDeviceData(string productNameStr, string iotIdStr, string productKeyStr, string tbid, int iotType)
    signal getCloudSliceList(int pageIndex, int pageSize)
    signal sigPersonalSpaceTutorial(string tips)
    signal sigModelAlwaysPopShow()
    signal getPurchasedList(string pageIndex, int pageSize)
    signal sigUserInfoDlgClicked()
    signal wizardClicked()
    signal getGroupPrintDeviceList()
    signal getClusterGcodeList(int pageIndex, int pageSize)
    signal addClusterTask(string seriesIds, string gcodeInfo)
    signal clusterConformPrinterStatus(string value, string status)
    signal clusterUploadGcode()
    signal sigSelectPrintControl(string type)
    signal sigChangeClusterTaskTime(string type)
    signal clusterGcodeSliceTypeList()
    signal clsterResetGcodeSliceType(string sliceId, string sliceId)
    signal printerMachineUpdate()

    Connections {
        target: standaloneWindow
        onCurStateChanged:
        {
            if(curState === 0 && standaloneWindow.functionType === 0)
            {
                idMenuBar.showFDMMenuBar()
            }
            else
            {
                idMenuBar.onlyShowOtherMenuBar()
            }
        }
        onFunctionTypeChanged:
        {
            if(standaloneWindow.curState === 0 && functionType === 0)
            {
                idMenuBar.showFDMMenuBar()
            }
            else
            {
                idMenuBar.onlyShowOtherMenuBar()
            }
        }
        onVisibilityChanged: if(isMaxed && visibility === 2) standaloneWindow.showMaximized()
    }

    function getAutoLogin(token, userId)
    {
        sigAutoLogin(token, userId)
    }

    function exitGetDeviceTask()
    {
        idPrintControlDlg.stopAllVideo()
        pauseGetDeviceTask("exit")
    }

    function restartVideo(strDid)
    {
        console.log("BasicMenuBar restartVideo")
        idPrintControlDlg.restartVideoPlay(strDid)
    }

    function controlEnabled(bEnabled)
    {
        Constants.bIsWizardShowing = !bEnabled
        Constants.bLeftToolBarEnabled = bEnabled
        Constants.bRightPanelEnabled = bEnabled
        Constants.bMenuBarEnabled = bEnabled
        Constants.bGLViewEnabled = bEnabled
    }

    function setPrintPreviewImage(previewImage, allLayers, deviceid)
    {
        idPrintControlDlg.setPreviewImage(previewImage,allLayers,deviceid)
    }

    function startFirstConfig()
    {
        idLanguageSelectDlg.visible = true
    }

    function startWizardComp()
    {
        var pRoot = rootLoader
        while (pRoot.parent !== null) {
            pRoot = pRoot.parent
        }
        wizardComp.createObject(pRoot, {model: rootLoader.item.wizardModel })
        idSkinBtn.enabled = false
        controlEnabled(false)
    }

    //    function showAddDlg()
    //    {
    //        // idMachineeWizard.cmbCount = idSelectPrinter.count
    //        // idMachineeWizard.visible=true
    //        // idMachineeWizard.clearShowParam()
    //        _idAddPrinterDlgNew.visible = true
    //    }

    function letSelectPrinterEnable(value)
    {
//        idSelectPrinter.enabled = value
        idMenuBar.enabled = value
    }

    function showModelLibraryDialog()
    {
        idModelLibraryInfo.visible = true
        if(isModelLibraryDlgShowed == false)
        {
            getCategoryList(7)
            isModelLibraryDlgShowed = true
        }
    }

    function selectPrintControlType(type)
    {
        sigSelectPrintControl(type)
    }

    function showGroupPrintInfoDlg()
    {
        if(isGroupPrintDlgShowed == false)
        {
            idGroupPrintDlg.onLeftButtonClick("taskcreate")
            clusterGcodeSliceTypeList()
            isGroupPrintDlgShowed = true
        }
        idGroupPrintDlg.visible = true
    }

    function closeGroupPrintInfoDlg()
    {
        idGroupPrintDlg.visible = false
    }

    function showLoginDialog(loginsource)
    {
        idLoginDlg.visible = true
        loginSRC = loginsource
    }

    function showUploadMsgDlg(param)
    {
        warringLogindlg.visible = true
        warringLogindlg.otherMsg = param
    }

    function showLogoutDialog()
    {
        sigUserInfoDlgClicked()
        idUserInfoDlg.setUserInfo(nickName, userID, imageUrl, usedStorageSpace, maxStorageSpace)
        //idUserInfoDlg.visible = true
        idUserInfoDlg.setUserInfoDlgShow("gcode")
    }

    function loginSuccess(userid,nickname, image, usedSpace, maxSpace)
    {
        idLoginDlg.resetLogindlg()
        idLoginDlg.visible = false

        nickName = nickname
        userID = userid
        isLogin = true
        imageUrl = image
        imageurl = image
        usedStorageSpace = usedSpace
        maxStorageSpace = maxSpace
        idLoginBtn.sourceSize = Qt.size(36,36)

        idUserInfoDlg.setUserInfo(nickName, userID, imageUrl, usedStorageSpace, maxStorageSpace)

        if(loginSRC === "gcodeUpload")
        {
            showGcodeUpload("gcodeUpload")
        }else if(loginSRC === "modelUpload")
        {
            showModelUpload("modelUpload")
        }else if(loginSRC === "modelLibraryCollect")
        {
            getModelGroupInfomation(idModelLibraryInfo.collectId)
        }else if(loginSRC === "modelGroupimport")
        {
            importModel(idModelLibraryInfo.modelId, idModelLibraryInfo.modelNum, 1)
            getModelGroupInfomation(idModelLibraryInfo.modelId)
        }
        else if(loginSRC === "modelDetailDownload")
        {
            importDetailItem(idModelLibraryInfo.modelDetailId, idModelLibraryInfo.modelDetailName, idModelLibraryInfo.modelDetailLink)
            getModelGroupInfomation(idModelLibraryInfo.modelId)
        }
        else if(loginSRC === "downloadModelItem")
        {
            sigDownLoadModelType("downloadModelItem")
        }
        else if(loginSRC === "downloadModelGroup")
        {
            sigDownLoadModelType("downloadModelGroup")
        }
        else if(loginSRC === "modelLocalFileUpload")
        {
            showModelUpload("modelLocalFileUpload")
        }
        else if(loginSRC === "clusteruploadgcode")
        {
            showGcodeUpload("clusteruploadgcode")
        }
        else if(loginSRC == "singleprint")
        {
            selectPrintControlType("singleprint")
        }
        else if(loginSRC == "clusterprint")
        {
            selectPrintControlType("clusterprint")
        }


        //get my printer list
        getPrinterList(1, 50);
        //get group print device list
        getGroupPrintDeviceList()
    }

    function loginFailed(errorMsg)
    {
        console.log("+++menubar loginFailed+++")
        idLoginDlg.showErrorMsg(errorMsg)
    }

    function setCurLanguage(curType)
    {
        curLagnuage = curType
    }

    Rectangle{
        id: idBottom
        height:1
        width:idHeader.width
        color:Constants.mainViewSplitLineColor
        anchors.top:idHeader.bottom
    }

    MouseArea{
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton //只处理鼠标左键
        property bool isDoubleClicked:false
        property point clickPos: "0,0"
        onPressed:
        {
            isDoubleClicked = false;
            clickPos = winPos.getWindowPos()//Qt.point(mouse.x,mouse.y)
        }
        onPositionChanged: {
            var delta = Qt.point(winPos.getWindowPos().x -clickPos.x, winPos.getWindowPos().y-clickPos.y)
            if(!isDoubleClicked && pressed && mainWindow.visibility !== Window.Maximized && mainWindow.visibility !== Window.FullScreen) {
                console.log("deltadelta=",delta)
//                var delta = Qt.point(winPos.getWindowPos().x -clickPos.x, winPos.getWindowPos().y-clickPos.y)
                clickPos = winPos.getWindowPos()

                mainWindow.x += delta.x
                mainWindow.y += delta.y

            }


            if(Math.abs(delta.y) > 20 && mainWindow.visibility === Window.Maximized && pressed && !isDoubleClicked)
            {
                isMaxed = false;
                mainWindow.showNormal();
                mainWindow.x = delta.x
                mainWindow.y = delta.y
                maxBtn.normalIconSource  = "qrc:/UI/photo/maxBtn.png"
            }
        }
        onDoubleClicked :
        {
            isDoubleClicked = true; // 这个时候一定不能响应onPositionChanged不然会一直置顶。
            if(isMaxed && mainWindow.visibility === Window.Maximized){
                isMaxed = false;
                mainWindow.showNormal();
                maxBtn.normalIconSource = "qrc:/UI/photo/maxBtn.png"/**/;
            }
            else{
                isMaxed = true;
                mainWindow.showMaximized();
                maxBtn.normalIconSource = "qrc:/UI/photo/normalBtn.png"/**/;
            }
        }
    }

    Image {
        id : logoImage
        anchors.left: idHeader.left
        anchors.leftMargin: 12
        anchors.verticalCenter: idHeader.verticalCenter
        height:sourceSize.height
        width: sourceSize.width
        source: "qrc:/UI/photo/LOGO.png"
        objectName: "blankItem"
    }

    StyledLabel//by TCJ
    {
        id:idText
        anchors.left: logoImage.right
        anchors.leftMargin: 5/*12*/
        anchors.verticalCenter: idHeader.verticalCenter
        height:contentHeight
        width: contentWidth
        text: "Creality Print"
    }
    Rectangle
    {
        id : idLeftSeparator
        anchors.left: idText.right
        anchors.leftMargin: 20
        anchors.top: parent.top
        anchors.topMargin: 14
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 14
        width: 1
        color: Constants.menuSplitLineColor
    }

    BasicMenuBarStyle
    {
        id: idMenuBar
        anchors.left: idLeftSeparator.right
        anchors.leftMargin: 10
        width:contentWidth
        height:idHeader.height
        anchors.verticalCenter: parent.verticalCenter
//        background: Rectangle
//        {
//            color: idHeader.color//Constants.itemBackgroundColor
//            anchors.fill: parent
//        }
        objectName: "mybasicmenubar"
    }
    Row{
        anchors.centerIn: parent
        spacing: 10
        visible: standaloneWindow.functionType === 0
        CusButton{
            anchors.verticalCenter: parent.verticalCenter
            id:prepairBtn
            width: 100 *  screenScaleFactor
            height: (isChecked ? 28 : 24) *  screenScaleFactor
            radius: 5
            checkable: true
            isChecked: standaloneWindow.curState === 0
            normalColor: Constants.topBtnBgColor_normal
            hoveredColor: Constants.topBtnBgColor_hovered
            pressedColor: Constants.topBtnBgColor_hovered
            normalBdColor: Constants.topBtnBorderColor_normal
            txtColor : isHovered || isChecked ? Constants.topBtnTextColor_hovered
                                              : Constants.topBtnTextColor_normal

            txtWeight: Font.Bold
            txtPointSize: Constants.labelLargeFontPointSize
            txtFamily: Constants.labelFontFamily
            txtBold: true
            txtContent:qsTr("Prepare")
            onClicked: standaloneWindow.curState = 0
        }

        CusButton{
            anchors.verticalCenter: parent.verticalCenter
            id: previewBtn
            width: 100 *  screenScaleFactor
            height: (isChecked ? 28 : 24) *  screenScaleFactor
            radius: 5
            enabled: false
            checkable: true
            isChecked: standaloneWindow.curState === 1
            normalColor: Constants.topBtnBgColor_normal
            hoveredColor: Constants.topBtnBgColor_hovered
            pressedColor: Constants.topBtnBgColor_hovered
            normalBdColor: Constants.topBtnBorderColor_normal
            txtColor : isHovered || isChecked ? Constants.topBtnTextColor_hovered
                                              : Constants.topBtnTextColor_normal
            txtWeight: Font.Bold
            txtPointSize: Constants.labelLargeFontPointSize
            txtFamily: Constants.labelFontFamily
            txtBold: true
            txtContent:qsTr("Preview")

            Connections {
                target: modelListData
                onRowChanged: previewBtn.enabled = row
            }

            onClicked: standaloneWindow.curState = 1
        }
        CusButton{
            anchors.verticalCenter: parent.verticalCenter
            width: 100 * screenScaleFactor
            height: (isChecked ? 28 : 24) *  screenScaleFactor
            radius: 5
            checkable: true
            isChecked: standaloneWindow.curState === 2
            normalColor: Constants.topBtnBgColor_normal
            hoveredColor: Constants.topBtnBgColor_hovered
            pressedColor: Constants.topBtnBgColor_hovered
            normalBdColor: Constants.topBtnBorderColor_normal
            txtColor : isHovered || isChecked ? Constants.topBtnTextColor_hovered
                                              : Constants.topBtnTextColor_normal

            txtWeight: Font.Bold
            txtPointSize: Constants.labelLargeFontPointSize
            txtFamily: Constants.labelFontFamily
            txtBold: true
            txtContent:qsTr("Device")
            onClicked: standaloneWindow.curState = 2
        }
    }

    ButtonGroup{
        id:titleGroup
    }

    //    CusSkinButton_Image{
    //        id: idPrintBtn
    //        isRound: true
    //        checkable: false
    //        anchors.verticalCenter: parent.verticalCenter
    //        anchors.right: parent.right
    //        anchors.rightMargin: 284
    //        ButtonGroup.group: titleGroup
    //        defaultBtnBgColor:"transparent"
    //        hoveredBtnBgColor:"#68686b"
    //        btnImgUrl:"qrc:/UI/photo/print_avatar_img.png"
    //        tipText: qsTr("Print Information")
    //        width: 36
    //        height: 36
    //        onClicked:
    //        {
    ////            idPrintingControlDlg.visible = true
    //        }
    //    }

    ImageButton_ToolTip {
        id:idLoginBtn
        isRound: true
        checkable: false
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: idRightSeparator.left
        anchors.rightMargin: 40
        defaultBtnBgColor:"transparent"
        hoveredBtnBgColor:Constants.tittleBarBtnColor
        btnImgUrl:"qrc:/UI/photo/userImg.png"
        tipText: isLogin ? qsTr("Login succeeded. Click to view your personal space.") : qsTr("Login")
        objectName: "logindBtn"
        width: 36 * screenScaleFactor
        height: 36 * screenScaleFactor
        //sourceSize:Qt.size(36,36)
        onClicked:
        {
            if(isLogin)
            {
                showLogoutDialog()
            }
            else
            {
                login()
            }
        }
        Connections{
            target: crealityUICCpp
            onSigLoginSuccess:{
                //console.log("+++++++++++++success!!!")
                idLoginBtn.sourceSize = Qt.size(36,36)
            }
        }
    }

    Rectangle
    {
        id : idRightSeparator
        anchors.right: idControlButtonsRow.left
        anchors.rightMargin: 40
        anchors.top: parent.top
        anchors.topMargin: 14
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 14
        width: 1
        color: Constants.menuSplitLineColor
    }

    Row{
        id : idControlButtonsRow
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.margins: 1
        spacing: 1
        layoutDirection: Qt.RightToLeft
        objectName:"controlButtonsRow"
        TitleBarBtn{
            id: closeBtn
            anchors.verticalCenter: parent.verticalCenter
            width: 60 * screenScaleFactor
            height: 48 * screenScaleFactor
            hoverBgColor: "#FD265A"
            borderwidth:0
            buttonRadius:0
            iconWidth: 12*  screenScaleFactor
            iconHeight: 12 *  screenScaleFactor
            normalIconSource: hovered ? "qrc:/UI/photo/closeBtn_d.png" : "qrc:/UI/photo/closeBtn.png"//"qrc:/UI/images/close.png"
            onClicked: {
                //window.close()
                //console.log("menu bar click close")
                closeWindow()
            }
        }
        TitleBarBtn{
            id:maxBtn
            anchors.verticalCenter: parent.verticalCenter
            width: closeBtn.width
            height: closeBtn.height
            borderwidth:0
            buttonRadius:0
            normalIconSource: isMaxed ?"qrc:/UI/photo/normalBtn.png" : "qrc:/UI/photo/maxBtn.png" //"qrc:/UI/images/maximized.png"
            iconWidth: 12*  screenScaleFactor
            iconHeight: 12 *  screenScaleFactor
            onClicked: {
                if(isMaxed)
                {
                    isMaxed = false
                    mainWindow.showNormal()
                    normalIconSource = "qrc:/UI/photo/maxBtn.png"
                }
                else {
                    isMaxed = true
                    mainWindow.showMaximized()
                    normalIconSource = "qrc:/UI/photo/normalBtn.png"
                }
            }
        }

        TitleBarBtn{
            id:minBtn;
            anchors.verticalCenter: parent.verticalCenter
            width: closeBtn.width
            height: closeBtn.height
            borderwidth:0
            buttonRadius:0
            normalIconSource: "qrc:/UI/photo/minBtn.png"//"qrc:/UI/images/minimized.png"
            iconWidth: 12 * screenScaleFactor
            iconHeight: 2 * screenScaleFactor
            onClicked: {
                doMinimized()
                //mainWindow.showMinimized()
            }
        }
    }

    //    AddPrinterDlgNew {
    //        id: _idAddPrinterDlgNew
    //        objectName: "AddPrinterDlg"
    //        visible: false
    //        property var isFristStart: "0"
    //        onAddPrinterMachine:
    //        {
    //            machineAdded(name)
    //            _idAddPrinterDlgNew.visible = false
    //        }
    //        onUpdatePrinterMachine:
    //        {
    //            printerMachineUpdate()
    //        }
    //    }

    // AddPrinterDlg {
    // 	id:idMachineeWizard
    // 	visible:false
    // 	objectName: "AddPrinterDlg"
    //     property var isFristStart: "0"
    // 	onAddMachine:function(machineName){
    // 		console.log(machineName)
    // 		machineAdded(machineName)
    // 	}
    // 	onSelectMachineChange:
    // 	{
    // 		selectmachineChanged(machineName)
    // 	}

    //     onVisibleChanged:
    //     {
    //         if(idMachineeWizard.visible == false)
    //         {
    //             if(isFristStart == "0")
    //             {
    //                 sigModelAlwaysPopShow()
    //                 isFristStart = "1"
    //             }
    //         }
    //     }
    // }

    LoginDlg {
        id:idLoginDlg
        visible:false
        objectName: "LoginDlg"
        onDialogClosed:
        {
            //dlgClose()
        }
        onLoginClicked:
        {
            loginByAccont(accont,password,logintype,autoLogin,phoneAreaCode)
        }
        onQrcodeLogin:
        {
            qrcodeImgUpdate()
        }
        onStopQrCodeUpdate:
        {
            dlgClose()
        }
        onGetCode:
        {
            getVerificationCode(phoneNum)
        }
        onQuickLoginClicked:
        {
            quickloginByAccont(phoneNumber,phoneAreaCode,verifyCode,autoLogin)
        }
    }

    UserInfoDlg {
        id:idUserInfoDlg
        visible:false
        objectName: "UserInfoDlg"
        onQuitClicked:
        {
            console.log("quitBotton onSigButtonClicked")
            logout()
            idUserInfoDlg.visible = false
            isLogin = false
            idLoginBtn.btnImgUrl = ""
            idLoginBtn.btnImgUrl = "qrc:/UI/photo/userImg.png"
            idLoginBtn.sourceSize = undefined
            //idPrintBtn.visible = false
        }
        onGetMyGCodeList:
        {
            getGCodeList(page, size)
        }
        onDeleteCurrentGcodeItem:
        {
            deleteGCode(gcodeId)
        }
        onGetMyModelList:
        {
            getModelList(page, size)
        }
        onGetMyFavoritedList:
        {
            getFavoritedList(page, size)
        }
        onDeleteCurrentModelItem:
        {
            deleteModel(modelId)
        }
        onCancelFavoritedModel:
        {
            cancelFavoriteModel(modelId)
            getModelGroupInfomation(modelId)
        }
        onImportModelItem:
        {
            importModel(modelId, modelcount, 2)
        }
        onImportGCodeItem:
        {
            importGCode(gcodeid, gcodename, gcodedownlink)
        }
        onShareModelGroup:
        {
            shareModel(modelsId)
        }
        onGetMyPrinterList:
        {
            //update
            getPrinterList(1, 50);
        }
//        onUserPrinterItemClick:
//        {
//            getGCodeListPrintControl(1, 50, "devices")

//            idPrintControlDlg.selectCurPrinterInfo(name, id, tbid, iotType)
//            idPrintControlDlg.startVideoPlay()
//            idPrintControlDlg.initPrintControlPage()

//            idPrintControlDlg.visible = true
//            idPrintControlDlg.initCameraInfo() //init syLicense
//        }
        onPauseUpdateDeviceTask:
        {
            pauseGetDeviceTask("loginout")
        }
        onSkipWebCrealityCloud:
        {
            skipUrlCrealityCloud()
        }
//        onPrintGcodeToControl:
//        {
//            getGCodeListPrintControl(1, 50, type)
//            idPrintControlDlg.initGcodeToPrint(id)
//            idPrintControlDlg.insertPrinterList()
//            idPrintControlDlg.visible = true
//        }
        onGetCloudSlicingList:
        {
            getCloudSliceList(page, size)
        }
        onPersonalSpaceTutorial:
        {
            sigPersonalSpaceTutorial(type)
        }
        onGetPurchasedModelList:
        {
            getPurchasedList(page, size)
        }
        onSigupdateDeviceStatus:
        {
            idPrintControlDlg.updateDeviceStatus()
            idPrintingControlDlg.updateDeviceStatus()
        }
        onSigupdatePrintCtrlInfo:
        {
            idPrintControlDlg.updateDeviceInformation(productName)
            idPrintingControlDlg.updateDeviceInformation(productName)
            idGroupPrintDlg.updateDeviceInformation(productName)
        }
    }

    GroupPrintInfoDlg{
        id: idGroupPrintDlg
        objectName: "GroupPrintDlg"
        visible: false

        onVisibleChanged:
        {
            if(idGroupPrintDlg.visible === true)
            {
                sigChangeClusterTaskTime("5000ms")
            }else
            {
                sigChangeClusterTaskTime("30000ms")
            }
        }
        onSigUpdateClusterDeviceList:
        {
            getGroupPrintDeviceList()
        }
        onSigGetClusterGcodeList:
        {
            getClusterGcodeList(page, pageSize)
        }
        onCreateClusterTask:
        {
            addClusterTask(ids, info)
        }
        onSigClusterConformPrinterStatus:
        {
            clusterConformPrinterStatus(value, status)
        }
        onSigClusterControlChildTaskStatus:
        {
            if(operation === "viewprintdetail")
            {
                //idUserInfoDlg.userPrinterItemClick(IotData.serNickNameMap[dnName], dnName, IotData.serTbIdMap[dnName], IotData.serIotType[dnName])
            }
            else
            {
                sigControlPrintGcode(dnName, IotData.serIotIdMap[dnName], IotData.serProductkeyMap[dnName], operation, IotData.serTbIdMap[dnName], IotData.serIotType[dnName])
            }
        }
        onUploadLocalGcodeFile:
        {
            clusterUploadGcode()
        }
        onResetGcodeSliceType:
        {
            clsterResetGcodeSliceType(gcodeId, sliceId)
        }
    }

    PrintingControlInfoDlg{
        id: idPrintingControlDlg
        objectName: "PrintingControlInfoDlg"
        visible: false

        onSigViewPrintingInfomation:
        {
            // getGCodeListPrintControl(1, 50, "gcode")

            // idPrintControlDlg.selectCurPrinterInfo(deviceName, deviceID, deviceTBId, iotType)
            // idPrintControlDlg.startVideoPlay()
            // idPrintControlDlg.initPrintControlPage()

            // idPrintControlDlg.visible = true
            // idPrintControlDlg.initCameraInfo() //init syLicense
            // idUserInfoDlg.userPrinterItemClick(IotData.serNickNameMap[deviceID], deviceID, IotData.serTbIdMap[deviceID], IotData.serIotType[deviceID])
        }

        //        onSigPrintingStatus:
        //        {
        //            if(isLogin)
        //            {
        //                idPrintBtn.visible = status
        //            }
        //            else{
        //                idPrintBtn.visible = false
        //            }

        //        }
    }

    PrintControlInfoDlg{
        id: idPrintControlDlg
        objectName: "PrintControlInfoDlg"
        visible: false
        onInitP2P:
        {
            initP2PPipelie(apilicense, strDid, strinit, portNum,productName)
        }
        onStartPrintGcode:
        {
            sigStartPrintGcode(url, productName, iotId, productKey, gcodeId, printerName, printerId, tbid, iotType)
        }
        onControlPrintGcode:
        {
            sigControlPrintGcode(productName, iotId, productKey, control, tbid, iotType)
        }
        onUpdatePreviewImage:
        {
            sigUpdatePreviewImage(printID,deviceID)
        }
        onGetPrintingPauseGcodeInfo:
        {
            sigGetPrintingGcodeInfo(printID)
        }
        onControlPrintTempSpeed:
        {
            sigControlPrintTempSpeed(productName, iotId, productKey, targetInfo, targetValue,tbid, iotType)
        }
        onInitCameraLicense:
        {
            sigInitCameraLicense(productName, iotId, productKey, did, license, tbid, iotType)
        }
        onUpdateDeviceData:
        {
            sigUpdateDeviceData(productName, iotId, productKey,tbid, iotType)
        }
        onChangeIotPollTime:
        {
            pauseGetDeviceTask(time)
        }
        onSigAddVideo:
        {
            idGroupPrintDlg.addVideoItem(deviceID)
        }
    }

    ModelLibraryInfoDlg{
        id: idModelLibraryInfo
        objectName: "ModelLibraryDlg"
        visible: false
        property var collectId: ""
        property var modelId: ""
        property var modelNum: 0
        property var modelDetailId: ""
        property var modelDetailName: ""
        property var modelDetailLink: ""
        onGetPageModelLibraryList:
        {
            getModelLibraryList(typeId, pageIndex, pageSize)
        }
        onImportModelLibraryItem:
        {
            modelId = modelGroupId
            modelNum = modelCount
            importModel(modelGroupId, modelCount, 1)
        }
        onGetModelGroupDetailInfo:
        {
            modelId = groupId
            getModelGroupDetail(groupId, modelCount)
        }
        onGetModelGroupInfo:
        {
            getModelGroupInfomation(id)
        }
        onGetSearchModelList:
        {
            searchModelList(keyword, pageIndex, pageSize)
        }
        onImportModelDetailItem:
        {
            modelDetailId = id
            modelDetailName = name
            modelDetailLink = link
            importDetailItem(id, name, link)
        }
        onShareModelGroup:
        {
            shareModel(id)
        }
        onCollectModelStatus:
        {
            collectId = id
            favoriteModel(id, status)
        }
        onDownLoadModelGroup:
        {
            sigDownLoadModelType(type)
        }
        onDownModelToLocall:
        {
            sigDownLoadModelToLocall(id, count, name, path)
        }
    }

    UploadMessageDlg{
        id:warringLogindlg
        msgText: qsTr("Not logged in to Creality Cloud, Unable to current operation, Do you want to log in?")
        visible: false
        messageType:0
        onSigOkButtonClicked:
        {
            warringLogindlg.close()
            showLoginDialog(warringLogindlg.otherMsg)
        }
    }

    LanguageSelectDlg{
        id: idLanguageSelectDlg
        visible: false
        onSigOkButtonClicked:
        {
            if(curLagnuage != languageIndex)
            {
                changeLanguage(languageIndex)
                curLagnuage = languageIndex
            }

            visible = false
            idServerSelectDlg.visible = true
        }
    }

    ServerSelectDlg{
        id: idServerSelectDlg
        visible: false
        objectName: "ServerSelectDlg"
        onSigOkButtonClicked:
        {
            idLoginDlg.setServerType(serverIndex)
            changeServer(serverIndex)
            visible = false
            //startWizardComp()
            showAddPrinterDlg()
            idSkinBtn.enabled = true
            controlEnabled(true)
        }
        onSigBackButtonClicked:
        {
            visible = false
            idLanguageSelectDlg.visible = true
        }
    }

    Loader
    {
        id:rootLoader
        source: "../secondqml/CusWizardHome.qml"
    }

    Component {
        id: wizardComp
        CusWizard {
            id: cusWizard
            anchors.fill: parent
            currentIndex: 0
            onWizardFinished: {
                destroy(cusWizard)
                showAddPrinterDlg()
                idSkinBtn.enabled = true
                controlEnabled(true)
            }
        }
    }
}
