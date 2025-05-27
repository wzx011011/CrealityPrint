import QtQuick 2.0
import QtQuick 2.10
import QtQuick.Controls 2.0 as QQC2
import com.cxsw.SceneEditor3D 1.0

//import CrealityUI 1.0
import "qrc:/CrealityUI"
import QtQuick.Controls 2.13
import Qt.labs.settings 1.1
import Qt.labs.platform 1.0
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2

QQC2.ApplicationWindow {
    id: standaloneWindow
    flags: Qt.FramelessWindowHint | Qt.Window | Qt.WindowMinimizeButtonHint
    title: qsTr("Creative3D")
//    width: (1280 * screenScaleFactor) > Screen.desktopAvailableWidth ? Screen.desktopAvailableWidth:(1280 * screenScaleFactor)
//    height:  (720 * screenScaleFactor) > Screen.desktopAvailableHeight? Screen.desktopAvailableHeight:(720 * screenScaleFactor)//400
    visible: false
    color: "white"//Constants.paneBackgroundColor
    minimumHeight: ( 720* screenScaleFactor) > Screen.desktopAvailableHeight? Screen.desktopAvailableHeight:(720 * screenScaleFactor)//400
    minimumWidth: ( 1280 * screenScaleFactor) > Screen.desktopAvailableWidth ? Screen.desktopAvailableWidth:(1280 * screenScaleFactor)

    property int tmpWidth: 0
//    property int dafultRightPanelWidth: Constants.rightPanelWidth
    property int defultRightPanelHeight: 630 * screenScaleFactor//Constants.rightPanelHeight//700

    property int curState: 0 //0: 准备中 1:预览中 2:设备
    property int functionType: 0//0: fdm, 1:激光, 2:cnc


    property var imageSource: (Constants.currentTheme === 0) ? "qrc:/UI/photo/bgImg_black.png" : "qrc:/UI/photo/bgImg.png"

    property bool hasModel: false //是否有模型
    property bool hasModelSelection: false //是否有模型被选中
    property bool isWindow: Qt.platform.os === "windows" ? true : false

    property alias leftToolBar: leftToolBar
    property alias statusBarItem: idStatusBar
    property alias g_Workflow: idWorkflow
    property alias g_rightPanel: swipeView

    function showLanPrinterComponet()
    {
        __LANload.sourceComponent = __printerComponet
    }

    function destoryLanPrinterComponet()
    {
        if(__LANload.status === Loader.Ready)
        {
            __LANload.wifiSwitch = false
            __LANload.sourceComponent = undefined
        }
    }

    Loader
    {
        id: __LANload
        property var curGCodePath: ""
        property bool wifiSwitch: false

        z: 1000
        focus: true
        anchors.fill: parent
        anchors.topMargin: idMenuBar.height
        visible: status == Loader.Ready

        onLoaded:
        {
            __LANload.item.setRealEntryPanel(wifiSwitch)
            if(wifiSwitch) __LANload.item.gCCodePathPanel = curGCodePath
        }
    }

    Component
    {
        id: __printerComponet
        LanPrinterPanel
        {

        }
    }

    DropArea{
        id: idDropArea
        anchors.fill: parent
        //enabled: (idLaserPanel.visible || idPlotterPanel.visible) ? false : true
        onDropped: {
            if (drop.hasUrls){
                idWorkflow.kernelUI.openFileWithDrop(drop.urls)
                console.log(drop.urls)
            }
        }
    }
    signal initAdvance()
    signal getRecommendModel()
    signal showModelLibraryDlg(bool isMoreBtn, string id, string name, int count, string author, string avtar, string ctime)

    function doMinimized() {
        if(!isWindow)
        {
            flags = Qt.Window | Qt.WindowFullscreenButtonHint | Qt.CustomizeWindowHint | Qt.WindowMinimizeButtonHint
        }
        else
        {
            flags = Qt.Window | Qt.FramelessWindowHint | Qt.WindowMinimizeButtonHint
        }
        visibility = Window.Minimized
    }

    function onHaveNoUpdate(version)
    {
        idUpdateDlg.show()
    }
    function onHaveUpdate(version,url,releaseNote)
    {
        if(version === "null")
        {
            onHaveNoUpdate(version)
            return
        }
        idReleaseNote.version = version
        idReleaseNote.projectName = idWorkflow.updateManager.getProjectName()
        idReleaseNote.url = url
        idReleaseNote.releaseNote = releaseNote
        idReleaseNote.autocheck = idWorkflow.updateManager.autocheck;
        console.log(releaseNote)
        //idReleaseNote.open()
        idReleaseNote.show()
    }
    function showReleaseNote()
    {
        idReleaseNote.projectName = idWorkflow.updateManager.getProjectName()
        idReleaseNote.version = idWorkflow.updateManager.version
        idWorkflow.updateManager.forceCheckForUpdates()
        //idReleaseNote.show()
    }
    function changeThemeColor(themeType)
    {
        Constants.currentTheme = themeType
//        leftToolBar.setTriColor()
    }
    //lisugui 2020-10-29
    function beforeCloseWindow()
    {
        idWorkflow.kernelUI.beforeCloseWindow()
    }
    function requestOpenFileDialog(receiver)
    {
        fileDialog.receiver = receiver
        if(!receiver)
            return
        console.log(receiver)
        fileDialog.folder = GlobalCache.meshImportFolder
        fileDialog.nameFilters = receiver.nameFilters()
        fileDialog.title = qsTr("Open")//receiver.title()
        //console.log("AAAA:"+receiver.nameFilters());
        fileDialog.open()
    }
    function requestSaveFileDialog(receiver)
    {
        if(!receiver) return
        //console.log("test ok!");
        fileSaveDialog.receiver = receiver
        fileSaveDialog.nameFilters = receiver.nameFilters()
        fileSaveDialog.title = qsTr("Save")//receiver.title()
        fileSaveDialog.fileMode = FileDialog.SaveFile

        fileSaveDialog.currentFile = "file:///" + receiver.currentMachineName() + "_" + receiver.currOpenFile()

        fileSaveDialog.open()
    }

    function showFdmPmDlg(filePath)
    {
        idFdmPmDlg.visible = true
        idFdmPmDlg.filePath = filePath
    }

    function showWizardDlg()
    {
        idWorkflow.startWizard()
        //initAdvance()
    }

    function showModelAlwaysIcom(show)
    {
        Constants.bModelAlwaysShow = show
    }

    function printerConfigBtnClicked()
    {
    }

    function supportTabBtnClicked()
    {
    }

    function letModelAlwaysPopShow()
    {
        if(!idLaserView.visible)
        {
            getRecommendModel()
        }
    }

    function modelAlwaysPopReallyShow()
    {
        idModelAlwaysShowDlg.show(idModelAlwaysShow.x + idModelAlwaysShow.width + 10, idModelAlwaysShow.y)
    }

    onClosing:
    {
        var a = idWorkflow.checkUnsavedChanges();
        if(a)
            idWorkflow.uninitialize();
        close.accepted = a;
    }

    function changeFDMView(){
        idPlotterView.visible = false
        idLaserView.visible = false

        paraSettingUI.onFDMView()
        idSwitch.fdmView()
    }

    function changeLaserView(){
        editorContent.visible = false
        showModelAlwaysIcom(false)

        idLaserView.visible = false
        idLaserView.visible = true
        idLaserView.focus = true

        switchModelLayout.visible = true
        idLaserPanel.visible = true

        //idPlotterPanel.visible = false
        idPlotterView.visible = false
        paraSettingUI.onLaserView()
        idSwitch.laserView()
    }

    function changePlotterView(){
        editorContent.visible = false
        showModelAlwaysIcom(false)

        idPlotterView.visible = false
        idPlotterView.visible = true
        idPlotterView.focus = true

        switchModelLayout.visible = true
        idLaserPanel.visible = true
        //idPlotterPanel.visible = false
        idLaserView.visible = false
        paraSettingUI.onPlotterView()
        idSwitch.plotterView()
    }

    AddPrinterDlgNew {
        id: _idAddPrinterDlgNew
        objectName: "AddPrinterDlg"
        visible: false
        property var isFristStart: "0"
        onAddPrinterMachine:
        {
            _idAddPrinterDlgNew.visible = false
            //添加之后自动选中添加的设备
            idConsumables.currentIndex = Qt.binding(function() {return paraSettingUI.getCurPrinterIndex()})
        }
        onUpdatePrinterMachine:
        {
            crealityCloudUI.onPrinterMachineUpdate()
        }
    }

    Component.onCompleted:
    {
        //menu language fuction
        //idWorkflow.kernelUI.initLanguage()
        //bind wizard
        Constants.screenScaleFactor = WizardUI.getScreenScaleFactor()
        WizardUI.setRootObjet(standaloneWindow)
        console.log("binding  WizardUI complete");

        editorContent.visible = true
        idWorkflow.initialize()

        idStatusBar.bind(idWorkflow.jobs)
        //2020/9/03 write by lisugui
        idWorkflow.updateManager.updateFound.connect(onHaveUpdate)
        idWorkflow.updateManager.updateNotFound.connect(onHaveNoUpdate)
        idWorkflow.updateManager.initAutoCheckVersionFlag();
        idWorkflow.updateManager.checkForUpdates();
        //by lisugui 2020/8/25 use to aboutUs menu getVersion
        idWorkflow.kernelUI.currentVersion = idWorkflow.updateManager.getCurrentVersion()
        // system init ok,some user autoTask here.
        idWorkflow.autoTask();

        crealityVersion = idWorkflow.updateManager.getCurrentVersion()
        idWorkflow.loadOver()

        console.log("MainWindow Component complete");
        showMaximized()
    }

    Connections
    {
        target: idWorkflow.kernelUI
        //Triggered when switching language,Handle menu bar switching separately
        onSigChangeMenuLanguage:
        {
        }
        onSigColseAction:
        {
            standaloneWindow.close()
        }
    }

    function previewShowHeightUpdate()
    {
        if(idRightPanelRect.bRightMenuVisible) return
    }

    background:Rectangle
    {
        anchors.fill:parent
        color: Constants.mainBackgroundColor
    }

    ResizeItem {
        enableSize: 8
        anchors.fill: parent
        focus: true
        cursorEnable : standaloneWindow.visibility !== Window.Maximized && standaloneWindow.visibility !== Window.FullScreen
        Component.onDestruction:
        {
            console.log("Resize destory !!!")
        }

        BasicMenuBar
        {
            id: idMenuBar
            width:parent.width
            //myLoad:content
            mainWindow:standaloneWindow
            enabled: Constants.bMenuBarEnabled
            onCloseWindow :
            {
                console.log("onCloseWindow")
                beforeCloseWindow()
                standaloneWindow.close()
            }
            onSkipUrlCrealityCloud:
            {
                actionCommands.getOpt("Creality Cloud Print").execute()
            }
            onSigModelAlwaysPopShow:
            {
                letModelAlwaysPopShow()
            }
        }

        FDMPreviewPanel
        {
            z: 3
            id:idFDMPreview
            visible: false//Constants.mainPreviewShow
            width: 300 * screenScaleFactor
            height: 634 * screenScaleFactor
            anchors.top:idMenuBar.bottom
            anchors.right: parent.right
            anchors.topMargin: 10
            anchors.rightMargin: 10
            slicemodelcontrol: slicePlugin.sliceModel()
            slicercontrol: slicePlugin.slicePlugin()
            fdmMainObj: sliceUI.mainObj()
            objectName: "FDMPreviewPanel"

            onSwitchWifiPrint:
            {
                __LANload.wifiSwitch = true
            }
            onSkipUrlCrealityCloud:
            {
                actionCommands.getOpt("Creality Cloud Print").execute()
            }
            onShowUploadWarningDlg:
            {
                idMenuBar.showUploadMsgDlg("gcodeUpload")
            }
            Connections
            {
                target: slicePlugin
                onSigSendGcodeInfo:
                {
                    __LANload.curGCodePath = gCodePath
                    if(__LANload.wifiSwitch) curState = 2
                    //console.log("__LANload.curGCodePath", gCodePath)
                }
            }
            Connections
            {
                target: sliceUI
                onSigSliceSuccess:
                {
                    Constants.mainPreviewShow = true
                    idFDMPreview.visible = true
                    idFDMPreview.sliceReport = obj
                    idFDMPreview.update()
                    idFDMPreview.startStaus = false
                    curState = 1
                }
            }
        }

        FDMFooterPanel {
            id : idSlicePreviewSelection

            z:3
            anchors.bottom: parent.bottom
            anchors.left: leftToolBar.right
            anchors.right: idFDMPreview.left
            anchors.bottomMargin: 10 * screenScaleFactor
            anchors.leftMargin: 10 * screenScaleFactor
            anchors.rightMargin: 10 * screenScaleFactor
            panelWidth: 830 * screenScaleFactor
            panelHeight: 104 * screenScaleFactor

            visible: idFDMPreview.visible
            previewflow : slicePlugin.sliceModel()

            currentLayer: (slicePlugin.sliceModel().layers>0? slicePlugin.sliceModel().layers-1:0)
            currentStep: slicePlugin.sliceModel().steps >0 ? slicePlugin.sliceModel().steps : 0
            layerMax:(slicePlugin.sliceModel().layers>0?slicePlugin.sliceModel().layers-1:0)
            stepMax:slicePlugin.sliceModel().steps >0 ? slicePlugin.sliceModel().steps : 0

            onLayerSliderChange:
            {
                if(slicePlugin.sliceModel())
                {
                    slicePlugin.sliceModel().slicePreviewSetOp("CurrentLayer", value, randomstep)
                    speedFlag = slicePlugin.sliceModel().slicePreviewGetOp("speedFlags")
                    randomstep = 0

                    idFDMPreview.stepViewObj.model = slicePlugin.sliceModel().getCurrentLayerGCodeList()
                }

            }
            onStepSliderChange:
            {
                if(slicePlugin.sliceModel())
                {
                    slicePlugin.sliceModel().slicePreviewSetOp("CurrentStep", value)
                    speedFlag = slicePlugin.sliceModel().slicePreviewGetOp("speedFlags")

                    idFDMPreview.stepViewObj.lineIndex = slicePlugin.sliceModel().findViewIndexFromStep(value)
                }
            }
            Connections{
                target: standaloneWindow
                onCurStateChanged:{
                    destoryLanPrinterComponet()
                    if(curState===0){
                        //结束预览
                        idSlicePreviewSelection.timebtn.stop()
                    }
                    else if (curState === 1){
                        //开始预览
                    }
                    else if(curState === 2)
                    {
                        showLanPrinterComponet()
                    }
                }
            }
        }

        ReleaseNote {
            id:idReleaseNote
            onAutochecked: {
                idWorkflow.updateManager.autocheck = true;
            }
            onUnautochecked: {
                idWorkflow.updateManager.autocheck = false;
            }
            onSkipVersion: {
                idWorkflow.updateManager.skipVersion(idReleaseNote.version);
            }
        }

        UsbControlDlg
        {
            id: idFdmPmDlg
            visible: false
        }

        QMLWorkflow
        {
            id: idWorkflow
            glQuickItem: editorContent
            kernelUI.leftToolbar: leftToolBar
            kernelUI.appWindow: standaloneWindow
            kernelUI.footer:idStatusBar
            kernelUI.rightPanel: idRightPanelRect
            kernelUI.glMainView: editorContent
            kernelUI.topbar: idMenuBar
            Component.onCompleted:
            {
                console.log("QMLWorkflow finished !!!")
            }
        }

        Item
        {
            id:idSplitView
            anchors.top: idMenuBar.bottom
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            //orientation: Qt.Horizontal
            LaserScene {
                id:idLaserView
                anchors.fill: parent
                objectName: "laserView"
                visible: false
                winWidth: standaloneWindow.width
                winHeight: standaloneWindow.height
                Keys.onPressed: {
                    if(idLaserView.visible || idPlotterView.visible)
                    {
                        switch(event.key) {
                        case Qt.Key_Delete:
                            if(idLaserView.visible)
                            {
                                idLaserView.deleteSelectObject()
                            }
                            if(idPlotterView.visible)
                            {
                                idPlotterView.deleteSelectObject()
                            }


                        }
                    }
                }
                onUpperObj:
                {
                    idLaserPanel.upperObject()
                }

                onBottomObj:
                {
                    idLaserPanel.bottomObject()
                }

            }
            PlotterScene {
                id:idPlotterView
                anchors.fill: parent
                objectName: "plotterView"
                visible: false
                winWidth: standaloneWindow.width
                winHeight: standaloneWindow.height
                Keys.onPressed: {
                    if(idLaserView.visible || idPlotterView.visible)
                    {
                        switch(event.key) {
                        case Qt.Key_Delete:
                            if(idLaserView.visible)
                            {
                                idLaserView.deleteSelectObject()
                            }
                            if(idPlotterView.visible)
                            {
                                idPlotterView.deleteSelectObject()
                            }


                        }
                    }
                }
                onUpperObj:
                {
                    idPlotterPanel.upperObject()
                }

                onBottomObj:
                {
                    idPlotterPanel.bottomObject()
                }
            }

            GLQuickItem
            {
                id: editorContent
                anchors.fill: parent
                visible: false
                objectName: "gLQuickItem"
                enabled: Constants.bGLViewEnabled
                z:-1
                function clearRClickMenuData()
                {
                    console.log("clearRClickMenuData")
                    var menuObj = contextMenu
                    var objListCount = menuObj.contentModel.count
                    for(var i=objListCount -1;i>=0;i--)
                    {
                        //takeItem is remove and return
                        var menuItemObj = menuObj.takeItem(i);
                        var subMenuObj = menuObj.takeMenu(i);
                    }
                }
                function requstShowRightMenu()
                {
                    console.log("requstShowRightMenu")
                    contextMenu.visible = idRightPanelRect.bRightMenuVisible
                    clearRClickMenuData()

                    if(idRightPanelRect.bRightMenuVisible)
                    {
                        contextMenu.initMenu()
                        contextMenu.popup()
                    }
                }
                BasicRClickMenu
                {
                    id:contextMenu
                    objectName: "rightbtnmenu"
                }
                Component.onCompleted:
                {
                    console.log("GLQuickItem finished !!!")
                }
                Component.onDestruction:
                {
                    console.log("GLQuickItem destory !!!")
                }
            }

            ColumnLayout {
                id:switchModelLayout
                objectName: "switchModelLayout"
                anchors.right: parent.right
                anchors.rightMargin: 0

                SwitchModel {
                    id:idSwitch
                    width: Constants.rightPanelWidth * screenScaleFactor
                    height: 45//50
                    objectName: "switchModel"
                    visible: false
                    property bool isShow: false
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    border.color: "yellow"
                    border.width: 2
                    onWidthChanged:{
                        if(isShow)
                        {
                            if(width<10)
                            {
                                visible=false
                            }
                            else
                            {
                                visible= true
                            }
                        }
                    }
                }

//                Item {
//                    id:idOtherRightItem
////                    implicitWidth: Constants.rightPanelWidth
//                    width: Constants.rightPanelWidth
//                    height: 602//Constants.rightPanelHeight
//                    clip: true
//                    objectName: "OtherRightItem"
//                    anchors.right: parent.right
//                    visible: idSwitch.visible
//                }
                Item {
                    id: idRIghtItem
                    width: 280 * screenScaleFactor
                    height: parent.height + 10
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    objectName: "rightItemObj"
                    enabled: Constants.bRightPanelEnabled
                    visible: !idSwitch.visible

                    Column{
                        anchors.top : parent.top
                        anchors.topMargin : 10
                        spacing: 4
                        width: parent.width
                        BasicTabBar {
                            id : idTabBar
                            property int tabBtnState: idTabBar.currentIndex+1
                            property int curMachineSeries: 0
                            property var type1: [qsTr("FDM")]
                            property var type2: [qsTr("FDM"), qsTr("Laser")]
                            property var type3: [qsTr("FDM"), qsTr("Laser"), qsTr("CNC")]
                            property var curType: type2
                            visible: !(curType === type1)
                            width : parent.width
                            height: 30 * screenScaleFactor
                            backgroundColor: Constants.right_panel_tab_background_color
                            currentIndex : 0
                            onCurrentIndexChanged: {
                                standaloneWindow.functionType = idTabBar.currentIndex
                                if (idTabBar.currentIndex === 0) {
                                    changeFDMView();
                                } else if (idTabBar.currentIndex === 1) {
                                    changeLaserView();
                                } else if (idTabBar.currentIndex === 2) {
                                    changePlotterView();
                                }
                            }

                            Repeater {
                                id:btnRepeater
                                model: idTabBar.curType
                                delegate: BasicTabButton {
                                    width: idTabBar.width / model.size
                                    height: parent.height
                                    buttonColor: checked ? Constants.right_panel_tab_button_checked_color
                                                         : Constants.right_panel_tab_button_default_color
                                    fontText: modelData
                                    textColor : checked ? Constants.right_panel_tab_text_checked_color
                                                        : Constants.right_panel_tab_text_default_color
                                    lBorderwidth:1
                                    tBorderwidth:1
                                    onClicked: {
                                        idTabBar.currentIndex = model.index
                                    }

                                    Component.onCompleted: {
                                        idTabBar.currentIndex = 0
                                    }
                                }
                            }
                        }

                        Rectangle{
//                            id : idPrinter
                            width: Constants.rightPanelWidth * screenScaleFactor
                            height: 590 * screenScaleFactor
                            radius: 5
                            color: Constants.right_panel_menu_border_color

                            Rectangle {
                                id: idPrinterInternal
                                anchors.fill: parent
                                anchors.topMargin: 4
                                anchors.bottomMargin: 1
                                anchors.leftMargin: 1
                                anchors.rightMargin: 1
//                                anchors.margins: 1

                                Layout.alignment: Qt.AlignTop

                                radius: parent.radius
                                color: Constants.right_panel_menu_background_color

                                Column {
                                    id: rightPanelView
                                    Layout.alignment: Qt.AlignTop
                                    spacing: 10

                                    PanelComponent{
                                        imgUrl: "qrc:/UI/photo/printerPanel.png"
                                        title: qsTr("Printer")
                                    }

                                    BasicCombobox_Metarial
                                    {
                                        id : idConsumables
                                        width: 260 * screenScaleFactor
                                        height: 28 * screenScaleFactor
                                        Layout.alignment: Qt.AlignHCenter
                                        anchors.horizontalCenter: parent.horizontalCenter
                                        clip: true
                                        model: paraSettingUI.printerList
                                        currentIndex : paraSettingUI.getCurPrinterIndex()

                                        onAddMaterialClick:{
                                            _idAddPrinterDlgNew.visible = true
                                            //切换机型，触发材料combox的index或者内容的修改
                                        }

                                        onSigManagerClick:{
                                            idManagerPrinterWizard.visible = true
                                        }
                                        onCurrentTextChanged:{

                                        }

                                        //切换item的时候调用一次
                                        onActivated:
                                        {
                                            paraSettingUI.onMachineChanged(currentText)
                                            var machineSeries = paraSettingUI.getMachineSeries(currentText);
                                            idTabBar.curMachineSeries = machineSeries

                                            if(idTabBar.curMachineSeries == 1)
                                                idTabBar.curType = idTabBar.type1
                                            else if(idTabBar.curMachineSeries == 2)
                                                idTabBar.curType = idTabBar.type2
                                            else
                                                idTabBar.curType = idTabBar.type3

                                            idTabBar.currentIndex = 0
                                            standaloneWindow.functionType = 0
                                            changeFDMView();
                                        }

                                        //初始化的时候调用一次
                                        Component.onCompleted: {
                                            paraSettingUI.onMachineChanged(currentText)
                                            var machineSeries = paraSettingUI.getMachineSeries(currentText);
                                            idTabBar.curMachineSeries = machineSeries

                                            if(idTabBar.curMachineSeries == 1)
                                                idTabBar.curType = idTabBar.type1
                                            else if(idTabBar.curMachineSeries == 2)
                                                idTabBar.curType = idTabBar.type2
                                            else
                                                idTabBar.curType = idTabBar.type3

                                            idTabBar.currentIndex = 0
                                            standaloneWindow.functionType = 0
                                            changeFDMView();
                                        }
                                    }

                                    StackLayout {
                                        id: swipeView
                                        width:parent.width
                                      height: 460 * screenScaleFactor
                                        currentIndex:idTabBar.currentIndex
                                        
                                        Item {
                                            id: idRightPanelRect
                                            property bool bRightMenuVisible: true
                                            objectName :"idRightPanelRect"

                                            function  sigShowRightMenu(visible)
                                            {
                                                bRightMenuVisible = visible
                                            }

                                            function mainWindowSliceShow()
                                            {
//                                                defultRightPanelHeight = 670/**/
                                                //                                    swipeView.height = Qt.binding(function(){return defultRightPanelHeight})
                                                Constants.mainPreviewShow = false
                                            }

                                            function mainWindowPreviewShow()
                                            {
                                                previewShowHeightUpdate()
                                                Constants.mainPreviewShow = true

                                                //隐藏
                                                idRIghtItem.visible = false

                                                //切片成功再显示显示
                                                //                                    idFDMPreview.visible = true
                                            }

                                            function mainPreviewChanged()
                                            {
                                                if(idTabBar.tabBtnState == 2){
                                                    CXXGlobal.previewMain()
                                                }
                                                else{
                                                }
                                                idTabBar.tabBtnState = 1
                                                idSupportTabBtn.supportSelected(false)
                                                idMenuBar.letSelectPrinterEnable(true)
                                                Constants.bModelAlwaysShow = true
                                            }
                                        }
                                        LaserPanel {
                                            id: idLaserPanel
                                            objectName: "objLaserItem"

                                            color: idPrinterInternal.color

                                            control: laserScene
                                            //                                itemModel: laserScene.drawObjectModel()
                                            settingsModel: laserScene.laserSettings()
                                            onVisibleChanged:{
                                                if(idLaserPanel.visible)
                                                {
                                                    idLaserPanel.updateImageInfo()
                                                }

                                                //                                    if(idLaserPanel.visible === false && idPlotterPanel.visible === false)
                                                //                                    {
                                                //                                        infoPanel.visible = false
                                                //                                    }
                                            }
                                            //                                onShowInfopanle:{
                                            //                                    infoPanel.visible = bShow
                                            //                                    if(infoPanel.visible)
                                            //                                    {
                                            //                                        infoPanel.modelName = imageName
                                            //                                        infoPanel.modelXSize = imageW
                                            //                                        infoPanel.modelYSize = imageH
                                            //                                        console.log("1111111111111", modelXSize, " ", modelYSize)
                                            //                                    }
                                            //                                }
                                            onDelObjects:{
                                                idLaserView.deleteSelectObject()
                                            }
                                        }
                                        PlotterPanel {
                                            id: idPlotterPanel
                                            objectName: "objPlotterItem"
                                            winWidth:standaloneWindow.width
                                            winHeight:standaloneWindow.height

                                            color: idPrinterInternal.color

                                            onVisibleChanged:{
                                                if(idPlotterPanel.visible)
                                                {
                                                    idPlotterPanel.updateImageInfo()
                                                }

                                                //                                    if(idLaserPanel.visible === false && idPlotterPanel.visible === false)
                                                //                                    {
                                                //                                        infoPanel.visible = false
                                                //                                    }
                                            }
                                            //                                onShowInfopanle:{
                                            //                                    infoPanel.visible = bShow
                                            //                                    if(infoPanel.visible)
                                            //                                    {
                                            //                                        infoPanel.modelName = imageName
                                            //                                        infoPanel.modelXSize = imageW
                                            //                                        infoPanel.modelYSize = imageH
                                            //                                    }
                                            //                                }
                                            onDelObjects:{
                                                idPlotterView.deleteSelectObject()
                                            }
                                        }

                                        Item {
                                            id: idSupportTabBtn
                                            objectName :"idSupportTabBtn"
                                            enabled : bTabClicked
                                            property bool bTabClicked: false
                                            signal supprotExecute()
                                            signal supportSelected(bool selectState)
                                            signal supportModelChange()
                                            Component.onCompleted:
                                            {
                                                //                                    supprotBtn.onClicked.connect(myShow)
                                            }
                                            function setSupportBtnEnabled(enabled)
                                            {
                                                bTabClicked = enabled
                                            }
                                            function myShow()
                                            {
                                                supprotExecute()
                                                idTabBar.tabBtnState = 2
                                                supportSelected(true)
                                                idMenuBar.letSelectPrinterEnable(false)
                                                Constants.bModelAlwaysShow = false
                                                idSupportTimer.start()
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        CusImglButton {
            id: openFileBtn
            enabledIconSource:"qrc:/UI/photo/openIcon.png"
            pressedIconSource:"qrc:/UI/photo/openIcon_d.png"
//            borderColor: "#3e4448"
            //            txtWeight: Font.Bold
            visible: idModelAlwaysShow.visible
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: idMenuBar.bottom
            anchors.topMargin: 10
            width: 140*screenScaleFactor
            height: 40*screenScaleFactor
            text: qsTr("Open File")
            state: "imgLeft"
            textAlign: 0
            onClicked:{
                actionCommands.getOpt("Open File").execute()
            }
        }

        CusImglButton
        {
            id: idModelAlwaysShow
            property alias selected: idModelAlwaysShow.bottonSelected
            anchors.top: idMenuBar.bottom
            anchors.topMargin : 10 * screenScaleFactor//50
            anchors.left: idMenuBar.left
            anchors.leftMargin: 10 * screenScaleFactor
            text: qsTr("Models")
            width: 58 *  screenScaleFactor
            height: 58 * screenScaleFactor
            borderWidth : 0
           // btnImgUrl: idModelAlwaysShowDlg.visible ? "qrc:/UI/photo/model_always_show_h.png" : (hovered ? "qrc:/UI/photo/model_always_show_h.png" : Constants.modelAlwaysBtnIcon)
            visible: standaloneWindow.curState == 0 && standaloneWindow.functionType == 0//Constants.bModelAlwaysShow
            state : "imgTop"
            enabledIconSource  : Constants.modelAlwaysBtnIcon
            highLightIconSource : "qrc:/UI/photo/model_always_show_h.png"
            pressedIconSource   : "qrc:/UI/photo/model_always_show_h.png"
            onClicked: {
                if(idModelAlwaysShowDlg.visible === false && idModelAlwaysShowDlg.visibleFlag == false)
                {

                    getRecommendModel()
                }
                else{

                    idModelAlwaysShowDlg.hide()
                }
            }

            onSelectedChanged: {
                if(selected )
                {
                    Constants.leftShowType = 1
                    if(mlcBtn.selected)
                        mlcBtn.selected = false
                }
                else
                {
                    Constants.leftShowType = 0
                }
                //if(mlcBtn.popDialogVisible) mlcBtn.popDialogVisible = false
            }
        }

        Connections{
            target: standaloneWindow
            onCurStateChanged: {
                //状态改变的时候进行一些组件状态的控制
                if(curState === 0)
                {
                    mlcBtn.enabled = true
                    mcBtn.enabled = true
                }
                else if(curState === 1)
                {
                    mlcBtn.enabled = false
                    mcBtn.enabled = false
                }else
                {
                    mlcBtn.enabled = false
                    mcBtn.enabled = false
                }
            }
        }

        ModelListCombo {
            id:mlcBtn
            anchors.top: idModelAlwaysShow.bottom
            anchors.left: idModelAlwaysShow.left
            anchors.topMargin: 10 * screenScaleFactor
            width: idModelAlwaysShow.width
            height: idModelAlwaysShow.height
            visible: standaloneWindow.curState == 0 && standaloneWindow.functionType == 0

            onSelectedChanged: {
                if(selected)
                {
//                    leftToolBar.switchPickMode()//Switch to Pick
                    Constants.leftShowType = 2
                    if(idModelAlwaysShow.selected)
                    {

                        idModelAlwaysShow.selected = false
                    }
                }
                else
                {
                    if(!mlcBtn.selected)
                        Constants.leftShowType = 0
                }
            }
        }

        LeftToolBar
        {
            id: leftToolBar
            property var enabledState: true
            mymodel: idWorkflow.kernelUI.lmodel
            anchors.top: mlcBtn.bottom
            anchors.topMargin: 10 * screenScaleFactor
            anchors.left: parent.left
            anchors.leftMargin: 10 * screenScaleFactor
            width: idModelAlwaysShow.width
            objectName: "leftToolObj"
            visible: standaloneWindow.curState == 0 && standaloneWindow.functionType == 0
            enabled: mlcBtn.modelNumber > 0 && enabledState
            Connections{
                target: idWorkflow.kernelUI
                onSigLeftToolBarVisible: {
//                    leftToolBar.opacity = isVisible ? 1 : 0.5
                    leftToolBar.enabledState = isVisible
                    leftToolBar.switchShowPopup(isVisible)

                    standaloneWindow.hasModel = isVisible
                }
            }
        }

        Timer//by TCJ
        {
            id:idSupportTimer
            interval: 500 //定时周期
            repeat:false
            triggeredOnStart: false
            onTriggered: {
                idSupportTabBtn.supportModelChange()
            }
        }

        MouseCombo{
            id: mcBtn
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            width: idModelAlwaysShow.width
            height: idModelAlwaysShow.height
//            radius: idModelAlwaysShow.radius
            visible: false // standaloneWindow.curState == 0 && standaloneWindow.functionType == 0

        }

        StyledStatusBar
        {
            id:idStatusBar
            objectName: "StatusBarObj"
            width: (standaloneWindow.width * 0.46) < 450 *  screenScaleFactor ? 450 *  screenScaleFactor : (standaloneWindow.width * 0.46)
            height: (standaloneWindow.height * 0.074) < 60 *  screenScaleFactor ? 60 *  screenScaleFactor: standaloneWindow.height * 0.074
        }

        FileDialog
        {
            id: fileDialog
            property var receiver
            fileMode: FileDialog.OpenFiles
            onAccepted:
            {
                //if(receiver) receiver.load(files)
                GlobalCache.meshImportFolder = folder
                if (fileDialog.files.length === 1){
                    receiver.fileOpen(fileDialog.files)
                }else{
                    console.log(files.length)
                    receiver.filesOpen(fileDialog.files)
                }
                receiver = null
            }

            onRejected:
            {
                console.log("open onRejected Canceled")
                receiver.cancelHandle()
            }
        }

        FileDialog
        {
            id: fileSaveDialog
            property var receiver
            onAccepted:
            {
                GlobalCache.meshImportFolder = folder
                console.log("folder ====" + folder)
                //console.log(files)
                receiver.fileSave(files)
                receiver = null
            }

            onRejected:
            {
                console.log("save onRejected Canceled")
                receiver.cancelHandle()
            }
        }

        AllMenuDialog
        {
            objectName: "allMenuDialog"
        }
    }
    onCurStateChanged: {
        //状态改变的时候进行一些组件状态的控制
        if(curState === 0)
        {
            //console.log("11111111111111 = ", curState)
            idRIghtItem.visible = true
//            leftToolBar.enabledState = true
        }
        else
        {
            idRIghtItem.visible = false
        }
    }

    BasicDialog
    {
        id: idUpdateDlg
        width: 400
        height: 200
        titleHeight : 30
        title: qsTr("Version Update")

        Rectangle{
            anchors.centerIn: parent
            width: parent.width/2
            height: parent.height/2
            color: "transparent"
            Text {
                id: name
                anchors.centerIn: parent
                font.family: Constants.labelFontFamily
                font.weight: Constants.labelFontWeight
                text: qsTr("You are currently on the latest version and do not need to update")
                font.pixelSize: Constants.labelFontPixelSize
                color:Constants.textColor
            }
        }
    }

    Timer{
        id: idModelALwaysShowDlgTimer
        interval: 200
        repeat: false
        onTriggered:{
            idModelAlwaysShowDlg.visibleFlag = false
        }

    }

    BasicMessageDialog
    {
        id: idDoubleMessageDlg
        width_x : 480
        property var receiver
        property var indexs
        property var messageDlgType
        onAccept:
        {
            if("deleteSelectModel" === messageDlgType)
            {
                if(receiver)
                {
                    receiver.remove(indexs)
                    if(checkedFlag){
                        receiver.writeReg(false)
                    }
                }
            }
            else if("deleteSelectMachine" === messageDlgType)
            {
                paramSetUI.onMachineDeleted(indexs)
                //                if(receiver)
                //                {
                //                    machineDelete(indexs)
                //                    if(checkedFlag){
                //                        receiver.writeReg(false)
                //                    }
                //                    idManagerPrinterWizard.visible = false
                //                }
            }
            else if("deleteSelectProfile" === messageDlgType)
            {
                if(receiver)
                {
                    deleteProfilebtn()
                    if(checkedFlag){
                        receiver.writeReg(false)
                    }
                }
            }

        }
        onCancel:
        {
            close()
        }
    }

    ManagerPrinterDlg
    {
        id: idManagerPrinterWizard
        visible: false
        objectName: "managerPrinterDlg"
        cmbModel: paraSettingUI.printerList//idSelectPrinter.model
        //        currentCmbIndex: paraSettingUI.getCurPrinterIndex()
        onVisibleChanged:{
            if(visible){
                console.log("33333333333333")
                currentCmbIndex = paraSettingUI.getCurPrinterIndex()
            }
        }

        onDelMachine:function(machineName)
        {
            console.log("machineName = " + machineName)
            //machineDelete(machineName)
            /*if(loadercontrol.isPopPage())*/{
                idDoubleMessageDlg.isInfo = false
                idDoubleMessageDlg.showDoubleBtn()
                idDoubleMessageDlg.showCheckBox(true)
                //                idDoubleMessageDlg.receiver = loadercontrol
                idDoubleMessageDlg.indexs = machineName
                idDoubleMessageDlg.messageDlgType = "deleteSelectMachine"
                idDoubleMessageDlg.messageText = qsTr("Are you sure to delete this printer?")
                idDoubleMessageDlg.show()
            }
            /*else*/{
                //                paraSettingUI.onMachineDeleted(machineName)
                //                idManagerPrinterWizard.visible = false
            }
        }

        onValuechanged:
        {
            //                        editProfiledchange(key,value)
            paraSettingUI.onValchanged(key, value)
        }
        onSaveConfig:
        {
            //            saveManagerConfig();
        }
        onSaveExtruder:
        {
            paraSettingUI.onSaveExtruderConfig(nextruder)
        }
        onResetConfig:
        {
            console.log("resetManagerConfig")
            //            resetManagerConfig();
            paraSettingUI.resetMachineParam(machineName)
            //            paraSettingUI.onManagerMachine(machineName)
        }
        onAddNewPrinterConfig:
        {
            console.log("onAddNewPrinterConfig")
            console.log(str)
            addNewPrinter(str)
        }
        onMachineChange: {
            console.log("right onMachineChange" + name)
            //            managerMachineChange(name)
        }
    }

    ModelAlwaysShowPopupDlg {
        id: idModelAlwaysShowDlg
        property var visibleFlag: false
        objectName: "idModelAlwaysShowDlg"
        visible: standaloneWindow.curState == 0 && standaloneWindow.functionType == 0 && Constants.leftShowType === 1

        onVisibleChanged:
        {
            idModelAlwaysShow.selected = visible
            if(idModelAlwaysShowDlg.visible === false)
            {
                idModelALwaysShowDlgTimer.restart()
                idModelAlwaysShowDlg.visibleFlag = true
            }
        }

        onSigModelAlwaysShowItemClick:
        {
            idModelAlwaysShowDlg.hide()
            showModelLibraryDlg(isTableShow, modelGroupId, btnNameText, modelCount, btnAuthorText, btnAvtarImage, modelTime)
        }

        Component.onCompleted: {
            // getRecommendModel()
        }
    }


}
