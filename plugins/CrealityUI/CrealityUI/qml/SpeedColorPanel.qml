import QtQuick 2.0
import QtQuick.Controls 2.0

Rectangle {
    id : idRect

    radius: 5
    border.width: 1
    border.color: Constants.right_panel_border_default_color

    StyledLabel {
        id: idSpeedLabel
        y:10
        anchors.left: parent.left
        anchors.leftMargin: 20
        color: Constants.right_panel_text_default_color
        //anchors.verticalCenter: idColorRect.verticalCenter
        text: qsTr("Speed (mm/s)")
    }

    ListView {
        id:gridiew
        height: colormodel.count * 20
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.top: idSpeedLabel.bottom
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
                        text: modelTextData
                        font.family: Constants.labelFontFamily
                        font.weight: Constants.labelFontWeight
                        font.pointSize: Constants.labelFontPointSize
                        color: Constants.right_panel_text_default_color

                    }
            }

            onClicked:
            {

            }
        }
    }

    ListModel
    {
        id:colormodel
        ListElement{modelColorData:"#FF0099";modelTextData : "500.00";}
        ListElement{modelColorData:"#FF66B3";modelTextData : "400.00";}
        ListElement{modelColorData:"#996205";modelTextData : "300.00";}
        ListElement{modelColorData:"#FF9900";modelTextData : "200.00";}
        ListElement{modelColorData:"#FFC86B";modelTextData : "150.00";}
        ListElement{modelColorData:"#006E99";modelTextData : "100.00";}
        ListElement{modelColorData:"#00CCFF";modelTextData : "70.00";}
        ListElement{modelColorData:"#94E1FF";modelTextData : "50.00";}
        ListElement{modelColorData:"#339933";modelTextData : "30.00";}
        ListElement{modelColorData:"#609B55";modelTextData : "10.00";}
    }
}
