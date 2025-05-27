import QtQuick 2.13
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
//import CrealityUI 1.0
//import "qrc:/CrealityUI"
import "../qml"
Rectangle {
    id:idSlicePreview
    property var slicemodelcontrol
    property var slicercontrol
    property var sliceReport
    property var fdmMainObj
    //property string gFCodePath
    //property string gFCodeImage

    property int defaultMainWidth: fdmMainObj.width
    property int defaultMainHeight: fdmMainObj.height

    property bool footOnlyShowChecked//: idSlicePreviewSelection.onlyShowCheck
    property bool startStaus//: idSlicePreviewSelection.isStart

    property alias stepViewObj: idSlicePreviewRight.stepViewObj

    //property alias  footOnlyShowChecked: idSlicePreviewSelection.onlyShowCheck
    //property alias  startStaus: idSlicePreviewSelection.isStart

    signal switchWifiPrint()
    signal skipUrlCrealityCloud()
    signal showUploadWarningDlg()

    function cancelPreviewBtnClicked()
    {
        if(idSlicePreview.visible === true)
        {
            basicComButton.sigButtonClicked()
        }
    }

    function clusterUploadGcodeBtn()
    {
        slicercontrol.clusterUploadLocalGcodeToAliyun()
    }

    function letClusterUploadGcodeDigShow()
    {
        idUploadGCode.clusterUploadGcodeShow()
        idUploadGCode.visible = true
    }

    function saveGCode()
    {
        //slice.sliceMainOp("beginSave")
        //lisugui 2021-3-27
        slicercontrol.sliceMainOp("savetmpgcode")
        statusObj.cancelJobButton.disconnect(enablePreview)
        statusObj.cancelJobButton.connect(enablePreview)
        standaloneWindow.leftToolBar.visible=false
    }

    function updateUploadProgressValue(value, total)
    {
        idUploadGCode.progressValue = value
    }

    function uploadGCodeSuccess(strjson)
    {
        idUploadGCode.uploadGcodeSuccess(strjson)
    }

    function uploadGCodeError()
    {
        idUploadGCode.visible = false
        idUploadGCode.updaloadGcodeFailed()
        idUploadGcodeFailedDlg.visible = true
    }

    function update()
    {
        idSlicePreviewRight.updateUI()
    }

    function uploadDlgShow(name)
    {
        idUploadGCode.show()
        idUploadGCode.setFileName(name)
    }

    function uploadBtnClicked()
    {
        slicercontrol.sliceMainOp("uploadSliceToCrealityCloud")
    }

    onVisibleChanged:
    {
        if(this.visible == true)
        {
            slicercontrol.sliceMainOp("fdmpreviewpanel.visible=true")
        }
    }

    visible: width > 10 ? true : false

    width: 300 * screenScaleFactor
    height: 634 * screenScaleFactor

    radius: 5
    color: Constants.right_panel_menu_background_color

    border.width: 1
    border.color: Constants.right_panel_border_default_color

    Connections{
      target: slicercontrol
      onSigUpdateProgress: idUploadGCode.progressValue = prog
      onSigUploadSuccess: idUploadGCode.uploadGcodeSuccess(responseStr)
    }

    Column {
      width: parent.width
      height: parent.height

      Rectangle {
        width: parent.width
        height: 10 * screenScaleFactor
        color: "transparent"
      }

      FDMRightPanel {
        id : idSlicePreviewRight

        width: parent.width
        height: 540 * screenScaleFactor
        color: "transparent"

        visible: idSlicePreview.width > 10 ? true : false

        onTabIndexChanged: {
          slicercontrol.sliceMainOp("sigSelectType", currentTabIndex)
        }

        onVisibleChanged: {
            if (visible) {
              idSlicePreviewRight.stepViewObj.model =
                  slicemodelcontrol.getCurrentLayerGCodeList()
            }
        }

        onChangePreview: {
          var param = (type << 8) + state
          slicercontrol.sliceMainOp("changeShowInfo", param)
        }

        onShowColorchanged: {
          slicemodelcontrol.setShowColor(type, bShow)
        }

        stepViewObj.onDoubleClickItem: {
          if (!slicemodelcontrol)
          if (slicemodelcontrol.findStepFromViewIndex(viewIndex) === -1) { return }
          idSlicePreviewSelection.currentStep = slicemodelcontrol.findStepFromViewIndex(viewIndex)
        }

        Component.onCompleted: {
            idSlicePreviewRight.stepViewObj.model =
                slicemodelcontrol.getCurrentLayerGCodeList()
          }
      }

      Rectangle {
        width: parent.width
        height: 1
        color: Constants.right_panel_menu_split_line_color
      }

      Grid {
        columns: 2
        spacing: 6 * screenScaleFactor
        padding: 10 * screenScaleFactor

        BasicButton {
          id: exportBtn
          width: 136 * screenScaleFactor
          height: 28 * screenScaleFactor
          btnRadius: 3
          text: qsTr("Export to Local")
          enabled: true
          btnText.color: Constants.right_panel_slice_text_default_color
          defaultBtnBgColor: Constants.right_panel_slice_button_default_color
          hoveredBtnBgColor: Constants.right_panel_slice_button_disable_color
          btnBorderW: 0
          onSigButtonClicked: {
            saveGCode()
          }
        }

        BasicButton {
          id: uploadBtn
          width: 136 * screenScaleFactor
          height: 28 * screenScaleFactor
          btnRadius: 3
          text: qsTr("Upload to Crealitycloud")
          enabled: true
          defaultBtnBgColor: Constants.right_panel_button_default_color
          hoveredBtnBgColor: Constants.right_panel_button_hovered_color
          btnBorderW: 1
          onSigButtonClicked: {
            slicercontrol.sliceMainOp("getUploadModelName")
            //idSelectPrint.visible = true
          }
          Connections {
            target: slicePlugin
            onSigSendModelName: (modelName !== "") ? uploadDlgShow(modelName) : showUploadWarningDlg()
          }
        }

        BasicButton {
          id: wifiPrintBtn
          width: 136 * screenScaleFactor
          height: 28 * screenScaleFactor
          btnRadius: 3
          defaultBtnBgColor: Constants.right_panel_button_default_color
          hoveredBtnBgColor: Constants.right_panel_button_hovered_color
          btnBorderW: 1
          text: qsTr("LAN Printing")
          onSigButtonClicked: {
  //saveGCode()
            switchWifiPrint()
            slicercontrol.sliceMainOp("updatetmpgcode", 3)//生成预览图和gcode
          }
        }

        BasicButton {
          id: usbPrint
          width: 136 * screenScaleFactor
          height: 28 * screenScaleFactor
          btnRadius: 3
          enabled: MachineManager.printerOutputDevices.length >= 1 && MachineManager.printerOutputDevices[0].activePrinter != null
          defaultBtnBgColor: Constants.right_panel_button_default_color
          hoveredBtnBgColor: Constants.right_panel_button_hovered_color
          btnBorderW: 1
          text: qsTr("USB Printing")
          onSigButtonClicked: {
            slicercontrol.sliceMainOp("usbOnlinePrint")
          }
        }

      }
    }

    UploadGCodeDlg {
      id: idUploadGCode

      onSigFileName: {
        slicercontrol.uploadToAliyun(filename)
      }

      onSigViewMyUpload: {
        slicercontrol.viewMyUploads()
        idUploadGCode.visible = false
      }

      onSigCloudPrinting: {
        skipUrlCrealityCloud()
        //slicercontrol.cloudPrinting(id)
        idUploadGCode.visible = false
      }
    }
/*
    ComPrinterSelectDlg
    {
        id: idSelectPrint
        property var receiver
        property var slice: receiver ? receiver.getSlicePlugin() : ""
        myTableModel: receiver ? receiver.getTableModel() : ""

        onSearchWifi:
        {
            receiver.startSearchWifi()
        }
        onSendFileFunction:
        {
            receiver.sendSliceFile()
        }
        onAccept:
        {
            idSelectPrint.close()
            //slice.sliceMainOp("beginSave")
            slicercontrol.sliceMainOp("getUploadModelName")
        }
        onCancel:
        {
            idSelectPrint.close()
            //slice.sliceMainOp("unpreview")
        }
        Connections {
            target: slicePlugin
            onSigSendModelName: (modelName !== "") ? uploadDlgShow(modelName) : showUploadWarningDlg()
        }
    }
    */

    UploadMessageDlg{
        id: idUploadGcodeFailedDlg
        msgText: qsTr("Failed to upload gcode!")
        cancelBtnVisible: false
        visible: false
        onSigOkButtonClicked:
        {
            idUploadGcodeFailedDlg.close()
        }
    }

    Connections{
        target: standaloneWindow
        onCurStateChanged: if(curState === 0) endPreview()
    }

    function startPreview()
    {

    }

    function endPreview()
    {
        idSlicePreviewRight.showPlatform(true)
        slicercontrol.sliceMainOp("unpreview")
        idSlicePreview.visible=false
        idSlicePreviewSelection.timebtn.stop()
        idSlicePreviewRight.revertStructureColor()
    }
}


