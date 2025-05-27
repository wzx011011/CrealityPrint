import QtQuick 2.0
import QtQuick.Controls 1.4 as T
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 2.0
//lsg
 import QtGraphicalEffects 1.12
import "qrc:/CrealityUI"
T.Slider
{
    id:control
    property real realStepSize: 1.0
    property real realFrom: 0.0
    property real realTo: 557
    property real valueText: 0


    property color grooveColor: Constants.grooveColor
    property color handleColor: Constants.handleColor
    property color handleBorderColor: Constants.handleBorderColor



    property color handleNormalColor: "#42BDD8"
    property color handleHoverColor: Qt.lighter(handleNormalColor)
    property color handlePressColor: Qt.darker(handleNormalColor)
    property color completeColor: "cadetblue"
    property color incompleteColor: Constants.progressBarBgColor//"powderblue"
	
	property var mysource : "qrc:/UI/photo/rangSliderPreview.png"
    property var mysource_d : "qrc:/UI/photo/rangSliderPreview_d.png"

    property int defaultHeight: 20

    property var sliderHeight: 4
    property var sliderHandle_Width: 10

    property var sliderHandle_Height: 20
    height: defaultHeight
    maximumValue:realTo
    minimumValue:realFrom
//    from: realFrom
    value: valueText
//    to: realTo

    stepSize: realStepSize

    style: SliderStyle {
        //保留滑块的背景凹槽
        groove:m_Slider
        //保存滑块句柄的项。可以通过控件属性访问滑块
        handle: m_Slider_Handle
    }

    Component
    {
        id: m_Slider
        Rectangle
        {
            id:idbackgroud2
            implicitHeight: sliderHeight
            radius: sliderHeight/2
            color: control.incompleteColor

            Rectangle {
                id : idbackgroud
                y: 0
                x:0
                width: __handlePos / maximumValue  * parent.width
                implicitHeight:parent.implicitHeight
                radius: 3

                LinearGradient {            ///--[Mark]
                            anchors.fill: parent
                            start: Qt.point(0, 0)
                            end: Qt.point(idbackgroud.width, 0)      ///1.横向渐变
                            gradient: Gradient {
                                GradientStop { position: 1; color: "#1E9BE2" }//"white" }
                                GradientStop { position: 0; color: "#1E9BE2" }
                            }
                            source: idbackgroud
                        }
            }
        }
    }
//    on__HandlePosChanged:
//    {
//        console.log("control.position =" + __handlePos/maximumValue)
//    }
    Component
    {
        id:m_Slider_Handle
         Rectangle {
             id :idHandle
              anchors.centerIn: parent
              color: "#1E9BE2"//"transparent"//handleColor  //control.pressed ? "red" : "blue"
              border.color: handleBorderColor
              border.width: control.pressed ? 1 : 0
              implicitWidth: 10//16
              implicitHeight: 10//12
              radius: 5
			//   Image {
			// 		width: sourceSize.width
			// 		height: sourceSize.height
			// 		source:  control.pressed ||control.hovered ? mysource_d : mysource
			// 	}
			  
              //LinearGradient {            ///--[Mark]
              //            anchors.fill: parent
              //            gradient: Gradient {
              //                GradientStop { position: 1;color: control.pressed
              //                                                  ?handlePressColor
              //                                                  :control.hovered
              //                                                    ?handleHoverColor
              //                                                    :"#3AC2D7"   }//"white" }
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
			  //
          }
    }
    ToolTip
    {
          parent: control.handle
          visible: control.pressed
          text: control.value.toFixed(1)
    }
}
