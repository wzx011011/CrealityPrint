import QtQuick 2.0
import QtQuick.Controls 2.0

Rectangle {
    id : idRect

    radius: 5
    border.width: 1
    border.color: Constants.right_panel_border_default_color

    signal changeShowColor(var type, var bShow)
    property bool isReset: false

    function resetChecked() {
      isReset = true
      isReset = false
    }

      StyledLabel {
          id: idTypeLabel
          y:10
          width: 90
          anchors.left: parent.left
          anchors.leftMargin: 20

          text: qsTr("Structure Type")
          font.family: Constants.labelFontFamily
          font.weight: Constants.labelFontWeight
          font.pointSize: Constants.labelFontPointSize
          color: Constants.right_panel_text_default_color
      }

      ListView {
          id:gridview
          height: colormodel.count * 20 * screenScaleFactor + 20 * screenScaleFactor
          anchors.left: parent.left
          anchors.leftMargin: 20
          anchors.top: idTypeLabel.bottom
          anchors.topMargin:  10
          model : colormodel
          delegate: idViewDelegate
          focus:true
      }

      Component
      {
          id: idViewDelegate

          Button {
              id: propertyButton
              width: parent.width
              height: 20 * screenScaleFactor
              contentItem:
                  Rectangle {
                      id: childItem
                      anchors.fill: parent
                      color: "transparent"
                      Rectangle
                      {
                          id:idColorRect
                          width: 20* screenScaleFactor
                          height: 20* screenScaleFactor
                          color : modelColorData
                          border.width: 1
                    //      border.color: propertyButton.hovered ? Constants.hoveredColor :propertyButton.checked ? Constants.selectionColor  :Constants.itemBackgroundColor
                      }

          Rectangle
                      {
                          id:idColorTextRect
                          width: 80* screenScaleFactor
                          height: 20* screenScaleFactor
            anchors.left: idColorRect.right
            anchors.leftMargin: 5
            anchors.verticalCenter: idColorRect.verticalCenter
            color: "transparent"
            Label {
              id: idColorText
              anchors.verticalCenter:idColorTextRect.verticalCenter
              color: Constants.textColor//enabled ? Constants.textColor : Constants.disabledTextColor
              text: modelTextData
              font.family: Constants.labelFontFamily
              font.weight: Constants.labelFontWeight
              font.pointSize: Constants.labelFontPointSize
            }

                      }

          StyleCheckBox
          {
            id : idChecked
            anchors.left: idColorTextRect.right
                          anchors.leftMargin: 80 * screenScaleFactor
            anchors.top: idColorTextRect.top
            checked : modelChecked
            onCheckedChanged: {
              changeShowColor(modelColorKey, idChecked.checked)
            }

            Connections {
              target: idRect
              onCheckStateChanged: {
                if(isReset)
                {
                  idChecked.checked = modelChecked
                }
                else
                {
                  idChecked.checked = idRect.checkState
                }
              }
            }
          }
              }
              background: Rectangle
              {
                  anchors.fill:parent
                  color:"transparent"
              }
              onClicked:
              {
                  console.log(idColorText.text + " :" + gridview.currentIndex)
              }
          }
      }

      ListModel
      {
          id:colormodel
          ListElement{modelColorData:"#772D28";modelTextData : qsTr("Outer Perimeter");modelColorKey:"1";modelChecked:true;}
          ListElement{modelColorData:"#028C05";modelTextData : qsTr("Inner Perimeter");modelColorKey:"2";modelChecked:true;}
          ListElement{modelColorData:"#FFB27F";modelTextData : qsTr("Skin");modelColorKey:"3";modelChecked:true;}
          ListElement{modelColorData:"#058C8C";modelTextData : qsTr("Support");modelColorKey:"4";modelChecked:true;}
          ListElement{modelColorData:"#511E54";modelTextData : qsTr("SkirtBrim");modelColorKey:"5";modelChecked:true;}
          ListElement{modelColorData:"#E5DB33";modelTextData : qsTr("Infill");modelColorKey:"6";modelChecked:true;}
          ListElement{modelColorData:"#B5BC38";modelTextData : qsTr("SupportInfill");modelColorKey:"7";modelChecked:true;}
          //ListElement{modelColorData:"#FF6B44";modelTextData : qsTr("MoveCombing");modelColorKey:"8";modelChecked:true;}
          //ListElement{modelColorData:"#66334C";modelTextData : qsTr("MoveRetraction");modelColorKey:"9";modelChecked:true;}
          ListElement{modelColorData:"#D63A11";modelTextData : qsTr("SupportInterface");modelColorKey:"10";modelChecked:true;}
          ListElement{modelColorData:"#339919";modelTextData : qsTr("PrimeTower");modelColorKey:"11";modelChecked:true;}
          ListElement{modelColorData:"#60595F";modelTextData : qsTr("Travel");modelColorKey:"13";modelChecked:false;}
          ListElement{modelColorData:"#0000FF";modelTextData : qsTr("FlowTravel");modelColorKey:"15";modelChecked:true;}
          ListElement{modelColorData:"#FF0000";modelTextData : qsTr("AdvanceTravel");modelColorKey:"16";modelChecked:true;}
      }
  }


