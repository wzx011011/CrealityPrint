import QtQuick 2.13
//import "./BasicComponent"
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import "../qml"
import "../controlQml"

Rectangle {
    id : __rootPanel
    color: "transparent"
    property var curSelectPrintingIndex: -1
    property alias gCCodePathPanel: __lanPrinterPanel.gCCodePath

    function setRealEntryPanel(value) {
        __lanPrinterPanel.setRealEntry(value)
    }

    Component.onCompleted: __repeaterModel.clear()

    ListModel
    {
        id: __repeaterModel
        //ListElement{modelData: "model1";printerIdKey : "1"}
    }

    Component
    {
        id: __test
        LanPrintingDetail {

        }
    }

    ColumnLayout
    {
        spacing: 0
        width: parent.width
        height: parent.height

        Rectangle
        {
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 30 * screenScaleFactor
            color: Constants.lanPrinter_panel_background

            Row
            {
                spacing: 1
                Button {
                    id: lanPrinter_list_btn
                    width: 100 * screenScaleFactor
                    height: 30 * screenScaleFactor
                    property bool isSelected: curSelectPrintingIndex < 0

                    background: Rectangle {
                        radius: 5
                        border.width: 1
                        border.color: Constants.lanPrinter_panel_border
                        color: (lanPrinter_list_btn.isSelected || lanPrinter_list_btn.hovered)
                               ? Constants.lanPrinter_panel_btn_hovered : Constants.lanPrinter_panel_btn_default
                    }

                    contentItem: Text {
                        font.weight: Font.Medium
                        font.family: Constants.mySystemFont.name
                        font.pointSize: Constants.labelFontPointSize
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        text: qsTr("LAN Printing")
                        color: (lanPrinter_list_btn.isSelected || lanPrinter_list_btn.hovered)
                               ? Constants.lanPrinter_panel_weight_txt : Constants.lanPrinter_panel_light_txt
                    }

                    onClicked:
                    {
                        curSelectPrintingIndex = -1
                        __testLoad.sourceComponent = undefined
                    }
                }
                Repeater {
                    model: __repeaterModel
                    delegate: Button {
                        id: lanPrinter_detail_btn
                        height: 30 * screenScaleFactor
                        width: detailBtnRow.width + detailBtnRow.spacing * 2
                        property bool isSelected: (curSelectPrintingIndex === index)

                        background: Rectangle {
                            radius: 5
                            border.width: 1
                            border.color: Constants.lanPrinter_panel_border
                            color: (lanPrinter_detail_btn.isSelected || lanPrinter_detail_btn.hovered)
                                   ? Constants.lanPrinter_panel_btn_hovered : Constants.lanPrinter_panel_btn_default
                        }

                        Row {
                            spacing: 10
                            id: detailBtnRow
                            height: parent.height
                            anchors.centerIn: parent

                            Text {
                                height: parent.height
                                font.weight: Font.Medium
                                font.family: Constants.mySystemFont.name
                                font.pointSize: Constants.labelFontPointSize
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                text: modelData
                                color: (lanPrinter_detail_btn.isSelected || lanPrinter_detail_btn.hovered)
                                       ? Constants.lanPrinter_panel_weight_txt : Constants.lanPrinter_panel_light_txt
                            }

                            Image {
                                width: 8 * screenScaleFactor
                                height: 8 * screenScaleFactor
                                anchors.verticalCenter: parent.verticalCenter
                                source: detailBtnArea.containsMouse ? "qrc:/UI/photo/closeBtn_d.png" :"qrc:/UI/photo/closeBtn.png"

                                MouseArea {
                                    id: detailBtnArea
                                    anchors.fill: parent
                                    hoverEnabled: true

                                    onReleased:
                                    {
                                        curSelectPrintingIndex = -1
                                        __testLoad.sourceComponent = undefined
                                        __repeaterModel.remove(index)
                                    }
                                }
                            }
                        }

                        onClicked:
                        {
                            if(isSelected) return
                            curSelectPrintingIndex = index

                            if(__testLoad.sourceComponent === null)
                            {
                                __testLoad.itemPrinterId = printerIdKey
                                __testLoad.sourceComponent = __test
                            }
                            else {
                                __testLoad.itemPrinterId = printerIdKey
                                __testLoad.item.deviceID = printerIdKey

                                __testLoad.item.stopPlayVideo()
                                __testLoad.item.updateShowData()
                                __testLoad.item.startPlayVideo()
                            }
                        }
                    }
                }
            }
        }

        Rectangle
        {
            Layout.preferredHeight: 1
            Layout.preferredWidth: parent.width
            color: Constants.lanPrinter_panel_crossline
        }

        Rectangle
        {
            Layout.fillHeight: true
            Layout.preferredWidth: parent.width

            LanPrinterListLocal
            {
                id: __lanPrinterPanel
                anchors.fill: parent

                function isModelContains(printerId) {
                    var result = -1
                    for(var index = 0; index < __repeaterModel.count; ++index)
                    {
                        var macAddress = __repeaterModel.get(index).printerIdKey
                        if(macAddress === printerId) result = index
                    }
                    return result
                }

                onClickDetail:
                {
                    var result = isModelContains(printerId)
                    curSelectPrintingIndex = (result < 0) ? __repeaterModel.count : result
                    if(result < 0) __repeaterModel.append({"modelData": printerName, "printerIdKey": printerId})

                    __testLoad.itemPrinterId = printerId
                    __testLoad.sourceComponent = __test
                }
                Loader
                {
                    id: __testLoad
                    property var itemPrinterId

                    focus: true
                    active: true
                    anchors.fill: parent
                    visible: status == Loader.Ready

                    onLoaded: {
                        item.deviceID = itemPrinterId
                        item.updateShowData()
                        item.startPlayVideo()
                    }
                }
            }
        }
    }
}
