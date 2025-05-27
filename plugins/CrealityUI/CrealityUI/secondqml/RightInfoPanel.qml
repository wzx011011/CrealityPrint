import QtQuick 2.10
import QtQuick.Controls 2.0
import QtQuick.Dialogs 1.1

import "../qml"
Rectangle {
    id:idItem

    property bool isRightShow : true
    property color defaultBgColor: Constants.itemBackgroundColor//"#061F3B"
    property var loadercontrol
    property var slicercontrol
    property string currentMachineType
    color: defaultBgColor
    border.width: 1
    border.color: Constants.rightPanelRectBgBorderColor
   // implicitWidth: parent.implicitWidth
    property alias enableSlice: idSliceBtn.enabled

    property alias managerDlpvisable: idManagerPrinterWizard.managerDlpvisable
    property alias managerFdmvisable: idManagerPrinterWizard.managerFdmvisable
    property alias currentPrinterText: idManagerPrinterWizard.currentCmbText
    property alias currentPrinterIndex: idManagerPrinterWizard.currentCmbIndex

    property int defultHeight: 740
    signal machineAdded(string machineName)
    signal machineDelete(string machineName)
    signal sigChangePreview()

    signal editProfiled()
    signal editAddProfiled()
    signal editProfiledchange(string key,string value)
    signal sigFoldBtn()
    signal resetbtn()
    signal saveProfilebtn()
    signal deleteProfilebtn()

    signal managerMachine()
    signal resetManagerConfig()
    signal saveManagerConfig()
    signal addNewPrinter(string newprinter)
    signal managerMachineChange(string name)

    signal sigMangerPrinterClicked()
    signal sigEditProfileClicked()
    signal sigAddProfileClicked()

    //save extruder
    signal saveExtruderConfig(int nextruder)
    radius: 0
    anchors.fill: parent
    function showWizards()
    {
        console.log("idSelectPrinter.cmbCount =" + idSelectPrinter.cmbCount)
        idMachineeWizard.cmbCount = idSelectPrinter.cmbCount
        idMachineeWizard.visible=true
    }

    function showEdit()
    {
        sigEditProfileClicked()
        updateProfileModel()

        editProfileWizard.visible=true
		console.log("showEdit SettingJson.initSettingData")
    }

    function updateProfileModel()
    {
        editProfiled()
        SettingJson.initSettingData()
        editProfileWizard.resetValue()
    }

    function updateProfileLanguage()
    {
        editProfileWizard.updateLanguage()
    }

    function showAddEdit()
    {
        sigAddProfileClicked()
         addeditprofile.visible = true
        //editProfileWizard.visible=true
        editAddProfiled()
    }
    function showManagerMachine()
    {
        idManagerPrinterWizard.currentCmbIndex = idSelectPrinter.currentCmbIndex
        managerMachine()
        idManagerPrinterWizard.visible = true
    }
    function showManagerMachine2(index,model)
    {
        idManagerPrinterWizard.cmbModel = model
        idManagerPrinterWizard.currentCmbIndex = index
        managerMachine()
        idManagerPrinterWizard.visible = true
    }

    function showWarring()
    {
         messageDialog.visible = true
    }

    function enableSliceBtn()
    {
        idSliceBtn.enabled = true
    }

    function uploadModelDlgCombobox(data, isLocalFile){
        idUploadModelDlg.insertListModeData(data, isLocalFile)
        idUploadModelDlg.show()
    }
    function uploadModelBtnClick(type)
    {
        if(type == "modelUpload")
        {
            slicercontrol.uploadModelBtnClick()
        }
        else if(type == "modelLocalFileUpload")
        {
            slicercontrol.uploadModelLocalFileBtnClick()
        }
    }
    function updateUploadProgressValue(value)
    {
        idUploadModelDlg.progressValue = value
    }
    function uploadModelSuccess()
    {
        idUploadModelDlg.uploadModelSuccess()
    }

    function uploadModelError()
    {
        idUploadModelDlg.visible = false
        idUploadModelDlg.uploadModelFailed()
        idUploadModelFailedDlg.visible = true
    }

    function initAdvanceParam()
    {
        editProfileWizard.initEditProfileByFilter()
    }

    function sliceClickFromSupport()
    {
        console.log("sliceClickFromSupport!!!!!")
        idSliceBtn.sigButtonClicked()
    }

    Grid
    {//by TCJ
        id: idCol
        spacing: 18//20
        x: 10
        /*topPadding : 20//生效*/
          y: 10
        width: parent.width - 20
        rows: 8
        objectName:"rightGrid"
        /*anchors.fill: parent*/
        SelectPrinter/*选择打印机*/
        {
            id:idSelectPrinter
            visible:false
            objectName:"selectMachine"
            title: qsTr("Printer")
            font.pixelSize: Constants.panelLargeFontPixelSize//16
            font.family: Constants.panelFontFamily
            font.bold: true
            width: parent.width  //260
            height: 100/*90*/
//            visible: true
            onAddPrinter:{
                showWizards()
            }
            onManagerPrinter:{
               sigMangerPrinterClicked()
               showManagerMachine()
            }
        }

        ModelList/*模型列表*/
        {
            title: qsTr("Model")
            objectName:"selectModel"
            font.pixelSize: Constants.panelLargeFontPixelSize//16
            font.family: Constants.panelFontFamily
            font.bold: true
            width: parent.width
            height:230//230 + (defultHeight - 700)/2
            onAddModel:{
                loadercontrol.add()
            }
            onUploadModel: {
                slicercontrol.uploadModelBtnClick()
            }
            onUploadModelLocalFile:
            {
                slicercontrol.uploadModelLocalFileBtnClick()
            }
            onDelModels:function(indexs){
                if(loadercontrol.isPopPage()){
                    idDoubleMessageDlg.isInfo = false
                    idDoubleMessageDlg.showDoubleBtn()
                    idDoubleMessageDlg.showCheckBox(true)
                    idDoubleMessageDlg.receiver = loadercontrol
                    idDoubleMessageDlg.indexs = indexs
                    idDoubleMessageDlg.messageDlgType = "deleteSelectModel"
                    idDoubleMessageDlg.messageText = qsTr("Do you Want to Delete SelectModel?")
                    idDoubleMessageDlg.show()
                }
                else
                    loadercontrol.remove(indexs)
            }
            onSelectModels:function(indexs){
                loadercontrol.selects(indexs)
            }
            onCenterAndLayout:{
                loadercontrol.layoutAll()
            }
			onLabelShow: {
				idSliceBtn.enabled = bLabelShow ? false : true
                idModels.supportBtnEnable = bLabelShow ? false : true
			}
        }
		
        Item {
            width: idItem.width - 2
            height : 2
            
            Rectangle {
                // anchors.left: idCol.left
                // anchors.leftMargin: -10
                x: -9
                width:parent.width > parent.height ?  parent.width : 2
                height: parent.width > parent.height ?  2 : parent.height
                color: Constants.splitLineColor
                Rectangle {
                    width: parent.width > parent.height ? parent.width : 1
                    height: parent.width > parent.height ? 1 : parent.height
                    color: Constants.splitLineColorDark
                }
            }
        }

        SelectPrinterModel/*选择参数配置*/
        {
            id:idModels
            title: qsTr("Profile")
            objectName: "selectProfile"
            font.pixelSize: Constants.panelLargeFontPixelSize//16
            font.family: Constants.panelFontFamily
            font.bold: true
            width:parent.width
            height:260//260 + (defultHeight- 700)/2
            model:objModel
            Component.onCompleted:{
                sigeditProfile.connect(showEdit)
                sigprofileModelChanged.connect(updateProfileModel)
                sigaddeditProfile.connect(showAddEdit)
            }
            onSigeditDelete:{
                //deleteProfilebtn()
                if(loadercontrol.isPopPage()){
                    idDoubleMessageDlg.isInfo = false
                    idDoubleMessageDlg.showDoubleBtn()
                    idDoubleMessageDlg.showCheckBox(true)
                    idDoubleMessageDlg.receiver = loadercontrol
                    idDoubleMessageDlg.messageDlgType = "deleteSelectProfile"
                    idDoubleMessageDlg.messageText = qsTr("Are you sure to delete this configuration profile?")
                    idDoubleMessageDlg.show()
                }
                else{
                    deleteProfilebtn()
                }
            }
            onSiggotoSupportTab:
            {
                loadercontrol.gotoSupportTab()
            }
        }

        Item {
            width: idItem.width - 2
            height : 2
            
            Rectangle {
                // anchors.left: idCol.left
                // anchors.leftMargin: -10
                x: -9
                width:parent.width > parent.height ?  parent.width : 2
                height: parent.width > parent.height ?  2 : parent.height
                color: Constants.splitLineColor
                Rectangle {
                    width: parent.width > parent.height ? parent.width : 1
                    height: parent.width > parent.height ? 1 : parent.height
                    color: Constants.splitLineColorDark
                }
            }
        }

        Item {
            width: parent.width
            height: 40

            SliceExecute
            {
                id:idSliceBtn
                text : qsTr("Start Slice")
                anchors.horizontalCenter: parent.horizontalCenter
                width:parent.width-20/*40*/
                //left: 20
                //anchors.left:parent.left
                //anchors.leftMargin: 20
                //anchors.horizontalCenter: parent.horizontalCenter
                height:48
                btnRadius : 3
				enabled : false
                onSigButtonClicked: {
                    var ret = slicercontrol.outPlatform()
                    if(ret)
                    {
                        idOutPlatform.visible = true
                    }
                    else{
                        SettingJson.updateGlobalSettingValue()
                        console.log("SliceExecute success")
                        //                sigChangePreview();
                        if(slicercontrol.sliceMainOp("detectFDMsupports") > 0)
                        {
                            idSliceBtn.enabled=false
                            //messageDialogdetect.visible=true //去除提示框 bug:001010026
                            slicercontrol.sliceMainOp("slice")
                        }
                        else
                        {
                            idSliceBtn.enabled=false
                            slicercontrol.sliceMainOp("setBottomForDLP")
                            slicercontrol.sliceMainOp("slice")
                        }
                    }
					
                }
            }
        }
    }

    UploadMessageDlg{
        id: idOutPlatform
        visible: false
        cancelBtnVisible: false
        messageType:0
        msgText: qsTr("Model out of range, please put 'red' model in the printer!")

        onSigOkButtonClicked:
        {
            idOutPlatform.visible = false
        }
    }

    UploadMessageDlg{
        id:messageDialogdetect
        messageType:1
        msgText: qsTr("Manual supports have been added but support option is disabledin the template, \n do you want to add support structures?")
        visible: false
        onSigOkButtonClicked:
        {
            messageDialogdetect.close()
            slicercontrol.sliceMainOp("slice")
        }
        onSigCancelButtonClicked:
        {
            messageDialogdetect.close()
            slicercontrol.sliceMainOp("cancelFDMsupports")
            slicercontrol.sliceMainOp("slice")
        }
    }


    // AddPrinterDlg {
    //     id:idMachineeWizard
    //     visible:false
    //     onAddMachine:function(machineName){
    //         console.log(machineName)
    //         machineAdded(machineName)
    //     }
    // }
    ManagerPrinterDlg
    {
        id:idManagerPrinterWizard
        visible: false

        cmbModel: idSelectPrinter.model
        onDelMachine:function(machineName)
        {
            console.log("machineName = " + machineName)
            //machineDelete(machineName)
            if(loadercontrol.isPopPage()){
                idDoubleMessageDlg.isInfo = false
                idDoubleMessageDlg.showDoubleBtn()
                idDoubleMessageDlg.showCheckBox(true)
                idDoubleMessageDlg.receiver = loadercontrol
                idDoubleMessageDlg.indexs = machineName
                idDoubleMessageDlg.messageDlgType = "deleteSelectMachine"
                idDoubleMessageDlg.messageText = qsTr("Are you sure to delete this printer?")
                idDoubleMessageDlg.show()
            }
            else{
                machineDelete(machineName)
                idManagerPrinterWizard.visible = false
            }
        }
        onValuechanged:
        {
            editProfiledchange(key,value)
        }
        onSaveConfig:
        {
            saveManagerConfig();
        }
        onSaveExtruder:
        {
            saveExtruderConfig(nextruder)
        }
        onResetConfig:
        {
            console.log("resetManagerConfig")
            resetManagerConfig();

        }
        onAddNewPrinterConfig:
        {
            console.log("onAddNewPrinterConfig")
            console.log(str)
            addNewPrinter(str)
        }
        onMachineChange: {
            console.log("right onMachineChange" + name)
            managerMachineChange(name)
        }
    }
    EditProfile{
        id:editProfileWizard
        objectName:"editprofileobj"
        visible: false

        onValchanged:
        {
            editProfiledchange(key,value)
        }
        onReseted: {
            resetbtn()
			console.log("showEdit SettingJson.initSettingData")
			SettingJson.initSettingData()
        }
        onSaveProfile:{
            saveProfilebtn()
        }

    }

    AddEditProfile{
        id:addeditprofile
        objectName:"addeditprofileobj"
        visible: false

        onOpenEditMenu:
        {
            addeditprofile.visible = false

            editProfiled()
            editProfileWizard.visible=true
            SettingJson.initSettingData()
            editProfileWizard.resetValue()
            console.log("open edit")
        }
        onOpenWarring:
        {
            console.log("onOpenWarring")
            //messageDialog.open()
            messageDialog.visible=true

        }
    }

    BasicDialog
    {
        id: messageDialog
        width: 400
        height: 200
        titleHeight : 30
        title: qsTr("Message")
        visible: false
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
                text: qsTr("The profile already exists! please enter again.")
                font.pixelSize: Constants.labelFontPixelSize
                color: Constants.textColor
            }
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
                if(receiver)
                {
                    machineDelete(indexs)
                    if(checkedFlag){                  
                        receiver.writeReg(false)
                    }
                    idManagerPrinterWizard.visible = false
                }
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



    UploadModelDlg{
        id: idUploadModelDlg
        onModelUpload:
        {
            slicercontrol.uploadModelToAliyun(categoryId, groupName, groupDesc, isShare, isOriginal, modelType, license, isCombine)
        }
        onViewMyModel:
        {
            slicercontrol.viewMyModel("mymodel")
            idUploadModelDlg.visible = false
        }
    }
    UploadMessageDlg{
        id: idUploadModelFailedDlg
        msgText: qsTr("Failed to upload model!") 
        cancelBtnVisible: false
        visible: false
        onSigOkButtonClicked:
        {
            idUploadModelFailedDlg.close()
        }
    }
    onHeightChanged:
    {
        defultHeight = height;
    }
}
