import QtQuick 2.0
import QtQuick.Controls 2.0
import "qrc:/CrealityUI"
Item {
    property alias modelName : model_name.text
    property alias modelNameWidth : model_name.width
    property var modelXSize
    property var modelYSize
    property var modelZSize
    property var displayFormat: "{0}x{1}x{2}mm"
    property alias modelSize : model_size.text
	property var isImage : false
	property var displayFormatImage: "{0}x{1}mm"
    property alias verticesSize:model_edge.text
    property alias faceSize:model_Triangular.text
    property alias errorNormals:model_normal_error.text
    property alias errorEdges:model_edge_error.text
	property alias errorHoles:model_error_holes.text
    property alias errorIntersects:model_error_intersects.text

    property var selected: false

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
        visible: errorNormals != 0 || errorEdges != 0 || errorHoles != 0 || errorIntersects != 0
        x: 185
        y: 165

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
                visible: selected && (errorNormals != 0 || errorEdges != 0 || errorHoles != 0 || errorIntersects != 0)
            }
        }

        Row {
            id:model_r
            StyledLabel {
                  text: qsTr("Model:")
                  font.family:  Constants.labelFontFamily
                  font.pixelSize: Constants.labelFontPixelSize
                  color: Constants.infoPanelColor                 
              }
            StyledLabel {
                  id:model_name
                  text: "zhu.stl"
                  elide: Text.ElideRight
                  font.pixelSize: Constants.labelFontPixelSize
                  color: Constants.infoPanelColor//"#5C5F63"
              }
        }
        Row {
            id:model_r2

            StyledLabel {
                  text: qsTr("Bounding Box:")
                  font.pixelSize: Constants.labelFontPixelSize

                  color: Constants.infoPanelColor
              }
            StyledLabel {
                  id:model_size
                  text: {isImage ? (modelXSize && modelYSize)? displayFormatImage.replace("{0}",modelXSize.toFixed(2)).replace("{1}",modelYSize.toFixed(2)) : "" :(modelXSize && modelYSize && modelZSize )? displayFormat.replace("{0}",modelXSize.toFixed(2)).replace("{1}",modelYSize.toFixed(2)).replace("{2}",modelZSize.toFixed(2)) : ""}
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
