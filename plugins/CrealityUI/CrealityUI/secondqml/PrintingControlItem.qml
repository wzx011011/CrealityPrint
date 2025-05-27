import QtQuick 2.10
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0
import CrealityUI 1.0
import "qrc:/CrealityUI"
import ".."
import "../qml"

Item{

    property var isCameraShow: false
    property var deviceName: "device"
    property var deviceID: ""
    property var deviceTBId: ""
    property var iotType: ""
    property var printTime: "- - -"
    property var printProgress: "30"
    signal sigViewPrintBtnClicked(var name, var id, var tbId, var iotType)


    width: 637
    height: 60
    Rectangle{
        width: parent.width
        height: parent.height
        color: "#D7D7D7"

        Row{
            anchors{
                 verticalCenter: parent.verticalCenter
            }
            width: parent.width
            height: parent.height - 24
            spacing: 20
            StyledLabel{
                width: 120
                height: 36
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment:Text.AlignHCenter
                elide: Text.ElideRight
                color: "#000000"
                text: deviceName
            }
            Row{
                width: 40
                height: 36
                Rectangle{
                    width: 10
                    height: 24
                    color: "transparent"
                }
                CusSkinButton_Image{
                    anchors{
                        verticalCenter: parent.verticalCenter
                    }
                    width: 20
                    height: 24
                    btnImgUrl: isCameraShow ? "qrc:/UI/photo/print_camera_h.png" : "qrc:/UI/photo/print_camera.png"
                }
                Rectangle{
                    width: 10
                    height: 24
                    color: "transparent"
                }
            }
            StyledLabel{
                width: 120
                height: 36
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment:Text.AlignHCenter
                elide: Text.ElideRight
                color: "#000000"
                text: printTime
            }
            StyledLabel{
                width: 120
                height: 36
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment:Text.AlignHCenter
                elide: Text.ElideRight
                color: "#000000"
                text: printProgress + "%"
            }
            BasicButton{
                id: idPrinterBtn
                width: 140
                height: 36
                btnRadius: 3
                btnBorderW: 0
                pixSize: 12
                hoveredBtnBgColor: "#1E9BE2"
                defaultBtnBgColor: "#BEBEBE"
                selectedBtnBgColor: "#1E9BE2"
                text: qsTr("View Print")
                onSigButtonClicked:
                {
                    sigViewPrintBtnClicked(deviceName, deviceID, deviceTBId, iotType)
                }
            }
        }
    }
}