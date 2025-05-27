import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.0
//import QtQuick.Dialogs 1.3

//import CrealityUI 1.0
//import "qrc:/CrealityUI"
import ".."
import "../qml"

BasicDialog
{
    id: editProfiledlg

    width: 700 * screenScaleFactor
    height: 557 * screenScaleFactor
    title: qsTr("Edit")
    //titleHeight : 30
    property int spacing: 5
    //add界面传过来的三个变量
    property string profileName:qsTr("new") //文件名
    property string profileMachine: "Ender-3" //机型
    property string profileMaterial: "PLA" //材料
    property string profileQuality: "high" //质量
    property bool isDefault: true
    property int addOrEdit: 0 //0: add, 1: edit

    property var layer_height:0 //layerheight.text
    property var line_width:0 //linewidth.text
    property var wall_line_count:0 //walllinecount.text
    property var infill_sparse_density:0 // infillsparsedensity.text
    property var material_print_temperature:0 //materialprinttemperature.text
    property var material_bed_temperature:0 //materialbedtemperature.text
    property var support_enable:0 //supportenable.checked
    property var support_angle:0 //supportangle.text
    property var adhesion_type:0 //  adhesiontype.modelText
    property var prime_tower_enable:0 //primetowerenable.checked
    property var min_skin_width_for_expansion:0 // minskinwidthforexpansion.text
    property var min_infill_area:0 //mininfillarea.text
    property var infill_before_walls:0 // infillbeforewalls.checked
    property var infill_wipe_dist:0 // infillwipedist.text
    property var zig_zaggify_infill:0 // zigzaggifyinfill.checked

    property var inputErrorStatus : 0x0000
	property var inputErrorStatus2 : 0x0000
    property var specialErrorStatus : 0x0000

    property var labelMap: {0:0}
    property var textBoxMap: {0:0}


    signal valchanged(string key,string value)
    signal reseted()
    signal saveProfile()
    signal canceled()

    signal advancedProfile()

    function initEditProfileByFilter()
    {
        console.log("initAdvanceParam")
        idEditProfileByFilter.initClassTypeList()
        idEditProfileByFilter.initProfileValueList()
        idEditProfileByFilter.showProfileType()
        updateLanguage()
    }

    function findTranslate(key)
    {
        var tranlateValue = ""
        if(key === "high")tranlateValue = qsTr("Dynamic")
        else if(key === "middle") tranlateValue= qsTr("Balanced")
        else if(key === "low")tranlateValue = qsTr("Speedy")
        else if(key === "best")tranlateValue = qsTr("Best quality")
        else if(key === "fast")tranlateValue = qsTr("Very Speedy")
        else
        {
            tranlateValue = key
        }
        return tranlateValue
    }

    property var translateMap: {0:0}
    function deleteMap()
    {
        modelType.clear()
    }
    //all combobox qsTr insert to Map
    function mapTranslate()
    {
        var modelMap = SettingJson.getSettingOptions("adhesion_type")
        for(var key in modelMap){
            console.log("Key = " +key + " value = " + modelMap[key])
            modelType.append({"key":key, "modelData":qsTr(modelMap[key])})
        }
    }
    function keyTranlateValue(data)
    {
        var value = translateMap[data]
        if(!value)
        {
            value = data
        }
        console.log("keyTranlateValue(data) =" + value)
        return  value
    }
    function valueTranlateKey(data)
    {
        var keyName = data
        for(var key in translateMap){
            if(translateMap[key] === data)
            {
                keyName = {"text" : key}.text
                break
            }
        }
        return  keyName
    }
    //cmb init translate
    function cmbTranslateUpdate()
    {
        deleteMap()
        mapTranslate()
    }

    function updateAdhesionType(data)
    {
        return keyTranlateValue(data)
    }

    function showEditProfileByFilter()
    {
        idEditProfileByFilter.updateComponent()
        idEditProfileByFilter.showProfileType()
        idEditProfileByFilter.profileName = editProfiledlg.profileName
        idEditProfileByFilter.visible=true
    }

    property var isReseting : false
    function resetValue()
    {
        isReseting = true
        supportenable.checked = SettingJson.support_enable

        var modelMap = SettingJson.getSettingOptions("adhesion_type")
        adhesiontype.currentIndex = SettingJson.findIndexByType(modelMap,SettingJson.adhesion_type)

        layerheight.defaultBackgroundColor = Constants.dialogItemRectBgColor
        linewidth.defaultBackgroundColor = Constants.dialogItemRectBgColor
        walllinecount.defaultBackgroundColor = Constants.dialogItemRectBgColor
        infillsparsedensity.defaultBackgroundColor = Constants.dialogItemRectBgColor
        materialprinttemperature.defaultBackgroundColor = Constants.dialogItemRectBgColor
        materialbedtemperature.defaultBackgroundColor = Constants.dialogItemRectBgColor
        supportangle.defaultBackgroundColor = Constants.dialogItemRectBgColor

        supportInfillRate.defaultBackgroundColor = Constants.dialogItemRectBgColor
        smallfeaturemaxlength.defaultBackgroundColor = Constants.dialogItemRectBgColor
        speedprint.defaultBackgroundColor = Constants.dialogItemRectBgColor
        speedinfill.defaultBackgroundColor = Constants.dialogItemRectBgColor
        speedwall.defaultBackgroundColor = Constants.dialogItemRectBgColor
        supportmaterialflow.defaultBackgroundColor = Constants.dialogItemRectBgColor
        infillmaterialflow.defaultBackgroundColor = Constants.dialogItemRectBgColor
        wallmaterialflow.defaultBackgroundColor = Constants.dialogItemRectBgColor
        smallfeaturespeedfactor.defaultBackgroundColor = Constants.dialogItemRectBgColor

        inputErrorStatus = 0x0000
		inputErrorStatus2 = 0x0000

        idEditProfileByFilter.resetComponent()

        isReseting = false
    }


    function editProfiledchanged(key,value)
    {
        console.log("editProfiledchanged key : " + key + " value : "+value)
        if(SettingJson.checkSettingValue(key,value))
        {
            valchanged(key,value)
            SettingJson.setSettingValue(key,value)
            console.log("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~SettingJson.setSettingValue()~~~~~~~~~~~~~~~~~~~~~~~~~~~~")
            if(!isReseting)
            {
                console.log("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~SettingJson.updateSettingData()~~~~~~~~~~~~~~~~~~~~~~~~~~~~")
                SettingJson.updateSettingData()
                idEditProfileByFilter.updateComponent()
            }
            return true
        }
        else
        {
            return false
        }
    }

    ListModel {
        id: specialModelType
    }

    function textFieldTextChanged(key, value)
    {
        valchanged(key,value)
    }

    function textFieldEditFinish(key, value)
    {
        console.log("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~textFieldEditFinish~~~~~~~~~~~~~~~~~~~~~~~~~~~~")
        var comType = SettingJson.getSettingType(key)
        if(comType == "bool")
        {
            editProfiledchanged(key,value)
        }
        else if(comType == "enum")
        {
            var selectKey = textBoxMap[key].model.get(value).key
            editProfiledchanged(key,selectKey)
        }
        else
        {
            if(editProfiledchanged(key,value))
            {
                textBoxMap[key].defaultBackgroundColor = Constants.dialogItemRectBgColor
                specialErrorStatus = specialErrorStatus & (0xFFFF - textBoxMap[key].errorCode)
            }
            else
            {
                textBoxMap[key].defaultBackgroundColor = "red"
                specialErrorStatus = specialErrorStatus | textBoxMap[key].errorCode
            }
        }

    }

    function updateLanguage()
    {
        cmbTranslateUpdate()
        idEditProfileByFilter.updateLanguage()
    }

    Item//Rectangle
    {
        id: rectangle
        x:30* screenScaleFactor
        y :25 + titleHeight
        width: parent.width-40* screenScaleFactor
        height: parent.height-titleHeight-50* screenScaleFactor
        Grid
        {
            width: parent.width
            height: parent.height
            rows: 6
            spacing: 20
            Row
            {
                width: parent.width
                height: 25* screenScaleFactor
                y: 5* screenScaleFactor
                spacing: 10
                StyledLabel {
                    id: element
                    text: qsTr("Profile:")
                    width: 163* screenScaleFactor
                    height: 25* screenScaleFactor
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignRight
                }
                StyledLabel {//by TCJ
                    id: textEdit
                    width: 160* screenScaleFactor
                    height: 25* screenScaleFactor
                    text: findTranslate(profileName)
                    verticalAlignment: Qt.AlignVCenter
                }

                StyledLabel {
                    id: elementMaterial
                    text: qsTr("Material:")
                    width: 160* screenScaleFactor
                    height: 25* screenScaleFactor
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignRight
                }

                CusText{
                    height:28* screenScaleFactor
                    width: 140* screenScaleFactor
                    fontText: editProfiledlg.profileMaterial
                    fontColor: "white"
                }

                //               BasicCombobox {
                //                   id:textEditMaterial
                //                   height:28
                //                   width: 140
                //                   enabled:false
                //                   model: materialModel
                //                   onCurrentIndexChanged:
                //                   {

                //                   }
                //               }
                // StyledLabel {//by TCJ
                //     id: textEditMaterial
                //     width: 100
                //     height: 25
                //     text: "PLA"
                //     verticalAlignment: Qt.AlignVCenter
                // }
            }

            Item {
                width: editProfiledlg.width - 13* screenScaleFactor
                height : 2

                Rectangle {
                    // anchors.left: idCol.left
                    // anchors.leftMargin: -10
                    x: -23* screenScaleFactor
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

            BasicGroupBox {
                id: basicGroupBox

                width: parent.width
                height: 340* screenScaleFactor
                BasicScrollView
                {
                    width: parent.width
                    height: 340* screenScaleFactor
                    hpolicy: ScrollBar.AlwaysOff
                    vpolicy: ScrollBar.AsNeeded
                    clip : true
                    Grid{
                        anchors.fill:parent
                        columns: 2
                        columnSpacing: 40
                        Grid{
                            id: idGrid1
                            columns: 2
                            rows: 10
                            rowSpacing: 7
                            columnSpacing: 10

                            StyledLabel {
                                id: element1
                                text: qsTr("Layer Height:")
                                width : 160* screenScaleFactor
                                height : 30* screenScaleFactor
                                font.pointSize:Constants.labelFontPointSize
                                strToolTip: qsTr(SettingJson.getDescription("layer_height"))
                                verticalAlignment: Qt.AlignVCenter
                                horizontalAlignment: Qt.AlignRight
                            }

                            BasicDialogTextField {
                                id: layerheight
                                width: 120* screenScaleFactor
                                height : 30* screenScaleFactor
                                text: SettingJson.layer_height
                                strToolTip:qsTr(SettingJson.getDescription("layer_height"))
                                onEditingFinished: {
                                    if(editProfiledchanged("layer_height",layerheight.text))
                                    {
                                        layerheight.defaultBackgroundColor = Constants.dialogItemRectBgColor
                                        inputErrorStatus = inputErrorStatus & 0xFFFE
                                    }
                                    else
                                    {
                                        layerheight.defaultBackgroundColor = "red"
                                        inputErrorStatus = inputErrorStatus | 0x0001
                                    }
                                }
                                onTextChanged:
                                {
                                    valchanged("layer_height",layerheight.text)
                                }
                                unitChar: "mm"
                            }

                            StyledLabel {
                                id: element2
                                width : 160* screenScaleFactor
                                height : 30* screenScaleFactor
                                text: qsTr("Line Width:")
                                strToolTip: qsTr(SettingJson.getDescription("line_width"))
                                verticalAlignment: Qt.AlignVCenter
                                horizontalAlignment: Qt.AlignRight
                            }

                            BasicDialogTextField {
                                id: linewidth
                                width: 120* screenScaleFactor
                                height : 30* screenScaleFactor
                                text: SettingJson.line_width
                                strToolTip: qsTr(SettingJson.getDescription("line_width"))
                                onEditingFinished: {
                                    if(editProfiledchanged("line_width",linewidth.text))
                                    {
                                        linewidth.defaultBackgroundColor = Constants.dialogItemRectBgColor
                                        inputErrorStatus = inputErrorStatus & 0xFFFD
                                    }
                                    else
                                    {
                                        linewidth.defaultBackgroundColor = "red"
                                        inputErrorStatus = inputErrorStatus | 0x0002
                                    }
                                }
                                onTextChanged:
                                {
                                    valchanged("line_width",linewidth.text)
                                }
                                unitChar: "mm"
                            }

                            StyledLabel {
                                id: element3
                                text: qsTr("Wall Line Count:")
                                width : 160* screenScaleFactor
                                height : 30* screenScaleFactor
                                strToolTip: qsTr(SettingJson.getDescription("wall_line_count"))
                                verticalAlignment: Qt.AlignVCenter
                                horizontalAlignment: Qt.AlignRight
                            }

                            BasicDialogTextField {
                                id: walllinecount
                                width: 120* screenScaleFactor
                                height : 30* screenScaleFactor
                                text: SettingJson.wall_line_count
                                strToolTip: qsTr(SettingJson.getDescription("wall_line_count"))
                                onEditingFinished: {
                                    if(editProfiledchanged("wall_line_count",walllinecount.text))
                                    {
                                        walllinecount.defaultBackgroundColor = Constants.dialogItemRectBgColor
                                        inputErrorStatus = inputErrorStatus & 0xFFFB
                                    }
                                    else
                                    {
                                        walllinecount.defaultBackgroundColor = "red"
                                        inputErrorStatus = inputErrorStatus | 0x0004
                                    }
                                }
                                onTextChanged:
                                {
                                    valchanged("wall_line_count",walllinecount.text)
                                }
                            }

                            StyledLabel {
                                id: element4
                                width : 160* screenScaleFactor
                                height : 30* screenScaleFactor
                                text: qsTr("Infill Sparse Density:")
                                strToolTip: qsTr(SettingJson.getDescription("infill_sparse_density"))
                                verticalAlignment: Qt.AlignVCenter
                                horizontalAlignment: Qt.AlignRight
                            }

                            BasicDialogTextField {
                                id: infillsparsedensity
                                width: 120* screenScaleFactor
                                height : 30* screenScaleFactor
                                text: SettingJson.infill_sparse_density
                                strToolTip: qsTr(SettingJson.getDescription("infill_sparse_density"))
                                onEditingFinished: {
                                    if(editProfiledchanged("infill_sparse_density",infillsparsedensity.text))
                                    {
                                        infillsparsedensity.defaultBackgroundColor = Constants.dialogItemRectBgColor
                                        inputErrorStatus = inputErrorStatus & 0xFFF7
                                    }
                                    else
                                    {
                                        infillsparsedensity.defaultBackgroundColor = "red"
                                        inputErrorStatus = inputErrorStatus | 0x0008
                                    }
                                }

                                onTextChanged:
                                {
                                    valchanged("infill_sparse_density",infillsparsedensity.text)
                                }
                                unitChar: "%"
                            }

                            StyledLabel {
                                id: element9
                                width : 160* screenScaleFactor
                                height : 30* screenScaleFactor
                                font.pointSize:9//Constants.labelLargeFontPointSize
                                text: qsTr("Adhesion Type:")
                                strToolTip: qsTr(SettingJson.getDescription("adhesion_type"))
                                verticalAlignment: Qt.AlignVCenter
                                horizontalAlignment: Qt.AlignRight
                            }

                            BasicCombobox {
                                id: adhesiontype
                                width: 120* screenScaleFactor
                                height : 30* screenScaleFactor
                                //currentIndex : -1
                                strToolTip: qsTr(SettingJson.getDescription("adhesion_type"))
                                model: ListModel {
                                    id: modelType
                                }
                                onCurrentIndexChanged: {
                                    editProfiledchanged("adhesion_type",modelType.get(currentIndex).key)
                                }
                                Component.onCompleted:{
                                    var modelMap = SettingJson.getSettingOptions("adhesion_type")
                                    for(var key in modelMap){
                                        console.log("Key = " +key + " value = " + modelMap[key])
                                        modelType.append({"key":key, "modelData":qsTr(modelMap[key])})
                                    }
                                }
                            }

                            StyledLabel {
                                id: element7
                                width : 160* screenScaleFactor
                                height : 30* screenScaleFactor
                                text: qsTr("Generate Support:")
                                strToolTip: qsTr(SettingJson.getDescription("support_enable"))
                                verticalAlignment: Qt.AlignVCenter
                                horizontalAlignment: Qt.AlignRight
                            }

                            StyleCheckBox {
                                id:supportenable
                                width: 129* screenScaleFactor
                                height : 30* screenScaleFactor
                                checked: SettingJson.support_enable
                                strToolTip: qsTr(SettingJson.getDescription("support_enable"))
                                onCheckedChanged: {
                                    console.log("support_enable changed")
                                    editProfiledchanged("support_enable",supportenable.checked.toString())
                                }
                            }

                            StyledLabel {
                                id: element8
                                width : 160* screenScaleFactor
                                height : 30* screenScaleFactor
                                font.pointSize:Constants.labelFontPointSize
                                text: qsTr("Support Overhang Angle:")
                                strToolTip: qsTr(SettingJson.getDescription("support_angle"))
                                verticalAlignment: Qt.AlignVCenter
                                horizontalAlignment: Qt.AlignRight
                            }

                            BasicDialogTextField {
                                id: supportangle
                                width: 120* screenScaleFactor
                                height : 30* screenScaleFactor
                                font.pointSize:Constants.labelFontPointSize
                                //validator: RegExpValidator{regExp: /^([1-9]|([1-9][0-9])|([1-2][0-9][0-9])|([3][0-5][0-9])|([0]{1}))$|^[3][6][0]$/}
                                text: SettingJson.support_angle
                                strToolTip: qsTr(SettingJson.getDescription("support_angle"))
                                onEditingFinished: {
                                    if(editProfiledchanged("support_angle",supportangle.text))
                                    {
                                        supportangle.defaultBackgroundColor = Constants.dialogItemRectBgColor
                                        inputErrorStatus = inputErrorStatus & 0xFFBF
                                    }
                                    else
                                    {
                                        supportangle.defaultBackgroundColor = "red"
                                        inputErrorStatus = inputErrorStatus | 0x0040
                                    }
                                }
                                onTextChanged:
                                {
                                    valchanged("support_angle",supportangle.text)
                                }
                                unitChar: "°"
                            }

                            StyledLabel {
                                id: element10
                                width : 160* screenScaleFactor
                                height : 30* screenScaleFactor
                                text: qsTr("Support Density:")
                                strToolTip: qsTr(SettingJson.getDescription("support_infill_rate"))
                                verticalAlignment: Qt.AlignVCenter
                                horizontalAlignment: Qt.AlignRight
                            }

                            BasicDialogTextField {
                                id: supportInfillRate
                                width: 120* screenScaleFactor
                                height : 30* screenScaleFactor
                                text: SettingJson.support_infill_rate
                                strToolTip: qsTr(SettingJson.getDescription("support_infill_rate"))
                                onEditingFinished: {
                                    if(editProfiledchanged("support_infill_rate",supportInfillRate.text))
                                    {
                                        supportInfillRate.defaultBackgroundColor = Constants.dialogItemRectBgColor
                                        inputErrorStatus = inputErrorStatus & 0xFF7F
                                    }
                                    else
                                    {
                                        supportInfillRate.defaultBackgroundColor = "red"
                                        inputErrorStatus = inputErrorStatus | 0x0080
                                    }
                                }

                                onTextChanged:
                                {
                                    valchanged("support_infill_rate",supportInfillRate.text)
                                }
                                unitChar: "%"
                            }

                            StyledLabel {
                                id: element11
                                width : 160* screenScaleFactor
                                height : 30* screenScaleFactor
                                text: qsTr("Small Feature Max Length:")
                                strToolTip: qsTr(SettingJson.getDescription("small_feature_max_length"))
                                verticalAlignment: Qt.AlignVCenter
                                horizontalAlignment: Qt.AlignRight
                            }

                            BasicDialogTextField {
                                id: smallfeaturemaxlength
                                width: 120* screenScaleFactor
                                height : 30* screenScaleFactor
                                text: SettingJson.small_feature_max_length
                                strToolTip: qsTr(SettingJson.getDescription("small_feature_max_length"))
                                onEditingFinished: {
                                    if(editProfiledchanged("small_feature_max_length",smallfeaturemaxlength.text))
                                    {
                                        smallfeaturemaxlength.defaultBackgroundColor = Constants.dialogItemRectBgColor
                                        inputErrorStatus = inputErrorStatus & 0xFEFF
                                    }
                                    else
                                    {
                                        smallfeaturemaxlength.defaultBackgroundColor = "red"
                                        inputErrorStatus = inputErrorStatus | 0x0100
                                    }
                                }

                                onTextChanged:
                                {
                                    valchanged("small_feature_max_length",smallfeaturemaxlength.text)
                                }
                                unitChar: "mm"
                            }

                        }
                        Grid{
                            id: idGrid2
                            columns: 2
                            rows: 10
                            rowSpacing: 7
                            columnSpacing:10

                            StyledLabel {
                                id: element5
                                width : 160* screenScaleFactor
                                height : 30* screenScaleFactor
                                text: qsTr("Printing Temperature:")
                                strToolTip: qsTr(SettingJson.getDescription("material_print_temperature"))
                                verticalAlignment: Qt.AlignVCenter
                                horizontalAlignment: Qt.AlignRight
                            }

                            BasicDialogTextField {
                                id: materialprinttemperature
                                width: 120* screenScaleFactor
                                height : 30* screenScaleFactor
                                text: SettingJson.material_print_temperature
                                strToolTip: qsTr(SettingJson.getDescription("material_print_temperature"))
                                onEditingFinished: {
                                    if(editProfiledchanged("material_print_temperature",materialprinttemperature.text))
                                    {
                                        materialprinttemperature.defaultBackgroundColor = Constants.dialogItemRectBgColor
                                        inputErrorStatus = inputErrorStatus & 0xFDFF
                                    }
                                    else
                                    {
                                        materialprinttemperature.defaultBackgroundColor = "red"
                                        inputErrorStatus = inputErrorStatus | 0x0200
                                    }
                                }
                                onTextChanged:
                                {
                                    valchanged("material_print_temperature",materialprinttemperature.text)
                                }
                                unitChar: "℃"
                            }

                            StyledLabel {
                                id: element6
                                width : 160* screenScaleFactor
                                height : 30* screenScaleFactor
                                text: qsTr("Build Plate Temperature:")
                                strToolTip: qsTr(SettingJson.getDescription("material_bed_temperature"))
                                verticalAlignment: Qt.AlignVCenter
                                horizontalAlignment: Qt.AlignRight
                            }

                            BasicDialogTextField {
                                id: materialbedtemperature
                                width: 120* screenScaleFactor
                                height : 30* screenScaleFactor
                                text: SettingJson.material_bed_temperature
                                strToolTip: qsTr(SettingJson.getDescription("material_bed_temperature"))
                                onEditingFinished: {
                                    if(editProfiledchanged("material_bed_temperature",materialbedtemperature.text))
                                    {
                                        materialbedtemperature.defaultBackgroundColor = Constants.dialogItemRectBgColor
                                        inputErrorStatus = inputErrorStatus & 0xFBFF
                                    }
                                    else
                                    {
                                        materialbedtemperature.defaultBackgroundColor = "red"
                                        inputErrorStatus = inputErrorStatus | 0x0400
                                    }
                                }
                                onTextChanged:
                                {
                                    valchanged("material_bed_temperature",materialbedtemperature.text)
                                }
                                unitChar: "℃"
                            }

                            StyledLabel {
                                id: element12
                                width : 160* screenScaleFactor
                                height : 30* screenScaleFactor
                                text: qsTr("Print Speed:")
                                strToolTip: qsTr(SettingJson.getDescription("speed_print"))
                                verticalAlignment: Qt.AlignVCenter
                                horizontalAlignment: Qt.AlignRight
                            }

                            BasicDialogTextField {
                                id: speedprint
                                width: 120* screenScaleFactor
                                height : 30* screenScaleFactor
                                text: SettingJson.speed_print
                                strToolTip: qsTr(SettingJson.getDescription("speed_print"))
                                onEditingFinished: {
                                    if(editProfiledchanged("speed_print",speedprint.text))
                                    {
                                        speedprint.defaultBackgroundColor = Constants.dialogItemRectBgColor
                                        inputErrorStatus = inputErrorStatus & 0xF7FF
                                    }
                                    else
                                    {
                                        speedprint.defaultBackgroundColor = "red"
                                        inputErrorStatus = inputErrorStatus | 0x0800
                                    }
                                }
                                onTextChanged:
                                {
                                    valchanged("speed_print",speedprint.text)
                                }
                                unitChar: "mm/s"
                            }

                            StyledLabel {
                                id: element13
                                width : 160* screenScaleFactor
                                height : 30* screenScaleFactor
                                text: qsTr("Infill Speed:")
                                strToolTip: qsTr(SettingJson.getDescription("speed_infill"))
                                verticalAlignment: Qt.AlignVCenter
                                horizontalAlignment: Qt.AlignRight
                            }

                            BasicDialogTextField {
                                id: speedinfill
                                width: 120* screenScaleFactor
                                height : 30* screenScaleFactor
                                text: SettingJson.speed_infill
                                strToolTip: qsTr(SettingJson.getDescription("speed_infill"))
                                onEditingFinished: {
                                    if(editProfiledchanged("speed_infill",speedinfill.text))
                                    {
                                        speedinfill.defaultBackgroundColor = Constants.dialogItemRectBgColor
                                        inputErrorStatus = inputErrorStatus & 0xEFFF
                                    }
                                    else
                                    {
                                        speedinfill.defaultBackgroundColor = "red"
                                        inputErrorStatus = inputErrorStatus | 0x1000
                                    }
                                }
                                onTextChanged:
                                {
                                    valchanged("speed_infill",speedinfill.text)
                                }
                                unitChar: "mm/s"
                            }

                            StyledLabel {
                                id: element14
                                width : 160* screenScaleFactor
                                height : 30* screenScaleFactor
                                text: qsTr("Wall Speed:")
                                strToolTip: qsTr(SettingJson.getDescription("speed_wall"))
                                verticalAlignment: Qt.AlignVCenter
                                horizontalAlignment: Qt.AlignRight
                            }

                            BasicDialogTextField {
                                id: speedwall
                                width: 120* screenScaleFactor
                                height : 30* screenScaleFactor
                                text: SettingJson.speed_wall
                                strToolTip: qsTr(SettingJson.getDescription("speed_wall"))
                                onEditingFinished: {
                                    if(editProfiledchanged("speed_wall",speedwall.text))
                                    {
                                        speedwall.defaultBackgroundColor = Constants.dialogItemRectBgColor
                                        inputErrorStatus = inputErrorStatus & 0xBFFF
                                    }
                                    else
                                    {
                                        speedwall.defaultBackgroundColor = "red"
                                        inputErrorStatus = inputErrorStatus | 0x4000
                                    }
                                }
                                onTextChanged:
                                {
                                    valchanged("speed_wall",speedwall.text)
                                }
                                unitChar: "mm/s"
                            }

                            StyledLabel {
                                id: element15
                                width : 160* screenScaleFactor
                                height : 30* screenScaleFactor
                                text: qsTr("Support Flow:")
                                strToolTip: qsTr(SettingJson.getDescription("support_material_flow"))
                                verticalAlignment: Qt.AlignVCenter
                                horizontalAlignment: Qt.AlignRight
                            }

                            BasicDialogTextField {
                                id: supportmaterialflow
                                width: 120* screenScaleFactor
                                height : 30* screenScaleFactor
                                text: SettingJson.support_material_flow
                                strToolTip: qsTr(SettingJson.getDescription("support_material_flow"))
                                onEditingFinished: {
                                    if(editProfiledchanged("support_material_flow",supportmaterialflow.text))
                                    {
                                        supportmaterialflow.defaultBackgroundColor = Constants.dialogItemRectBgColor
                                        inputErrorStatus = inputErrorStatus & 0x7FFF
                                    }
                                    else
                                    {
                                        supportmaterialflow.defaultBackgroundColor = "red"
                                        inputErrorStatus = inputErrorStatus | 0x8000
                                    }
                                }
                                onTextChanged:
                                {
                                    valchanged("support_material_flow",supportmaterialflow.text)
                                }
                                unitChar: "%"
                            }

                            StyledLabel {
                                id: element16
                                width : 160* screenScaleFactor
                                height : 30* screenScaleFactor
                                text: qsTr("Infill Flow:")
                                strToolTip: qsTr(SettingJson.getDescription("infill_material_flow"))
                                verticalAlignment: Qt.AlignVCenter
                                horizontalAlignment: Qt.AlignRight
                            }

                            BasicDialogTextField {
                                id: infillmaterialflow
                                width: 120* screenScaleFactor
                                height : 30* screenScaleFactor
                                text: SettingJson.infill_material_flow
                                strToolTip: qsTr(SettingJson.getDescription("infill_material_flow"))
                                onEditingFinished: {
                                    if(editProfiledchanged("infill_material_flow",infillmaterialflow.text))
                                    {
                                        infillmaterialflow.defaultBackgroundColor = Constants.dialogItemRectBgColor
                                        inputErrorStatus2 = inputErrorStatus2 & 0xFFFE
                                    }
                                    else
                                    {
                                        infillmaterialflow.defaultBackgroundColor = "red"
                                        inputErrorStatus2 = inputErrorStatus2 | 0x0001
                                    }
                                }
                                onTextChanged:
                                {
                                    valchanged("infill_material_flow",infillmaterialflow.text)
                                }
                                unitChar: "%"
                            }

                            StyledLabel {
                                id: element17
                                width : 160* screenScaleFactor
                                height : 30* screenScaleFactor
                                text: qsTr("Wall Flow:")
                                strToolTip: qsTr(SettingJson.getDescription("wall_material_flow"))
                                verticalAlignment: Qt.AlignVCenter
                                horizontalAlignment: Qt.AlignRight
                            }

                            BasicDialogTextField {
                                id: wallmaterialflow
                                width: 120* screenScaleFactor
                                height : 30* screenScaleFactor
                                text: SettingJson.wall_material_flow
                                strToolTip: qsTr(SettingJson.getDescription("wall_material_flow"))
                                onEditingFinished: {
                                    if(editProfiledchanged("wall_material_flow",wallmaterialflow.text))
                                    {
                                        wallmaterialflow.defaultBackgroundColor = Constants.dialogItemRectBgColor
                                        inputErrorStatus2 = inputErrorStatus2 & 0xFFFD
                                    }
                                    else
                                    {
                                        wallmaterialflow.defaultBackgroundColor = "red"
                                        inputErrorStatus2 = inputErrorStatus2 | 0x0002
                                    }
                                }
                                onTextChanged:
                                {
                                    valchanged("wall_material_flow",wallmaterialflow.text)
                                }
                                unitChar: "%"
                            }

                            StyledLabel {
                                id: element18
                                width : 160* screenScaleFactor
                                height : 30* screenScaleFactor
                                text: qsTr("Small Feature Speed:")
                                strToolTip: qsTr(SettingJson.getDescription("small_feature_speed_factor"))
                                verticalAlignment: Qt.AlignVCenter
                                horizontalAlignment: Qt.AlignRight
                            }

                            BasicDialogTextField {
                                id: smallfeaturespeedfactor
                                width: 120* screenScaleFactor
                                height : 30* screenScaleFactor
                                text: SettingJson.small_feature_speed_factor
                                strToolTip: qsTr(SettingJson.getDescription("small_feature_speed_factor"))
                                onEditingFinished: {
                                    if(editProfiledchanged("small_feature_speed_factor",smallfeaturespeedfactor.text))
                                    {
                                        smallfeaturespeedfactor.defaultBackgroundColor = Constants.dialogItemRectBgColor
                                        inputErrorStatus2 = inputErrorStatus2 & 0xFFFB
                                    }
                                    else
                                    {
                                        smallfeaturespeedfactor.defaultBackgroundColor = "red"
                                        inputErrorStatus2 = inputErrorStatus2 | 0x0004
                                    }
                                }
                                onTextChanged:
                                {
                                    valchanged("small_feature_speed_factor",smallfeaturespeedfactor.text)
                                }
                                unitChar: "%"
                            }
                        }
                    }
                }
            }

            Item {
                width: editProfiledlg.width - 13* screenScaleFactor
                height : 2* screenScaleFactor

                Rectangle {
                    // anchors.left: idCol.left
                    // anchors.leftMargin: -10
                    x: -23* screenScaleFactor
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
                height:  30* screenScaleFactor
                Grid
                {
                    //                    x:(parent.width - width) /2
                    columns: 4
                    spacing: 10
                    width: 430* screenScaleFactor
                    height: 30* screenScaleFactor
                    anchors.centerIn:parent
                    BasicButton {
                        id: basicComButton1
                        width: 98* screenScaleFactor
                        height: 28* screenScaleFactor
                        text: qsTr("Save")//(isDefault && addOrEdit == 1)? qsTr("Save as") : qsTr("Save")
                        btnRadius:3
                        btnBorderW:0
                        defaultBtnBgColor: Constants.profileBtnColor
                        hoveredBtnBgColor: Constants.profileBtnHoverColor
                        onSigButtonClicked:
                        {
                            if((inputErrorStatus + specialErrorStatus + inputErrorStatus2) == 0)
                            {
                                saveProfile()
                                editProfiledlg.close()
                            }
                            else
                            {
                                idMessageDlg.show()
                            }
                        }
                    }

                    BasicButton {
                        id: basicComButton
                        width: 98* screenScaleFactor
                        height: 28* screenScaleFactor
                        text: qsTr("Reset")
                        btnRadius:3
                        btnBorderW:0
                        defaultBtnBgColor: Constants.profileBtnColor
                        hoveredBtnBgColor: Constants.profileBtnHoverColor
                        onSigButtonClicked:
                        {
                            reseted()
                            resetValue()
                        }
                    }

                    BasicButton {
                        id: basicComButton3
                        width: 98* screenScaleFactor
                        height: 28* screenScaleFactor
                        text: enabled ? qsTr("Advance Setting") : qsTr("Loading")+"("+ countDown.timeCnt +"s)"
                        btnRadius:3
                        btnBorderW:0
                        defaultBtnBgColor: Constants.profileBtnColor
                        hoveredBtnBgColor: Constants.profileBtnHoverColor
                        onSigButtonClicked:
                        {
                            if(idEditProfileByFilter.getCreateFinished())
                            {
                                showEditProfileByFilter()
                            }
                            else
                            {
                                countDown.timeCnt = 10
                                countDown.start()
                            }
                        }
                    }

                    BasicButton
                    {
                        id : comCancel
                        width: 98* screenScaleFactor
                        height: 28* screenScaleFactor

                        text: qsTr("Cancel")
                        btnRadius:3
                        btnBorderW:0
                        defaultBtnBgColor: Constants.profileBtnColor
                        hoveredBtnBgColor: Constants.profileBtnHoverColor

                        onSigButtonClicked:
                        {
                            canceled()
                            editProfiledlg.close()
                        }
                    }
                }

            }

        }
    }

    Timer{
        id:countDown;
        interval: 1000;
        repeat: true;
        triggeredOnStart: true;
        property var timeCnt:10
        onTriggered: {
            timeCnt -= 1;
            //basicComButton3.text = qsTr("Loading")+"("+ timeCnt +"s)"
            basicComButton3.enabled = false

            if(timeCnt < 0 )
            {
                //basicComButton3.text = qsTr("Advance Setting")
                basicComButton3.enabled = true
                countDown.stop()
            }
            if(idEditProfileByFilter.getCreateFinished())
            {
                console.log("timeout timeCnt:"+ timeCnt)
                showEditProfileByFilter()

                //basicComButton3.text = qsTr("Advance Setting")
                basicComButton3.enabled = true
                countDown.stop()
            }
        }
    }

    BasicDialog
    {
        id: idMessageDlg
        width: 400* screenScaleFactor
        height: 200* screenScaleFactor
        titleHeight : 30
        title: qsTr("Message")

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
                text: qsTr("Invalid parameter!!")
                font.pixelSize: Constants.labelFontPixelSize
                color: Constants.textColor
            }
        }
    }

    EditProfileByFilter
    {
        id: idEditProfileByFilter
        objectName:"editprofilebyfilterobj"
        onValchangedAd:
        {
            valchanged(key,value)
        }
        onResetedAd: {
            reseted()
        }
        onSaveProfileAd:{
            saveProfile()
			
            //reseted()
			SettingJson.updateSettingData()
            idEditProfileByFilter.updateComponent()
			
            resetValue()
        }
    }

    onDialogClosed:
    {

    }
}

