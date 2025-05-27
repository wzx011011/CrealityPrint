import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.impl 2.12
import QtQuick.Templates 2.12 as T
import QtGraphicalEffects 1.12
import "qrc:/CrealityUI"
T.Slider {
    id: control
//    stepSize: 1
//    property real realStepSize: 1.0
//    property real realFrom: 0.0
//    property real realTo: 557
//    property real valueText: 0
//    to : realTo
//    from : realFrom
//    value: valueText
//    stepSize: realStepSize
    property alias maximumValue:control.to
    property alias minimumValue:control.from

    property real realStepSize: 1.0
    property real realFrom: 0.0
    property real realTo: 557
    property real valueText: 0
    to:realTo
    from:realFrom
    value: valueText
    stepSize: realStepSize


    property bool acceptWheel: true
    property color handleBorderColor: "black"
    property color handleNormalColor: "#42BDD8"
    property color handleHoverColor: Qt.lighter(handleNormalColor)
    property color handlePressColor: Qt.darker(handleNormalColor)
    property color completeColor: "cadetblue"
    property color incompleteColor: Constants.progressBarBgColor//"powderblue"

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                           implicitHandleWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitHandleHeight + topPadding + bottomPadding)
							 
	property var mysource : "qrc:/UI/photo/rangSliderPreview.png"
	
    property var mysource_d : "qrc:/UI/photo/rangSliderPreview_d.png"

    padding: 6//horizontal? height/4: width/4;

    handle: Rectangle {
        id: idHandle
        x: control.leftPadding + (control.horizontal ? control.visualPosition * (control.availableWidth - width) : (control.availableWidth - width) / 2)
        y: control.topPadding + (control.horizontal ? (control.availableHeight - height) / 2 : control.visualPosition * (control.availableHeight - height))
        width: control.horizontal?16:12
        height: control.horizontal?12:16
        color: "transparent"
        radius: 6
		
		Image {
            width: sourceSize.width
            height: sourceSize.height
            source:  control.pressed ||control.hovered ? mysource_d : mysource
        }
		
        //color: control.pressed
        //       ?handlePressColor
        //       :control.hovered
        //         ?handleHoverColor
        //         :handleNormalColor
        //LinearGradient {            ///--[Mark]
        //            anchors.fill: parent
        //            gradient: Gradient {
        //                GradientStop { position: 1;color: control.pressed
        //                                                  ?handlePressColor
        //                                                  :control.hovered
        //                                                    ?handleHoverColor
        //                                                    :"#3AC2D7"   }
        //                GradientStop { position: 0; color: control.pressed
        //                                                   ?handlePressColor
        //                                                   :control.hovered
        //                                                     ?handleHoverColor
        //                                                     :"#9BF2FF" }
        //            }
        //            source:idHandle
        //            start: Qt.point(0, 0)
        //            end: Qt.point(idHandle.width, 0)      ///1.横向渐变
        //        }
    }

    background: Rectangle {
        id : idbackgroud
        //use control.horizontal eg：
        //control.availableHeight To remove the height of padding
        //So if the control is very high, y is calculated only according to padding, which may be pasted on it
        //use (control.availableHeight - height) / 2 to align
        x: control.leftPadding + (control.horizontal ? 0 : (control.availableWidth - width) / 2)
        y: control.topPadding + (control.horizontal ? (control.availableHeight - height) / 2 : 0)
        implicitWidth: control.horizontal ? 200 : 4
        implicitHeight: control.horizontal ? 4 : 200
        width: control.horizontal ? control.availableWidth : implicitWidth
        height: control.horizontal ? implicitHeight : control.availableHeight
        radius: 2
        color: control.incompleteColor
        scale: control.horizontal && control.mirrored ? -1 : 1


        Rectangle {
            id : idbackgroud2
            y: control.horizontal ? 0 : control.visualPosition * parent.height
            width: control.horizontal ? control.position * parent.width : parent.width
            height: control.horizontal ? parent.height : control.position * parent.height
            radius: 3
            LinearGradient {            ///--[Mark]
                        anchors.fill: parent
                        start: Qt.point(0, 0)
                        end: Qt.point(idbackgroud2.width, 0)      ///1.横向渐变
                        gradient: Gradient {
                            GradientStop { position: 1; color: "#1E9BE2" }//"white" }
                            GradientStop { position: 0; color: "#1E9BE2" }
                        }
                        source: idbackgroud2
                    }
        }
    }
    ToolTip
    {
          id: idHandleToop
          visible: control.pressed
          text: control.value.toFixed(1)
    }
}
