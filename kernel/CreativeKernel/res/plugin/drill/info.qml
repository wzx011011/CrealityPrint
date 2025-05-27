import QtQuick 2.0
import QtQuick.Controls 2.5
import CrealityUI 1.0
import "qrc:/CrealityUI"
Rectangle
{
    id:idControl
    width: 280
    height: 210/*240*/
	y: -20
    property var com
    color: Constants.itemBackgroundColor
    function execute()
    {
        console.log("execute")
        idPlatform.bottonSelected=true;
        com.startDrill()
    }
    Component.onCompleted:
    {
        //com.onSizeChanged.disconnect(execute)
        //com.onSizeChanged.connect(execute)
    }
    radius: 0

    Column
    {
        spacing: 10
		
		Rectangle
        {
            height: 30
            width: idControl.width
            color: Constants.dialogTitleColor
			radius: 0
            Rectangle
            {
                color: parent.color
                y:10
                height: 20
                width: parent.width
            }
            StyledLabel {
                color: Constants.dialogTitleTextColor
                text: qsTr("Drill")
                x:5
                anchors.verticalCenter: parent.verticalCenter
            }
        }

		Item//Rectangle
        {
            width: 220
            height: 120/*144*/
            x: 20
			Column
            {
				spacing: 10
				Row
				{
					height: 30
					width:idControl.width
					StyledLabel {
					    id:idLiftLabel
					    color: Constants.textColor
					    text: qsTr("Sharp:")
					    height: 30
					    width : 50
					    verticalAlignment: Qt.AlignVCenter
					}
		
					BasicCombobox {
						id:idpolygonType
						height : 30
						width : idControl.width - idSizeLabel.width - 40
		
						model: ListModel {
							id: modelType
							ListElement { text: qsTr("Circle");}
							ListElement { text: qsTr("Triangle");}
							ListElement { text: qsTr("Square");}
						}
						currentIndex:com ? com.polygon : 0
						onCurrentIndexChanged: {
							if(com)
							{
								com.polygon = currentIndex
							
								console.log(modelType.get(currentIndex).text)  
								console.log(currentIndex)
							}
							
						}
					}
				}
		
				Row{
					id:idSizeRow
		
					StyledLabel {
						id:idSizeLabel
						color: Constants.textColor
						text: qsTr("Size:")
						height: 30
						width : 50
						verticalAlignment: Qt.AlignVCenter
					}
					LeftToolSpinBox
					{
					id: idSize
					height : 30
					width : idControl.width - idSizeLabel.width - 40
					realStepSize:0.5
					realFrom:0
					realTo:9999
					realValue: com.size
					onValueEdited:
					{
						com.size = idSize.realValue
					}
					}
				}
		
				Row{
					id:idDepthRow
					StyledLabel {
						id:idDepthLabel
						color: Constants.textColor
						text: qsTr("Depth:")
						height: 30
						width : 50
						verticalAlignment: Qt.AlignVCenter
					}
					LeftToolSpinBox
					{
						id: idDepth
						height : 30
						width : idControl.width - idDepthLabel.width - 40
						realStepSize:1
						realFrom:0
						realTo:9999
						realValue: com.depth
						onValueEdited:
						{
							com.depth = idDepth.realValue
						}
					}
				}
			}
		}

		BasicButton {
            id : idPlatform
            width: 240/*100*/
            height: 32
            text : bottonSelected ? qsTr("Cancel Drilling") : qsTr("Start Drilling")
			btnRadius:3
			btnBorderW:0
			defaultBtnBgColor: Constants.profileBtnColor
        hoveredBtnBgColor: Constants.profileBtnHoverColor
			property var bottonSelected: true
			anchors.horizontalCenter: parent.horizontalCenter

            onSigButtonClicked:
            {
                if(!bottonSelected)
                    com.startDrill()
                else
                {
                    com.endDrill()
                }
                bottonSelected =!bottonSelected
            }
        }
    }

}
