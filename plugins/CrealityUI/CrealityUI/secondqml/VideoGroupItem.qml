import QtQuick 2.10
import QtQuick.Controls 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0 as QQC2
import CrealityUI 1.0
import "qrc:/CrealityUI"
import ".."
import "../qml"

import com.cxsw.SceneEditor3D 1.0
import QtMultimedia 5.12
import QtGraphicalEffects 1.12

Item
{
    width:480
    height:345

    property var devicename: ""
    property var gcodeFile: ""
    property var printState: ""
    property alias progressValue: progressBar.value

    DropShadow {
        id: idDropShadow
        anchors.fill: idGroupRect
        visible:true
        //radius: 0
        samples: 30
        //spread:0.8
        source: idGroupRect
        color: Constants.dropShadowColor
    }

    MouseArea
    {
        anchors.fill: parent
        hoverEnabled:true
        onEntered:
        {
            idDropShadow.visible = true
        }
        onExited:
        {
            idDropShadow.visible = false
        }
    }
    function loadVideoUrl(urlStr)
    {
        console.log("loadVideoUrl(urlStr) =" + urlStr)
        // idvideoItem.start(urlStr)
    }

    function stopVideo()
    {
        idvideoItem.stop()
    }
    Rectangle
    {
        id: idGroupRect
        //width:parent.width
       // height:parent.height
        anchors.fill: parent
        anchors.margins: 10
        radius:10
        

        Column {
            x:10
            y:10
            width: parent.width - 20
            height:parent.height - 20
            spacing:5

            Rectangle{
                width:parent.width
                height:254
                color: "#333333"
                QMLPlayer {
                    id: idvideoItem
                    width: parent.width
                    height: 254
                    visible: true
                }
            }

            Row {
                spacing:10
                StyledLabel{
                    width: 80
                    height: 20
                    font.pixelSize:12 
                    color: "#333333"
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignLeft
                    text: qsTr("Device Name:")
                }
                StyledLabel{
                    width: 120
                    height: 20
                    font.pixelSize:12 
                    color: "#333333"
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignLeft
                    text: devicename
                    elide: Text.ElideRight
                }

                StyledLabel{
                    width: 80
                    height: 20
                    font.pixelSize:12 
                    color: "#333333"
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignLeft
                    text: qsTr("Printing File:")
                }
                StyledLabel{
                    width: 130
                    height: 20
                    font.pixelSize:12 
                    color: "#333333"
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignLeft
                    text: gcodeFile
                    elide: Text.ElideRight
                }
            }

            Column{
                Row{
                    StyledLabel{
                        id: idPrintState
                        width: 80
                        height:20
                        font.pixelSize:12
                        color: "#333333"
                        verticalAlignment: Qt.AlignVCenter
                        horizontalAlignment: Qt.AlignLeft
                        text: printState
                    }
                    StyledLabel{
                        id: idPrintValue
                        width: idGroupRect.width - 100
                        height:20
                        font.pixelSize:12
                        color: "#333333"
                        verticalAlignment: Qt.AlignVCenter
                        horizontalAlignment: Qt.AlignRight
                        text: {
                            if(progressBar.value == 0)
                            {
                                return ""
                            }
                            else{
                                return progressBar.value + "%"
                            }
                        }
                    }
                }
                ProgressBar{
                    id: progressBar
                    from: 0
                    to:100
                    value: 0
                    width: idGroupRect.width - 20
                    height: 3

                    background: Rectangle {   
                        implicitWidth: progressBar.width
                        implicitHeight: progressBar.height
                        color: "#ECECEC"//Constants.progressBarBgColor
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

