import QtQuick 2.10
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0
import CrealityUI 1.0
import "qrc:/CrealityUI"
import ".."
import "../qml"

Button{

    property var equipmentName: ""
    property var equipmentStatus: ""
    property var equipmentID: ""
    property var printerImage: "qrc:/UI/photo/machineImage_Printer_Box.png"
    property var printerName: ""
    property var printerStatus: ""
    property var printerStatusShow: ""
    property var printerIp: ""
    property var tFCardStatus: ""
    property var videoStatus: 0
    property var shangYunAPILicense: ""
    property var shangYunDIDString: ""
    property var shangYunInitString: ""
    property alias progressValue: progressBar.value
    property alias progressVisible: progressBar.visible
    property var itemEnable: false

    signal sigButtonClicked(var deviceName, var deviceID)

    id: idUserPrinterItemBtn
    width: (239*2 + 20) * screenScaleFactor
    height: 200 * screenScaleFactor
    padding: 15 * screenScaleFactor

    MouseArea
    {
        anchors.fill: parent
        hoverEnabled: true
        onClicked:
        {
            if(itemEnable)
            {
                //console.log("UseInfoPinterItem Clicked!!!")
                sigButtonClicked(equipmentName, equipmentID)
            }
        }
    }

    contentItem: Item{
        width: parent.width
        height:parent.height
        Row{
            width: parent.width
            height: parent.width
            Column{
                width: 200 * screenScaleFactor
                height: parent.height
                Image{
                    id: idPrinterImage
                    width:200 * screenScaleFactor
                    height: 140 * screenScaleFactor
                    mipmap: true
                    smooth: true
                    cache: false
                    asynchronous: true
                    opacity: itemEnable ? 1 : 0.2
                    fillMode: Image.PreserveAspectFit
                    source: (printerImage != undefined) ? printerImage : "qrc:/UI/photo/machineImage_Printer_Box.png"
                }
                
                StyledLabel{
                    width: 200 * screenScaleFactor
                    height: 30 * screenScaleFactor
                    font.pointSize: Constants.labelFontPointSize
                    verticalAlignment: Qt.AlignBottom
                    horizontalAlignment:Text.AlignHCenter
                    color: "#333333"
                    text: printerName
                    elide: Text.ElideRight
                }
            }
            Rectangle{
                width: 10 * screenScaleFactor
                height: 200 * screenScaleFactor
                opacity: 1
                color: "transparent"
            }
           
            Column{
                width: parent.width - 200 * screenScaleFactor - 20 * screenScaleFactor
                height:parent.height
                spacing: 10 * screenScaleFactor
                Row{
                    width: parent.width
                    StyledLabel{
                        width: 100 * screenScaleFactor
                        height: 20 * screenScaleFactor
                        font.pointSize: Constants.labelFontPointSize
                        verticalAlignment: Qt.AlignVCenter
                        horizontalAlignment: Qt.AlignLeft
                        color: "#333333"
                        text: qsTr("Device Name:")
                        elide: Text.ElideRight
                    }
                    StyledLabel{
                        width: parent.width - 100
                        height: 20 * screenScaleFactor
                        font.pointSize: Constants.labelFontPointSize
                        verticalAlignment: Qt.AlignVCenter
                        horizontalAlignment: Qt.AlignLeft
                        color: "#333333"
                        text: equipmentName
                        elide: Text.ElideRight
                    }
                }
                Row{
                    width: parent.width
                    StyledLabel{
                        width: 100 * screenScaleFactor
                        height: 20 * screenScaleFactor
                        font.pointSize: Constants.labelFontPointSize
                        verticalAlignment: Qt.AlignVCenter
                        horizontalAlignment: Qt.AlignLeft
                        color: "#333333"
                        text: qsTr("Device ID:")
                        elide: Text.ElideRight
                    }
                    StyledLabel{
                        width: parent.width - 100 * screenScaleFactor
                        height: 20 * screenScaleFactor
                        font.pointSize: Constants.labelFontPointSize
                        verticalAlignment: Qt.AlignVCenter
                        horizontalAlignment: Qt.AlignLeft
                        color: "#333333"
                        text: equipmentID
                        elide: Text.ElideRight
                    }
                }
                Row{
                    width: parent.width
                    StyledLabel{
                        width: 100 * screenScaleFactor
                        height: 20 * screenScaleFactor
                        font.pointSize: Constants.labelFontPointSize
                        verticalAlignment: Qt.AlignVCenter
                        horizontalAlignment: Qt.AlignLeft
                        color: "#333333"
                        text: qsTr("Status:")
                        elide: Text.ElideRight
                    }
                    StyledLabel{
                        width: parent.width - 100 * screenScaleFactor
                        height: 20 * screenScaleFactor
                        font.pointSize: Constants.labelFontPointSize
                        verticalAlignment: Qt.AlignVCenter
                        horizontalAlignment: Qt.AlignLeft
                        color: "#333333"
                        text: equipmentStatus
                        elide: Text.ElideRight
                    }
                }
                Row{
                    width: parent.width
                    StyledLabel{
                        width: 100 * screenScaleFactor
                        height: 20 * screenScaleFactor
                        font.pointSize: Constants.labelFontPointSize
                        verticalAlignment: Qt.AlignVCenter
                        horizontalAlignment: Qt.AlignLeft
                        color: "#333333"
                        text: qsTr("TF Card Status:")
                        elide: Text.ElideRight
                    }
                    StyledLabel{
                        width: parent.width - 100 * screenScaleFactor
                        height: 20 * screenScaleFactor
                        font.pointSize: Constants.labelFontPointSize
                        verticalAlignment: Qt.AlignVCenter
                        horizontalAlignment: Qt.AlignLeft
                        color: "#333333"
                        text: tFCardStatus
                        elide: Text.ElideRight
                    }
                }
                Row{
                    width: parent.width
                    StyledLabel{
                        width: 100 * screenScaleFactor
                        height: 20 * screenScaleFactor
                        font.pointSize: Constants.labelFontPointSize
                        verticalAlignment: Qt.AlignVCenter
                        horizontalAlignment: Qt.AlignLeft
                        color: "#333333"
                        text: qsTr("Printer Status:")
                        elide: Text.ElideRight
                    }
                    StyledLabel{
                        width: parent.width - 100 * screenScaleFactor
                        height: 20 * screenScaleFactor
                        font.pointSize: Constants.labelFontPointSize
                        verticalAlignment: Qt.AlignVCenter
                        horizontalAlignment: Qt.AlignLeft
                        color: "#333333"
                        text: printerStatusShow
                        elide: Text.ElideRight
                    }
                }
                Row{
                    width: parent.width
                    StyledLabel{
                        width: 100 * screenScaleFactor
                        height: 20 * screenScaleFactor
                        font.pointSize: Constants.labelFontPointSize
                        verticalAlignment: Qt.AlignVCenter
                        horizontalAlignment: Qt.AlignLeft
                        color: "#333333"
                        text: qsTr("IP:")
                        elide: Text.ElideRight
                    }
                    StyledLabel{
                        width: parent.width - 100 * screenScaleFactor
                        height: 20 * screenScaleFactor
                        font.pointSize: Constants.labelFontPointSize
                        verticalAlignment: Qt.AlignVCenter
                        horizontalAlignment: Qt.AlignLeft
                        color: "#333333"
                        text: printerIp
                        elide: Text.ElideRight
                    }
                }
                Row{
                    width: parent.width
                    // Rectangle{
                    //     width: 100
                    //     height: 10
                    //     color: "transparent"
                    // }
                    ProgressBar{
                        id: progressBar
                        visible: false
                        from: 0
                        to:100
                        value: 0
                        width: parent.width
                        height: 3 * screenScaleFactor

                        background: Rectangle {   
                            implicitWidth: progressBar.width
                            implicitHeight: progressBar.height
                            color: "#BABABA"
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
            }
        }
    }
    
    background: Rectangle {
        width: parent.width
        height: parent.height
        color: Constants.userInfoPrintItem
        opacity: itemEnable ? 1 : 0.2
        border.width: itemEnable ? (idUserPrinterItemBtn.hovered ? 2 : 1) : 0
        border.color: idUserPrinterItemBtn.hovered ? "#1E9BE2" : Constants.modleItemBorderColor
    }
}
