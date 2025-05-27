import QtQuick 2.0
import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
//import CrealityUI 1.0
//import "qrc:/CrealityUI"
import "../qml"
BasicDialog
{
    id: control

    property alias cmbModel: idCmb.model
    property alias currentCmbIndex :idCmb.currentIndex
    property alias currentCmbText :idCmb.currentText

    property var managerDlpvisable: false
    property alias managerFdmvisable: idFdmGroupBox.managerFdmvisable

    signal delMachine(var machineName);
    signal valuechanged(string key,string value)
    signal resetConfig(string machineName)
    signal saveConfig()
    signal addNewPrinterConfig(string str)
    signal machineChange(string name)
    //save extruder
    signal saveExtruder(int nextruder)

    width: 650*screenScaleFactor
    height: 515*screenScaleFactor
    titleHeight : 20*screenScaleFactor
    title : qsTr("Manage Printer")//by TCJ
    objectName: "printerManagerDlg"

    onVisibleChanged: {
        if(visible)
            paramSetUI.onManagerMachine()
    }

    Grid {
        id: rectangle
        x:30*screenScaleFactor
        y :20*screenScaleFactor + titleHeight
        width: parent.width
        height: parent.height-titleHeight-40*screenScaleFactor
        rows: 5
        spacing: 10
        Grid
        {
            id: idTypeGrid
            columns: 2
            y: 20*screenScaleFactor
            spacing: 20
            width : 500*screenScaleFactor
            height : 30*screenScaleFactor
            StyledLabel {
                id: element
                text: qsTr("Printer Type:")
                font.pointSize: 13
                width: 120*screenScaleFactor
                height: 30*screenScaleFactor
                verticalAlignment: Qt.AlignVCenter
                horizontalAlignment: Qt.AlignRight
            }
            BasicCombobox
            {
                id:idCmb
                width: 300*screenScaleFactor
                height: 30*screenScaleFactor
                objectName: "machineManagerCommbox"
//                currentIndex : paraSettingUI.getCurPrinterIndex()

                onCurrentTextChanged:
                {
                    console.log("onCurrentTextChanged =" +currentText)
                    //                    machineChange(currentText)
                    paraSettingUI.onManagerMachineChanged(currentText)
                }
            }
        }

        Item {
            width: control.width - 12*screenScaleFactor
            height : 2*screenScaleFactor
            
            Rectangle {
                // anchors.left: idCol.left
                // anchors.leftMargin: -10
                x: -24*screenScaleFactor
                width:parent.width > parent.height ?  parent.width : 2*screenScaleFactor
                height: parent.width > parent.height ?  2*screenScaleFactor : parent.height
                color: "transparent"//Constants.splitLineColor
                Rectangle {
                    width: parent.width > parent.height ? parent.width : 1*screenScaleFactor
                    height: parent.width > parent.height ? 1 : parent.height
                    color: Constants.splitLineColorDark
                }
            }
        }
        
        Item {
            id: name
            x: 10*screenScaleFactor
            width: 615*screenScaleFactor
            height: 345*screenScaleFactor
            ManagerFdmGroupShow
            {
                id: idFdmGroupBox
                anchors.fill: parent
                objectName: "fdmMachineParemeter"
                //  machine_name : idCmb.currentText
                onValchanged:
                {
                    valuechanged(key,value)
                }
            }
        }

        Item {
            width: control.width - 12*screenScaleFactor
            height : 2*screenScaleFactor
            
            Rectangle {
                // anchors.left: idCol.left
                // anchors.leftMargin: -10
                x: -24*screenScaleFactor
                width:parent.width > parent.height ?  parent.width : 2*screenScaleFactor
                height: parent.width > parent.height ?  2 : parent.height
                color: Constants.splitLineColor
                Rectangle {
                    width: parent.width > parent.height ? parent.width : 1*screenScaleFactor
                    height: parent.width > parent.height ? 1 : parent.height
                    color: Constants.splitLineColorDark
                }
            }
        }

        Item {
            x:(parent.width - width) / 2
            width: contentItem.width
            height : 30*screenScaleFactor
            Grid
            {
                x: 80*screenScaleFactor
                columns: 5
                spacing: 10
                width: contentItem.width
                height : 30*screenScaleFactor
                // BasicDialogButton //by TCJ
                // {
                //     id: idNewCopy
                //     text:qsTr("New")
                //     width: 100
                //     height: 30
                //     onSigButtonClicked:
                //     {
                //         idAddNewPrinter.visible=true
                //     }
                // }
                BasicDialogButton
                {
                    id: idNewCopy3
                    text:qsTr("Save")
                    width: 100*screenScaleFactor
                    height: 30*screenScaleFactor
                    btnRadius:3
                    btnBorderW:0
                    defaultBtnBgColor: Constants.profileBtnColor
                    hoveredBtnBgColor: Constants.profileBtnHoverColor
                    onSigButtonClicked:
                    {
                        control.close();
                        saveConfig();
                        paraSettingUI.onSaveManageConfig(idCmb.currentText)
                        if(idFdmGroupBox.visible)
                        {
                            saveExtruder(idFdmGroupBox.machine_extruder_count)
                        }

                    }
                }
                BasicDialogButton
                {
                    id: idReset
                    text:qsTr("Reset")
                    width: 100*screenScaleFactor
                    height: 30*screenScaleFactor
                    btnRadius:3
                    btnBorderW:0
                    defaultBtnBgColor: Constants.profileBtnColor
                    hoveredBtnBgColor: Constants.profileBtnHoverColor
                    onSigButtonClicked:
                    {
                        resetConfig(idCmb.currentText)
                    }
                }

                BasicDialogButton
                {
                    id: idNewCopy4
                    text:qsTr("Delete")
                    width: 100*screenScaleFactor
                    height: 30*screenScaleFactor
                    btnRadius:3
                    btnBorderW:0
                    defaultBtnBgColor: Constants.profileBtnColor
                    hoveredBtnBgColor: Constants.profileBtnHoverColor
                    onSigButtonClicked:
                    {
                        control.close();
                        delMachine(idCmb.currentText)
                    }
                }

                BasicDialogButton
                {
                    id: idNewCopy5
                    text:qsTr("Cancel")
                    width: 100*screenScaleFactor
                    height: 30*screenScaleFactor
                    btnRadius:3
                    btnBorderW:0
                    defaultBtnBgColor: Constants.profileBtnColor
                    hoveredBtnBgColor: Constants.profileBtnHoverColor
                    onSigButtonClicked:
                    {
                        control.close();
                        resetConfig()
                    }
                }
            }

        }

    }

    AddEditPrinter
    {
        id:idAddNewPrinter
        model_printer:cmbModel
        currentIndex_printer:idCmb.currentIndex
        objectName: "addeditprinterobj"
        visible:false
        onPrinteradd:
        {
            addNewPrinterConfig(newprintertext)
        }
    }
}
