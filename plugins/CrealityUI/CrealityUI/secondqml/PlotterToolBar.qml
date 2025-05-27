import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.13
import "../qml"
//ColumnLayout {
Rectangle {
    width: 58 * screenScaleFactor
    height: __col.height// 501 * screenScaleFactor
    radius: 5
    color: Constants.themeColor
    signal pickMode()
    signal ellipseMode()
    signal rectMode()
    signal imageMode()
    signal textMode()
    signal lineMode()
    Column
    {
        id : __col
        spacing: 1
        anchors.horizontalCenter: parent.horizontalCenter

        CusImglButton{
            id:idPickupBtn
            width: 54 * screenScaleFactor
            height: 54 * screenScaleFactor
            borderWidth : 0
            state : "imgTop"
            text: qsTr("Pickup")
            visible: true
            enabledIconSource: Constants.laserPickImg
            pressedIconSource:"qrc:/UI/images/laser_pick_s.png"
            highLightIconSource: "qrc:/UI/images/laser_pick_s.png"
            defaultBtnBgColor: Constants.leftBtnBgColor_normal
            hoveredBtnBgColor: Constants.leftBtnBgColor_hovered
            selectedBtnBgColor: Constants.leftBtnBgColor_selected
            onClicked: function(){
                pickMode()
                this.bottonSelected=true
                //idLineBtn.bottonSelected=false
                idRectBtn.bottonSelected=false
                idEllipseBtn.bottonSelected=false
                idFontBtn.bottonSelected=false
            }
        }
	
        CusImglButton{
            id:idImageBtn
            width: 54 * screenScaleFactor
            height: 54 * screenScaleFactor
            borderWidth : 0
            state : "imgTop"
            text: qsTr("Image")
            visible: true
            enabledIconSource:Constants.laserImageImg
            pressedIconSource:"qrc:/UI/images/laser_img_s.png"
            highLightIconSource: "qrc:/UI/images/laser_img_s.png"
            defaultBtnBgColor: Constants.leftBtnBgColor_normal
            hoveredBtnBgColor: Constants.leftBtnBgColor_hovered
            selectedBtnBgColor: Constants.leftBtnBgColor_selected
            onClicked:function(){

                imageMode()
               // this.enabled=false
                //idRect.enabled=true

            }
        }
        CusImglButton{
            id:idFontBtn
            width: 54 * screenScaleFactor
            height: 54 * screenScaleFactor
            borderWidth : 0
            state : "imgTop"
            text: qsTr("Text")
            visible: false
            enabledIconSource:Constants.laserFontImg
            pressedIconSource:"qrc:/UI/images/laser_font_s.png"
            highLightIconSource: "qrc:/UI/images/laser_font_s.png"
            defaultBtnBgColor: Constants.leftBtnBgColor_normal
            hoveredBtnBgColor: Constants.leftBtnBgColor_hovered
            selectedBtnBgColor: Constants.leftBtnBgColor_selected
            onClicked:function(){
                textMode()
                //this.enabled=false
                this.bottonSelected=true
                //idLineBtn.bottonSelected=false
                idRectBtn.bottonSelected=false
                idEllipseBtn.bottonSelected=false
                idPickupBtn.bottonSelected=false
            }
        }
/*EnableButton{
                    id:idLineBtn
                    width: 48
                    height: 48
                    text: qsTr("Line")
                    visible: true
                    enabledIconSource:"qrc:/UI/images/laser_line.png"
                    pressedIconSource:"qrc:/UI/images/laser_line_s.png"
                    disabledIconSource: "qrc:/UI/images/laser_line_s.png"
                    selectedBgColor:Constants.selectionColor
                    hoveredBgColor:Constants.hoveredColor
                    defaultBgColor:Constants.itemBackgroundColor
                    onEnabledButtonClicked: function(){
                        lineMode()
                        //this.enabled=false
                        this.bottonSelected=true
                        idRectBtn.bottonSelected=false
                        idEllipseBtn.bottonSelected=false
						idPickupBtn.bottonSelected=false
                        idFontBtn.bottonSelected=false
                    }
    }*/
        CusImglButton{
            id:idRectBtn
            width: 54 * screenScaleFactor
            height: 54 * screenScaleFactor
            borderWidth : 0
            state : "imgTop"
            text: qsTr("Rectangle")
            visible: false
            enabledIconSource:Constants.laserRectImg
            pressedIconSource:"qrc:/UI/images/laser_rect_s.png"
            highLightIconSource: "qrc:/UI/images/laser_rect_s.png"
            defaultBtnBgColor: Constants.leftBtnBgColor_normal
            hoveredBtnBgColor: Constants.leftBtnBgColor_hovered
            selectedBtnBgColor: Constants.leftBtnBgColor_selected
            onClicked: function(){
                rectMode()
                this.bottonSelected=true
                //idLineBtn.bottonSelected=false
                idEllipseBtn.bottonSelected=false
                idPickupBtn.bottonSelected=false
                idFontBtn.bottonSelected=false
            }
        }
        CusImglButton{
            id:idEllipseBtn
            width: 54 * screenScaleFactor
            height: 54 * screenScaleFactor
            borderWidth : 0
            state : "imgTop"
            text: qsTr("Circle")
            visible: false
            enabledIconSource:Constants.laserArcImg
            pressedIconSource:"qrc:/UI/images/laser_arc_s.png"
            highLightIconSource: "qrc:/UI/images/laser_arc_s.png"
            defaultBtnBgColor: Constants.leftBtnBgColor_normal
            hoveredBtnBgColor: Constants.leftBtnBgColor_hovered
            selectedBtnBgColor: Constants.leftBtnBgColor_selected
            onClicked: function(){
                ellipseMode()
                this.bottonSelected=true
                //idLineBtn.bottonSelected=false
                idRectBtn.bottonSelected=false
                idPickupBtn.bottonSelected=false
                idFontBtn.bottonSelected=false
            }
        }
    }
}
