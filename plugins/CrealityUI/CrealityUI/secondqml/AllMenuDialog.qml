import QtQuick 2.0
import QtQuick.Controls 2.0
import com.cxsw.SceneEditor3D 1.0
import "qrc:/CrealityUI"

Item {
    function requestMenuDialog(receiver,menuObjName)
    {
        console.log("requestMenuDialog success")
        if(menuObjName === "languageObj")
        {
            idLanguage.show()
            idLanguage.receiver = receiver
            idLanguage.timeInterval = receiver.getMinutes().toFixed(1)
            idLanguage.cmbMoneyCurrentIndex = receiver.currentMoneyIndex()
            //idLanguage.currentLanguageIndex = receiver.currentLanguageIndex()
        }
        else if(menuObjName === "newProjectObj")
        {
            idNewProject.show()
            idNewProject.receiver = receiver
        }
        else if(menuObjName === "cloneNumObj")//by TCJ
        {
            idCloneNumDlg.show()
            idCloneNumDlg.receiver = receiver
            //console.log("display cloneN")
        }
        else if(menuObjName === "aboutUsObj")
        {
            idAboutUs.show()
            idAboutUs.receiver = receiver
        }
		else if(menuObjName === "updateObj")
        {
            showReleaseNote()
//			idReleaseNote.projectName = idWorkflow.updateManager.getProjectName()
//			idWorkflow.updateManager.checkForUpdates()
//            idReleaseNote.show()
//            idReleaseNote.receiver = receiver
        }
        else if(menuObjName === "closeProfile")
        {
            idClosePro.ignoreBtnVisible = true
            idClosePro.okBtnText = qsTr("Save")
            idClosePro.ignoreBtnText = qsTr("Exit")
            idClosePro.cancelBtnText = qsTr("Cancel")
            idClosePro.show()
            idClosePro.receiver = receiver
        }
        else if(menuObjName === "messageDlg")
        {       
            idConfirmMessageDlg.receiver = receiver
            idConfirmMessageDlg.cancelBtnVisible = true
            idConfirmMessageDlg.isInfo = true
            idConfirmMessageDlg.msgText = receiver.getMessageText()
            idConfirmMessageDlg.show()
        }
        else if(menuObjName === "deletemessageDlg")
        {
            idDoubleMessageDlg.showDoubleBtn()            
            idDoubleMessageDlg.receiver = receiver
            idDoubleMessageDlg.showCheckBox(true)
            idDoubleMessageDlg.typename = menuObjName
            idDoubleMessageDlg.messageText = receiver.getMessageText()
            idDoubleMessageDlg.show()
        }
        else if(menuObjName === "messageSingleBtnDlg")
        {
            idConfirmMessageDlg.isInfo = true
            idConfirmMessageDlg.cancelBtnVisible = false
            idConfirmMessageDlg.receiver = receiver
            idConfirmMessageDlg.msgText = receiver.getMessageText()
            idConfirmMessageDlg.show()
        }
        else if(menuObjName === "importimageDlg")
        {
			idImportImageConfigDlg.initTextVelue()
			
            idImportImageConfigDlg.show()
            idImportImageConfigDlg.receiver = receiver
        }
		else if(menuObjName === "messageTooBigDlg")
		{
            idTripleMessageDlg.ignoreBtnVisible = true
            idTripleMessageDlg.okBtnText = qsTr("Yes")
            idTripleMessageDlg.ignoreBtnText = qsTr("Delete model")
            idTripleMessageDlg.cancelBtnText = qsTr("Ignore")

			idTripleMessageDlg.receiver = receiver
            idTripleMessageDlg.msgText = receiver.getMessageText()
            idTripleMessageDlg.show()
		}
        else if(menuObjName === "modelRepairMessageDlg")
		{
			idModelRepairMessageDlg.showTripleBtn()
			idModelRepairMessageDlg.isInfo = false
			idModelRepairMessageDlg.receiver = receiver
            idModelRepairMessageDlg.messageText = qsTr("There are exceptions or errors in the model. What should be done?")
            idModelRepairMessageDlg.show()
		}
        else if(menuObjName === "sliceSuccessDlg")
        {
            idSelectPrint.receiver = receiver
            idSelectPrint.show()
        }
        else if(menuObjName === "laserSaveSuccessDlg")
        {
            idLaserSelectPrint.receiver = receiver
            idLaserSelectPrint.show()
        }
		else if(menuObjName === "deleteSupportDlg")
        {           
            idConfirmMessageDlg.receiver = receiver
            idConfirmMessageDlg.cancelBtnVisible = true
            idConfirmMessageDlg.msgText = qsTr("Whether to remove  all supports")
            idConfirmMessageDlg.show()
        }
        else if(menuObjName == "openDefaultCx3d")
        {         
            idOpenDefaultProjectDlg.receiver = receiver
            idOpenDefaultProjectDlg.visible = true
        }
        else if(menuObjName == "openRecentlyFileDlg")
        {
            idOpenRecentlyFileDlg.visible = true
        }
        else if(menuObjName == "messageNoModelDlg")
        {
            idMessageNoBtn.visible = true
        }
        else if(menuObjName === "repaircmdDlg")
        {

            idRepairdlg.receiver = receiver
            idRepairdlg.updateErrorInfo()
            idRepairdlg.show()
        }
    }
    function showDiag()
    {
        idMaterial.show()
    }

    LanguagePreferDlg
    {
        id : idLanguage
        visible: false
        objectName: "languageObj"
        property var receiver
        /*onSigCurrentIndex:
        {
            receiver.setCurrentIndex(index)
        }*/
        /*onSigValChanged:
        {
            receiver.onSigLanguagChanged(key,value)
        }*/
        onSaveLanguageConfig:
        {
            receiver.setCurrentMinutes(timeInterval)
            receiver.setMoneyCurrentIndex(cmbMoneyCurrentIndex)
            receiver.saveLanguageConfig()
        }
    }

    ManageMaterialDlg{
        id : idMaterial
        visible: false
        objectName: "managematerialObj"
    }
    
    AddNewProjectDlg
    {
        id: idNewProject
        visible: false
        objectName: "newProjectObj"
        property var receiver
        onSigProject:
        {
            receiver.saveProject(name);
        }
    }

    CloneNumDlg
    {
        id: idCloneNumDlg
        visible: false
        objectName: "cloneNumObj"
        property var receiver
        onSigNumber:
        {
            receiver.clone(clonenum);
        }
    }

    AboutUsDlg
    {
        id: idAboutUs
        visible: false
        objectName: "aboutUsObj"
        property var receiver
        website: receiver ? receiver.getWebsite() : ""
        version: receiver ? receiver.getCurrentVersion() : ""

    }

    BasicDialog
    {
        id: idMessageNoBtn
        width: 400
        height: 200
        titleHeight : 30
        title: qsTr("Message")

        Rectangle{
            anchors.centerIn: parent
            width: parent.width/2
            height: parent.height/2
            color: Constants.dialogContentBgColor//"#535353"
            Text {
                anchors.centerIn: parent
                font.family: Constants.labelFontFamily
                font.weight: Constants.labelFontWeight
                text: qsTr("No model selected to save")
                font.pixelSize: Constants.labelFontPixelSize
                color: Constants.textColor
            }
        }
    }
	

    // CloseProFileDlg
    // {
    //     id: idClosePro
    //     objectName: "closeProfile"
    //     property var receiver
    //     onSigSaveFile:
    //     {
    //         receiver.saveFile()
    //     }
    //     onSigNoSaveFile :
    //     {
    //         receiver.noSaveFile()
    //     }
    //     onDelDefaulltProject:
    //     {
    //         receiver.delDefaultProject()
    //     }
    // }
    RepairPanelDlg
    {
        id : idRepairdlg
        objectName: "repaircmdDlg"
        property var receiver

        function updateErrorInfo()
        {
            faceSize = receiver ? receiver.getFacesizeAll() : 0
            errorNormals = receiver ? receiver.getErrorNormalsAll() : 0
            errorEdges = receiver ? receiver.getErrorEdgesAll() : 0
            verticessize = receiver ? receiver.getVerticessizeAll() : 0
            shells = receiver ? receiver.getshellsAll() : 0
            errorHoles = receiver ? receiver.getholesAll() : 0
            errorIntersects = receiver ? receiver.getIntersectsAll() : 0
        }

        onAccept:
        {
            //            if(noneedAccept)return
            if(receiver)
                receiver.acceptRepair()
        }
        onCancel:
        {
            if(receiver)
                receiver.cancel()
        }
    }
    UploadMessageDlg{
        id:idClosePro
        objectName: "closeProfile"
        msgText: qsTr("Save Project Before Exiting?")
        visible: false
        messageType:1
        property var receiver
        onSigOkButtonClicked:
        {
            idClosePro.close()
            receiver.saveFile()
        }
        onSigIgnoreButtonClicked:
        {
            idClosePro.close()
            receiver.noSaveFile()
            receiver.delDefaultProject()
        }
    }

    BasicMessageDialog
    {
        id: idMessageDlg
        objectName: "messageDlg"
        property var receiver
		width_x : 315
        onAccept:
        {           
            if(receiver)
            receiver.accept()
        }
        onCancel:
        {
            receiver.cancel()
        }
    }

    UploadMessageDlg{
        id: idOpenDefaultProjectDlg
        objectName: "openDefaultProjectDlg"
        property var receiver
        visible: false
        messageType:0
        //cancelBtnVisible: false
        msgText: qsTr("The software closes abnormally, whether to open the saved temporary file?")

        onSigOkButtonClicked:
        {
            if(receiver)
                receiver.accept()
            idOpenDefaultProjectDlg.visible = false
        }
        onSigCancelButtonClicked:
        {
            if(receiver)
                receiver.reject()
            idOpenDefaultProjectDlg.visible = false
        }
    }

    UploadMessageDlg{
        id: idOpenRecentlyFileDlg
        objectName: "openRecentlyFileDlg"
        visible: false
        cancelBtnVisible: false
        messageType:1
        msgText: qsTr("The file does not exist or the path has changed, the file cannot be accessed!")

        onSigOkButtonClicked:
        {
            idOpenRecentlyFileDlg.visible = false
        }
    }
	
	BasicMessageDialog
    {
        id: idDoubleMessageDlg
        objectName: "openMessageDlg"
		width_x : 480
        isInfo: typename === "deletemessageDlg" ? false : true
        property var receiver
        property var typename
        onAccept:
        {       
            if(receiver)
			{
				receiver.accept()
                if(typename === "deletemessageDlg")
                {
                    if(checkedFlag){
                        receiver.writeReg(false)
                    }
                }
			}
        }
        onCancel:
        {
			if(receiver)
			{
				receiver.cancel()
			}
        }
        onDialogClosed:
        {
            if(receiver)
			{
				receiver.cancel()
			}
        }
    }

    UploadMessageDlg
    {
        id: idConfirmMessageDlg
        property var receiver
        onSigOkButtonClicked:
        {   
            idConfirmMessageDlg.close() 
            if(receiver)
			{
				receiver.accept()
			}
        }
        onSigCancelButtonClicked:
        {
            idConfirmMessageDlg.close()
			if(receiver)
			{
				receiver.cancel()
			}
        }
    }
	
	UploadMessageDlg
    {
        id: idTripleMessageDlg
        objectName: "tooBigMessageDlg"
        property var receiver
        messageType:0
        onSigOkButtonClicked:
        {
            if(receiver)
			{
				receiver.adaptPlatform()//accept()
			}
            idTripleMessageDlg.close()
        }
        onSigIgnoreButtonClicked:
        {
			if(receiver)
			{
				receiver.delSelectedModel()//cancel()
			}
            idTripleMessageDlg.close()
        }
    }

    BasicMessageDialog
    {
        id: idModelRepairMessageDlg
        objectName: "modelRepairMessageDlg"
		width_x : 480
        property var receiver
		yesBtnText: qsTr("Repair model")
		noBtnText: qsTr("Delete model")
		ignoreBtnText: qsTr("Ignore")
        onAccept:
        {
            if(receiver)
			{
				receiver.repairModel()

			}
        }
        onCancel:
        {
			if(receiver)
			{
				receiver.delSelectedModel()
			}
        }
		onIgnore:
		{
			//do nothing
		}
    }

    ImportImageDlg
    {
        id:idImportImageConfigDlg
        objectName: "importimageDlg"
        property var receiver
        onAccept:
        {
            console.log("parseFloat(myBase).toFixed(2) =" + parseFloat(myBase).toFixed(2))
            console.log("parseFloat(myHeight).toFixed(2) =" + parseFloat(myHeight).toFixed(2))
            console.log("parseFloat(myWidth).toFixed(2) =" + parseFloat(myWidth).toFixed(2))
            receiver.setBaseHeight(parseFloat(myBase).toFixed(2))
            receiver.setMaxHeight(parseFloat(myHeight).toFixed(2))
            receiver.setMeshWidth(parseFloat(myWidth).toFixed(2))
            receiver.setLighterOrDarker(myCmbCurrentIndex)
            receiver.setBlur(mySmothing)
            receiver.accept()
        }
        onCancel:
        {
            receiver.cancel()
        }
    }

    ComPrinterSelectDlg
    {
        id:idSelectPrint
        property var receiver
        property var slice: receiver ? receiver.getSlicePlugin() : ""
        myTableModel: receiver ?receiver.getTableModel() : ""
        onSearchWifi:
        {
            receiver.startSearchWifi()
        }
        onSendFileFunction:
        {
            receiver.sendSliceFile()
        }
        onAccept:
        {
            //slice.sliceMainOp("beginSave")
            idSelectPrint.close()
            slice.sliceMainOp("uploadSliceToCrealityCloud")
            //close()
        }

        onCancel:
        {
//            slice.sliceMainOp("unpreview")
            close()
        }
    }

    ComLaserPrinterSelectDlg
    {
        id:idLaserSelectPrint
        property var receiver
        property var laserscene: receiver ? receiver.getLaserScene() : ""
        myTableModel: receiver ?receiver.getTableModel() : ""
        onSearchWifi:
        {
            receiver.startSearchWifi()
        }
        onSendFileFunction:
        {
            receiver.sendSliceFile()
        }
        onAccept:
        {
            laserscene.saveLocalGcodeFile()
            close()
        }

        onCancel:
        {
//            slice.sliceMainOp("unpreview")
            close()
        }
    }
    
}
