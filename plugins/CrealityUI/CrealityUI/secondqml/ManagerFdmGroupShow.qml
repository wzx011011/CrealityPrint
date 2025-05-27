import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
//import CrealityUI 1.0
//import "qrc:/CrealityUI"
import ".."
import "../qml"
Item
{
    //    height: 500
    //    property var machine_shape:keyTranslateValue(idPlatform.displayText)
    property var machine_name:""
    property alias machine_shape:idPlatform.modelText
    property alias machine_gcode_flavor:idFlavor.modelText
    property alias machine_center_is_zero: idCenterOrigin.checked
    //property alias machine_name: idMachinetype.text
    property alias machine_height: idPrinterHeight.text
    property alias machine_width: idPrinterWidth.text

    property alias machine_start_gcode: idMachineGcode_Start.text
    property alias machine_end_gcode: idMachineGcode_End.text

    property alias machine_heated_bed: idHotBed.checked
    property alias has_preview_img: idHas_preview_img.checked
    property alias preview_img_type: idpic.modelText
    property alias screen_size: idScreen.modelText
    //
    property alias machine_extruder_count : idNozzlesNumber.text//3
    property alias machine_head_with_fans_polygon : idHeadAndSize.text //4
    property alias machine_depth:idMachineDepth.text//5
    property alias machine_nozzle_size: idNozzleDiameter.text //6

    property alias machine_extruder_type: idExtruderType.modelText //6


    property alias managerFdmvisable: idFdmGroupBox.visible
    signal valchanged(string key,string value)
    //if idNozzlesNumber.text = 2, emit extruderShow
    signal sigExtruderShow()
    //Extrusion
    property alias currentTabIndex: idTabBar.currentIndex
    property alias machine_extruder_start_code_0 :idExtrusion0_Start.text
    property alias machine_extruder_end_code_0: idExtrusion0_End.text
    property alias machine_extruder_start_code_1: idExtrusion1_Start.text
    property alias machine_extruder_end_code_1: idExtrusion1_End.text
    //type =0 is Extrusion0
    signal valChanged_Extruder(string key,string value,int type)
    signal extruderCntChanged(int cnt)

    function keyTranslateValue(key)
    {
        var value = ""
        if(key === "cirle")value = qsTr("cirle")
        else if(key === "rectangle")value = qsTr("rectangle")
        else if(key === "Distal")value = qsTr("Distal")
        else if(key === "Proximal")value = qsTr("Proximal")
        else if(key === "RepRap (Marlin/Sprinter)")value = qsTr("Marlin")
        else if(key === "MACH3(Creality)")value = qsTr("Mach3(Creality)")
        else value = key
        return value
    }
    function valueTranslateKey(value)
    {
        var key = ""
        if(value === qsTr("cirle"))key = "cirle"
        else if(value === qsTr("rectangle"))key = "rectangle"
        else if(value === qsTr("Distal"))key = "Distal"
        else if(value === qsTr("Proximal"))key = "Proximal"
        else if(key === "RepRap (Marlin/Sprinter)")value = qsTr("Marlin")
        else if(key === "MACH3(Creality)")value = qsTr("Mach3(Creality)")
        else key = value
        return key
    }
    function initValExtruder()
    {
        if(Number(idNozzlesNumber.text) >= 1)
        {
            idExtrusion0_Start.text = "\nT0 ;switch to extruder 0\nG92 E0 ;reset extruder distance\nG1 F2000 E93 ;load filament\nG92 E0 ;reset extruder distance\nM104 S200";
            idExtrusion0_End.text = "\nG92 E0 ;reset extruder distance\nG1 F800 E-5 ;short retract\nG1 F2400 X295 Y265 ;move near prime tower\nG1 F2000 E-93 ;long retract for filament removal\nG92 E0 ;reset extruder distance\nG90"
        }

        if(Number(idNozzlesNumber.text) >= 2)
        {
            idExtrusion1_Start.text = "\nT1 ;switch to extruder 1\nG92 E0 ;reset extruder distance\nG1 F2000 E93 ;load filament\nG92 E0 ;reset extruder distance\nM104 S200"
            idExtrusion1_End.text ="\nG92 E1 ;reset extruder distance\nG1 F800 E-5 ;short retract\nG1 F2400 X295 Y265 ;move near prime tower\nG1 F2000 E-93 ;long retract for filament removal\nG92 E0 ;reset extruder distance\nG90"
        }
    }
    function updateValExtruder()
    {
        if(Number(idNozzlesNumber.text) >= 1)
        {
            valChanged_Extruder("machine_extruder_start_code",idExtrusion0_Start.text,0)
            valChanged_Extruder("machine_extruder_end_code",idExtrusion0_End.text,0)
        }

        if(Number(idNozzlesNumber.text) >= 2)
        {
            valChanged_Extruder("machine_extruder_start_code",idExtrusion1_Start.text,1)
            valChanged_Extruder("machine_extruder_end_code",idExtrusion1_End.text,1)
        }
    }

    //cmb init translate
    function cmbTranslateUpdate()
    {
        idPlatform.displayText= keyTranslateValue(idPlatform.modelText)
        //idExtruderType.displayText = keyTranslateValue(idExtruderType.modelText)
    }
    BasicTabBar
    {
        id : idTabBar
        backgroundColor: "transparent"
        BasicTabButton
        {
            text: qsTr("Basic Parameters")
            width: 120*screenScaleFactor
            height: 30*screenScaleFactor
            //buttonColor: (checked||hovered)? Constants.itemBackgroundColor : "#454545"
        }
        BasicTabButton
        {
            text: qsTr("Machine G-Code")
            width: 120*screenScaleFactor
            height: 30*screenScaleFactor
            visible: true
            //buttonColor: (checked||hovered)? Constants.itemBackgroundColor : "#454545"
        }
        BasicTabButton
        {
            text: qsTr("Extrusion 1")
            width: 120*screenScaleFactor
            height: 30*screenScaleFactor
            visible: Number(idNozzlesNumber.text) >= 1 ? true : false
            //buttonColor: (checked||hovered)? Constants.itemBackgroundColor : "#454545"
        }
        BasicTabButton
        {
            text: qsTr("Extrusion 2")
            width: 120*screenScaleFactor
            height: 30*screenScaleFactor
            visible: Number(idNozzlesNumber.text) >= 2 ? true : false
            //buttonColor: (checked||hovered)? Constants.itemBackgroundColor : "#454545"
        }
    }
    StackLayout {
        id: swipeView
        x:10*screenScaleFactor
        width:parent.width - 10*screenScaleFactor
        height: parent.height - 60*screenScaleFactor
        currentIndex:idTabBar.currentIndex;
        anchors.top: idTabBar.bottom
        anchors.topMargin: 10*screenScaleFactor

        Item {
            implicitWidth: parent.width
            implicitHeight: parent.height
            Item
            {
                id: idFdmGroupBox
                visible: true
                width: parent.width
                height: parent.height
                //defaultBgColor: "white"
                //textColor:"black"
                //font.pointSize: 16
                //title: qsTr("Basic Parameters")
                Grid
                {
                    id : idMainGrid
                    spacing: 40
                    columns: 2
                    Grid {
                        id: idGrid1
                        columns: 2
                        rows: 3/*4*/
                        spacing: 10
                        StyledLabel {
                            id: label1
                            height: 30*screenScaleFactor
                            width : 160*screenScaleFactor
                            text: qsTr("Build Plate Shape:")
                            verticalAlignment: Qt.AlignVCenter
                            horizontalAlignment: Qt.AlignRight
                        }
                        BasicCombobox {
                            id: idPlatform
                            height: 30*screenScaleFactor
                            width : 130*screenScaleFactor
                            font.pointSize: Constants.labelLargeFontPointSize
                            currentIndex : 0
                            property var modelText: "0"
                            onModelTextChanged: {
                                displayText = keyTranslateValue(modelText)
                            }

                            model: ListModel {
                                //ListElement { text: qsTr("cirle");}
                                ListElement { text: qsTr("rectangle");}
                            }
                            onCurrentTextChanged: {
                                console.log("displayText" + currentText);

                                displayText = currentText
                                modelText = valueTranslateKey(displayText)

                                valchanged("machine_shape",valueTranslateKey(currentText))
                            }
                        }



                        StyledLabel {
                            id: label5
                            width : 160*screenScaleFactor
                            height: 30*screenScaleFactor
                            text: qsTr("Machine Height(Max):")
                            verticalAlignment: Qt.AlignVCenter
                            horizontalAlignment: Qt.AlignRight
                        }

                        BasicDialogTextField {
                            id: idPrinterHeight
                            height: 30*screenScaleFactor
                            // text: machine_height
                            font.pointSize:Constants.labelLargeFontPointSize
                            width : 130*screenScaleFactor
                            verticalAlignment: Qt.AlignVCenter
                            onEditingFinished:{
                                if(text < 1 || text === "")
                                {
                                    text = 1
                                }
                                valchanged("machine_height",idPrinterHeight.text)
                            }
                            validator: RegExpValidator {
                                regExp:  /^(?!(0[0-9]{0,}$))[0-9]{1,}[.]{0,}[0-9]{0,}$/
                            }
                            unitChar: "mm"
                        }
                        StyledLabel {
                            id: label7
                            width : 160*screenScaleFactor
                            height: 30*screenScaleFactor
                            text: qsTr("Machine Width(Max):")
                            verticalAlignment: Qt.AlignVCenter
                            horizontalAlignment: Qt.AlignRight
                        }
                        BasicDialogTextField {
                            id: idPrinterWidth
                            height: 30*screenScaleFactor
                            font.pointSize:Constants.labelLargeFontPointSize
                            width : 130*screenScaleFactor
                            verticalAlignment: Qt.AlignVCenter
                            unitChar: "mm"
                            onActiveFocusChanged:
                            {
                                if(text < 1 || text === "")
                                {
                                    text = 1
                                }
                            }
                            onEditingFinished:{
                                valchanged("machine_width",idPrinterWidth.text)
                            }
                            validator: RegExpValidator {
                                regExp:  /^(?!(0[0-9]{0,}$))[0-9]{1,}[.]{0,}[0-9]{0,}$/
                            }
                        }

                    }
                    Grid {
                        id: idGrid2
                        columns: 2
                        rows: 3/*4*/
                        columnSpacing: 10
                        rowSpacing: 10
                        StyledLabel {
                            id: label2
                            height: 30*screenScaleFactor
                            width : 130*screenScaleFactor
                            text: qsTr("Number of Extruders:")
                            verticalAlignment: Qt.AlignVCenter
                            horizontalAlignment: Qt.AlignRight
                        }
                        BasicDialogTextField {
                            id: idNozzlesNumber
                            height: 30*screenScaleFactor
                            //      text: machine_extruder_count
                            font.pointSize:Constants.labelLargeFontPointSize
                            width:100*screenScaleFactor
                            verticalAlignment: Qt.AlignVCenter
                            placeholderText : qsTr("please input 1-2")
                            validator: RegExpValidator {
                                regExp:   /^[12]$/
                            }
                            onActiveFocusChanged:
                            {
                                if(text === "")
                                {
                                    text = 1
                                }
                            }
                            onTextChanged:
                            {
                                if(text === ""){return}
                                valchanged("machine_extruder_count",idNozzlesNumber.text)
                                if(text == "2")
                                {
                                    //initValExtruder();
                                    //sigExtruderShow();
                                    //updateValExtruder();
                                    extruderCntChanged(idNozzlesNumber.text)
                                }
                            }
                        }
                        StyledLabel {
                            id: label8
                            height: 30*screenScaleFactor
                            width : 130*screenScaleFactor
                            text: qsTr("Nozzle Diameter:")
                            verticalAlignment: Qt.AlignVCenter
                            horizontalAlignment: Qt.AlignRight
                        }
                        BasicDialogTextField {
                            id: idNozzleDiameter
                            width:100*screenScaleFactor
                            height: 30*screenScaleFactor
                            //     text: machine_nozzle_size
                            font.pointSize:Constants.labelLargeFontPointSize
                            verticalAlignment: Qt.AlignVCenter
                            unitChar: "mm"
                            onActiveFocusChanged:
                            {
                                if(text < 0.1 || text === "")
                                {
                                    text = 0.1
                                }
                            }
                            onEditingFinished:{
                                valchanged("machine_nozzle_size",idNozzleDiameter.text)
                            }
                        }
                        StyledLabel {
                            id: label6
                            height: 30*screenScaleFactor
                            width : 130*screenScaleFactor
                            text: qsTr("Machine Depth:")
                            verticalAlignment: Qt.AlignVCenter
                            horizontalAlignment: Qt.AlignRight
                        }

                        BasicDialogTextField {
                            id: idMachineDepth
                            height: 30*screenScaleFactor
                            width : 100*screenScaleFactor
                            //         text: machine_depth
                            font.pointSize:Constants.labelLargeFontPointSize

                            verticalAlignment: Qt.AlignVCenter
                            unitChar: "mm"
                            onActiveFocusChanged:
                            {
                                if(text < 1 || text === "")
                                {
                                    text = 1
                                }
                            }
                            onEditingFinished:{

                                valchanged("machine_depth",idMachineDepth.text)
                            }
                            validator: RegExpValidator {
                                regExp:  /^(?!(0[0-9]{0,}$))[0-9]{1,}[.]{0,}[0-9]{0,}$/
                            }
                        }
                    }
                }
                StyledLabel {
                    id: idHeadAndSizeLabel
                    anchors.left: idMainGrid.left
                    anchors.top: idMainGrid.bottom
                    anchors.topMargin: 10*screenScaleFactor
                    width : 160*screenScaleFactor
                    height: 30*screenScaleFactor
                    text: qsTr("Machine Head & Fan Polygon:")
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignRight
                }

                BasicDialogTextField {
                    id: idHeadAndSize
                    anchors.left: idHeadAndSizeLabel.right
                    anchors.leftMargin: 10*screenScaleFactor
                    anchors.top: idMainGrid.bottom
                    anchors.topMargin: 10*screenScaleFactor
                    width:350*screenScaleFactor
                    height: 30*screenScaleFactor
                    //  text: machine_head_with_fans_polygon
                    font.pointSize:Constants.labelLargeFontPointSize
                    verticalAlignment: Qt.AlignVCenter
                    onTextEdited:
                    {
                        valchanged("machine_head_with_fans_polygon",idHeadAndSize.text)
                    }
                }

                Column
                {
                    anchors.left: idHeadAndSizeLabel.left
                    anchors.top: idHeadAndSizeLabel.bottom
                    anchors.leftMargin: 0
                    anchors.topMargin: 10*screenScaleFactor
                    spacing: 10
                    Row {
                        spacing:10
                        StyledLabel {//by TCJ
                            id: label4
                            width: 160*screenScaleFactor
                            height: 30*screenScaleFactor
                            text: qsTr("Generate Preview:")
                            verticalAlignment: Qt.AlignVCenter
                            horizontalAlignment: Qt.AlignRight
                        }

                        StyleCheckBox {
                            id: idHas_preview_img
                            width: 30*screenScaleFactor
                            height: 30*screenScaleFactor
                            checked: false
                            onCheckedChanged:
                            {
                                valchanged("has_preview_img",idHas_preview_img.checked.toString())
                            }
                        }
                    }

                    Row {
                        spacing:10
                        visible: idHas_preview_img.checked
                        StyledLabel {
                            id: label_Screen
                            height: 30*screenScaleFactor
                            width : 160*screenScaleFactor
                            text: qsTr("Screen Size:")
                            //visible: idHas_preview_img.checked
                            verticalAlignment: Qt.AlignVCenter
                            horizontalAlignment: Qt.AlignRight
                        }
                        BasicCombobox {
                            id: idScreen
                            height : 30*screenScaleFactor
                            width : 130*screenScaleFactor
                            font.pointSize:Constants.labelLargeFontPointSize
                            //visible: idHas_preview_img.checked
                            //enabled: idHas_preview_img.checked
                            property var modelText: ""
                            currentIndex : 0
                            enabled:(machine_name != "Sermoon D3") && (machine_name != "CR-200B Pro")
                            model: ListModel {
                                id: screensSizeModel
                                ListElement {key:"Sermoon D3"; modelData: "480*800";}
                                ListElement {key:"CR-200B Pro"; modelData: "480*272";}
                                ListElement {key:"CR-10 Inspire"; modelData: "600*1024";}
                            }

                            onModelTextChanged: {
                                if(modelText == "Sermoon D3")
                                {
                                    currentIndex = 0
                                }
                                else if(modelText == "CR-200B Pro")
                                {
                                    currentIndex = 1
                                }
                                else if(modelText == "CR-10 Inspire")
                                {
                                    currentIndex = 2
                                }
                            }
                            onCurrentIndexChanged: {
                                modelText = screensSizeModel.get(currentIndex).key
                                valchanged("screen_size",modelText)
                            }
                        }

                        Rectangle{
                            width:20*screenScaleFactor
                            height:30*screenScaleFactor
                            color:"transparent"
                        }

                        StyledLabel {
                            id: label_pic
                            //anchors.topMargin: 10
                            height: 30*screenScaleFactor
                            width : 130*screenScaleFactor
                            text: qsTr("Preview Type:")
                            //visible: idHas_preview_img.checked
                            verticalAlignment: Qt.AlignVCenter
                            horizontalAlignment: Qt.AlignRight
                        }
                        BasicCombobox {
                            id: idpic
                            height : 30*screenScaleFactor
                            width : 100*screenScaleFactor
                            font.pointSize:Constants.labelLargeFontPointSize
                            //visible: idHas_preview_img.checked
                            //enabled: idHas_preview_img.checked
                            property var modelText: ""
                            currentIndex : 0
                            model: ListModel {
                                id: picTypeModel
                                ListElement {key:"bmp"; modelData: "bmp";}
                                ListElement {key:"png"; modelData: "png";}
                                ListElement {key:"jpg"; modelData: "jpg";}
                            }

                            onModelTextChanged: {
                                if(modelText == "bmp")
                                {
                                    currentIndex = 0
                                }
                                else if(modelText == "png")
                                {
                                    currentIndex = 1
                                }
                                else
                                {
                                    currentIndex = 2
                                }
                            }
                            onCurrentIndexChanged: {
                                modelText = picTypeModel.get(currentIndex).key
                                valchanged("preview_img_type",modelText)
                            }
                        }
                    }

                    Row{
                        spacing:10
                        StyledLabel {
                            id: label10
                            height: 30*screenScaleFactor
                            width : 160*screenScaleFactor
                            text: qsTr("Extruder Type:")
                            verticalAlignment: Qt.AlignVCenter
                            horizontalAlignment: Qt.AlignRight
                        }

                        BasicCombobox {
                            id: idExtruderType
                            height: 30*screenScaleFactor
                            width : 130*screenScaleFactor
                            font.pointSize:Constants.labelLargeFontPointSize
                            currentIndex: 0
                            property var modelText: ""
                            displayText: model.get(currentIndex).modelData
                            model: ListModel {
                                id: modelType
                                ListElement {key:"Distal"; modelData: qsTr("Distal");}
                                ListElement {key:"Proximal"; modelData: qsTr("Proximal");}
                            }
                            onModelTextChanged: {
                                if(modelText == "Distal")
                                {
                                    currentIndex = 0
                                }
                                else
                                {
                                    currentIndex = 1
                                }
                            }
                            onCurrentIndexChanged: {
                                modelText = modelType.get(currentIndex).key
                                valchanged("machine_extruder_type",modelText)
                            }

                        }

                        Rectangle{
                            width:20*screenScaleFactor
                            height:30*screenScaleFactor
                            color:"transparent"
                        }

                        StyledLabel {
                            id: label9
                            height: 30*screenScaleFactor
                            width : 130*screenScaleFactor
                            text: qsTr("Has Heated Build Plate:")
                            verticalAlignment: Qt.AlignVCenter
                            horizontalAlignment: Qt.AlignRight
                        }

                        StyleCheckBox {
                            id: idHotBed
                            height: 30*screenScaleFactor
                            width : 60*screenScaleFactor
                            onCheckedChanged:
                            {
                                valchanged("machine_heated_bed",idHotBed.checked.toString())
                            }
                        }
                    }

                    Row{
                        spacing:10
                        StyledLabel {
                            id: labelFlavor
                            height: 30*screenScaleFactor
                            width : 160*screenScaleFactor
                            text: qsTr("G-code Flavor:")
                            verticalAlignment: Qt.AlignVCenter
                            horizontalAlignment: Qt.AlignRight
                        }
                        BasicCombobox {
                            id: idFlavor
                            height: 30*screenScaleFactor
                            width : 130*screenScaleFactor
                            font.pointSize:Constants.labelLargeFontPointSize
                            currentIndex : 0
                            property var modelText: ""

                            displayText: model.get(currentIndex).modelData
                            model: ListModel {
                                id: modelFlavor
                                ListElement {key:"RepRap (Marlin/Sprinter)"; modelData: qsTr("Marlin")}
                                ListElement {key:"MACH3(Creality)"; modelData: qsTr("MACH3(Creality)")}
                            }
                            onModelTextChanged: {
                                if(modelText == "RepRap (Marlin/Sprinter)")
                                {
                                    currentIndex = 0
                                }
                                else
                                {
                                    currentIndex = 1
                                }
                            }
                            onCurrentIndexChanged: {
                                modelText = modelFlavor.get(currentIndex).key
                                valchanged("machine_gcode_flavor",modelText)
                            }
                        }

                        Rectangle{
                            width:20*screenScaleFactor
                            height:30*screenScaleFactor
                            color:"transparent"
                        }


                        StyledLabel {
                            id: labelCenterOrigin
                            height: 30*screenScaleFactor
                            width : 130*screenScaleFactor
                            text: qsTr("Is Center Origin:")
                            verticalAlignment: Qt.AlignVCenter
                            horizontalAlignment: Qt.AlignRight
                        }

                        StyleCheckBox {
                            id: idCenterOrigin
                            height: 30*screenScaleFactor
                            width : 60*screenScaleFactor
                            onCheckedChanged:
                            {
                                valchanged("machine_center_is_zero",idCenterOrigin.checked.toString())
                            }
                        }
                    }
                }

                // StyledLabel {//by TCJ
                //     id: label4
                //     anchors.left: idHeadAndSizeLabel.left
                //     anchors.top: idHeadAndSizeLabel.bottom
                //     anchors.leftMargin: 0
                //     anchors.topMargin: 10
                //     width: 160
                //     height: 30
                //     text: qsTr("Generate Preview:")
                //     verticalAlignment: Qt.AlignVCenter
                //     horizontalAlignment: Qt.AlignRight
                // }

                // StyleCheckBox {
                //     id: idHas_preview_img
                //     anchors.left: idHeadAndSizeLabel.right
                //     anchors.leftMargin: 10
                //     anchors.top: idHeadAndSizeLabel.bottom
                //     anchors.topMargin: 10
                //     width: 30//60
                //     height: 30
                //     onCheckedChanged:
                //     {
                //         valchanged("has_preview_img",idHas_preview_img.checked.toString())
                //     }
                // }

                // StyledLabel {
                //     id: label_Screen
                //     anchors.left: label4.left
                //     anchors.top: label4.bottom
                //     anchors.leftMargin: 0
                //     anchors.topMargin: 10
                //     height: 30
                //     width : 160
                //     text: qsTr("Screen Size:")
                //     visible: idHas_preview_img.checked
                //     verticalAlignment: Qt.AlignVCenter
                //     horizontalAlignment: Qt.AlignRight
                // }
                // BasicCombobox {
                // 	id: idScreen
                //     anchors.left: label_Screen.right
                //     anchors.leftMargin: 10
                //     anchors.top: label_Screen.top
                //     anchors.topMargin: 0
                // 	height : 30
                // 	width : 130
                //     font.pointSize:Constants.labelLargeFontPointSize
                //     visible: idHas_preview_img.checked
                //     enabled: idHas_preview_img.checked
                //     property var modelText: ""
                //     currentIndex : 0
                // 	model: ListModel {
                // 		id: screensSizeModel
                //         ListElement {key:"Sermoon D3"; modelData: "480*800";}
                // 		ListElement {key:"CR-200B Pro"; modelData: "480*272";}
                // 	}

                //     onModelTextChanged: {
                //         if(modelText == "Sermoon D3")
                //         {
                //             currentIndex = 0
                //         }
                //         else if(modelText == "CR-200B Pro")
                //         {
                //             currentIndex = 1
                //         }
                //     }
                //     onCurrentIndexChanged: {
                //         modelText = screensSizeModel.get(currentIndex).key
                //         valchanged("screen_size",modelText)
                //     }
                // }

                // StyledLabel {
                //     id: label_pic
                //     anchors.left: idScreen.right
                //     anchors.top: idScreen.top
                //     anchors.leftMargin: 140
                //     //anchors.topMargin: 10
                //     height: 30
                //     width : 130
                //     text: qsTr("Preview Type:")
                //     visible: idHas_preview_img.checked
                //     verticalAlignment: Qt.AlignVCenter
                //     horizontalAlignment: Qt.AlignRight
                // }
                // BasicCombobox {
                // 	id: idpic
                //     anchors.left: label_pic.right
                //     anchors.leftMargin: 10
                //     anchors.top: label_pic.top
                //     anchors.topMargin: 0
                // 	height : 30
                // 	width : 100
                //     font.pointSize:Constants.labelLargeFontPointSize
                //     visible: idHas_preview_img.checked
                //     enabled: idHas_preview_img.checked
                //     property var modelText: ""
                //     currentIndex : 0
                // 	model: ListModel {
                // 		id: picTypeModel
                //         ListElement {key:"bmp"; modelData: "bmp";}
                // 		ListElement {key:"png"; modelData: "png";}
                // 		ListElement {key:"jpg"; modelData: "jpg";}
                // 	}

                //     onModelTextChanged: {
                //         if(modelText == "bmp")
                //         {
                //             currentIndex = 0
                //         }
                //         else if(modelText == "png")
                //         {
                //             currentIndex = 1
                //         }
                //         else
                //         {
                //             currentIndex = 2
                //         }
                //     }
                //     onCurrentIndexChanged: {
                //         modelText = picTypeModel.get(currentIndex).key
                //         valchanged("preview_img_type",modelText)
                //     }
                // }


                // StyledLabel {
                //     id: label10
                //     anchors.left: label_Screen.left
                //     anchors.top: label_Screen.bottom
                //     anchors.leftMargin: 0
                //     anchors.topMargin: 5
                //     height: 30
                //     width : 160
                //     text: qsTr("Extruder Type:")
                //     verticalAlignment: Qt.AlignVCenter
                //     horizontalAlignment: Qt.AlignRight
                // }

                // BasicCombobox {
                //     id: idExtruderType
                //     height: 30
                //     width : 130
                //     font.pointSize:Constants.labelLargeFontPointSize
                //     currentIndex: 0
                //     property var modelText: ""
                //     anchors.left: label10.right
                //     anchors.leftMargin: 10
                //     anchors.top: label10.top
                //     anchors.topMargin: 0
                //     displayText: model.get(currentIndex).modelData
                //     model: ListModel {
                //         id: modelType
                //         ListElement {key:"Distal"; modelData: qsTr("Distal");}
                //         ListElement {key:"Proximal"; modelData: qsTr("Proximal");}
                //     }
                //     onModelTextChanged: {
                //         if(modelText == "Distal")
                //         {
                //             currentIndex = 0
                //         }
                //         else
                //         {
                //             currentIndex = 1
                //         }
                //     }
                //     onCurrentIndexChanged: {
                //         modelText = modelType.get(currentIndex).key
                //         valchanged("machine_extruder_type",modelText)
                //     }

                // }

                // StyledLabel {
                //     id: label9
                //     anchors.left: idExtruderType.right
                //     anchors.top: label10.top
                //     anchors.leftMargin: 40
                //     anchors.topMargin: 0
                //     height: 30
                //     width : 130
                //     text: qsTr("Has Heated Build Plate:")
                //     verticalAlignment: Qt.AlignVCenter
                //     horizontalAlignment: Qt.AlignRight
                // }

                // StyleCheckBox {
                //     id: idHotBed
                //     anchors.left: label9.right
                //     anchors.leftMargin: 10
                //     anchors.top: label9.top
                //     //anchors.topMargin: 10
                //     height: 30
                //     width : 60
                //     onCheckedChanged:
                //     {
                //         valchanged("machine_heated_bed",idHotBed.checked.toString())
                //     }
                //     //  checked: machine_heated_bed ===0 ? false : true
                // }

                //start
                // StyledLabel {
                // 	id: labelFlavor
                // 	anchors.left: label10.left
                //                 anchors.leftMargin: 0
                //                 anchors.top: label10.bottom
                //                 anchors.topMargin: 5
                // 	height: 30
                // 	width : 160
                // 	text: qsTr("G-code Flavor:")
                // 	verticalAlignment: Qt.AlignVCenter
                // 	horizontalAlignment: Qt.AlignRight
                // }
                // BasicCombobox {
                // 	id: idFlavor
                // 	anchors.left: labelFlavor.right
                //                 anchors.leftMargin: 10
                //                 anchors.top: labelFlavor.top
                //                 anchors.topMargin: 0
                // 	height: 30
                // 	width : 130
                // 	font.pointSize:Constants.labelLargeFontPointSize
                // 	currentIndex : 0
                // 	property var modelText: ""

                //     displayText: model.get(currentIndex).modelData
                //     model: ListModel {
                // 		id: modelFlavor
                // 		ListElement {key:"RepRap (Marlin/Sprinter)"; modelData: qsTr("Marlin")}
                // 		ListElement {key:"MACH3(Creality)"; modelData: qsTr("MACH3(Creality)")}
                // 	}
                //     onModelTextChanged: {
                //         if(modelText == "RepRap (Marlin/Sprinter)")
                //         {
                //             currentIndex = 0
                //         }
                //         else
                //         {
                //             currentIndex = 1
                //         }
                //     }
                //     onCurrentIndexChanged: {
                //         modelText = modelFlavor.get(currentIndex).key
                //         valchanged("machine_gcode_flavor",modelText)
                //     }
                // }


                // //////////////////////////
                // StyledLabel {
                // 	id: labelCenterOrigin
                // 	anchors.left: label9.left
                //     anchors.leftMargin: 0
                //     anchors.top: labelFlavor.top
                //     //anchors.topMargin: 5
                // 	height: 30
                // 	width : 130
                // 	text: qsTr("Is Center Origin:")
                // 	verticalAlignment: Qt.AlignVCenter
                // 	horizontalAlignment: Qt.AlignRight
                // }

                // StyleCheckBox {
                // 	id: idCenterOrigin
                // 	anchors.left: labelCenterOrigin.right
                //     anchors.leftMargin: 10
                //     anchors.top: labelCenterOrigin.top
                //     //anchors.topMargin: 5
                // 	height: 30
                // 	width : 60
                // 	onCheckedChanged:
                // 	{
                // 		valchanged("machine_center_is_zero",idCenterOrigin.checked.toString())
                // 	}
                // }
                //end

                // StyleCheckBox {
                //     id: idIsDistal
                //     anchors.left: label10.right
                //     anchors.leftMargin: 10
                //     anchors.top: label10.top
                //     anchors.topMargin: 0
                //     height: 30
                //     width : 60
                //     onCheckedChanged:
                //     {
                //         valchanged("machine_extruder_type",idIsDistal.checked.toString())
                //     }
                // }

            }

        }
        Item {
            implicitWidth: parent.width
            implicitHeight: parent.height
            Grid
            {
                y:10*screenScaleFactor
                x:20*screenScaleFactor
                columns: 2
                rows: 2
                spacing: 10
                StyledLabel {
                    text: qsTr("Start Code Describe")
                    width: 120*screenScaleFactor
                    height: 30*screenScaleFactor
                }

                BasicScrollView {
                    id: idMachineGcode_flick

                    width: 410*screenScaleFactor
                    height : 130*screenScaleFactor
                    BasicDialogTextArea
                    {
                        id: idMachineGcode_Start
                        width: 410*screenScaleFactor
                        height : 130*screenScaleFactor
                        focus: true
                        wrapMode: TextEdit.Wrap
                        onEditingFinished:
                        {
                            console.log("machine_start_gcode: "+ text)
                            valchanged("machine_start_gcode",text)
                        }
                    }
                }
                StyledLabel {
                    text: qsTr("End Code Describe")
                    width: 120*screenScaleFactor
                    height: 30*screenScaleFactor
                }

                BasicScrollView {
                    id: idMachineGcode_flick1

                    width: 410*screenScaleFactor
                    height : 130*screenScaleFactor
                    BasicDialogTextArea
                    {
                        id:idMachineGcode_End
                        width: 410*screenScaleFactor
                        height : 130*screenScaleFactor
                        wrapMode: TextEdit.Wrap
                        onEditingFinished:
                        {
                            console.log("machine_end_gcode: "+ text)
                            valchanged("machine_end_gcode",text)
                        }
                    }
                }
            }

        }
        Item {
            implicitWidth: parent.width
            implicitHeight: parent.height
            Grid
            {
                y:10*screenScaleFactor
                x:20*screenScaleFactor
                columns: 2
                rows: 2
                spacing: 10
                StyledLabel {
                    text: qsTr("Start Code Describe")
                    width: 120*screenScaleFactor
                    height: 30*screenScaleFactor
                }

                BasicScrollView {
                    id: idExtrusion0_flick

                    width: 410*screenScaleFactor
                    height : 130*screenScaleFactor
                    BasicDialogTextArea
                    {
                        id: idExtrusion0_Start
                        //anchors.fill: parent
                        // width: 350
                        // height : 120
                        //text: "\nT0 ;switch to extruder 0\nG92 E0 ;reset extruder distance\nG1 F2000 E93 ;load filament\nG92 E0 ;reset extruder distance\nM104 S200"
                        focus: true
                        wrapMode: TextEdit.Wrap
                        onEditingFinished:
                        {
                            valChanged_Extruder("machine_extruder_start_code",text,0)
                        }
                    }
                }
                StyledLabel {
                    text: qsTr("End Code Describe")
                    width: 120*screenScaleFactor
                    height: 30*screenScaleFactor
                }

                BasicScrollView {
                    id: idExtrusion0_flick1

                    width: 410*screenScaleFactor
                    height : 130*screenScaleFactor
                    BasicDialogTextArea
                    {
                        id:idExtrusion0_End
                        width: 410*screenScaleFactor
                        height : 130*screenScaleFactor
                        // text: "\nG92 E0 ;reset extruder distance\nG1 F800 E-5 ;short retract\nG1 F2400 X295 Y265 ;move near prime tower\nG1 F2000 E-93 ;long retract for filament removal\nG92 E0 ;reset extruder distance\nG90"
                        wrapMode: TextEdit.Wrap
                        onEditingFinished:
                        {
                            valChanged_Extruder("machine_extruder_end_code",text,0)
                        }
                    }
                }
            }

        }
        Item {
            implicitWidth: parent.width
            implicitHeight: parent.height
            Grid
            {
                y:10*screenScaleFactor
                x:20*screenScaleFactor
                columns: 2
                rows: 2
                spacing: 10
                StyledLabel {
                    text: qsTr("Start Code Describe")
                    width: 120*screenScaleFactor
                    height: 30*screenScaleFactor
                }
                BasicScrollView {
                    id: idExtrusion1_flick

                    width: 410*screenScaleFactor
                    height : 130*screenScaleFactor
                    BasicDialogTextArea
                    {
                        id: idExtrusion1_Start
                        width: 410*screenScaleFactor
                        height : 130*screenScaleFactor
                        //text: "\nT1 ;switch to extruder 1\nG92 E0 ;reset extruder distance\nG1 F2000 E93 ;load filament\nG92 E0 ;reset extruder distance\nM104 S200"
                        onEditingFinished:
                        {
                            valChanged_Extruder("machine_extruder_start_code",text,1)
                        }
                    }
                }

                StyledLabel {
                    text: qsTr("End Code Describe")
                    width: 120*screenScaleFactor
                    height: 30*screenScaleFactor
                }
                BasicScrollView {
                    id: idExtrusion1_flick0

                    width: 410*screenScaleFactor
                    height : 130*screenScaleFactor
                    BasicDialogTextArea
                    {
                        id:idExtrusion1_End
                        width: 410*screenScaleFactor
                        height : 130*screenScaleFactor
                        text: "\nG92 E1 ;reset extruder distance\nG1 F800 E-5 ;short retract\nG1 F2400 X295 Y265 ;move near prime tower\nG1 F2000 E-93 ;long retract for filament removal\nG92 E0 ;reset extruder distance\nG90"
                        onEditingFinished:
                        {
                            valChanged_Extruder("machine_extruder_end_code",text,1)
                        }
                    }
                }
            }

        }
    }
}


