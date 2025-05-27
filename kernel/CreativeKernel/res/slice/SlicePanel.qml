import QtQuick 2.0
import Qt.labs.platform 1.1
import CrealityUI 1.0
import "qrc:/CrealityUI"
import QtQuick.Layouts 1.3

Item {
    id : idslice
    property var load
    property var slice
    property var slicemodel

    property var autoconfig
    property var settingconfig

    property var softwarefunction
    property var getData

    property var mainObj
    //控制显示silderPreview
    property var sliderShowObj
    anchors.fill: parent
    property var machineType
    //    implicitWidth: 260
    property var defultLeftToolWidth : 48

    //    property int defaultWidth: mainObj.width -200
    signal sigSliceView(bool visible)

    function previewGcode(){
        slice.sliceMainOp("gcodeFilePreview")
    }
    //    function setGcodePreviewMode(){
    //        slice.preview()
    //    }

    //
    property var statusObj

    onHeightChanged:
    {
        console.log("hieght22 =" + height)
    }

    HalotBoxTotalRightUIPanel
    {
        id:paramPanel
        width: 280 * screenScaleFactor
    }

    Connections{
        target: sliceUI
        onSigSliceSuccess:{
            setEnablePreviewMode(obj)
            standaloneWindow.curState = 1
        }
        onSigStartPreview:{
            standaloneWindow.curState = 1
            idslice.machineType = deviceType
            setPreviewMode()
        }
    }

    Connections{
        target: standaloneWindow
        onCurStateChanged:{
            if(curState===0){
                cancelPreview()

            }else{
                setPreviewMode()
            }
        }
    }

    function setPreviewMode()
    {
//        leftToolBar.visible=false
        Constants.bModelAlwaysShow = false
        paramPanel.visible=false
        parent.sigShowRightMenu(false)
        parent.mainWindowPreviewShow()
    }

    function setEnablePreviewMode(report)
    {
        console.log("slice set enable preview model");
        if(!report)
        {
            console.log("get report failed")
            return ;
        }
        slicePlugin.sliceMainOp("preview")
    }

    function cancelPreview()
    {
        paramPanel.visible=true
//        leftToolBar.width = defultLeftToolWidth
//        leftToolBar.visible=false
        parent.sigShowRightMenu(paramPanel.visible)
        parent.mainWindowSliceShow()
        Constants.bModelAlwaysShow = true
        sliderShowObj.visible = true
    }
    function saveGCode()
    {
        //lisugui 2021-3-27
        slice.sliceMainOp("savetmpgcode")
        statusObj.cancelJobButton.disconnect(enablePreview)
        statusObj.cancelJobButton.connect(enablePreview)
//        leftToolBar.visible=false
    }

    function updateGCode()
    {
        slice.sliceMainOp("updatetmpgcode")
        statusObj.cancelJobButton.disconnect(enablePreview)
        statusObj.cancelJobButton.connect(enablePreview)
//        leftToolBar.visible=false
    }

    function disEnablePreview()
    {
        //idDLPePreview.enabled = false
    }
    function enablePreview()
    {
        //idDLPePreview.enabled = true
    }

    function showWizards()
    {
        //console.log("showWizards")
        idRightPanel.showWizards()
    }
}

