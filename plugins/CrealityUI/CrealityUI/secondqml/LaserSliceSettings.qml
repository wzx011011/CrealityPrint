import QtQuick 2.0
import QtQuick.Controls 2.5
import "../qml"
Item {
    width: 262
    signal genLaserGcode()
    //property var settingsModel
    //height: parent.height
    implicitHeight:250 - 170
    anchors.leftMargin: 23
    Column{
        spacing:2//5
		// Label {
        //     color: Constants.textColor
        //     font.family: Constants.panelFontFamily
        //     text:qsTr("Slice Settings")
        //     font.pixelSize: 16
        //     font.bold: true
		// 	width: 100
		// 	height:30
        // }
		/*
		Row{
			spacing:5
			Label {
                id:idOpenCode
                width: 90
				height: 28
                font.pixelSize: 14
                font.family: Constants.labelFontFamily
                text:qsTr("Laser Open")
                color: "#FFFFFF"
				verticalAlignment: Qt.AlignVCenter
				horizontalAlignment: Qt.AlignLeft
            }
            BasicDialogTextField {
                width: 135
                height: 28
                horizontalAlignment: Text.AlignLeft
                text: settingsModel.laserOpenGode
                onTextChanged: {
                    settingsModel.laserOpenGode = text;
                }
            }
			
		}
		
		Row{
			spacing:5
			Label {
                id:idCloseCode
                width: 90
				height: 28
                color: "#FFFFFF"
                text:qsTr("Laser Close")

                font.pixelSize: 14
                font.family: Constants.labelFontFamily
				verticalAlignment: Qt.AlignVCenter
				horizontalAlignment: Qt.AlignLeft
            }
            BasicDialogTextField {
                width: 135
                height: 28
                text: settingsModel.laserCloseGode
                onTextChanged: {
                    settingsModel.laserCloseGode = text;
                }
            }
		}
		
		Row{
			spacing:5
			Label {
                id:idEmptyRunSpeed
                width: 90
				height: 28
                color: "#FFFFFF"
                text:qsTr("Travel Speed")
                font.pixelSize: 14
                font.family: Constants.labelFontFamily
				verticalAlignment: Qt.AlignVCenter
				horizontalAlignment: Qt.AlignLeft
            }
            BasicDialogTextField {
                width: 135
                height: 28
                text: settingsModel.laserEmptyRunSpeed
				validator: RegExpValidator { regExp: /^[1-9]\d{0,2}$/ }
				unitChar:"mm/s"
                onTextChanged: {
                    settingsModel.laserEmptyRunSpeed = parseInt(text);
                }
            }
		}
		
		Row{
			spacing:5
			Label {
                id:idRunSpeed
                width: 90
				height: 28
                color: "#FFFFFF"
                text:qsTr("Run Speed")
                font.pixelSize: 14
                font.family: Constants.labelFontFamily
				verticalAlignment: Qt.AlignVCenter
				horizontalAlignment: Qt.AlignLeft
            }
            BasicDialogTextField {
                width: 135
                height: 28
                text: settingsModel.laserRunSpeed
				validator: RegExpValidator { regExp: /^[1-9]\d{0,2}$/ }
				unitChar:"mm/s"
                onTextChanged: {
                    settingsModel.laserRunSpeed = parseInt(text);
                }
            }
		}
		
		Row{
			spacing:5
			Label {
                id:idRunCount
                width: 90
				height: 28
                color: "#FFFFFF"
                text:qsTr("Run Count")
                font.pixelSize: 14
                font.family: Constants.labelFontFamily
				verticalAlignment: Qt.AlignVCenter
				horizontalAlignment: Qt.AlignLeft
            }
            BasicDialogTextField {
                width: 135
                height: 28
                text: settingsModel.laserRunCount
				validator: RegExpValidator { regExp: /^[1-9]\d{0,1}$/ }
				unitChar: qsTr("times")
                onTextChanged: {
                    settingsModel.laserRunCount = parseInt(text);
                }
            }
		}*/
		/*
        Row{
			spacing:5
			Label {
                id:idPowerMax
                width: 90
				height: 28
                font.pixelSize: 14
                font.family: Constants.labelFontFamily
                text:qsTr("最大功率")
                color: "#FFFFFF"
				verticalAlignment: Qt.AlignVCenter
				horizontalAlignment: Qt.AlignLeft
            }
            BasicDialogTextField {
                width: 135
                height: 28
                horizontalAlignment: Text.AlignLeft
                text: settingsModel.laserPowerMax
                onTextChanged: {
                    settingsModel.laserPowerMax = parseInt(text);
                }
            }
			
		}
        Row{
			spacing:5
			Label {
                id:idWorkSpeed
                width: 90
				height: 28
                font.pixelSize: 14
                font.family: Constants.labelFontFamily
                text:qsTr("工作速度")
                color: "#FFFFFF"
				verticalAlignment: Qt.AlignVCenter
				horizontalAlignment: Qt.AlignLeft
            }
            BasicDialogTextField {
                width: 135
                height: 28
                horizontalAlignment: Text.AlignLeft
                text: settingsModel.laserWorkSpeed
                onTextChanged: {
                    settingsModel.laserWorkSpeed = parseInt(text);
                }
            }
			
		}
        Row{
			spacing:5
			Label {
                id:idJogSpeed
                width: 90
				height: 28
                font.pixelSize: 14
                font.family: Constants.labelFontFamily
                text:qsTr("空驶速度")
                color: "#FFFFFF"
				verticalAlignment: Qt.AlignVCenter
				horizontalAlignment: Qt.AlignLeft
            }
            BasicDialogTextField {
                width: 135
                height: 28
                horizontalAlignment: Text.AlignLeft
                text: settingsModel.laserJogSpeed
                onTextChanged: {
                    settingsModel.laserJogSpeed = parseInt(text);
                }
            }
			
		}
        Row{
			spacing:5
			Label {
                id:idDensity
                width: 90
				height: 28
                font.pixelSize: 14
                font.family: Constants.labelFontFamily
                text:qsTr("雕刻分辨率")
                color: "#FFFFFF"
				verticalAlignment: Qt.AlignVCenter
				horizontalAlignment: Qt.AlignLeft
            }
            BasicDialogTextField {
                width: 135
                height: 28
                horizontalAlignment: Text.AlignLeft
                text: settingsModel.laserDensity
                onTextChanged: {
                    settingsModel.laserDensity = parseFloat(text);
                }
            }
			
		}*/

        /*Row{
			spacing:5
			Label {
                width: 90
				height: 28
                font.pixelSize: 14
                font.family: Constants.labelFontFamily
                text:qsTr("Gcode Style")
                color: "#FFFFFF"
				verticalAlignment: Qt.AlignVCenter
				horizontalAlignment: Qt.AlignLeft
            }
            BasicCombobox {
                width: 135
                height: 28

                model: ListModel {
                    id: gcodeStyle
                    ListElement { text: qsTr("GRBL");}
                    ListElement { text: qsTr("Marlin");}
                }
                currentIndex: settingsModel ? settingsModel.laserGcoStyle-1 : -1
                onCurrentIndexChanged: {                                          
                        settingsModel.laserGcoStyle = currentIndex+1;
                }
            }
		}*/

        // Row{
		// 	spacing:5
		// 	Label {
        //         width: 90
		// 		height: 28
        //         font.pixelSize: 14
        //         font.family: Constants.labelFontFamily
        //         text:qsTr("Run Count")
        //         color: Constants.textColor
		// 		verticalAlignment: Qt.AlignVCenter
		// 		horizontalAlignment: Qt.AlignLeft
        //     }
        //     BasicDialogTextField {
        //         width: 135
        //         height: 28
        //         horizontalAlignment: Text.AlignLeft
        //         text: settingsModel.laserTotalNum
        //         onTextChanged: {
        //             settingsModel.laserTotalNum = parseInt(text);
        //         }
        //     }			
		// }
        // Row{
		// 	spacing:5
		// 	Label {
        //         width: 90
		// 		height: 28
        //         font.pixelSize: 14
        //         font.family: Constants.labelFontFamily
        //         text:qsTr("Power Rate")
        //         color: Constants.textColor
		// 		verticalAlignment: Qt.AlignVCenter
		// 		horizontalAlignment: Qt.AlignLeft
        //     }
        //     BasicDialogTextField {
        //         width: 135
        //         height: 28
        //         horizontalAlignment: Text.AlignLeft
        //         unitChar:"%"
        //         text: settingsModel.laserPowerRate
        //         onTextChanged: {
        //             settingsModel.laserPowerRate = parseInt(text);
        //         }
        //     }
			
		// }
        // Row{
		// 	spacing:5
		// 	Label {
        //         width: 90
		// 		height: 28
        //         font.pixelSize: 14
        //         font.family: Constants.labelFontFamily
        //         text:qsTr("Speed Rate")
        //         color: Constants.textColor
		// 		verticalAlignment: Qt.AlignVCenter
		// 		horizontalAlignment: Qt.AlignLeft
        //     }
        //     BasicDialogTextField {
        //         width: 135
        //         height: 28
        //         horizontalAlignment: Text.AlignLeft
        //         unitChar:"%"
        //         text: settingsModel.laserSpeedRate
        //         onTextChanged: {
        //             settingsModel.laserSpeedRate = parseInt(text);
        //         }
        //     }
			
		// }
        // Row{
		// 	spacing:5
		// 	Label {
        //         width: 90
		// 		height: 28
        //         font.pixelSize: 14
        //         font.family: Constants.labelFontFamily               
        //         text:qsTr("Travel Speed")
        //         color: Constants.textColor
		// 		verticalAlignment: Qt.AlignVCenter
		// 		horizontalAlignment: Qt.AlignLeft
        //     }
        //     BasicDialogTextField {
        //         width: 135
        //         height: 28
        //         horizontalAlignment: Text.AlignLeft
        //         unitChar:"mm/min"
        //         text: settingsModel.laserJogSpeed
        //         onTextChanged: {
        //             settingsModel.laserJogSpeed = parseInt(text);
        //         }
        //     }
			
		// }
        // Row{
		// 	spacing:5
		// 	Label {
        //         width: 90
		// 		height: 28
        //         font.pixelSize: 14
        //         font.family: Constants.labelFontFamily                
        //         text:qsTr("Work Speed")
        //         color: Constants.textColor
		// 		verticalAlignment: Qt.AlignVCenter
		// 		horizontalAlignment: Qt.AlignLeft
        //     }
        //     BasicDialogTextField {
        //         width: 135
        //         height: 28
        //         horizontalAlignment: Text.AlignLeft
        //         unitChar:"mm/min"
        //         text: settingsModel.laserWorkSpeed
        //         onTextChanged: {
        //             settingsModel.laserWorkSpeed = parseInt(text);
        //         }
        //     }
			
		// }
		// Label {
        //     color: Constants.textColor
        //     font.family: Constants.labelFontFamily
        //     text:""
        //     font.pixelSize: 16
		// 	width: 150
		// 	height:2//20
        // }

        // Item {
        //     width: 265
        //     height : 2
            
        //     Rectangle {
        //         // anchors.left: idCol.left
        //         // anchors.leftMargin: -10
        //         x: -20
        //         width:parent.width > parent.height ?  parent.width : 2
        //         height: parent.width > parent.height ?  2 : parent.height
        //         color: Constants.splitLineColor
        //         Rectangle {
        //             width: parent.width > parent.height ? parent.width : 1
        //             height: parent.width > parent.height ? 1 : parent.height
        //             color: Constants.splitLineColorDark
        //         }
        //     }
        // }

        Item{
            width: parent.width
            height : 10
        }
		BasicButton
        {
            id: control
            width: 230
            height:40
            text: qsTr("Generate GCode")
            btnTextColor: "#FFFFFF"
			defaultBtnBgColor : "#1E9BE2"//"#3cd5ed"
            btnRadius: 3
            btnBorderW: 0
            pixSize: Constants.panelLargeFontPixelSize
            fontFamily: Constants.panelFontFamily
            onSigButtonClicked:
            {
                genLaserGcode()
            }
        }
    }
}
