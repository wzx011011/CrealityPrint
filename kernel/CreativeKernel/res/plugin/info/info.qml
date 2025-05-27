import QtQuick 2.10
import QtQuick.Controls 2.0
import CrealityUI 1.0
import "qrc:/CrealityUI"
Rectangle{
    color: Constants.currentTheme == 0 ? Qt.rgba(0,0,0, 0.2) : Qt.rgba(1,1,1, 0.5)
    //anchors.right: parent.right
    id:idSelectInfo
	anchors.bottom : parent.bottom
    anchors.bottomMargin: 20 + 55
    anchors.left : parent.left
    anchors.leftMargin: 30
	//x:20

    width: 280
    height: infoPanelVisible || infoPanelMulitVisible ? (erroredges == 0 && errornormals == 0 ? 200 : 240) : 100
    property var size
    property var modelname:""
    property var verticessize:0
    property var facesize:0
    property var errornormals:0
    property var erroredges:0
	property var errorholes:0
    property var errorintersect:0
    property var errormodel:0

    property var modeldata
    property var infoPanelVisible: true
    property var infoPanelMiniVisible: false
    property var infoPanelMulitVisible: false
    signal sigRepair()
	signal sigUpdateInfo()

	function updateInfo(){
                        sigUpdateInfo();
						console.log("info qml")
                   }

    visible: false
//    objectName: "infoshowObj"
    InfoPanelRepair {
        id: infoPanel
        //anchors.fill: parent
        x: 20
        y: 15
        visible: infoPanelVisible
        modelNameWidth: 200
        modelName:modelname
        modelXSize:size.x
        modelYSize:size.y
        modelZSize:size.z
        verticesSize:verticessize
        faceSize:facesize
        errorNormals:errornormals
        errorEdges:erroredges
        errorHoles:errorholes
        errorIntersects:errorintersect

        onVisibleChanged:
        {
            infoPanel.selected = visible
        }

        onSigRepairBtnClicked:
        {
            sigRepair()
        }
    }

    InfoPanelMini {
        id: infoPanelmini
        errorModel: errormodel
        x: 20
        y: 15
        visible: infoPanelMiniVisible
    }

    InfoPanelMulit {
        id: infoPanelMulit
        errorModel: errormodel
        x: 20
        y: 15
        visible: infoPanelMulitVisible
        verticesSize:verticessize
        faceSize:facesize
        errorNormals:errornormals
        errorEdges:erroredges
		errorHoles:errorholes
        errorIntersects:errorintersect
        onSigRepairBtnClicked:
        {
            sigRepair()
        }
        // onVisibleChanged:
        // {
        //     if(Constants.mainPreviewShow)
        //     {
        //         visible =false
        //         return
        //     }
        // }
    }

    onVisibleChanged: {
        if(Constants.mainPreviewShow)
        {
            visible =false
            return
        }
        
        leftToolBar.showPop = infoPanel.visible
        //console.log(infoPanel.visible)
    }


}

