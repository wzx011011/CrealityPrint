import QtQuick 2.10
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0 as QQC2
import CrealityUI 1.0
import "../qml"
BasicDialogV4
{
    signal addPrinterMachine(var name)
    signal updatePrinterMachine()

    id: idAddPrinterDlg
    width: 1200*screenScaleFactor
    height: 780*screenScaleFactor
    titleHeight : 30*screenScaleFactor
    title: qsTr("Add Printer")

    function updateMachineStatus(status)
    {
        if(status == "error")
        {
            idUpdateMachineDlg.msgText = qsTr("Update Failed!")
            idUpdateMachineDlg.visible = true
        }
        else if(status == "success")
        {
            idUpdateMachineDlg.msgText = qsTr("Printer information has been updated, please restart the software!")
            idUpdateMachineDlg.visible = true
        }
    }

    Timer{
        id: idUpdateMachineBtnTimer
        interval: 20000
        repeat: false
        onTriggered:{
            _idUpdateMachineInfoBtn.enabled = true
        }
    }

    bdContentItem:Rectangle{
        color: Constants.themeColor
        ColumnLayout{
            anchors.fill: parent
            anchors.margins: 20*screenScaleFactor
            anchors.topMargin: 35*screenScaleFactor
            anchors.bottomMargin: 30*screenScaleFactor
            spacing: 25
            RowLayout{
                Layout.fillWidth: true
                Layout.leftMargin: 20*screenScaleFactor
                Layout.rightMargin: 20*screenScaleFactor
                Layout.preferredWidth: 40*screenScaleFactor
                BasicButton{
                    id: _idUpdateMachineInfoBtn
                    Layout.preferredWidth: 120*screenScaleFactor
                    Layout.preferredHeight: 28*screenScaleFactor
                    pointSize: Constants.labelFontPointSize
                    btnRadius: 14
                    btnBorderW: 0
                    defaultBtnBgColor: Constants.typeModelBtnListHoveredColor
                    hoveredBtnBgColor: "#1E9BE2"
                    text: qsTr("Printer Update")
                    visible:false
                    onSigButtonClicked:
                    {
                        idAddPrinterDlg.updatePrinterMachine()
                    }
                }

                Item{
                    Layout.fillWidth: true
                }

                BasicLoginTextEdit
                {
//                    anchors{
//                        verticalCenter: parent.verticalCenter
//                    }
                    id: idSearch
                    visible: false
                    placeholderText: qsTr("Search")
                    Layout.preferredWidth: 300*screenScaleFactor
                    Layout.preferredHeight: 28*screenScaleFactor
                    font.pointSize:Constants.labelFontPointSize
                    headImageSrc:hovered ? Constants.sourchBtnImg_d : Constants.sourchBtnImg
                    tailImageSrc: hovered && idSearch.text != "" ? Constants.clearBtnImg : ""
                    hoveredTailImageSrc: Constants.clearBtnImg_d
                    radius : 14
                    text: ""
                    onEditingFinished:
                    {

                    }
                    onTailBtnClicked:
                    {

                    }
                }
                BasicButton {
                    id: idSearchButton
                    visible: false
//                    anchors{
//                        verticalCenter: parent.verticalCenter
//                    }
                    Layout.preferredWidth: 70*screenScaleFactor
                    Layout.preferredHeight: 30*screenScaleFactor
                    btnRadius:13
                    btnBorderW:enabled ? 0 : 1
                    pointSize: Constants.labelLargeFontPointSize
                    enabled: idSearch.text != "" ? true : false
                    defaultBtnBgColor: enabled ?  Constants.searchBtnHoveredColor : Constants.searchBtnDisableColor
                    hoveredBtnBgColor: Constants.typeBtnHoveredColor
                    text: qsTr("Search")
                    onSigButtonClicked:
                    {

                    }
                }
            }
            RowLayout{
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.leftMargin: 20*screenScaleFactor

                spacing: 20
                ColumnLayout{
                    Layout.preferredWidth: 140*screenScaleFactor
                    Layout.fillHeight: true
                    spacing: 1
                    BasicButton{
                        Layout.preferredWidth: 140*screenScaleFactor
                        Layout.preferredHeight: 36*screenScaleFactor
                        pointSize: Constants.labelLargeFontPointSize
                        btnRadius: 3
                        btnBorderW: 0
                        defaultBtnBgColor: Constants.dialogItemRectBgColor
                        hoveredBtnBgColor: Constants.dialogItemRectBgColor
                        text:qsTr("Printer")
                    }
                    BasicButton{
                        id: idEnderBtn
                        x: 10*screenScaleFactor
                        Layout.preferredWidth: 120*screenScaleFactor
                        Layout.preferredHeight: 26*screenScaleFactor
                        pointSize: Constants.labelFontPointSize
                        btnRadius: 3
                        btnBorderW: 0
                        btnSelected: true
                        defaultBtnBgColor: Constants.dialogContentBgColor
                        hoveredBtnBgColor: Constants.typeModelBtnListHoveredColor
                        selectedBtnBgColor: "#1E9BE2"
                        text:qsTr("ENDER series")
                        onSigButtonClicked:
                        {
                            idEnderBtn.btnSelected = true
                            idCrBtn.btnSelected = false
                            idSermoonBtn.btnSelected = false
                            idOthersBtn.btnSelected = false
                            cwAddPrinterModel.changePrinterDataSource(1)
                        }
                    }
                    BasicButton{
                        id: idCrBtn
                        x: 10*screenScaleFactor
                        Layout.preferredWidth: 120*screenScaleFactor
                        Layout.preferredHeight: 26*screenScaleFactor
                        pointSize: Constants.labelFontPointSize
                        btnRadius: 3
                        btnBorderW: 0
                        defaultBtnBgColor: Constants.dialogContentBgColor
                        hoveredBtnBgColor: Constants.typeModelBtnListHoveredColor
                        selectedBtnBgColor: "#1E9BE2"
                        text:qsTr("CR series")
                        onSigButtonClicked:
                        {
                            idEnderBtn.btnSelected = false
                            idCrBtn.btnSelected = true
                            idSermoonBtn.btnSelected = false
                            idOthersBtn.btnSelected = false
                            cwAddPrinterModel.changePrinterDataSource(2)
                        }
                    }
                    BasicButton{
                        id: idSermoonBtn
                        x: 10*screenScaleFactor
                        Layout.preferredWidth: 120*screenScaleFactor
                        Layout.preferredHeight: 26*screenScaleFactor
                        pointSize:Constants.labelFontPointSize
                        btnRadius: 3
                        btnBorderW: 0
                        defaultBtnBgColor: Constants.dialogContentBgColor
                        hoveredBtnBgColor: Constants.typeModelBtnListHoveredColor
                        selectedBtnBgColor: "#1E9BE2"
                        text:qsTr("Sermoon series")
                        onSigButtonClicked:
                        {
                            idEnderBtn.btnSelected = false
                            idCrBtn.btnSelected = false
                            idSermoonBtn.btnSelected = true
                            idOthersBtn.btnSelected = false
                            cwAddPrinterModel.changePrinterDataSource(3)
                        }
                    }
					BasicButton{
						id: idPrintscreen
                        x: 10*screenScaleFactor
                        Layout.preferredWidth: 120*screenScaleFactor
                        Layout.preferredHeight: 26*screenScaleFactor
                        pixSize: 12
                        btnRadius: 3
                        btnBorderW: 0
                        defaultBtnBgColor: Constants.dialogContentBgColor
                        hoveredBtnBgColor: Constants.typeModelBtnListHoveredColor
                        selectedBtnBgColor: "#1E9BE2"
						text:qsTr("Sonic Pad")
						onSigButtonClicked:
						{
							idEnderBtn.btnSelected = false
							idCrBtn.btnSelected = false
							idSermoonBtn.btnSelected = false
							idOthersBtn.btnSelected = false
							idPrintscreen.btnSelected = true
							cwAddPrinterModel.changePrinterDataSource(5)
						}
					}
                    BasicButton{
                        id:idOthersBtn
                        x: 10*screenScaleFactor
                        Layout.preferredWidth: 120*screenScaleFactor
                        Layout.preferredHeight: 26*screenScaleFactor
                        pointSize:Constants.labelFontPointSize
                        btnRadius: 3
                        btnBorderW: 0
                        defaultBtnBgColor: Constants.dialogContentBgColor
                        hoveredBtnBgColor: Constants.typeModelBtnListHoveredColor
                        selectedBtnBgColor: "#1E9BE2"
                        text:qsTr("Others")
                        onSigButtonClicked:
                        {
                            idEnderBtn.btnSelected = false
                            idCrBtn.btnSelected = false
                            idSermoonBtn.btnSelected = false
                            idOthersBtn.btnSelected = true
                            cwAddPrinterModel.changePrinterDataSource(4)
                        }
                    }
                    Item{
                        Layout.fillHeight: true
                    }
                }

                Item{

                }

                BasicScrollView
                {
                    id: idModelScrollView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    visible: true
                    hpolicy: ScrollBar.AlwaysOff
                    vpolicy: ScrollBar.AsNeeded
                    clip : true
                    Item{
                        width: parent.width
                        implicitHeight: flow.height
                        Flow{
                            id:flow
                            spacing: 20
                            width: parent.width
                            //                            height: 2000

                            Repeater{
                                id: rept
                                model: cwAddPrinterModel
                                delegate: AddPrinterViewItem{
                                    imageUrl: cusPrinterImageUrl
                                    printerName: cusprinterName
                                    machineDepth: cusMachineDepth
                                    machineWidth: cusMachineWidth
                                    machineHeight: cusMachineheight
                                    nozzleSize: cusNozzleSize

                                    onAddMachine:
                                    {
                                        paraSettingUI.onMachineAdded(printerName)
                                        addPrinterMachine(machineName)
                                        //                                        idAddPrinterDlg.close()
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Connections{
        target: crealityCloudUI
        onSigUpdateMachineStatus:{
            if(msg == "error")
            {
                idUpdateMachineDlg.msgText = qsTr("Update Failed!")
                idUpdateMachineDlg.visible = true
            }
            else if(msg == "success")
            {
                idUpdateMachineDlg.msgText = qsTr("Printer information has been updated, please restart the software!")
                idUpdateMachineDlg.visible = true
            }
        }
    }

    UploadMessageDlg{
        id: idUpdateMachineDlg
        msgText: qsTr("Printer information has been updated, please restart the software!")
        cancelBtnVisible: false
        visible: false
        onSigOkButtonClicked:
        {
            idUpdateMachineDlg.close()
        }
    }
}
