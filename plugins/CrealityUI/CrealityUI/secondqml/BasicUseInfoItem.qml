import QtQuick 2.10
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0
import CrealityUI 1.0
import "qrc:/CrealityUI"
import ".."
import "../qml"
Rectangle {
    color:"transparent"
    border.width: 1
    border.color: Constants.modleItemBorderColor
    width: 241*screenScaleFactor
    height: 202*screenScaleFactor


    property var baseName: ""
    property var baseImage: ""
    property var baseId: ""
    property var downloadlink:""
    property var modelcount: ""
    property var baseType: 1 // 1: gcode; 2: model
    property var printVisible: true
    property var shareVisible: !printVisible
    property var deleteVisible: true
    property var importBtnVisible:true
    property var useType: 0
    property var isPreserveAspectCrop: false

    signal deleteCurrentItem(var id)
    signal importCurrentItem(var id, var name, var downloadlink)
    signal importModel(var id, var count)
    signal sigShareModel(var id)
    signal sigGcodePrint(var id)

    function setAnimatedImageStatus(value)
    {
        idAnimatedImage.visible = value
        idImport.visible = !value
    }


    Column{
        x:1
        y:1
        Rectangle {
            width: 239*screenScaleFactor
            height: 164*screenScaleFactor
            opacity: 1
            color: "#D7D7D7"

            Image{
                width: 239*screenScaleFactor
                height: 164 *screenScaleFactor  
                mipmap: true
                smooth: true
                cache: false
                asynchronous: true
                fillMode: isPreserveAspectCrop ? Image.PreserveAspectCrop : Image.PreserveAspectFit
                source: baseImage
            }
        }
        Rectangle {
            width: 239*screenScaleFactor
            height: 36*screenScaleFactor
            opacity: 1
            color: Constants.userinfoBasicItemRecColor//"#404040"
            Row{
                width: 219 *screenScaleFactor
                anchors.top: parent.top
                anchors.topMargin: 8*screenScaleFactor
                anchors.left: parent.left
                anchors.leftMargin: 10*screenScaleFactor
                spacing: 20
                StyledLabel{
                    width: {
                        if(printVisible === true ){
                            return parent.width-idPrint.width-idImport.width-idDelete.width-60
                        }
                        else(printVisible === false)
                        {
                            if(shareVisible === false && deleteVisible === false)
                            {
                                return parent.width-idImport.width - 20
                            }
                            return parent.width-idImport.width-idDelete.width-idModelShare.width - 60
                        }
                    }
                    height: parent.height
                    font.pointSize:Constants.labelFontPointSize
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignLeft
                    text: baseName
                    elide: Text.ElideRight
                }
                AnimatedImage{
                    id: idAnimatedImage
                    visible: false
                    width: 18*screenScaleFactor
                    height: 18*screenScaleFactor
                    opacity: 0.5
                    source: "qrc:/UI/photo/loading_g.gif"
                }
                CusSkinButton_Image{
                    id: idImport
                    width:17*screenScaleFactor
                    height: 18*screenScaleFactor
                    visible: true
                    btnImgNormal: "qrc:/UI/photo/userinfo_export.png"
                    btnImgHovered: Constants.userinfoExport_HImg
                    btnImgPressed: Constants.userinfoExport_HImg
                    tipText: {
                        return printVisible ? qsTr("Import Gcode") : qsTr("Import Model")
                    }
                    onClicked:
                    {	
                        if(baseType === 1)
                        {
                            importCurrentItem(baseId, baseName, downloadlink)
                        }
                        else if(baseType === 2)
                        {
                            importModel(baseId, baseName)
                        }
                        setAnimatedImageStatus(true)
                    }
                }
                CusSkinButton_Image{
                    id: idModelShare
                    width: 19*screenScaleFactor
                    height: 18*screenScaleFactor
                    visible: shareVisible
                    btnImgNormal: "qrc:/UI/photo/userinfo_share.png"
                    btnImgHovered: Constants.userinfoShare_HImg
                    btnImgPressed: Constants.userinfoShare_HImg
                    tipText: qsTr("Share The Model Group(Copy Link)")
                    onClicked:
                    {	
                    sigShareModel(baseId)
                    }

                }
                CusSkinButton_Image{
                    id: idPrint
                    width:19*screenScaleFactor
                    height: 18*screenScaleFactor
                    visible: printVisible
                    btnImgNormal: "qrc:/UI/photo/userinfo_print.png"
                    btnImgHovered: Constants.userinfoPrint_HImg
                    btnImgPressed: Constants.userinfoPrint_HImg
                    tipText: qsTr("Print Gcode")
                    onClicked:
                    {	
                        sigGcodePrint(baseId)
                    }
                }
                CusSkinButton_Image{
                    id: idDelete
                    width:14*screenScaleFactor
                    height: 17*screenScaleFactor
                    visible: deleteVisible
                    btnImgNormal: "qrc:/UI/photo/userinfo_delete.png"
                    btnImgHovered: Constants.userinfoDel_HImg
                    btnImgPressed: Constants.userinfoDel_HImg
                    tipText: {
                        if(useType === 3){
                            return qsTr("UnCollect")
                        }else{
                            return printVisible ? qsTr("Delete Gcode") : qsTr("Delete Model")
                        }
                    }
                    onClicked:
                    {	
                        deleteCurrentItem(baseId)
                    }
                }
            }
        }
    }

}
