import QtQuick 2.0
import QtQuick.Controls 2.0

Rectangle {
    id : idRect

    radius: 5
    border.width: 1
    border.color: Constants.right_panel_border_default_color

    property var nozzlenum : 1

    StyledLabel {
        id: idNozzleLabel
        y:10
        anchors.left: parent.left
        anchors.leftMargin: 20

        //anchors.verticalCenter: idColorRect.verticalCenter
        text: qsTr("Nozzle")
        color:  Constants.right_panel_text_default_color
    }

    ListView {
        id:gridiew
        height: nozzlenum * 20 * screenScaleFactor
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.top: idNozzleLabel.bottom
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
            width: 100 * screenScaleFactor
            height: 20 * screenScaleFactor
            contentItem:
                Rectangle {
                    id: childItem
                    anchors.fill: parent
                    color: Constants.right_panel_menu_background_color
                    Rectangle
                    {
                        id:idColorRect
                        width: 20 * screenScaleFactor
                        height: 20 * screenScaleFactor
                        color : modelColorData
                        border.width: 1
                  //      border.color: propertyButton.hovered ? Constants.hoveredColor :propertyButton.checked ? Constants.selectionColor  :Constants.itemBackgroundColor
                    }

                    Label {
                        id: idColorText
                        anchors.left: idColorRect.right
                        anchors.leftMargin: 5
                        anchors.verticalCenter: idColorRect.verticalCenter
                        color: Constants.right_panel_text_default_color
                        font.family: Constants.labelFontFamily
                        font.weight: Constants.labelFontWeight
                        text: modelTextData
                        font.pointSize: Constants.labelFontPointSize

                    }
            }
//                background: Rectangle
//                {
//                    anchors.fill:parent
//                    color:"transparent"
//                }
            onClicked:
            {
                console.log(idColorText.text + " :" + gridview.currentIndex)
            }
        }
    }

    ListModel
    {
        id:colormodel
        ListElement{modelColorData:"#FF8080";modelTextData : qsTr("Nozzle 1");}
        ListElement{modelColorData:"#80FF80";modelTextData : qsTr("Nozzle 2");}
  ListElement{modelColorData:"#8080FF";modelTextData : qsTr("Nozzle 3");}
  ListElement{modelColorData:"#FFFF80";modelTextData : qsTr("Nozzle 4");}
  ListElement{modelColorData:"#FF80FF";modelTextData : qsTr("Nozzle 5");}
  ListElement{modelColorData:"#80FFFF";modelTextData : qsTr("Nozzle 6");}
    }
}
 

