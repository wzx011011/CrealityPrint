import CrealityUI 1.0
import Qt.labs.platform 1.1
import QtGraphicalEffects 1.12
import QtQml 2.13
import QtQuick 2.13
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import "qrc:/CrealityUI"

Rectangle {

    id: rootRec
    property string curProfileName: ""
    property bool curProfileIsDefault: true

    function startSlice() {
        var ret = slicePlugin.outPlatform()
        if(ret)
        {
            idOutPlatform.visible = true
        }
        else {
            SettingJson.updateGlobalSettingValue()
            //console.log("SliceExecute success")
            if(slicePlugin.sliceMainOp("detectFDMsupports") > 0)
            {
                //idSliceBtn.enabled=false
                slicePlugin.sliceMainOp("slice")
            }
            else {
                //idSliceBtn.enabled=false
                slicePlugin.sliceMainOp("setBottomForDLP")
                slicePlugin.sliceMainOp("slice")
            }
        }
    }
    function initAdvanceParam() {
        editProfileWizard.initEditProfileByFilter()
    }
    
    function updateProfileLanguage()
    {
        editProfileWizard.updateLanguage()
    }
    
    Connections{
        target: parameterUI
        onSigLanguageChanged:{
            console.log("updateProfileLanguage")
            updateProfileLanguage()
        }
    }

    width: 280*screenScaleFactor
    color: "transparent"

    ColumnLayout {
        spacing: 10 *screenScaleFactor
        //        anchors.margins: 1 *screenScaleFactor
        anchors.fill: parent

        PanelComponent {
            id: modelListC
            imgUrl: "qrc:/UI/photo/materialPanel.png"
            title: qsTr("Material")

            topLineEnabled: true
        }

        BasicCombobox_Metarial {
            id: metarialComb
            Layout.preferredWidth: 260*screenScaleFactor
            Layout.preferredHeight: 28*screenScaleFactor
            Layout.alignment: Qt.AlignHCenter
            clip: true
            addBtnVisible: false
            model: profileListModel.currentMetrials
            currentIndex: 0
            onAddMaterialClick: {
                actionCommands.getOpt("Manage Materials").execute();
                //console.log("Add Printer+++");
            }
            //onDelModelData:
            onCurrentTextChanged: {
                //console.log("---onCurrentTextChanged=-=-");
                profileListModel.currentMetrial(currentText);
                paramCfgListView.currentIndex = 0;
            }
            Connections{
                target: idConsumables
                onCurrentIndexChanged:{
                    metarialComb.currentIndex = 0
                    // profileListModel.currentMetrial(currentText);
                }
            }
        }

        PanelComponent {
            id: paramCfgC
            imgUrl: "qrc:/UI/photo/paramPanel.png"
            title: qsTr("Parameter config")

            topLineEnabled: true

            CusImglButton {
                id: importBtn

                width: 28*screenScaleFactor
                height: 28*screenScaleFactor
                anchors.right: parent.right
                anchors.rightMargin: 53*screenScaleFactor
                anchors.verticalCenter: parent.verticalCenter
                defaultBtnBgColor: Constants.right_panel_button_default_color
                hoveredBtnBgColor: Constants.right_panel_button_hovered_color
                selectedBtnBgColor: Constants.right_panel_button_checked_color
                font.family: Constants.labelFontFamily
                font.weight: Constants.labelFontWeight
                enabledIconSource: "qrc:/Photo/right_import.png"
                highLightIconSource: "qrc:/Photo/right_import.png"
                pressedIconSource: "qrc:/Photo/right_import.png"
                btnRadius: 5
                borderWidth: 0
                onClicked: {
                    printerController.importProfile();
                }
            }

            CusImglButton {
                id: exportBtn

                width: 28*screenScaleFactor
                height: 28*screenScaleFactor
                anchors.right: parent.right
                anchors.rightMargin: 20
                anchors.verticalCenter: parent.verticalCenter
                defaultBtnBgColor: Constants.right_panel_button_default_color
                hoveredBtnBgColor: Constants.right_panel_button_hovered_color
                selectedBtnBgColor: Constants.right_panel_button_checked_color
                font.family: Constants.labelFontFamily
                font.weight: Constants.labelFontWeight
                enabledIconSource: "qrc:/Photo/right_export.png"
                highLightIconSource: "qrc:/Photo/right_export.png"
                pressedIconSource: "qrc:/Photo/right_export.png"
                btnRadius: 5
                //                btnTipType : 0
                borderWidth: 0
                onClicked: {
                    printerController.exportProfile();
                }
            }

        }

        Rectangle {
            Layout.preferredWidth: 258*screenScaleFactor
            Layout.preferredHeight: 266*screenScaleFactor
            border.color: Constants.right_panel_menu_border_color
            border.width: 1
            color: Constants.right_panel_combobox_background_color
            Layout.alignment: Qt.AlignHCenter
            radius: 5
            clip: true

            CusListView {
                id: paramCfgListView
                anchors.fill: parent
                anchors.margins: 2
                spacing: 0
                model: profileListModel
                focus: true
                delegate: paramCfgCom

                onCurrentIndexChanged: {
                    var isDefault = profileListModel.setSelectIndex(currentIndex);
                    if (isDefault)
                    {
                        __deleteButton.enabled = false
                        editProfileWizard.isDefault = true
                    }
                    else {
                        __deleteButton.enabled = true
                        editProfileWizard.isDefault = false
                    }
                }

                Component.onCompleted: {
                    console.log("=================== ", paramCfgListView.currentIndex)
                    paramCfgListView.currentIndex = 1
                    console.log("----------------- ", paramCfgListView.currentIndex)
                }

                Connections {
                    target: profileListModel
                    onSigAddSuccess: paramCfgListView.currentIndex = curIndex
                }
            }

            //            Connections {
            //                //paramCfgListView.model = profileList
            //                target: sliceUI
            //                onSigGetProfileList: console.log("profileListModelData = ", profileList);
            //            }
        }

        RowLayout {
            spacing: 6
            Layout.alignment: Qt.AlignHCenter

            CusButton {
                Layout.preferredWidth: 82*screenScaleFactor
                Layout.preferredHeight: 24*screenScaleFactor

                id: __addButton
                enabled: true
                showToolTip: true
                shadowEnabled: false
                normalColor: Constants.right_panel_button_default_color
                hoveredColor: Constants.right_panel_button_hovered_color
                pressedColor: Constants.right_panel_button_checked_color
                normalBdColor: Constants.right_panel_border_default_color
                hoveredBdColor: Constants.right_panel_border_hovered_color
                pressedBdColor: Constants.right_panel_border_checked_color
                txtColor: Constants.right_panel_text_default_color
                txtContent: qsTr("Add")

                onClicked: {
                    paramSetUI.onEditAddProfiled()
                    addeditprofile.visible = true
                    addeditprofile.model = paramSetUI.getPrinterList()
                    addeditprofile.currentIndex = paramSetUI.getCurPrinterIndex()
                    addeditprofile.modelprofile = paramSetUI.getProfileList()
                    addeditprofile.modelprofileIndex = paramSetUI.getCurProfileIndex()
                    addeditprofile.modelmaterial = profileListModel.currentMetrials
                    addeditprofile.modelmaterialIndex = metarialComb.currentIndex
                }
            }

            CusButton {
                Layout.preferredWidth: 82*screenScaleFactor
                Layout.preferredHeight: 24*screenScaleFactor

                id: __editButton
                enabled: true
                shadowEnabled: false
                normalColor: Constants.right_panel_button_default_color
                hoveredColor: Constants.right_panel_button_hovered_color
                pressedColor: Constants.right_panel_button_checked_color
                normalBdColor: Constants.right_panel_border_default_color
                hoveredBdColor: Constants.right_panel_border_hovered_color
                pressedBdColor: Constants.right_panel_border_checked_color
                txtColor: Constants.right_panel_text_default_color
                txtContent: qsTr("Edit")

                onClicked: {
                    onEdit();
                }
            }

            CusButton {
                Layout.preferredWidth: 82*screenScaleFactor
                Layout.preferredHeight: 24*screenScaleFactor

                id: __deleteButton
                enabled: rootRec.curProfileIsDefault
                shadowEnabled: false
                normalColor: Constants.right_panel_button_default_color
                hoveredColor: Constants.right_panel_button_hovered_color
                pressedColor: Constants.right_panel_button_checked_color
                normalBdColor: Constants.right_panel_border_default_color
                hoveredBdColor: Constants.right_panel_border_hovered_color
                pressedBdColor: Constants.right_panel_border_checked_color
                txtColor: Constants.right_panel_text_default_color
                txtContent: qsTr("Delete")

                onClicked: {
                    var res = paramSetUI.onDeleteProfilebtn()
                    if(res) paramCfgListView.currentIndex = 0
                }
            }
        }

        Rectangle {
            Layout.preferredWidth: rootRec.width
            Layout.preferredHeight: 1
            color: Constants.right_panel_menu_split_line_color
        }

        CusButton {
            id: idSliceBtn

            //            anchors.bottom: parent.anchors.bottom
            //            anchors.bottomMargin: 21 * screenScaleFactor
            Layout.bottomMargin: 21 * screenScaleFactor
            Layout.fillWidth: true
            Layout.leftMargin: 11 * screenScaleFactor
            Layout.rightMargin: 11 * screenScaleFactor
            Layout.preferredHeight: 48 * screenScaleFactor

            txtContent: qsTr("Slice")
            txtColor: enabled ? Constants.right_panel_slice_text_default_color
                              : Constants.right_panel_slice_text_disable_color
            normalColor: enabled ? Constants.right_panel_slice_button_default_color
                                 : Constants.right_panel_slice_button_disable_color
            hoveredColor: Constants.right_panel_slice_button_hovered_color
            pressedColor: Constants.right_panel_slice_button_checked_color
            radius: 5
            shadowEnabled: false
            enabled: false
            opacity : enabled ? 1 : 0.5
            txtPointSize: Constants.labelLargeFontPointSize
            txtFamily: Constants.panelFontFamily
            onClicked: {
                standaloneWindow.curState = 1
            }

            Connections {
                target: modelListData
                onRowChanged: {
                    idSliceBtn.enabled = row;
                }
            }

        }
    }

    Connections {
        target: standaloneWindow
        onCurStateChanged: if(standaloneWindow.curState === 1) startSlice()
    }

    Connections {
        target: standaloneWindow.g_Workflow
        onSigInitAdvanceParam: {
            initAdvanceParam();
        }
    }

    Loader {
        id: slicerPg
        onLoaded: slicerPg.item.show()
    }

    AddEditProfile {
        id: addeditprofile
        objectName: "addeditprofileobj"
        visible: false
        onOpenEditMenu: {
            addeditprofile.visible = false;
            editProfileWizard.addOrEdit = 0;
            editProfileWizard.profileName = addeditprofile.profileName;
            editProfileWizard.profileMachine = addeditprofile.profileMachine;
            console.log("111111111111 =", addeditprofile.profileMaterial)
            editProfileWizard.profileMaterial = addeditprofile.profileMaterial;
            editProfileWizard.profileQuality = addeditprofile.profileQuality;
            editProfileWizard.visible = true;
            parameterUI.resetProfile();
            SettingJson.initSettingData();
            editProfileWizard.resetValue();
            console.log("open edit");
        }
        onOpenWarring: {
            console.log("onOpenWarring");
            //messageDialog.open()
            messageDialog.visible = true;
        }
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        //        folder: shortcuts.home
        currentFile: "file:///" + "xx_userName_xx"
        fileMode: FileDialog.SaveFile
        nameFilters: ["Profiles (*.default)", "All files (*)"]
        onAccepted: {
            var dstPath = String(fileDialog.currentFile);
            console.log("path:", dstPath);
            var srcProfileFullPath = parameterUI.getCurProfilePath();
            profileListModel.copyFileToPath(srcProfileFullPath, fileDialog.currentFile);
            editProfileWizard.close();
        }
        onRejected: {
            editProfileWizard.close();
        }
    }

    EditProfile {
        id: editProfileWizard

        visible: false
        //        profileMaterial: metarialComb.currentText
        onCanceled: {
            if (editProfileWizard.addOrEdit == 0)
                parameterUI.deleteProfile();
        }

        onValchanged: {
        }

        onReseted: {
            // resetbtn();
            parameterUI.resetProfile(true);
            console.log("showEdit SettingJson.initSettingData");
            SettingJson.initSettingData();
        }

        onSaveProfile: {
            parameterUI.onSaveProfilebtn();
        }
    }

    UploadMessageDlg {
        id: idOutPlatform

        visible: false
        cancelBtnVisible: false
        messageType: 0
        msgText: qsTr("Model out of range, please put 'red' model in the printer!")
        onSigOkButtonClicked: {
            idOutPlatform.visible = false;
        }
    }

    BasicDialog {
        id: messageDialog

        width: 400*screenScaleFactor
        height: 200*screenScaleFactor
        titleHeight: 30*screenScaleFactor
        title: qsTr("Message")
        visible: false

        Rectangle {
            anchors.centerIn: parent
            width: parent.width / 2
            height: parent.height / 2
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

    BasicMessageDialog {
        id: idDoubleMessageDlg

        property var receiver
        property var indexs
        property var messageDlgType

        width_x: 480*screenScaleFactor
        onAccept: {
            if ("deleteSelectModel" === messageDlgType) {
                if (receiver) {
                    receiver.remove(indexs);
                    if (checkedFlag)
                        receiver.writeReg(false);

                }
            } else if ("deleteSelectMachine" === messageDlgType) {
                if (receiver) {
                    machineDelete(indexs);
                    if (checkedFlag)
                        receiver.writeReg(false);

                    idManagerPrinterWizard.visible = false;
                }
            } else if ("deleteSelectProfile" === messageDlgType) {
                if (receiver) {
                    deleteProfilebtn();
                    if (checkedFlag)
                        receiver.writeReg(false);

                }
            }
        }
        onCancel: {
            close();
        }
    }

    Component {
        id: imgBtnCom

        CusImglButton {
            width: 58*screenScaleFactor
            height: 28*screenScaleFactor
            //id: control
            defaultBtnBgColor: Constants.right_panel_item_default_color
            hoveredBtnBgColor: Constants.right_panel_item_hovered_color
            selectedBtnBgColor: Constants.right_panel_item_checked_color
            font.family: Constants.labelFontFamily
            font.weight: Constants.labelFontWeight
            enabledIconSource: imgSource
            highLightIconSource: hoverImgSource
            pressedIconSource: hoverImgSource
            btnRadius: 5
            enabled: !(model.index === 1 || model.index === 2)
            shadowEnabled: true

            onClicked: {
                if (model.index === 0) HalotContext.obj("FileOSManager").qOpen()
                //else if (model.index === 1) {}
                //else if (model.index === 2) {}
                else if (model.index === 3) HalotContext.obj("ModelListController").remove()
            }
        }

    }

    Component {
        id: listViewCom

        Rectangle {
            id: item_delegate

            property bool checked: isChecked
            property bool isHovered: false
            property string normalColor: isHovered ? Constants.right_panel_item_hovered_color
                                                   : Constants.right_panel_item_default_color

            width: 258*screenScaleFactor
            height: 26*screenScaleFactor
            color: checked ? Constants.right_panel_item_checked_color : normalColor

            Image {
                id: img1

                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                source: checked ? "qrc:/UI/photo/right_fileLogo_d.png" : "qrc:/UI/photo/right_fileLogo.png"
            }

            CusText {
                anchors.left: img1.right
                anchors.leftMargin: 7
                anchors.verticalCenter: parent.verticalCenter
                fontText: modelName
                fontColor: Constants.right_panel_text_default_color
                hAlign: 0
            }

            Connections {
                target: modelListView
                onCheckOne: {
                    item_delegate.checked = (idx === index);
                    isChecked = checked;
                }
                onCheckMul: {
                    if (idx > modelListView.mulBegin)
                        item_delegate.checked = (index >= modelListView.mulBegin && index <= idx);
                    else
                        item_delegate.checked = (index <= modelListView.mulBegin && index >= idx);
                    isChecked = checked;
                }
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                propagateComposedEvents: true
                onEntered: {
                    parent.isHovered = true;
                }
                onExited: {
                    parent.isHovered = false;
                }
                onClicked: {
                    modelListView.currentIndex = index;
                    mouse.accepted = false;
                    switch (mouse.modifiers) {
                    case Qt.ControlModifier:
                        item_delegate.checked = !item_delegate.checked;
                        isChecked = checked;
                        console.log("Qt.ControlModifier index =", index);
                        break;
                    case Qt.ShiftModifier:
                        modelListView.checkMul(index);
                        console.log("Qt.ShiftModifier index =", index);
                        break;
                    default:
                        modelListView.checkOne(index);
                        modelListView.mulBegin = index;
                        break;
                    }
                    console.log("onClickedonClicked =" + index);
                    HalotContext.obj("ModelListController").selects();
                }

                Image {
                    id: img2

                    property bool isOpen: isVisible
                    property bool isHovered: false
                    property string eyeOpen: isHovered ? "qrc:/UI/photo/eye_open_d.png" : "qrc:/UI/photo/eye_open.png"
                    property string eyeClose: isHovered ? "qrc:/UI/photo/eye_close_d.png" : "qrc:/UI/photo/eye_close.png"

                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    source: isOpen ? eyeOpen : eyeClose

                    MouseArea {
                        hoverEnabled: true
                        anchors.fill: parent
                        onClicked: {
                            img2.isOpen = !img2.isOpen;
                            HalotContext.obj("ModelListController").modelHide(index, img2.isOpen);
                        }
                        onEntered: {
                            img2.isHovered = true;
                        }
                        onExited: {
                            img2.isHovered = false;
                        }
                    }

                }

            }

        }

    }

    Component {
        id: paramCfgCom

        Rectangle {
            property bool checked: isChecked
            property bool isHovered: false
            property string normalColor: isHovered ? Constants.right_panel_item_hovered_color
                                                   : Constants.right_panel_item_default_color

            width: paramCfgListView.width
            height: 24*screenScaleFactor
            color: ListView.isCurrentItem ? Constants.right_panel_item_checked_color : normalColor



            Image {
                id: img1

                property string defaultImage: getDefaultImg(profileLayerHeight.toFixed(2))
                property string checkedImage: getCheckImg(profileLayerHeight.toFixed(2))

                width: 12*screenScaleFactor
                height: 12*screenScaleFactor

                anchors.left: parent.left
                anchors.leftMargin: 10*screenScaleFactor
                anchors.verticalCenter: parent.verticalCenter
                source: paramCfgListView.currentIndex === model.index ? checkedImage : defaultImage

                Component.onCompleted: {
                    getDefaultImg(materialHeight)
                }

                function getCheckImg(materialHeight){
                    var checkedImage;
                    if(materialHeight === "0.10"){
                        checkedImage = Constants.right_panel_quality_high_checked_image
                    }
                    if(materialHeight === "0.15"){
                        checkedImage = Constants.right_panel_quality_middle_checked_image
                    }
                    if(materialHeight === "0.20"){
                        checkedImage = Constants.right_panel_quality_low_checked_image
                    }
                    if(materialHeight === "0.28"){
                        checkedImage = Constants.right_panel_quality_verylow_checked_image
                    }
                    if(!profileIsDefault){
                        checkedImage = Constants.right_panel_quality_custom_checked_image
                    }
                    return checkedImage;
                }

                function getDefaultImg(materialHeight){
                    if(!profileIsDefault){
                        return Constants.right_panel_quality_custom_default_image
                    }
                    if(materialHeight === "0.10"){
                        return Constants.right_panel_quality_high_default_image
                    }
                    if(materialHeight === "0.15"){
                        return Constants.right_panel_quality_middle_default_image
                    }
                    if(materialHeight === "0.20"){
                        return Constants.right_panel_quality_low_default_image
                    }
                    if(materialHeight === "0.28"){
                        return Constants.right_panel_quality_verylow_default_image
                    }else{
                        return ""
                    }
                }
            }

            CusText {
                id: profileText

                anchors.left: img1.right
                anchors.leftMargin: 7
                anchors.verticalCenter: parent.verticalCenter
                fontText: profileName
                fontColor: Constants.right_panel_text_default_color
                hAlign: 0
            }

            CusText {
                id: profileLayerText

                anchors.left: profileText.right
                anchors.leftMargin: 7
                anchors.verticalCenter: parent.verticalCenter
                fontText: profileLayerHeight.toFixed(2) + "mm"
                fontColor: Constants.right_panel_text_default_color
                hAlign: 0
            }

            //            Component.completed: {
            //              index_text = "new"
            //              if (model.index <= 3) {
            //                index_text = "" += index_text
            //              }

            //              img1.defaultImage = "qrc:/UI/photo/config_quality_" += model.index += ".png"
            //              img1.checkedImage = "qrc:/UI/photo/config_quality_checked_" += model.index += ".png"
            //            }

            ToolTip {
                id: tipCtrl
                visible: parent.isHovered
                //timeout: 2000
                delay: 100
                width: 400*screenScaleFactor
                implicitHeight: idTextArea.contentHeight + 40

                background: Rectangle {
                    anchors.fill: parent
                    color: "transparent"
                }

                contentItem: TextArea{
                    id: idTextArea
                    text: qsTr("The main parameter configuration of this mode: Layer Height(") + SettingJson.layer_height + qsTr("mm),Line Width(") + SettingJson.line_width + qsTr("mm),Wall Line Count(") + SettingJson.wall_line_count + qsTr("mm),Infill Density(") + SettingJson.infill_sparse_density + qsTr("%),Build Plate Adhesion Type(") + qsTr(SettingJson.adhesion_type) + qsTr("),Printing Temperature(") + SettingJson.material_print_temperature + qsTr("℃),Build Plate Temperature(") + SettingJson.material_bed_temperature + qsTr("℃),Print Speed(") + SettingJson.speed_print + qsTr("mm/s)")
                    wrapMode: TextEdit.WordWrap
                    color: Constants.textColor
                    font.family: Constants.labelFontFamily
                    font.weight: Constants.labelFontWeight
                    font.pointSize:Constants.labelLargeFontPointSize
                    readOnly: true
                    background: Rectangle
                    {
                        anchors.fill : parent
                        color: Constants.tooltipBgColor
                        border.width:1
                    }
                }
            }

            MouseArea {
                id:ma
                anchors.fill: parent
                hoverEnabled: true
                propagateComposedEvents: true
                onEntered: {
                    parent.isHovered = true;
                }
                onExited: {
                    parent.isHovered = false;
                }
                onClicked: {
                    paramCfgListView.currentIndex = index;
                    mouse.accepted = false;
                }

                onDoubleClicked: {
                    onEdit()
                }
                Component.onCompleted: {
                    console.log("4444444")
                    if(profileName === qsTr("Balanced")){
                        console.log("555555555555555")
                        ma.clicked()
                    }
                }
            }
        }
    }
    function onEdit(){
        paramSetUI.onEditProfileClicked()
        paramSetUI.onEditProfiled()
        SettingJson.initSettingData()
        editProfileWizard.addOrEdit = 1
        editProfileWizard.resetValue()
        editProfileWizard.visible = true
    }

}
