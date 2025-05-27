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

    property var btnNameText: "name"
    property var btnAuthorText: "author"
    property var btnModelImage: ""
    property var btnAvtarImage: ""
    property var modelGroupId: ""
    property var modelCount: 0
    property var modelTime: ""

    property var imageObj: ""
    property var isCrealityImage: false
    property var avtarImageObj: ""
    property var isCrealityAvtarImage: false
    signal sigButtonClicked(var id, var name, var count, var author, var avtar, var ctime)
    signal sigButtonDownClicked(var groupid, var count)

    function crealityModelImage()
    {
        if(btnModelImage != "")
        {
            if(!isCrealityImage)
            {
                var componentGcode = Qt.createComponent("../secondqml/BaseCircularImage.qml")
                if(componentGcode.status === Component.Ready)
                {
                    // imageObj = componentGcode.createObject(idModelImageRow, {"width": 225, 
                    //                                 "height": 225, 
                    //                                 "mipmap": true,
                    //                                 "smooth": true,
                    //                                 "cache": false,
                    //                                 "asynchronous": true,
                    //                                 "fillMode": Image.PreserveAspectFit,
                    //                                 "source": btnModelImage})
                    imageObj = componentGcode.createObject(idModelImageRow, {"width": 225*screenScaleFactor, 
                                                    "height": 225*screenScaleFactor, 
                                                    "radiusImg": 10,
                                                    "isPreserveAspectCrop": true,
                                                    "isLeftBottomRadius": false, 
                                                    "isRightBottomRadius": false,
                                                    "img_src": btnModelImage})
                    isCrealityImage = true
                }
            }
            
        }

        if(btnModelImage != "")
        {
            if(!isCrealityAvtarImage)
            {
                var componentGcode = Qt.createComponent("../secondqml/BaseCircularImage.qml")
                if(componentGcode.status === Component.Ready)
                {
                    avtarImageObj = componentGcode.createObject(idAvtarImageRow, {"width": 25*screenScaleFactor, 
                                                    "height": 25*screenScaleFactor, 
                                                    "img_src": btnAvtarImage})
                    isCrealityAvtarImage = true
                }
            }
        }
    }

    function destroyModelImage()
    {
        if(imageObj && isCrealityImage == true)
        {
            isCrealityImage = false
            imageObj.destroy()
            delete imageObj
        }

        if(avtarImageObj && isCrealityAvtarImage == true)
        {
            isCrealityAvtarImage = false
            avtarImageObj.destroy()
            delete avtarImageObj
        }
    }

    function setAnimatedImageStatus(value)
    {
        idAnimatedImage.visible = value
        idModelImport.visible = !value
    }

    width: 235 * screenScaleFactor
    height: 295 * screenScaleFactor
    Button{
        id: btnModelItem
        width: parent.width
        height: parent.height

        MouseArea
        {
            anchors.fill: parent
            hoverEnabled:true
            property var oldMouseY : 0
            z:btnModelItem.z - 1
            onPositionChanged:
            {
                if(Math.abs(oldMouseY-mouseY) < 5 )
                {
                    return ;
                }
                oldMouseY = mouseY
                //btnModelItem.y = 0
            }
            onExited:
            {
                //console.log("==========onExited==========")
                oldMouseY = 0
                //btnModelItem.y = -5
            }
            onClicked:
            {
                sigButtonClicked(modelGroupId, btnNameText, modelCount, btnAuthorText, btnAvtarImage, modelTime)
            }
        }
        //y:  btnModelItem.hovered ? 0 : -5

        contentItem:Item{
            width: parent.width
            height: parent.height
            Column{
                anchors{
                    horizontalCenter: parent.horizontalCenter
                    verticalCenter: parent.verticalCenter
                }
                Rectangle {
                    width: 225 * screenScaleFactor
                    height: 225 * screenScaleFactor
                    radius: 10
                    opacity: 1
                    color: "#E9E9E9"
                    Rectangle{
                        anchors.bottom: parent.bottom
                        width: parent.width
                        height: 10
                        color: "#E9E9E9"
                    }
                    Row{
                        id: idModelImageRow
                        width: parent.width
                        height: parent.height
                    }

                    // Image{
                    //     width: 225
                    //     height: 225   
                    //     mipmap: true
                    //     smooth: true
                    //     cache: false
                    //     asynchronous: true
                    //     fillMode: Image.PreserveAspectFit
                    //     source: btnModelImage
                    // }
                }
                Rectangle{
                    id: idContentRect
                    width: 225 * screenScaleFactor
                    height: 60 * screenScaleFactor
                    radius: 10
                    border.width: 1/*1*/
                    border.color: Constants.modleItemBorderColor
                    color: "#FFFFFF"
                    Rectangle{
                        anchors.top: parent.top
                        width: parent.width
                        height: 10
                        color: "#FFFFFF"
                        border.width: 1/*1*/
                        border.color: Constants.modleItemBorderColor
                        Rectangle{
                            anchors.top: parent.top
                            anchors.left: parent.left
                            anchors.leftMargin: 1
                            width: parent.width-2
                            height: 10
                            color: "#FFFFFF"
                        }
                    }
                    Row{
                        spacing: 8
                        Rectangle{
                            width: 2
                            height: 60 * screenScaleFactor
                            color: "transparent"
                        }
                        Column{
                            spacing: 6
                            Rectangle{
                                width: 147 * screenScaleFactor
                                height: 4
                                color: "transparent"
                            }
                            StyledLabel{
                                width: 147 * screenScaleFactor
                                height: 12 * screenScaleFactor
                                elide: Text.ElideRight
                                font.pointSize:Constants.labelFontPointSize
                                color: "#333333"
                                text: btnNameText
                            }
                            Row{
                                spacing: 5
                                Row{
                                    id: idAvtarImageRow
                                    width: 24 * screenScaleFactor
                                    height: 24 * screenScaleFactor
                                }
                                // BaseCircularImage{
                                //     width: 24
                                //     height: 24   
                                //     img_src: btnAvtarImage
                                // }
                                StyledLabel{
                                    anchors{
                                        verticalCenter: parent.verticalCenter
                                    }
                                    width: 118 * screenScaleFactor
                                    height: 12 * screenScaleFactor
                                    elide: Text.ElideRight
                                    color: "#333333"
                                    text: btnAuthorText
                                }
                            }
                        }
                        Row{
                            width: 60 * screenScaleFactor
                            height: 60 * screenScaleFactor
                            Rectangle{
                                width: 20 * screenScaleFactor
                                height: 60 * screenScaleFactor
                                color: "transparent"
                            }
                            CusSkinButton_Image{
                                id: idModelImport
                                anchors{
                                    //horizontalCenter: parent.horizontalCenter
                                    verticalCenter: parent.verticalCenter
                                }
                                width: 16 * screenScaleFactor
                                height: 16 * screenScaleFactor
                                tipText: qsTr("Download")
                                btnImgNormal: "qrc:/UI/photo/model_library_download.png"
                                btnImgHovered: "qrc:/UI/photo/model_library_download_h.png"
                                btnImgPressed: "qrc:/UI/photo/model_library_download_h.png"

                                onClicked:
                                {	
                                    sigButtonDownClicked(modelGroupId, btnNameText)
                                    //setAnimatedImageStatus(true)
                                }
                            }
                            AnimatedImage{
                                anchors{
                                    verticalCenter: parent.verticalCenter
                                }
                                id: idAnimatedImage
                                width: 20 * screenScaleFactor
                                height: 20 * screenScaleFactor
                                visible: false
                                source: "qrc:/UI/photo/loading.gif"
                            }
                        }
                    }
                }
            }
        }
        background: Rectangle {
            width: parent.width
            height: parent.height
            color: "transparent" 
            //border.width: 1/*1*/
            //border.color: Constants.modleItemBorderColor
        }
    }
    DropShadow {
        id: idDropShadow
        anchors.fill: btnModelItem
        visible: btnModelItem.hovered ? true :false
        //radius: 8
        samples: 30
        source: btnModelItem
        color: Constants.dropShadowColor
    }
}
