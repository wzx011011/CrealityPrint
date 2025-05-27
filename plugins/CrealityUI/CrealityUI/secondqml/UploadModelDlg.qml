import QtQuick 2.0
import QtQuick.Controls 2.12
import CrealityUI 1.0
import "qrc:/CrealityUI"
import "../qml"

BasicDialog{
    id: idDialog
    width: 600
    height: 433 - 28 - 5
    titleHeight : 30
    title: qsTr("Upload Model")
    property var progressValue: 0.0
    property bool m_isLocalFile: false
    signal modelUpload(var categoryId, var groupName, var groupDesc, var isShare, var isOriginal, var modelType, var license, var isCombine)
    signal viewMyModel()

    onDialogClosed:
    {
        idModelGroupInput.text = ""
        idDescText.text = ""
        progressValue = 0
        idOriginalCheckBox.checked = false
        idLicenseCombobox.currentIndex = 0
        idShareCheckBox.checked = false
        idCombineCheckBox.checked = true
        idDialog.height = 433 - 28 - 5

        idLogoImageColumn.visible = true
        grid_wrapper.visible = true
        idBtnGroup.visible = true
        idprogressBar.visible = false
        idUploadSuccess.visible = false
    }
    function initDialogQMLData()
    {
        idModelGroupInput.text = ""
        idDescText.text = ""
        progressValue = 0
        idOriginalCheckBox.checked = false
        idLicenseCombobox.currentIndex = 0
        idShareCheckBox.checked = false
        idCombineCheckBox.checked = true
        idDialog.height = 433 - 28 - 5

        idLogoImageColumn.visible = true
        grid_wrapper.visible = true
        idBtnGroup.visible = true
        idprogressBar.visible = false
        idUploadSuccess.visible = false
    }

    function uploadModelFailed()
    {
        idModelGroupInput.text = ""
        idDescText.text = ""
        progressValue = 0
        idOriginalCheckBox.checked = false
        idLicenseCombobox.currentIndex = 0
        idShareCheckBox.checked = false
        idCombineCheckBox.checked = true
        idDialog.height = 433 - 28 - 5

        idLogoImageColumn.visible = true
        grid_wrapper.visible = true
        idBtnGroup.visible = true
        idprogressBar.visible = false
        idUploadSuccess.visible = false
    }

    function uploadModelSuccess()
    {
        idLogoImageColumn.visible = false
        grid_wrapper.visible = false
        idBtnGroup.visible = false
        idprogressBar.visible = false
        idUploadSuccess.visible = true
    }

    function insertListModeData(data, isLocalFile){
        idGroupTypeModel.clear()
        idGroupTypeCombobox.currentIndex = 0
        var objectArray = JSON.parse(data);
        var objResult = objectArray.result.list;
        for ( var key in objResult ) {
            idGroupTypeModel.append({"key": objResult[key].id, "modelData": objResult[key].name})
        }
        m_isLocalFile = isLocalFile
        //update idModelTypeModel list when language changed
        idModelTypeModel.clear()
        idModelTypeCombobox.currentIndex = 0
        idModelTypeModel.append({"key":1, "modelData":qsTr("Rendering Blue")})
        idModelTypeModel.append({"key":2, "modelData":qsTr("Rendering Light Green")})
        idModelTypeModel.append({"key":3, "modelData":qsTr("Rendering Green")})
        idModelTypeModel.append({"key":4, "modelData":qsTr("Rendering Yellow")})
        idModelTypeModel.append({"key":5, "modelData":qsTr("Rendering Pink")})
    }
    Column{
        id: idLogoImageColumn
        y: 30
        Rectangle {
            id:logoRect
            x: (idDialog.width - logoRect.width)/2
            width: idDialog.width
            height: 74
            color: "transparent"
            Row
            {
                width: logoImage.width + idText.contentWidth
                anchors{
                    horizontalCenter: parent.horizontalCenter
                    verticalCenter: parent.verticalCenter
                }
                spacing: 10
                Image {
                    id : logoImage
                    height:sourceSize.height
                    width: sourceSize.width
                    source: "qrc:/UI/photo/CloudLogo.png"
                }
                StyledLabel
                {
                    id:idText
                    height:logoImage.sourceSize.height
                    text: qsTr("Creality Cloud")
                    font.pixelSize:20 
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignLeft
                }
            }
        }
        // Item {
        //     id: name
        //     x: 5
        //     width:idDialog.width - 10
        //     height: 2
        //     BasicSeparator
        //     {
        //         anchors.fill: parent
        //     }
        // }

        Item {
            width: idItem.width - 2
            height : 2
            
            Rectangle {
                x: 6
                width:idDialog.width - 12
                height: 2
                color: Constants.splitLineColor
                Rectangle {
                    width: parent.width
                    height: 1
                    color: Constants.splitLineColorDark
                }
            }
        }
    }
    Column{
        id: grid_wrapper
        anchors.top: idLogoImageColumn.bottom
        anchors.topMargin: 25
        anchors.bottomMargin: 25
        anchors.left: parent.left
        anchors.leftMargin: 55
        anchors.rightMargin: 55
        width: idDialog.width
        spacing: 5
        Row{
            StyledLabel {
                id: idGroupNameLabel
                width:100
                height:28
                text: qsTr("Group Name:")
                font.pixelSize: 12
                verticalAlignment: Qt.AlignVCenter
                horizontalAlignment: Qt.AlignLeft
            }
            BasicDialogTextField {
                id: idModelGroupInput
                font.pixelSize:12
                placeholderText: qsTr("Please enter the model group name")
                baseValidator:RegExpValidator { regExp: /^\S{100}$/ }
                width: grid_wrapper.width-idGroupNameLabel.width-110
                height : 28
                text: ""
            }
        }
        Row{
            StyledLabel {
                id: idGroupDescLabel
                width:100
                height:28
                text: qsTr("Description:")
                font.pixelSize: 12
                verticalAlignment: Qt.AlignVCenter
                horizontalAlignment: Qt.AlignLeft
            }
            ScrollView {
                id: idScrollView
                width: grid_wrapper.width-idGroupNameLabel.width-110
                height: 56
                TextArea {
                    id:idDescText
                    width: idScrollView.width
                    height:idScrollView.height
                    wrapMode: TextEdit.Wrap
                    placeholderText: qsTr("Please enter the model group description")
                    placeholderTextColor: Qt.lighter(Constants.textColor, 0.8)
                    text: ""
                    color: Qt.lighter(Constants.textColor, 0.8)//Constants.textColor
                    font.pixelSize: 12
                    font.family: Constants.labelFontFamily
                    font.weight: Constants.labelFontWeight

                }
                background: Rectangle {
                    //radius: 5
                    border.color: Constants.dialogItemRectBgBorderColor
                    color: Constants.dialogItemRectBgColor//"transparent"
                }
            }
        }
        Row{
            StyledLabel {
                id: idGroupType
                width:100
                height:28
                text: qsTr("Group Type:")
                font.pixelSize: 12
                verticalAlignment: Qt.AlignVCenter
                horizontalAlignment: Qt.AlignLeft
            }
            BasicCombobox{
                id: idGroupTypeCombobox
                height:28
                width: grid_wrapper.width-idGroupNameLabel.width-110
                font.pixelSize : 12
                popPadding : 2
                currentIndex: 0
                model: ListModel {
                    id: idGroupTypeModel
                }
            }
        }
        Row{
            StyledLabel {
                id: idModelType
                width:100
                height:28
                text: qsTr("Rendering color:")
                font.pixelSize: 12
                verticalAlignment: Qt.AlignVCenter
                horizontalAlignment: Qt.AlignLeft
            }
            BasicCombobox{
                id: idModelTypeCombobox
                height:28
                width: grid_wrapper.width-idGroupNameLabel.width-110
                font.pixelSize : 12
                popPadding : 2
                currentIndex: 0
                model: ListModel {
                    id: idModelTypeModel
                }
            }
        }
        Row{
            spacing: 30
            x: 100
            StyleCheckBox
            {
                id :idOriginalCheckBox
                width: 100
                height: 18
                text: qsTr("Original")
                visible: true
                checked: false
                onCheckedChanged:
                {
                    if(idOriginalCheckBox.checked === true)
                    {
                        idDialog.height = 433
                    }
                    else{
                        idDialog.height = 433 - 28 - 5
                    }
                }
            }
            StyleCheckBox
            {
                id :idShareCheckBox
                width: 100
                height: 18
                text: qsTr("Share")
                visible: true
            }
            StyleCheckBox
            {
                id :idCombineCheckBox
                width: 100
                height: 18
                text: qsTr("Combintion")
                visible: !m_isLocalFile
                checked: true
            }
        }
        Row{
            id: idLicenseRow
            spacing:4
            visible: idOriginalCheckBox.checked
            StyledLabel {
                id: idLicenseLabel
                width:100-4
                height:28
                text: qsTr("License Type:")
                font.pixelSize: 12
                verticalAlignment: Qt.AlignVCenter
                horizontalAlignment: Qt.AlignLeft
            }
            BasicCombobox{
                id: idLicenseCombobox
                height:28
                width: grid_wrapper.width-idGroupNameLabel.width-110
                font.pixelSize : 12
                popPadding : 2
                currentIndex: 0
                model: ListModel {
                    id: idLicenseModel
                    ListElement{text : "CC BY";}
                    ListElement{text : "CC BY-SA";}
                    ListElement{text : "CC BY-NC";}
                    ListElement{text : "CC BY-NC-SA";}
                    ListElement{text : "CC BY-ND";}
                    ListElement{text : "CC BY-NC-ND";}
                    ListElement{text : "CC0";}
                }
            }
            CusSkinButton_Image{
                y: 5
                width: 16
                height: 16
                btnImgNormal: "qrc:/UI/photo/model_library_license.png"
                btnImgHovered: "qrc:/UI/photo/model_library_license_h.png"
                btnImgPressed: "qrc:/UI/photo/model_library_license_h.png"

                onClicked:
                {	
                    idLicenseDesDlg.visible = true
                }
            }
        }
    }
    Row{
        id: idBtnGroup
        anchors{
            horizontalCenter: parent.horizontalCenter
            //verticalCenter: parent.verticalCenter
        }
        anchors.top: grid_wrapper.bottom
        anchors.topMargin: 25
        spacing: 10
        BasicButton{
            width: 125
            height: 28
            btnRadius:3
            btnBorderW:0
            defaultBtnBgColor: Constants.profileBtnColor
        hoveredBtnBgColor: Constants.profileBtnHoverColor
            text: qsTr("Upload")
            enabled: (idModelGroupInput.text != "")&&(idDescText.text != "")
            onSigButtonClicked:
            {
                idLogoImageColumn.visible = false
                grid_wrapper.visible = false
                idBtnGroup.visible = false
                idDialog.height = 223
                idprogressBar.visible = true
                idUploadSuccess.visible = false

                var license = "";
                if(idLicenseRow.visible != true){
                    license = ""
                }
                else{
                    license = idLicenseModel.get(idLicenseCombobox.currentIndex).text
                }
                modelUpload(idGroupTypeModel.get(idGroupTypeCombobox.currentIndex).key, idModelGroupInput.text, idDescText.text,
                    idShareCheckBox.checked, idOriginalCheckBox.checked, idModelTypeModel.get(idModelTypeCombobox.currentIndex).key, license, (m_isLocalFile ? 3: idCombineCheckBox.checked))
            }
        }
        BasicButton{
            width: 125
            height: 28
            btnRadius:3
            btnBorderW:0
            defaultBtnBgColor: Constants.profileBtnColor
        hoveredBtnBgColor: Constants.profileBtnHoverColor
            text: qsTr("Cancel")
            onSigButtonClicked:
            {
                idDialog.close();
            }
        }
    }
    Column{
        id: idprogressBar
        x: 96
        y: 108
        visible: false
        spacing: 10
        StyledLabel{
            anchors{
                horizontalCenter: parent.horizontalCenter
                //verticalCenter: parent.verticalCenter
            }
            text: progressValue + "%"
            font.pixelSize: 12
        }
        ProgressBar{
            id: progressBar
            from: 0
            to:100
            value: progressValue
            width: 408
            height: 3

            background: Rectangle {   
                implicitWidth: progressBar.width
                implicitHeight: progressBar.height
                color: "#303030"
            }

            contentItem: Item {  
                Rectangle {
                    width: progressBar.visualPosition * progressBar.width
                    height: progressBar.height
                    color: "#1E9BE2"
                }
            }
        }
    }
    Rectangle{
        id: idUploadSuccess
        visible: false
        y: 35
        width: idDialog.width
        height: idDialog.height - titleHeight
        color: "transparent"
        Column{
            anchors{
                horizontalCenter: parent.horizontalCenter
                verticalCenter: parent.verticalCenter
            }
            spacing: 27
            Row{
                spacing: 10
                Image{
                    id: idFinishImage
                    height:sourceSize.height
                    width: sourceSize.width
                    source: "qrc:/UI/photo/upload_success_image.png"//"qrc:/UI/photo/UploadSuccess.png"
                }
                StyledLabel
                {
                    id:idFinishText
                    height:logoImage.sourceSize.height
                    text: qsTr("Uploaded Successfully")
                    font.pixelSize:14
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignLeft
                }
            }
            BasicButton{
                anchors{
                    horizontalCenter: parent.horizontalCenter
                    //verticalCenter: parent.verticalCenter
                }
                width: 125
                height: 28
                btnRadius:0
                btnBorderW:0
                //defaultBtnBgColor: "#787878"
                pixSize: 12
                text: qsTr("View My Model")
                onSigButtonClicked:
                {
                    viewMyModel()
                    initDialogQMLData()
                }
            }
        }
        
    }

    LicenseDescriptionDlg{
        id:idLicenseDesDlg
        visible:false
    }
}