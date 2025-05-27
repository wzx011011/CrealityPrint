import QtQuick 2.10
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0
import CrealityUI 1.0
import "qrc:/CrealityUI"
import ".."
import "../qml"
Item{
    property var modelname: ""
    property var modelid: ""
    property var modellink: ""
    property var modeSize: ""
    property var btnIsSelected: false
    property var keystr: 0

    signal sigBtnDetailClicked(var key)
    signal sigBtnDownLoadDetailModel(var key, var name, var link)

    function setAnimatedImageStatus(value)
    {
        idAnimatedImage.visible = value
    }

    implicitWidth: 280
    implicitHeight: 36
    Row{
        width: parent.width
        height: parent.height
        spacing: 20
        BasicButton{
            width: 140//+100
            height: 36
            btnRadius: 3
            btnBorderW: 0
            pixSize: 12
            btnTextColor: btnIsSelected ? "#FFFFFF" : "#666666"
            btnText.width: 140-10//+100-10
            defaultBtnBgColor: "#FFFFFF"
            hoveredBtnBgColor: "#E1E1E1"
            selectedBtnBgColor: "#1E9BE2"
            btnSelected: btnIsSelected
            text: modelname

            onSigButtonClicked:
            {
                sigBtnDetailClicked(modelid)
            }
        }
        StyledLabel{
            width: 80
            height: 36
            font.pixelSize:12
            color: "#999999"
            text: {
                if(modeSize > 1024*1024*1024)
                {
                    return Math.ceil(modeSize/(1024*1024*1024))+"GB"
                }
                else if(modeSize > 1024*1024)
                {
                    return Math.ceil(modeSize/(1024*1024))+"MB"
                }
                else if(modeSize > 1024)
                {
                    return Math.ceil(modeSize/1024)+"KB"
                }
                else
                {
                    return modeSize+"B"
                }
            }
            verticalAlignment: Qt.AlignVCenter
        }
        CusSkinButton_Image{
            // anchors{
            //     verticalCenter: parent.verticalCenter
            // }
            width:36
            height:36
            visible: btnIsSelected
            tipText: qsTr("Import")
            btnImgNormal: "qrc:/UI/photo/model_detail_item_import.png"
            btnImgHovered: "qrc:/UI/photo/model_detail_item_import_h.png"
            btnImgPressed: "qrc:/UI/photo/model_detail_item_import_h.png"
            onClicked:
            {
                sigBtnDownLoadDetailModel(modelid, modelname, "import")
            }
        }
        CusSkinButton_Image{
            // anchors{
            //     verticalCenter: parent.verticalCenter
            // }
            width:36
            height:36
            visible: btnIsSelected
            tipText: qsTr("Download")
            btnImgNormal: "qrc:/UI/photo/model_detail_item_down.png"
            btnImgHovered: "qrc:/UI/photo/model_detail_item_down_h.png"
            btnImgPressed: "qrc:/UI/photo/model_detail_item_down_h.png"
            onClicked:
            {
                sigBtnDownLoadDetailModel(modelid, modelname, "download")
            }
        }
        AnimatedImage{
            anchors{
                verticalCenter: parent.verticalCenter
            }
            id: idAnimatedImage
            width: 20
            height: 20
            visible: false
            source: "qrc:/UI/photo/loading.gif"
        }
    }
}