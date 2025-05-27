import QtQuick 2.0
import QtQuick.Controls 2.0
import "qrc:/CrealityUI"
Item {
	property var isImage : false

    property var errorModel : 0
    property alias verticesSize:model_edge.text
    property alias faceSize:model_Triangular.text
    property alias errorNormals:model_normal_error.text
    property alias errorEdges:model_edge_error.text
	property alias errorHoles:model_error_holes.text
    property alias errorIntersects:model_error_intersects.text
    signal sigRepairBtnClicked()

    BasicDialogButton {
        id: repairBtn
        width: 60
        height: 28
        text: qsTr("Repair")
        btnRadius:3
        btnBorderW:0
        defaultBtnBgColor: "#1E9BE2"//Constants.profileBtnColor
        hoveredBtnBgColor: "#1EB6E2"//Constants.profileBtnHoverColor

        x: 185
        y: 140

        // anchors.right: parent.right
        // anchors.rightMargin: 15
        // anchors.bottom:  parent.bottom
        // anchors.bottomMargin: 15

        onSigButtonClicked:
        {
            sigRepairBtnClicked()
        }
    }

    //color:"red"
    //visible: modelXSize>0?true:false
    Column
    {
        spacing: 6
        Row {
            Image {
                id : logoImage
                height:sourceSize.height
                width: sourceSize.width
                source: "qrc:/UI/photo/upload_msg.png"
            }
        }

        Row {
            id:model_r
            StyledLabel {
                  text: errorModel + " " + qsTr("Model(s) Invalid")
                  font.family:  Constants.labelFontFamily
                  font.pixelSize: Constants.labelFontPixelSize
                  color: Constants.infoPanelColor                 
            }
        }

        Row {
            id:model_r3
            StyledLabel {
                  text: qsTr("Total Triangles:")
                  font.family:  Constants.labelFontFamily
                  font.pixelSize: Constants.labelFontPixelSize
                  color: Constants.infoPanelColor                 
              }
            StyledLabel {
                  id:model_Triangular
                  text: "100000"
                  elide: Text.ElideRight
                  font.pixelSize: Constants.labelFontPixelSize
                  color: Constants.infoPanelColor//"#5C5F63"
              }
        }

        Row {
            id:model_r4
            StyledLabel {
                  text: qsTr("Total Vertices:")
                  font.family:  Constants.labelFontFamily
                  font.pixelSize: Constants.labelFontPixelSize
                  color: Constants.infoPanelColor                 
              }
            StyledLabel {
                  id:model_edge
                  text: "500000"
                  elide: Text.ElideRight
                  font.pixelSize: Constants.labelFontPixelSize
                  color: Constants.infoPanelColor//"#5C5F63"
              }
        }

        Row {
            id:model_r5
            StyledLabel {
                  text: qsTr("Illegal Edge:")
                  font.family:  Constants.labelFontFamily
                  font.pixelSize: Constants.labelFontPixelSize
                  color: Constants.infoPanelColor                 
              }
            StyledLabel {
                  id:model_edge_error
                  text: "500000"
                  elide: Text.ElideRight
                  font.pixelSize: Constants.labelFontPixelSize
                  color: Constants.infoPanelColor//"#5C5F63"
              }
        }

        Row {
            id:model_r6
            StyledLabel {
                  text: qsTr("Illegal Normal:")
                  font.family:  Constants.labelFontFamily
                  font.pixelSize: Constants.labelFontPixelSize
                  color: Constants.infoPanelColor                 
              }
            StyledLabel {
                  id:model_normal_error
                  text: "500000"
                  elide: Text.ElideRight
                  font.pixelSize: Constants.labelFontPixelSize
                  color: Constants.infoPanelColor//"#5C5F63"
              }
        }
		
		Row {
            id:model_r7
            StyledLabel {
                  text: qsTr("Hole Number:")
                  font.family:  Constants.labelFontFamily
                  font.pixelSize: Constants.labelFontPixelSize
                  color: Constants.infoPanelColor                 
              }
            StyledLabel {
                  id:model_error_holes
                  text: "500000"
                  elide: Text.ElideRight
                  font.pixelSize: Constants.labelFontPixelSize
                  color: Constants.infoPanelColor//"#5C5F63"
              }
        }
		
		Row {
			visible: false
            id:model_r8
            StyledLabel {
                  text: qsTr("Self_intersections num:")
                  font.family:  Constants.labelFontFamily
                  font.pixelSize: Constants.labelFontPixelSize
                  color: Constants.infoPanelColor                 
              }
            StyledLabel {
                  id:model_error_intersects
                  text: "500000"
                  elide: Text.ElideRight
                  font.pixelSize: Constants.labelFontPixelSize
                  color: Constants.infoPanelColor//"#5C5F63"
              }
        }
    }

}
