import QtQuick 2.0
import QtQuick.Layouts 1.13
import QtQuick.Controls 2.5

import "../qml"

Rectangle {
  id: laserPanelRect

  property var itemModel
  property var control
  property alias settingsModel: idSliceParameters.settingsModel
  property alias currentIndex: idListView.currentIndex
  property var selShape
  property int winWidth: 1920
  property int winHeight: 1080
  property int themeStyle: Constants.currentTheme

  Layout.fillWidth: true

  objectName: "objLaserPanel"
  color: "transparent"

  signal showInfopanle(var bShow, var imageName, var imageW, var imageH)
  signal delObjects()

  onThemeStyleChanged: {
    var oldIndex = idListView.currentIndex
        
    for(var index = 0; index < idListView.count; index++) {
      selShape = control.selectObject(index)
      if (selShape.dragType === "text") {
        selShape.txt.color= Constants.textColor
      }
    }
		
    selShape = control.selectObject(oldIndex)
  }
	
  onWinWidthChanged: {
    var oldPosX = control.origin.x
    var oldPosY = control.origin.y
    var oldIndex = idListView.currentIndex
    control.origin.x = winWidth * 100 / 2 - 450 / 3 - 60 * 3 //winWidth*100/2
        
    for(var index = 0; index < idListView.count; index++) {
      selShape = control.selectObject(index)
      idLaserShapeSettings.updateImagePos(oldPosX,oldPosY)
    }
		
    selShape = control.selectObject(oldIndex)
  }
	
  onWinHeightChanged: {
    var oldPosX = control.origin.x
    var oldPosY = control.origin.y
    var oldIndex = idListView.currentIndex
    control.origin.y = (winHeight - 48) * 100 / 2 + 450 / 3 + 60 * 3 //(winHeight-48)*100/2
		
    for(var index = 0; index < idListView.count; index++) {
      selShape = control.selectObject(index)
      idLaserShapeSettings.updateImagePos(oldPosX,oldPosY)
    }
		
    selShape = control.selectObject(oldIndex)
  }

  function checkObjState() {
    var oldIndex = idListView.currentIndex
    for(var index = 0; index < idListView.count; index++) {
      selShape = control.selectObject(index)
      if(selShape.hasError) {
        selShape = control.selectObject(oldIndex)
        return true
      }
    }

    selShape = control.selectObject(oldIndex)
    return false
  }
	
  function initImageSettings() {
    laserImage.initState()
    idLaserShapeSettings.initImagePos()
  }

  function handleCtrlSelectObject(array) {
    idListView.checkMulHandle(array)
  }

  function clearModelListIndex() {
    idListView.clearAllIndex()
  }

  function updateImageInfo() {
    if (selShape !== null && selShape.dragType === "image") {
      idImageSettings.visible = true
      showInfopanle(true, selShape.imageName, selShape.imageW, selShape.imageH)
    } else {
      showInfopanle(false, 0,0,0)
    }
  }
	
  function upperObject() {
    var zValue = selShape.z
    var oldIndex = idListView.currentIndex
    var zMax = zValue

    for(var index = 0; index < idListView.count; index++) {
      var obj = control.selectObject(index)
      if(obj.z > zMax) {
        zMax = obj.z
      }

      if(obj.z > zValue) {
        obj.z--
      }
    }

    selShape = control.selectObject(oldIndex)
    selShape.z = zMax
  }
	
  function bottomObject() {
    var zValue = selShape.z
    var oldIndex = idListView.currentIndex
    var zMin = zValue

    for(var index = 0; index < idListView.count; index++) {
      var obj = control.selectObject(index)
      if(obj.z < zMin) {
        zMin = obj.z
      }

      if(obj.z < zValue) {
        obj.z++
      }
    }

    selShape = control.selectObject(oldIndex)
    selShape.z = zMin
  }

  Column {
    spacing: 10 * screenScaleFactor
    anchors.fill: parent

    PanelComponent {
      id: idTitle
      imgUrl: "qrc:/UI/photo/materialPanel.png"
      title: qsTr("Object List")
      topLineEnabled: true

      StyleCheckBox {
        id: idcheckbox
        width: 48 * screenScaleFactor
        height: 18 * screenScaleFactor
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: 18 * screenScaleFactor
        text: qsTr("all")
        checked: false

        onCheckedChanged: {
          if(checked) {
            idListView.checkAllIndex()
          } else{
            idListView.clearAllIndex()
          }
        }
      }
    }

    Rectangle {
      id: idListViewArea
      x: (parent.width - width) / 2
      width: 258 * screenScaleFactor
      height: 100 * screenScaleFactor

      radius: 5
      border.width: 1
      border.color: Constants.right_panel_border_default_color
      color: Constants.right_panel_combobox_background_color

      LaserPlotterModelList {
        id: idListView

        anchors.fill: parent
        anchors.margins: 1

        clip: true
        focus: true
        mode: laserPanelRect.itemModel
        currentLaserPlotterIndex: currentIndex

        onCheckIndex: {
          selShape = control.selectObject(selectIndex)
          idcheckbox.checked = false
        }

        onCurrentLaserPlotterIndexChanged: {
          console.log("currentLaserPlotterIndex :" + currentLaserPlotterIndex)
          if(currentLaserPlotterIndex == -1) {
            idDeleteBtn.enabled = false
            showInfopanle(false, 0,0,0)
          } else {
            idDeleteBtn.enabled = true
          }
        }

        onCurrentItemChanged: {
          selShape = control.selectObject(idListView.currentIndex);
          idImageSettings.visible = false
          idFontSettings.visible = false
          if(selShape !== null) {
            if(selShape.dragType==="image") {
              idImageSettings.visible = true
              idFontSettings.visible = false
              showInfopanle(true, selShape.imageName, selShape.imageW, selShape.imageH)
            } else if(selShape.dragType==="text") {
              idImageSettings.visible = false
              idFontSettings.visible = true
              idfontSetting.fontText = selShape.txt.text
              showInfopanle(false, 0,0,0)
            } else {
              showInfopanle(false, 0,0,0)
            }
          }
        }

        onCheckMulIndex: {
          control.selectMulObject(idxArray)
          console.log("indexArray.length:" + indexArray.length +
                      "idListView.count:" + idListView.count)
          if(indexArray.length == idListView.count) {
            idcheckbox.checked = true
          }
        }

        onClearAllIndex: {
          //console.log("LaserPanel clearAllIndex ......");
          idImageSettings.visible=false
          idFontSettings.visible=false
          control.clearSelectAllObject()
          if(selShape) {
            //console.log("selShape---:",selShape)
            if(selShape.dragType==="image") {
              showInfopanle(false, 0,0,0)
            }
            selShape = null;
          }
          currentIndex = -1
        }
      }
    }

    CusButton {
      id: idDeleteBtn

      x: (parent.width - width) / 2
      width: 258 * screenScaleFactor
      height: 28 * screenScaleFactor

      txtContent: qsTr("Delete Object")
      txtColor: Constants.right_panel_text_default_color

      normalColor: enabled ? Constants.right_panel_button_default_color
                           : Constants.right_panel_button_disable_color
      hoveredColor: Constants.right_panel_button_hovered_color
      pressedColor: Constants.right_panel_button_checked_color

      normalBdColor: enabled ? Constants.right_panel_border_default_color
                             : Constants.right_panel_border_disable_color
      hoveredBdColor: Constants.right_panel_border_hovered_color
      pressedBdColor: Constants.right_panel_border_checked_color

      radius: 5

      shadowEnabled: false

      enabled: true
      opacity : enabeld ? 1 : 0.5

      txtPointSize: Constants.labelFontPointSize
      txtFamily: Constants.panelFontFamily

      onClicked: {
        delObjects()
      }
    }

    PanelComponent {
      id: configTitle
      imgUrl: "qrc:/UI/photo/paramPanel.png"
      title: qsTr("Configure")
      topLineEnabled: true
    }

    BasicScrollView {
      x : 3 * screenScaleFactor
      width: parent.width - x * 2
      height: parent.height - idTitle.height
                            - idListViewArea.height
                            - idDeleteBtn.height
                            - configTitle.height
                            - genLaserBTn.height
                            - parent.spacing * 3

      clip : true

      hpolicy: ScrollBar.AlwaysOff
      vpolicy: ScrollBar.AsNeeded

      Column {
        width: parent.width
        spacing: 10 * screenScaleFactor

        LaserFoldItem {
          id: idLaserShapeItem

          title: qsTr("Parameter")
          width: parent.width

          accordionContent: Item {
            width: parent.width
            height: 210 * screenScaleFactor

            LaserShapeSettings{
              id: idLaserShapeSettings

              onObjectChanged: {
                control.objectChanged(obj,
                                      oldX, oldY, oldWidth, oldHeight, oldRotation,
                                      newX, newY, newWidth, newHeight, newRotation)
              }
            }
          }
        }

        LaserFoldItem {
          id:idImageSettings

          visible:false
          width: parent.width
          title: qsTr("Processing Mode")

          accordionContent: Item {
            width: parent.width
            height: 300 * screenScaleFactor

            LaserImageSettings {
              id: laserImage

              anchors.rightMargin: 0
              anchors.bottomMargin: 0
              anchors.leftMargin: 18
              anchors.topMargin: 0
              anchors.fill: parent

              onVectorSelected: {
                control.vectorImage(selShape)
              }

              onBlackSelected: {
                control.blackImage(selShape)
              }

              onGraySelected: {
                control.grayImage(selShape)
              }

              onDitheringImage: {
                control.DitheringImage(selShape)
              }

              onThresholdChanged: function(value) {
                if(selShape) {
                  //selShape.threshold=value
                  control.imageThresholdChanged(selShape,value)
                }
              }

              onOriginalImageShow: {
                control.imageOriginalShowChanged(selShape,value)
              }

              onM4modeChanged: {
                control.m4modeChanged(selShape,value)
              }

              onReverseImage: {
                control.imageReverseChanged(selShape,value)
              }

              onContrastValueChanged: {
                control.imageContrastChanged(selShape,value)
              }

              onBrightnessValueChanged: {
                control.imageBrightnessChanged(selShape,value)
              }

              onWhiteValueChanged: {
                control.imageWhiteValueChanged(selShape,value)
              }

              onFlipModelValueChanged: {
                control.imageFlipModelValueChanged(selShape,value)
              }
            }
          }
        }

        LaserFoldItem {
          id: idFontSettings

          visible:false
          width: parent.width
          title: qsTr("Word")

          accordionContent: Item {
            width: parent.width
            height: 130 * screenScaleFactor

            LaserFontSettings {
              id: idfontSetting

              anchors.rightMargin: 0
              anchors.bottomMargin: 0
              anchors.leftMargin: 18
              anchors.topMargin: 0
              anchors.fill: parent

              onFontFamilyChanged: {
                selShape.txt.font.family = fontFamily
                selShape.txt.font.styleName = fontStyle
                selShape.txt.font.pointSize = fontSize
              }

              onFontTextChanged: {
                selShape.txt.text = fontText
              }

              onFontSizeChanged: {
                selShape.txt.font.family = fontFamily
                selShape.txt.font.styleName = fontStyle
                selShape.txt.font.pointSize = fontSize
              }

              onFontStyleChanged: {
                selShape.txt.font.family = fontFamily
                selShape.txt.font.styleName = fontStyle
                selShape.txt.font.pointSize = fontSize
              }
            }
          }
        }

        LaserFoldItem {
          width: parent.width
          title: qsTr("Working Parameters")

          accordionContent: Item {
            id: idSliceParameters

            property var settingsModel

            width: parent.width
            height: 120 * screenScaleFactor

            Column {
              spacing: 5

              Rectangle {
                width: parent.width
                height: 10 * screenScaleFactor
                color: "transparent"
              }

              Row {
                spacing:5
                //visible: false

                Rectangle{
                  width: 13 * screenScaleFactor
                  height: parent.height
                  color: "transparent"
                }

                Label {
                  width: 110 * screenScaleFactor
                  height: 28 * screenScaleFactor
                  font.pointSize: Constants.labelFontPointSize
                  font.family: Constants.labelFontFamily
                  text: qsTr("Run Count")
                  color: Constants.textColor
                  verticalAlignment: Qt.AlignVCenter
                  horizontalAlignment: Qt.AlignLeft
                }

                BasicDialogTextField {
                  width: 120 * screenScaleFactor
                  height: 28 * screenScaleFactor
                  horizontalAlignment: Text.AlignLeft
                  text: settingsModel.laserTotalNum
                  onTextChanged: {
                    settingsModel.laserTotalNum = parseInt(text);
                  }
                }
              }

              Row {
                spacing:5

                Rectangle {
                  width: 13* screenScaleFactor
                  height: parent.height
                  color: "transparent"
                }

                Label {
                  width: 110* screenScaleFactor
                  height: 28* screenScaleFactor
                  verticalAlignment: Qt.AlignVCenter
                  horizontalAlignment: Qt.AlignLeft

                  font.pointSize:Constants.labelFontPointSize
                  font.family: Constants.labelFontFamily
                  text:qsTr("Power Rate")
                  color: Constants.textColor
                }

                BasicDialogTextField {
                  width: 120 * screenScaleFactor
                  height: 28 * screenScaleFactor
                  horizontalAlignment: Text.AlignLeft

                  unitChar:"%"
                  text: settingsModel.laserPowerRate

                  onTextChanged: {
                    if(parseInt(text) > 100) {
                      text = 100
                    }
                    settingsModel.laserPowerRate = parseInt(text);
                  }
                }
              }

              Row {
                  spacing:5
                  visible: false

                  Rectangle{
                    width: 13 * screenScaleFactor
                    height: parent.height
                    color: "transparent"
                  }

                  Label {
                    width: 110* screenScaleFactor
                    height: 28* screenScaleFactor
                    font.pointSize:Constants.labelFontPointSize
                    font.family: Constants.labelFontFamily

                    text: qsTr("Speed Rate")
                    color: Constants.textColor
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignLeft
                  }

                  BasicDialogTextField {
                    width: 120 * screenScaleFactor
                    height: 28 * screenScaleFactor
                    horizontalAlignment: Text.AlignLeft

                    unitChar: "%"
                    text: settingsModel.laserSpeedRate
                    onTextChanged: {
                      settingsModel.laserSpeedRate = parseInt(text);
                    }
                  }

              }

              Row {
                spacing:5
                visible: false

                Rectangle {
                  width: 13 * screenScaleFactor
                  height: parent.height
                  color: "transparent"
                }

                Label {
                  width: 110* screenScaleFactor
                  height: 28* screenScaleFactor
                  font.pointSize: Constants.labelFontPointSize
                  font.family: Constants.labelFontFamily

                  text:qsTr("Travel Speed")
                  color: Constants.textColor
                  verticalAlignment: Qt.AlignVCenter
                  horizontalAlignment: Qt.AlignLeft
                }

                BasicDialogTextField {
                  width: 120* screenScaleFactor
                  height: 28* screenScaleFactor
                  horizontalAlignment: Text.AlignLeft

                  unitChar: "mm/min"
                  text: settingsModel.laserJogSpeed
                  onTextChanged: {
                    settingsModel.laserJogSpeed = parseInt(text);
                  }
                }
              }

              Row {
                spacing:5
                //visible: false

                Rectangle {
                  width: 13 * screenScaleFactor
                  height: parent.height
                  color: "transparent"
                }

                Label {
                  width: 110* screenScaleFactor
                  height: 28* screenScaleFactor
                  verticalAlignment: Qt.AlignVCenter
                  horizontalAlignment: Qt.AlignLeft

                  font.pointSize: Constants.labelFontPointSize
                  font.family: Constants.labelFontFamily
                  text:qsTr("Work Speed")
                  color: Constants.textColor
                }

                BasicDialogTextField {
                  width: 120* screenScaleFactor
                  height: 28* screenScaleFactor
                  horizontalAlignment: Text.AlignLeft

                  unitChar:"mm/min"
                  text: settingsModel.laserWorkSpeed

                  onTextChanged: {
                    if(parseInt(text) > 6000) {
                      text = 6000
                    }
                    settingsModel.laserWorkSpeed = parseInt(text);
                  }
                }
              }
            }
          }
        }
      }
    }

    Rectangle {
      width: 278 * screenScaleFactor
      height: 1
      color: Constants.right_panel_menu_split_line_color
    }

    CusButton {
      id: genLaserBTn

      x: (parent.width - width) / 2
      width: 258 * screenScaleFactor
      height: 48 * screenScaleFactor

      txtContent: qsTr("Generate GCode")
      txtColor: Constants.right_panel_slice_text_default_color
      normalColor: enabled ? Constants.right_panel_slice_button_default_color
                           : Constants.right_panel_slice_button_disable_color
      hoveredColor: Constants.right_panel_slice_button_hovered_color
      pressedColor: Constants.right_panel_slice_button_checked_color
      radius: 5
      shadowEnabled: false
      enabled: true
      opacity : enabeld ? 1 : 0.5
      txtPointSize: Constants.labelLargeFontPointSize
      txtFamily: Constants.panelFontFamily

      onClicked: {
        if(checkObjState()) {
          idOutPlatform.visible = true
        } else {
          control.genLaserGcode()
        }
      }
    }
  }

  UploadMessageDlg {
    id: idOutPlatform

    visible: false
    cancelBtnVisible: false
    messageType: 0
    msgText: qsTr("Model out of range, please put 'red' model in the printer!")

    onSigOkButtonClicked: {
      idOutPlatform.visible = false
    }
  }
}
