import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
//import CrealityUI 1.0
//import "qrc:/CrealityUI"
import ".."
import "../qml"

BasicDialog {
    id: idDialog
    width: 500
    height: 450
    titleHeight : 30
    property int spacing: 5
    title: qsTr("Manage Materials")
//    color : "white"//"#061F3B"

    property var modelcontrol

    property alias material: idmaterialAll.model
    property alias currentindex:idmaterialAll.currentIndex
    property bool bmachinetype: false
    property var unitstring: "¥/m"
    property double costValue: 0

    //property val name: value
    function init(obj)
    {
        modelcontrol=obj
        console.log("init func")
    }
    function machinetype(type)
    {
        bmachinetype=type
        if(type)
        {
            fdmtype.visible=true
            dlptype.visible=false
        }
        else
        {
            fdmtype.visible=false
            dlptype.visible=true
        }
    }

    function materials(names)
    {
        material=names
    }
    function initValue(index)
    {
        if(idmaterialAll.currentIndex !== index)
            idmaterialAll.currentIndex=index

        if(bmachinetype)
        {
            iddiameter.text=modelcontrol.getValue("material_diameter")
            idtype.text=modelcontrol.getValue("material_type")
            idtemperature.text=modelcontrol.getValue("material_print_temperature")
            idbrand.text=modelcontrol.getValue("material_brand")
            costValue=modelcontrol.getValue("material_cost")
            if(modelcontrol.getMoneyType() == "$")
            {
                costValue = costValue/ 7.0
                unitstring = "$/m"
            }
        }
        else
        {
            idtypedlp.text=modelcontrol.getValue("material_type")
            idsensitivity.text=modelcontrol.getValue("sensitivity")
        }
    }

    function writeValue()
    {
        if(bmachinetype)
        {
            iddiameter.text=modelcontrol.getValue("material_diameter")
            idtype.text=modelcontrol.getValue("material_type")
            idtemperature.text=modelcontrol.getValue("material_print_temperature")
            idbrand.text=modelcontrol.getValue("material_brand")
            costValue=modelcontrol.getValue("material_cost")
            if(modelcontrol.getMoneyType() == "$")
            {
                costValue = costValue/ 7.0
                unitstring = "$/m"
            }
        }
        else
        {
            idtypedlp.text=modelcontrol.getValue("material_type")
            idsensitivity.text=modelcontrol.getValue("sensitivity")
        }
    }

    onVisibleChanged:
    {
        var moneyType = modelcontrol.getMoneyType()
        if(moneyType  == "$")
        {
            unitstring = "$/m"
        }
        else
        {
            unitstring = "¥/m"
        }
    }

    Grid
    {
//        x:20
        y:titleHeight + 20
       rows: 5
       height: parent.height - 30
       width :450
       spacing: 10
       x : (idDialog.width - width ) / 2
       Item
       {
           width: 400
           height:30
           Row
           {
               x:20
               spacing: 10
               StyledLabel {
                   height : 28
                   width: 135

                   text: qsTr("Material Select:") + " "
                   horizontalAlignment: Qt.AlignRight
                   verticalAlignment: Qt.AlignVCenter
               }
               BasicCombobox {
                   id:idmaterialAll
       //            y:titleHeight + 18
       //            x:180
                   //currentIndex: 1
                   height:28
                   width: 250
                   onCurrentIndexChanged:
                   {
                       modelcontrol.materialChanged(idmaterialAll.textAt(currentIndex))
                       writeValue()
                   }
               }
           }

       }
       BasicSeparator
       {
           width: parent.width
           height : 2
       }

       BasicGroupBox
       {
           id: idGroup
           x:20
//           y:titleHeight + 70
           title: qsTr("General parameters")
//           font.pixelSize : 16
           width: 400
           height:280

           //fdm
           Grid
           {
               id:fdmtype
               visible: false
//               x:20
               y:10
               rowSpacing: 8//20
               columnSpacing: 10//60
               rows: 5
               columns: 2
               StyledLabel {
                   width: 140
                   height: 28
                   Layout.row: 0
                   Layout.column: 1
                   text: qsTr("Material Type:") + " "
                   horizontalAlignment: Qt.AlignRight
                   verticalAlignment: Qt.AlignVCenter

               }
               BasicTextField {
                   id: idtype
                   Layout.row: 0
                   Layout.column: 2
                   implicitHeight:28
                   implicitWidth: 250
                   verticalAlignment: Qt.AlignVCenter

                   onEditingFinished:{
                       modelcontrol.valueChanged("material_type",idtype.text)
                       console.log("material_type = "+idtype.text);
                   }
               }

                StyledLabel {
                   width: 140
                   height: 28
                   Layout.row: 1
                   Layout.column: 1
                   text: qsTr("Brand:") + " "
                   horizontalAlignment: Qt.AlignRight
                   verticalAlignment: Qt.AlignVCenter
               }
               BasicTextField {
                   id: idbrand
                   Layout.row: 1
                   Layout.column: 2
                   implicitHeight:28
                   implicitWidth: 250
//                   width: 200
//                   height: 30
                   verticalAlignment: Qt.AlignVCenter
                   onEditingFinished:{
                      modelcontrol.valueChanged("material_brand",idbrand.text)
                   }
               }
               StyledLabel {
                   Layout.row: 2
                   Layout.column: 1
                   width: 140
                   height: 28
                   text: qsTr("Diameter:") + " "
                   horizontalAlignment: Qt.AlignRight
                   verticalAlignment: Qt.AlignVCenter

               }
               BasicDialogTextField {
                   id: iddiameter
                   Layout.row: 2
                   Layout.column: 2
                   implicitHeight:28
                   implicitWidth: 250
                   verticalAlignment: Qt.AlignVCenter
                   onEditingFinished:{
                      modelcontrol.valueChanged("material_diameter",iddiameter.text)
                   }
               }
               
               StyledLabel {
                   width: 140
                   height: 28
                   Layout.row: 3
                   Layout.column: 1
                   text: qsTr("Printing Temperature:") + " "
                   horizontalAlignment: Qt.AlignRight
                   verticalAlignment: Qt.AlignVCenter

               }
               BasicDialogTextField {
                   id: idtemperature
                   Layout.row: 3
                   Layout.column: 2
                   implicitHeight:28
                   implicitWidth: 250
//                   width: 200
//                   height: 30

                   verticalAlignment: Qt.AlignVCenter
                   onEditingFinished:{
                      modelcontrol.valueChanged("material_print_temperature",idtemperature.text)
                   }
               }
               
               StyledLabel {
                   width: 140
                   height: 28
                   Layout.row: 4
                   Layout.column: 1
                   text: qsTr("Filament Cost:") + " "
                   horizontalAlignment: Qt.AlignRight
                   verticalAlignment: Qt.AlignVCenter

               }
               BasicDialogTextField {
                   id: idcost
                   Layout.row: 4
                   Layout.column: 2
                   text: Number(costValue).toFixed(2)
                   implicitHeight:28
                   implicitWidth: 250
                   unitChar: unitstring
                   verticalAlignment: Qt.AlignVCenter
                   onEditingFinished:{
                        var val = idcost.text
                        if(modelcontrol.getMoneyType() == "$")
                        {
                            val = idcost.text * 7.0
                        }
                        modelcontrol.valueChanged("material_cost",val)
                   }
               }
           }

           //dlp
           GridLayout
           {
               id:dlptype
               visible: false
//               x:20
               y:10
               rowSpacing: 20
               columnSpacing: 60
               StyledLabel {
                   Layout.column: 1
                   width: 140
                   text: qsTr("Material Type:") + " "
                   horizontalAlignment: Qt.AlignRight
                   verticalAlignment: Qt.AlignVCenter

               }
               BasicDialogTextField {
                   id: idtypedlp
                   Layout.column: 2
                   implicitHeight:28
                   implicitWidth: 250
//                   width: 200
//                   height: 30
                   verticalAlignment: Qt.AlignVCenter
                   onEditingFinished:{
                      modelcontrol.valueChanged("material_type",idtypedlp.text)
                      console.log("material_type = "+idtypedlp.text);
                   }
               }

               StyledLabel {
                   Layout.row: 1
                   Layout.column: 1
                   text: qsTr("Sensitivity:") + " "
                   horizontalAlignment: Qt.AlignRight
                   verticalAlignment: Qt.AlignVCenter

               }
               BasicDialogTextField {
                   id: idsensitivity
                   Layout.row: 1
                   Layout.column: 2
                   implicitHeight:28
                   implicitWidth: 250

                   verticalAlignment: Qt.AlignVCenter
                   onEditingFinished:{
                       modelcontrol.valueChanged("sensitivity",idsensitivity.text)
                   }
               }
           }
       }
       BasicSeparator
       {
           width: parent.width
           height : 2
       }

        Item {//
            width: parent.width
            height:  50
            Grid
            {
                width: 350//parent.width
                height : 50
                columns: 4
                spacing: 10
                anchors.centerIn:parent//

                BasicDialogButton
                {
                    id : idSave
                    width:80
                    height: 30
                    text: qsTr("New")
                    btnRadius:3
                    btnBorderW:0
                    defaultBtnBgColor: Constants.profileBtnColor
                    hoveredBtnBgColor: Constants.profileBtnHoverColor
                    onSigButtonClicked:
                    {
                        addeditmaterial.visible=true
                    }
                }

                BasicDialogButton
                {
                    id : idsave
                    width:80
                    height: 30
                    text: qsTr("Save")
                    btnRadius:3
                    btnBorderW:0
                    defaultBtnBgColor: Constants.profileBtnColor
                    hoveredBtnBgColor: Constants.profileBtnHoverColor
                    onSigButtonClicked:
                    {
                        if(fdmtype.visible === true && iddiameter.text === "" && idtype.text === "" && idtemperature.text === ""
                                && idbrand.text === "" && idcost.text === "")
                        {
                            messageDialog.show();
							return
                        }
                        // if(dlptype.visible === true && idtypedlp.text === "" && idsensitivity.text === "")
                        // {
                        //     messageDialog.show();
						// 	return
                        // }

                        var val = idcost.text
                        if(modelcontrol.getMoneyType() == "$")
                        {
                            val = idcost.text * 7.0
                        }
                        modelcontrol.valueChanged("material_cost",val)
                        idDialog.close()
                    }
                }

                BasicDialogButton
                {
                    id : idreset
                    width:80
                    height: 30
                    text: qsTr("Reset")
                    btnRadius:3
                    btnBorderW:0
                    defaultBtnBgColor: Constants.profileBtnColor
                    hoveredBtnBgColor: Constants.profileBtnHoverColor
                    onSigButtonClicked:
                    {
                        modelcontrol.resetMaterial()
                        writeValue()
                    }
                }
                
                BasicDialogButton//BasicComButton
                {
                    id : iddelete
                    width:80
                    height: 30
                    text: qsTr("Delete")
                    btnRadius:3
                    btnBorderW:0
                    defaultBtnBgColor: Constants.profileBtnColor
                    hoveredBtnBgColor: Constants.profileBtnHoverColor
                    onSigButtonClicked:
                    {
                        modelcontrol.deleteMaterial()
                    }
                }
                //    BasicDialogButton//BasicComButton//by TCJ
                //    {
                //        id : idcancel
                //        width:80
                //        height: 30
                //        text: qsTr("Cancel")
                //        onSigButtonClicked:
                //        {
                //            idDialog.close();
                //        }
                //    }
                

            }
            AddEditMaterial{
                    id:addeditmaterial
                    model_material:material
                    //objectName: "addeditprinterobj"
                    visible:false
                    onMaterialadd:
                    {
                        modelcontrol.addMaterial(newmaterialtext,currentText_printer)
                    }
                }
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
                id: warringdlgname
                anchors.centerIn: parent
                font.family: Constants.labelFontFamily
                font.weight: Constants.labelFontWeight
                text: qsTr("All parameters cannot be empty at the same time!!")
                font.pixelSize: Constants.labelFontPixelSize
                color:Constants.textColor
            }
        }
    }

}
