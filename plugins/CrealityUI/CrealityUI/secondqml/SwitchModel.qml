import QtQuick 2.0
import QtQuick.Layouts 1.13
import QtQuick.Controls 2.0
import "../qml"
Rectangle {
    property var currentModel: "laser"//"fdm"
    property var laserImgList: [Constants.switchModelLaserImgEn, Constants.switchModelLaserImg]
    property var laserImgHList: [Constants.switchModelLaserImgEn_H, Constants.switchModelLaserImg_H]
    property var laserImg: ""
    property var laserImgH: ""
    property var languageType: 1
    property var hasCNC: false
    signal fdmView()
    signal laserView()
    signal plotterView()
    color: Constants.switchModelBgColor//"#333333"
    border.width: 1
    border.color: Constants.rightPanelRectBgBorderColor
    implicitWidth: 270
    implicitHeight: 45//50
    anchors.top: parent.top
    anchors.topMargin: 10
    function onChangeLanguage(value)//0->en, 1->other
    {
        if(value == 0){
            
            laserImg = laserImgList[value]
            laserImgH = laserImgHList[value]
            laserLogo.width = 49;
            laserLogo.height = 13;
            console.log("onChangeLanguage 0 "+ laserImg + " " + laserImgH)
        }
        else if(value == 1){
            laserImg = laserImgList[value]
            laserImgH = laserImgHList[value]
            laserLogo.width = 32;
            laserLogo.height = 16;
            console.log("onChangeLanguage 1 "+ laserImg + " " + laserImgH)
        }
        languageType = value
    }

    function changeToFdm()
    {
        currentModel="fdm"
        fdmView()
        Constants.bModelAlwaysShow = true
    }

    MouseArea {
        anchors.fill: parent
    }

    Row {
        y:4//10
        x:1
        height: parent.height
        spacing: 0
        Button {
            id:idFdm
            text: qsTr("FDM")
            width:hasCNC ? 89:134
            height:40
            //            enabled: currentModel!="fdm"
            onClicked: {
                if(currentModel === "fdm")return
                currentModel="fdm"
                fdmView()
                Constants.bModelAlwaysShow = true
            }
            contentItem: /*Text {
                     text: idFdm.text
                     font.family: Constants.labelFontFamily
                     font.weight: font.Bold
                     font.pixelSize: 16
                     color: enabled?"#FFFFFF":"#999999"
                     horizontalAlignment: Text.AlignHCenter
                     verticalAlignment: Text.AlignVCenter
                     elide: Text.ElideRight
                 }*/
                         Item {
                anchors.fill: parent
                Image {
                    anchors.centerIn: parent
                    width: 32
                    height: 13
                    source: currentModel === "fdm" || idFdm.hovered ? Constants.switchModelFDMImg_H : Constants.switchModelFDMImg

                    //                            enabled ? Constants.switchModelFDMImg : Constants.switchModelFDMImg_H
                }
            }
            background: Rectangle {
                implicitWidth: 135
                implicitHeight: 40
                color: currentModel === "fdm" ?  Constants.switchModeSelectedlBgColor :  Constants.switchModelBgColor
                //enabled ? Constants.switchModelBgColor : Constants.switchModeSelectedlBgColor//"#333333":"#535353"
                radius: 0
            }
        }

        Button {
            id:idLaser
            text: qsTr("Laser")
            width:hasCNC ? 89:134
            height:40
            enabled: currentModel!="laser"
            onClicked: {
                if(currentModel === "laser")return
                currentModel="laser"
                laserView()
                Constants.bModelAlwaysShow = false
            }
            contentItem:
                Item {
                anchors.fill: parent
                Image {
                    anchors.centerIn: parent
                    id: laserLogo
                    width: 32
                    height: 16
                    source: languageType == 1 ? (currentModel=== "laser" || idLaser.hovered ? Constants.switchModelLaserImg_H : Constants.switchModelLaserImg ) :
                                                (currentModel=== "laser" || idLaser.hovered ? Constants.switchModelLaserImgEn_H : Constants.switchModelLaserImgEn)
                }
            }
            background: Rectangle {
                implicitWidth: 135
                implicitHeight: 40
                color: currentModel=== "laser" ? Constants.switchModeSelectedlBgColor : Constants.switchModelBgColor
                //opacity: enabled ? 1 : 0.3
                //border.color: control.down ? "#535353" : "#21be2b"
                //border.width: 1
                radius: 0
            }
        }
        Button {
            id:idGrapher
            text: qsTr("Plotter")
            width:hasCNC ? 90:134
            height:40
            enabled: currentModel!="plotter"
            visible: hasCNC
            onClicked: {
                if(currentModel === "plotter")return
                currentModel="plotter"
                plotterView()
            }
            contentItem:
                Item {
                anchors.fill: parent
                Image {
                    anchors.centerIn: parent
                    width: 32
                    height: 13
                    source: currentModel === "plotter" || idGrapher.hovered ?  Constants.switchModelCNCImg_H : Constants.switchModelCNCImg
                }
            }
            background: Rectangle {
                implicitWidth: 135
                implicitHeight: 40
                color: currentModel === "plotter" ? Constants.switchModeSelectedlBgColor : Constants.switchModelBgColor
                radius: 0
            }
        }
    }
}
