import QtQuick 2.10
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0 as QQC2
import CrealityUI 1.0
import QtGraphicalEffects 1.12
import "qrc:/CrealityUI"
import ".."
import "../qml"

Item{
    width: 307*screenScaleFactor
    height: 296*screenScaleFactor
    
    property var imageUrl: "qrc:/UI/photo/machineImage_Printer_Box.png"
    property var printerName: "Ender-3 V2"
    property var machineDepth: 220
    property var machineWidth: 220
    property var machineHeight: 220
    property var nozzleSize: 0.40
    property bool isHovered: false

    signal addMachine(var machineName)

    MouseArea{
        anchors.fill: parent
        hoverEnabled:true
        // onClicked:
        // {
        // }
        onEntered:
        {
            isHovered = true
        }
        onExited:
        {
            isHovered = false
        }
    }

    Rectangle{
        width: parent.width
        height:parent.height
        border.width: isHovered ? 3 : 0
        border.color: "#1E9BE2"
        radius: 9
        Column{
            y:isHovered ? 3 : 0
            width: parent.width
            height:parent.height
            Rectangle{
                x: isHovered ? 3 : 0
                y: isHovered ? 3 : 0
                width: isHovered ? parent.width - 6 : parent.width
                height: isHovered ? 211*screenScaleFactor : 214*screenScaleFactor
                color: "#E9E9E9"
                radius: 9
                Image{
                    id: _idprinterImage
                    anchors.left: parent.left
                    anchors.leftMargin: 16
                    anchors.top: parent.top
                    anchors.topMargin: 12
                    width: 275*screenScaleFactor
                    height: 190*screenScaleFactor
                    mipmap: true
                    smooth: true
                    cache: false
                    asynchronous: true
                    fillMode: Image.PreserveAspectFit
                    source: imageUrl
                }
                Rectangle{
                    anchors.top: _idprinterImage.bottom
                    width: parent.width
                    height: isHovered ? 9*screenScaleFactor : 12*screenScaleFactor
                    color: "#E9E9E9"
                }
            }
            Rectangle{
                x: isHovered ? 3 : 0
                width: isHovered ? parent.width - 6 : parent.width
                height: isHovered ? 79*screenScaleFactor : 82*screenScaleFactor
                radius: 9
                color: "#FFFFFF"
                border.width: 1
                border.color: Constants.modleItemBorderColor
                Rectangle{
                    anchors.top: parent.top
                    width: parent.width
                    height: 9
                    color: "#FFFFFF"
                    border.width: 1
                    border.color: Constants.modleItemBorderColor
                    Rectangle{
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.leftMargin: 1
                        width: parent.width-2
                        height: 9
                        color: "#FFFFFF"
                    }
                }
                Row{
                    anchors.top: parent.top
                    anchors.topMargin: 9
                    anchors.left: parent.left
                    anchors.leftMargin: 15
                    width: parent.width - 30
                    height: parent.height - 21
                    Column{
                        width: parent.width / 2 + 10
                        height: parent.height
                        Row{
                            width: parent.width
                            height: parent.height / 4
                            StyledLabel{
                                width: parent.width / 3 * 2
                                height: parent.height
                                color: "#333333"
                                text: qsTr("Machine Depth:") + " "
                            }
                            StyledLabel{
                                width: parent.width / 3 
                                height: parent.height
                                color: "#333333"
                                text: machineDepth + "mm"
                            }
                        }
                        Row{
                            width: parent.width
                            height: parent.height / 4
                            StyledLabel{
                                width: parent.width / 3 * 2
                                height: parent.height
                                color: "#333333"
                                text: qsTr("Machine Width:") + " "
                            }
                            StyledLabel{
                                width: parent.width / 3 
                                height: parent.height
                                color: "#333333"
                                text: machineWidth + "mm"
                            }
                        }
                        Row{
                            width: parent.width
                            height: parent.height / 4
                            StyledLabel{
                                width: parent.width / 3 * 2
                                height: parent.height
                                color: "#333333"
                                text: qsTr("Machine Height:") + " "
                            }
                            StyledLabel{
                                width: parent.width / 3 
                                height: parent.height
                                color: "#333333"
                                text: machineHeight + "mm"
                            }
                        }
                        Row{
                            width: parent.width
                            height: parent.height / 4
                            StyledLabel{
                                width: parent.width / 3 * 2
                                height: parent.height
                                color: "#333333"
                                text: qsTr("Nozzle Size:") + " "
                            }
                            StyledLabel{
                                width: parent.width / 3 
                                height: parent.height
                                color: "#333333"
                                text: nozzleSize.toFixed(2) + "mm"
                            }
                        }
                    }
                    Column{
                        width: parent.width / 2 - 10
                        height: parent.height
                        StyledLabel{
                            width: parent.width
                            height: parent.height / 2
                            font.pointSize:13
                            horizontalAlignment:Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            color: "#333333"
                            text: printerName
                        }
                        BasicButton{
                            anchors{
                                horizontalCenter: parent.horizontalCenter
                                //verticalCenter: parent.verticalCenter
                            }
                            width: 110*screenScaleFactor
                            height: parent.height / 2
                            btnTextColor: hovered ? "#FFFFFF" : "#1E9BE2"
                            pointSize:Constants.labelFontPointSize
                            btnBorderW: 0
                            defaultBtnBgColor : "#E2F5FF"
                            hoveredBtnBgColor: "#1EB6E2"
                            text: qsTr("Add")
                            onSigButtonClicked:
                            {
                                addMachine(printerName)
                            }
                        }
                    }
                }
            }
        }
    }
}
