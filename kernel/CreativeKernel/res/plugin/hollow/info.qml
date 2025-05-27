import QtQuick 2.0
import QtQuick.Controls 2.5
import CrealityUI 1.0
import "qrc:/CrealityUI"
Rectangle
{
    id:idControl
    width: 220
    height: 160/*200*/
	y: -20
    property var com
    color: Constants.itemBackgroundColor
    function execute()
    {
        __dlg.show()
    }
    radius: 6
DlgTest
{
    id:__dlg
}
    Column
    {
        //x:10
        //anchors.top: idLabel.top
        //anchors.topMargin: 40
        spacing: 20
		
		Rectangle
        {
            height: 30
            width: idControl.width
            color: Constants.dialogTitleColor
			radius: 5
            Rectangle
            {
                color: parent.color
                y:10
                height: 20
                width: parent.width
            }
            StyledLabel {
                color: "#FFFFFF"
                text: qsTr("Hollow")
                x:5
                anchors.verticalCenter: parent.verticalCenter
            }
        }
		
		Item//Rectangle
        {
            width: 220
            height: 40/*80*/
            x: 15
			Column
            {
				spacing: 10
				Row
				{
					height: 30
					width:idControl.width
					StyledLabel {
						id:idLiftLabel
						color: "#FFFFFF"
						text: qsTr("Thickness:")
						height: 30
						verticalAlignment: Qt.AlignVCenter
					}
					StyledSpinBox
					{
					id: idThinkness
					height : 30
					width : idControl.width - idLiftLabel.width - 25
					realStepSize:0.5
					realFrom:0
					realTo:100
					realValue: com ? com.getThickness() : 1
					onValueEdited:
					{
							if(idThinkness.realValue > (com.getBoxMinSize() / 2))
							{
								idPlatform.enabled = false;
							}
							else
							{
								idPlatform.enabled = true;
							}
					}
					}
				}
				/*Row {
					id:idRanger
					visible:false
					StyledSlider
					{
						id : layeSlider
						y : 5
						width: idControl.width - 25
						realFrom : 0
						realStepSize : 1
						realTo : com.modeHeight
						grooveColor: "#9DA3A7"
						handleColor: "#3492FF"
						sliderHeight: 5
						sliderHandle_Width: 10
						sliderHandle_Height: 20
						onValueChanged:
						{
							com.preview(layeSlider.value)
						}
					}
				}*/
			}
		}

        BasicButton{
            id : idPlatform
            width: 190/*100*/
            height: 32
            anchors.horizontalCenter: parent.horizontalCenter
            text : qsTr("Hollow")
            onSigButtonClicked:
            {
                console.log("idThinkness.value =" + idThinkness.realValue)
                com.hollow(idThinkness.realValue)
            }
        }
    }





//    Row{
//        id:idT
//        Label {
//            text: "厚度"
//            color: "#FFFFFF"
//        }
//        StyledSpinBox {
//           id:idThinkness
//           realStepSize:0.5
//           realFrom:-9999
//           realTo:9999
//           realValue: 1
//           onTextPressed: {

//           }
//           onValueEdited:{


//           }
//        }
//        Label {
//            text: "mm"
//            font.pixelSize: 20
//            color: "#FFFFFF"
//        }
//    }


}
