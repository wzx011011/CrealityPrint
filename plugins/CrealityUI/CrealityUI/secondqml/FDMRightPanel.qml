import QtQml 2.13
import QtQuick 2.13
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import "../qml"

Rectangle {
  id:idFDMRight

  property int marginSpacing: 10 * screenScaleFactor

  //Print Report
  property string printTime: sliceReport ? sliceReport.printing_time() : ""
  property double materialLength: sliceReport ? sliceReport.material_length() : ""
  property int materialWeight: sliceReport ? sliceReport.material_weight() : ""
  property double materialMoney: sliceReport ? sliceReport.material_money() : ""
  property color backReportPanelColor: Constants.itemBackgroundColor//"#061F3B"
  //Show in Preview

  //Color show
  //    property alias colorCmb: idColorCmb
  property var cmbCurrerntText
  signal currentCmbIndexChange()
  signal showColorchanged(var type, var bShow)

  //lisugui 2020-12-9
  //tabBar
  property alias currentTabIndex  : idTabBar2.currentIndex
  signal tabIndexChanged()

  /////

  //btn
  signal saveToLocal()
  signal usbOnlinePrint()
  signal cancelPreview()
  signal changePreview(var type, var state)

  //    property var moneyType :sliceReport ? sliceReport.getMoneyType() : ""
  property var moneyType :sliceReport ? tranlateMoneyType(sliceReport.getMoneyType()) : ""

  property alias stepViewObj: idStepCodeview

  function updateUI()
  {
      if(sliceReport)
      {
          sliceReport.reportMoneyTypeChanged.disconnect(changedType)
          sliceReport.reportMoneyTypeChanged.connect(changedType)
      }
  }
  function changedType()
  {
      console.log(" sliceReport.onReportMoneyTypeChanged ====")
      moneyType = tranlateMoneyType(sliceReport.getMoneyType())
      materialMoney = sliceReport.material_money()
  }

  function showPlatform(state)
  {
      console.log("222222")
      idPrintPlatForm.checked = state
      idNozzle.checked = state
  }

  function tranlateMoneyType(type)
  {
      if(type === "RMB")return "￥"
      return type
  }

  function setHighlightRow(step)
  {
      //        idGCodeTextEdit.select(400,500)
  }

  function revertStructureColor()
  {
      idStructure.resetChecked()
  }

  Column {
    anchors.fill: parent
    anchors.margins: idFDMRight.marginSpacing
    spacing: idFDMRight.marginSpacing

    Label {
      text: qsTr("Print Report")
      color: Constants.right_panel_text_default_color
      font.pointSize: Constants.labelLargeFontPointSize
      font.family: Constants.panelFontFamily
      font.bold: true
    }

    Grid {
      columns: 2
      spacing: idFDMRight.marginSpacing

      Repeater {
        model: [
          qsTr("Printing Time") + ": " + printTime,
          qsTr("Material Length") + ": " + materialLength.toFixed(2) + " m",
          qsTr("Material Weight") + ": " + materialWeight + " G",
          qsTr("Material Cost") + ": " + materialMoney + " " + moneyType
        ]

        delegate: Label {
          verticalAlignment: Qt.AlignVCenter
          text: modelData
          color: Constants.right_panel_text_default_color
          font.pointSize: Constants.labelFontPointSize
          font.family: Constants.panelFontFamily
        }
      }
    }

    Rectangle {
      x: -idFDMRight.marginSpacing
      width: parent.width + idFDMRight.marginSpacing * 2
      height: 1
      color: Constants.right_panel_menu_split_line_color
    }

    Label {
      anchors.left: parent.anchors.left
      anchors.right: parent.anchors.right
      anchors.leftMargin: idFDMRight.marginSpacing
      anchors.rightMargin:  idFDMRight.marginSpacing

      text: qsTr("Show in Preview")
      color: Constants.right_panel_text_default_color
      font.pointSize: Constants.labelLargeFontPointSize
      font.family: Constants.panelFontFamily
      font.bold: true
    }

    Grid {
      columns: 2
      columnSpacing: 30 * screenScaleFactor

      StyleCheckBox {
        id: idPrintPlatForm
        text: qsTr("Printing Platform")
        checkedState: 2         //0 unchecked  2 checked
        height: 20 * screenScaleFactor
        onCheckedChanged: {
          if (idPrintPlatForm.checkedState == 0) {
            changePreview(0, 0)
          } else {
            changePreview(0, 1)
          }
        }
      }

      StyleCheckBox {
        id: idNozzle
        height: 20 * screenScaleFactor
        text: qsTr("Nozzle")
        checkedState: 2
        onCheckedChanged: {
          if (idNozzle.checkedState == 0) {
            changePreview(1, 0)
          } else {
            changePreview(1, 1)
          }
        }
      }
    }

    Rectangle {
      x: -idFDMRight.marginSpacing
      width: parent.width + idFDMRight.marginSpacing * 2
      height: 1
      color: Constants.right_panel_menu_split_line_color
    }

    BasicTabBar {
      id: idTabBar
      Layout.fillWidth: true
      Layout.margins: idFDMRight.marginSpacing
      height: 30 * screenScaleFactor
      spacing: idFDMRight.marginSpacing
      backgroundColor: "transparent"
      currentIndex: 0

      BasicTabButton {
        id: idColor
        text: qsTr("Color Show")
        width: 100 * screenScaleFactor
        height : 30 * screenScaleFactor
        textColor: checked || hovered ? Constants.right_panel_slice_text_default_color
                                      : Constants.right_panel_text_default_color
        buttonColor : checked || hovered ? Constants.right_panel_slice_button_default_color
                                         : Constants.right_panel_button_default_color
        buttonBorder.width: checked || hovered ? 0 : 1
        buttonBorder.color: Constants.right_panel_border_default_color
        font.pointSize: Constants.labelLargeFontPointSize
        font.family: Constants.panelFontFamily
        font.bold: true
      }

      BasicTabButton {
        id: idStructureTab
        text: qsTr("G-Code")
        width: 100 * screenScaleFactor
        height : 30 * screenScaleFactor
        textColor: checked || hovered ? Constants.right_panel_slice_text_default_color
                                      : Constants.right_panel_text_default_color
        buttonColor : checked || hovered ? Constants.right_panel_slice_button_default_color
                                         : Constants.right_panel_button_default_color
        buttonBorder.width: checked || hovered ? 0 : 1
        buttonBorder.color: Constants.right_panel_border_default_color
        font.pointSize: Constants.labelLargeFontPointSize
        font.family: Constants.panelFontFamily
        font.bold: true
      }
    }

    StackLayout {
      id: swipeView

      width: parent.width
      height: 310 * screenScaleFactor
      currentIndex: idTabBar.currentIndex

      Column {
        id: idColorShow2
        anchors.fill: parent

        BasicTabBar {
          id : idTabBar2

          spacing: 3 * screenScaleFactor
          height: 24 * screenScaleFactor + spacing * 2

          backgroundColor:Constants.itemBackgroundColor//"#535353"
          currentIndex : 1

          onCurrentIndexChanged: {
            tabIndexChanged()
          }

          Repeater {
            model: [
              qsTr("Speed"),
              qsTr("Structure"),
              qsTr("Nozzle")
            ]

            delegate: BasicTabButton {
              anchors.top: idTabBar2.bottom
              width: 72 * screenScaleFactor
              height: 24 * screenScaleFactor

              text: modelData
              buttonColor: checked || hovered ? Constants.right_panel_button_checked_color
                                              : Constants.right_panel_button_default_color
              textColor: Constants.right_panel_text_default_color
              buttonBorder.width: 1
              buttonBorder.color: Constants.right_panel_border_default_color
            }
          }
        }

        StackLayout {
          id: swipeView2

          anchors.top: idTabBar2.bottom
          anchors.bottom: idColorShow2.bottom
          anchors.left: parent.left
          anchors.right: parent.right

          currentIndex:idTabBar2.currentIndex

          SpeedColorPanel {
            id: idSpeedPanel
            color: "transparent"
          }

          StructureColorPanel {
            id : idStructure
            color: "transparent"
            onChangeShowColor: {
              showColorchanged(type, bShow)
            }
          }

          NozzleColorPanel {
            id:idNozzlePanel
            color: "transparent"
            Connections {
              target: sliceUI
              onNozzleColorChange:{
                idNozzlePanel.nozzlenum = count
              }
            }
          }
        }
      }

      CusTextEditListView {
        id :idStepCodeview
        width: parent.width
        height: parent.height
        backgroundRadius:5
        backgroundColor: "transparent"
        backgroundBorder.color: Constants.right_panel_border_default_color
      }
    }
  }
}

