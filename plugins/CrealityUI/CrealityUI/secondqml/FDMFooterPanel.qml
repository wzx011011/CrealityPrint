import QtQuick 2.13
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.13

import ".."
import "../qml"

Rectangle {
  color: "transparent"

  property alias panelWidth: idFDMFooter.width
  property int panelHeight: 104 * screenScaleFactor

  height: panelHeight

  property color defaultBgColor: Constants.itemBackgroundColor
  property color labelColor: Constants.textColor

  property var  previewflow

  //Animation

  property bool isStart: false
  signal speedSliderChange(var value)
  //Control options
  property var cmbCurrerntText
  property var cmbCurrentIndex : idColorCmb.currentIndex

  signal currentCmbIndexChange()

  property var onlyLayerNum : idLayerNum.realValue

  property var speedFlag : 1
  property var randomstep : 0

  //Layer display range
  property alias currentLayer : layeSlider.value
  property alias currentStep : stepSlider.value
  property alias layerMax : layeSlider.maximumValue
  property alias stepMax : stepSlider.maximumValue
  signal layerSliderChange(var value)
  signal stepSliderChange(var value)
  property alias timebtn : idTimerDown

  //
  property alias onlyShowCheck: idNozzle.checked

  Rectangle {
    id:idFDMFooter

    anchors.centerIn: parent
    height: parent.height

    color: defaultBgColor
    border.color: Constants.right_panel_border_default_color
    border.width: 1
    radius: 5

    onVisibleChanged: {
      if (visible) {
        idColorCmb.displayText = idColorModel.get(idColorCmb.currentIndex).modeldata
      }
    }

    RowLayout {
      x: 20 * screenScaleFactor
      y: 10 * screenScaleFactor
      width: parent.width - x * 2
      height: parent.height - y * 2

      spacing: 20 * screenScaleFactor

      ColumnLayout {
        Layout.alignment: Qt.AlignCenter
        Layout.preferredWidth: 180 * screenScaleFactor
        Layout.fillHeight: true

        BasicDialogButton {
          Layout.alignment: Qt.AlignCenter
          Layout.fillWidth: true
          Layout.preferredHeight: 28 * screenScaleFactor

          text : isStart ? qsTr("Suspend") : qsTr("Start")
          btnTextColor: Constants.right_panel_slice_text_default_color
          defaultBtnBgColor : Constants.right_panel_slice_button_default_color
          hoveredBtnBgColor : Constants.right_panel_slice_button_hovered_color
          btnBorderW: 0
          btnRadius: 5

          onSigButtonClicked: {
            if(layeSlider.value === layeSlider.maximumValue) {
              layeSlider.value = 0;
            }

            //startOrSuspend()

            if(isStart) {
              idTimerDown.stop()
              isStart =! isStart;
              previewflow.slicePreviewSetOp("IsAnimate", 0);
            } else {
              idTimerDown.start();
              isStart =! isStart;
              previewflow.slicePreviewSetOp("IsAnimate", 1);
            }
          }
        }

        RowLayout {
          Layout.alignment: Qt.AlignCenter
          Layout.fillWidth: true
          Layout.preferredHeight: 28 * screenScaleFactor

          StyledLabel {
            Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
            text: qsTr("Printing Speed:")
            color: labelColor
          }

          StyledSlider {
            id : speedSlider

            Layout.fillWidth: true
            Layout.alignment: Qt.AlignVCenter

            sliderHeight: 2 * screenScaleFactor

            value: 50
            minimumValue: 1
            maximumValue: 150

            onValueChanged: {
              speedSliderChange(value)
            }
          }
        }
      }

      Rectangle {
        width: 1
        height: parent.height
        color: Constants.right_panel_menu_split_line_color
      }

      GridLayout {
        Layout.alignment: Qt.AlignCenter
        Layout.preferredWidth: 240 * screenScaleFactor
        Layout.fillHeight: true

        columns: 4

        Rectangle {
          width:1
          height: 1
        }

        Label {
          Layout.alignment: Qt.AlignCenter
          Layout.preferredHeight: 28 * screenScaleFactor

          text: qsTr("Preview Way")
          font.pointSize: Constants.labelFontPointSize
          font.family: Constants.labelFontFamily
          font.weight: Constants.labelFontWeight
          color: labelColor
          verticalAlignment: Text.AlignVCenter

          onTextChanged: {
            console.log("Preview Way changed !!")
            idColorCmb.displayText = idColorModel.get(idColorCmb.currentIndex).modeldata
          }
        }

        BasicCombobox {
          id: idColorCmb

          Layout.alignment: Qt.AlignCenter
          Layout.fillWidth: true
          Layout.preferredHeight: 28 * screenScaleFactor

          cmbRadius: 5
          clip: true

          //打印速度 ,结构 ,喷头
          model: ListModel {
            id: idColorModel
            ListElement { modeldata: qsTr("Each Step") }
            ListElement { modeldata: qsTr("Each Layer") }
          }

          currentIndex: 0
          onCurrentIndexChanged: {
            cmbCurrerntText = idColorModel.get(currentIndex).modeldata
            displayText = cmbCurrerntText
            currentCmbIndexChange()
          }
        }

        Rectangle {
          width:1
          height: 1
        }

        StyleCheckBox {
          id: idNozzle

          Layout.alignment: Qt.AlignCenter
          Layout.preferredHeight: 28 * screenScaleFactor

          checkedState: 0

          onCheckedChanged: {
            console.log("checkState = " + checkedState)
            if(checkedState === 0) {
              previewflow.slicePreviewCommonOp("unsetOnlyLayer")
            } else {
              console.log("onlyLayerNum = " + onlyLayerNum)
              previewflow.slicePreviewSetOp("OnlyLayer", onlyLayerNum)
            }
          }
        }

        Label {
          Layout.alignment: Qt.AlignCenter
          Layout.preferredHeight: 28 * screenScaleFactor

          text: qsTr("Only Show")
          font.pointSize: Constants.labelFontPointSize
          font.family: Constants.labelFontFamily
          font.weight: Constants.labelFontWeight
          color: labelColor
          verticalAlignment: Text.AlignVCenter
        }

        StyledSpinBox {
          id: idLayerNum

          Layout.alignment: Qt.AlignCenter
          Layout.fillWidth: true
          Layout.preferredHeight: 28 * screenScaleFactor

          radius: 5

          realFrom: 0
          realTo: currentLayer
          realStepSize: 1
          realValue: 0
          unitchar: ""

          textObj.validator:IntValidator {
            bottom: 0
            top: 99999
          }

          onValueEdited: {
            if (idNozzle.checkedState > 0) {
              console.log("edit Value =" + idLayerNum.realValue)
              previewflow.slicePreviewSetOp("OnlyLayer", idLayerNum.realValue)
            }
          }
        }

        Label {
          Layout.alignment: Qt.AlignCenter
          Layout.preferredHeight: 28 * screenScaleFactor

          text: qsTr("Layer")
          color: labelColor
          verticalAlignment: Text.AlignVCenter
          horizontalAlignment: Text.AlignHCenter
        }
      }

      Rectangle {
        width: 1
        height: parent.height
        color: Constants.right_panel_menu_split_line_color
      }

      GridLayout {
        Layout.alignment: Qt.AlignCenter
        Layout.preferredWidth: 330 * screenScaleFactor
        Layout.fillHeight: true

        columns: 3

        Label {
          Layout.alignment: Qt.AlignCenter
          Layout.preferredHeight: 28 * screenScaleFactor

          verticalAlignment: Qt.AlignVCenter
          horizontalAlignment: Qt.AlignHCenter

          text: qsTr("Steps Number:")
          font.family: Constants.labelFontFamily
          font.weight: Constants.labelFontWeight
          font.pointSize: Constants.labelFontPointSize
          color: labelColor
        }

        StyledSlider {
          id : stepSlider

          Layout.alignment: Qt.AlignCenter
          Layout.fillWidth: true
          Layout.preferredHeight: 28 * screenScaleFactor

          minimumValue :0
          sliderHeight: 2

          onValueChanged: {
            sliderStepInput.value = value
            stepSliderChange(value)
          }
        }

        StyledSpinBox {
          id: sliderStepInput

          Layout.alignment: Qt.AlignCenter
          Layout.preferredWidth: 70 * screenScaleFactor
          Layout.preferredHeight: 28 * screenScaleFactor

          radius: 5

          realStepSize: 1
          realFrom: 0
          realTo: stepSlider.maximumValue
          realValue: stepSlider.value
          value: stepSlider.value
          decimals: 0
          unitchar: ""

          textObj.validator:IntValidator {
            bottom : 0
            top : 99999
          }

          onValueEdited: {
            stepSlider.value = realValue
          }
        }

        Label {
          Layout.alignment: Qt.AlignCenter
          Layout.preferredHeight: 28 * screenScaleFactor

          verticalAlignment: Qt.AlignVCenter
          horizontalAlignment: Qt.AlignHCenter

          text: qsTr("Layers Number:")
          font.family: Constants.labelFontFamily
          font.weight: Constants.labelFontWeight
          font.pointSize: Constants.labelFontPointSize
          color: labelColor
        }

        StyledSlider {
            id : layeSlider

            Layout.alignment: Qt.AlignCenter
            Layout.fillWidth: true
            Layout.preferredHeight: 28 * screenScaleFactor

            stepSize: 1
            sliderHeight: 2
            minimumValue: 0
            maximumValue: 40

            onValueChanged: {
              layerInput.value = value
              layerSliderChange(value)
            }
        }

        StyledSpinBox {
          id: layerInput

          Layout.alignment: Qt.AlignCenter
          Layout.preferredWidth: 70 * screenScaleFactor
          Layout.preferredHeight: 28 * screenScaleFactor

          radius: 5

          realStepSize: 1
          realFrom: 0
          realTo: layeSlider.maximumValue
          realValue: layeSlider.value
          value: layeSlider.value
          decimals: 0
          unitchar: ""

          textObj.validator:IntValidator {
            bottom: 0
            top: 99999
          }

          onValueEdited: {
            layeSlider.value = value
          }
        }
      }

      Timer {
        id:idTimerDown;
        interval: speedSlider.value > 0 ? (3000 / (speedSlider.value * speedFlag)) : 3000;
        repeat: true;
        triggeredOnStart: true;

        onTriggered: {
          if (cmbCurrentIndex === 0) {
            randomstep = 1
            stepSlider.value += 1

            if (stepSlider.value >= stepSlider.maximumValue) {
              layeSlider.value += 1

              if (layeSlider.value > layeSlider.maximumValue) {
                idTimerDown.stop();
                previewflow.slicePreviewSetOp("IsAnimate", 0);
              } else if(layeSlider.value === layeSlider.maximumValue) {
              } else {
                stepSlider.value = 0
              }
            }
          } else {
            layeSlider.value += 1

            if (layeSlider.value > layeSlider.maximumValue) {
              idTimerDown.stop();
              previewflow.slicePreviewSetOp("IsAnimate", 0);
            } else if(stepSlider.value === stepSlider.maximumValue) {
            }
          }
        }
      }
    }
  }
}
