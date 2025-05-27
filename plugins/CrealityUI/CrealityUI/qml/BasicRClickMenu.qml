import QtQuick 2.12
import QtQuick.Controls 2.5//2.13
import QtGraphicalEffects 1.12
import "../secondqml"
BasicMenuStyle
{
    id: idRootMenu
    property int  maxImplicitWidth: 200
   // background: Rectangle{color: "#061f3b";anchors.fill: parent}
    function createMenuItem(name,actionshortcut,actionsource,actionIcon,cmd,bSeparator,bEnable)
    {
        //console.log("name,"+name,",bEnable =" + bEnable)
        var component = Qt.createComponent("BasicMenuItem.qml");
        if (component.status === Component.Ready)
        {
            var obje = component.createObject(idRootMenu, {actionName:name});
            obje.actionName = name;
            obje.actionShortcut = actionshortcut;
            if(actionIcon.length > 0)
            {
                obje.icon.source = actionIcon;
            }
            obje.actionSource = actionsource;
            obje.actionCmdItem = cmd;
            obje.separatorVisible= bSeparator;
            obje.enabled = bEnable;
            idRootMenu.addItem(obje)
        }
    }

    function initMenu()
    {
        var actionMaps = rClickComandsData.getCommandsMap();
        var listObj = actionMaps[0]
        for(var i = 0; i<listObj.length ; i++)
        {
            var strName = listObj[i].name;
            var strShortcut = listObj[i].shortcut;
            var strSource = listObj[i].source;
            var strIcon = listObj[i].icon;
            var bSubMenu = listObj[i].bSubMenu;
            var bSeparator = listObj[i].bSeparator;
            var bEnable = listObj[i].enabled;
            var bVisible= listObj[i].visible;
            if(!bVisible)continue

           if(bSubMenu)
           {
               var component = Qt.createComponent("BasicRClickSubMenu.qml");
               var obje = component.createObject(idRootMenu, {title:strName});
               obje.mymodel = listObj[i].subMenuActionmodel;
               obje.myItemObj = listObj[i];
               obje.separator = bSeparator;
               obje.enabled = bEnable;
               idRootMenu.addMenu(obje);
           }
           else
           {
               createMenuItem(strName,strShortcut,strSource,strIcon,listObj[i],bSeparator,bEnable);
           }
        }
    }

    function uploadModelDlgCombobox(data, isLocalFile){
        idUploadModelDlg.insertListModeData(data, isLocalFile)
        idUploadModelDlg.show()
    }

    function uploadModelError()
    {
        idUploadModelDlg.visible = false
        idUploadModelDlg.uploadModelFailed()
        idUploadModelFailedDlg.visible = true
    }

    function updateUploadProgressValue(value)
    {
        idUploadModelDlg.progressValue = value
    }

    function uploadModelSuccess()
    {
        idUploadModelDlg.uploadModelSuccess()
    }

    UploadModelDlg{
        id: idUploadModelDlg
        onModelUpload:
        {
            slicePlugin.uploadModelToAliyun(categoryId, groupName, groupDesc, isShare, isOriginal, modelType, license, isCombine)
        }
        onViewMyModel:
        {
            slicePlugin.viewMyModel("mymodel")
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

    background: Rectangle {
        implicitWidth:maxImplicitWidth + 20
        color: Constants.itemBackgroundColor//"#061F3B"
        border.width: 1
        border.color: "black"

        Rectangle
		{
			id: mainLayout			
			anchors.fill: parent			
			anchors.margins: 5
			color: Constants.themeColor
			opacity: 1
		}
	
		DropShadow {
			anchors.fill: mainLayout
			horizontalOffset: 5
			verticalOffset: 5
			radius: 8
			samples: 17
			source: mainLayout
            color: Constants.dropShadowColor // "#333333"
		}
    }
    Component.onCompleted:
    {
//        initMenu()
//        var menuItemObj = idRootMenu.takeItem(5)
        maxImplicitWidth = implicitContentWidth > maxImplicitWidth ? implicitContentWidth :maxImplicitWidth;
        initMenu()
    }

    //上传模型的功能
    function uploadModelBtnClick(type)
    {
        console.log("99999999999 =", type)
        if(type == "modelUpload")
        {
            slicePlugin.uploadModelBtnClick()
        }
        else if(type == "modelLocalFileUpload")
        {
            slicePlugin.uploadModelLocalFileBtnClick()
        }
    }
}
