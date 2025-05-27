import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Controls 2.0 as QQC2
import "qrc:/CrealityUI"
ComboBox {
    id: control
    //像源码一样，定义一个全局的样式，然后取全局样式中对应的颜色
    //checked选中状态，down按下状态，hovered悬停状态
    property color textColor: Constants.textColor          //文字颜色
    property color backgroundTheme: Constants.dialogItemRectBgColor//"#FFFFFF"
    property color backgroundColor: control.down
                                    ? backgroundTheme
                                    : control.hovered
                                      ? Qt.lighter(backgroundTheme)
                                      : backgroundTheme
    property color itemHighlightColor:Constants.cmbPopupColor_pressed//"#42BDD8"
    property color itemNormalColor: Constants.cmbPopupColor//"#E3EBEE"
//    property color indicatorColor: "#383C3E"     //勾选的指示器的颜色
    property  color  itemBorderColor: Constants.dialogItemRectBgBorderColor
    property int cmbRadius: 0
    property int showCount: 5              //最多显示的item个数
    //
    property int cmbHeight: 28
    property int popHeight: 25
    property var mysource : Constants.areaCodeComboboxImg
    property var headImageSrc: ""
    property var popPadding: 20
    property var contentPadding: 10
    property alias contentFontPointSize: idContent.font.pointSize
    property var leftItemWidth: popRectWidth/5
	
	property var strToolTip: ""
	property var popRectWidth: control.width
    property var borderWidth: 1
	property var keyStr: ""
	signal comboBoxIndexChanged(var key, var value)
	
	QQC2.ToolTip {
        id: tipCtrl
        visible: hovered&&strToolTip ? true : false
        //timeout: 2000
		delay: 100
		width: 400
		
		background: Rectangle {
            anchors.fill: parent
            color: "transparent"
        }
		
		contentItem: QQC2.TextArea{
			text: strToolTip
			width: parent.width
			wrapMode: TextEdit.WordWrap
			color: Constants.textColor
			font.family: Constants.labelFontFamily
			font.weight: Constants.labelFontWeight
			font.pixelSize: 14
			readOnly: true
			background: Rectangle
			{
				anchors.fill : parent
				color: Constants.tooltipBgColor
				border.width:1
				//border.color:hovered ? "lightblue" : "#bdbebf"
			}
        }
    }

    function findTranslate(key)/*by TCJ*/
    {

        var tranlateValue = ""
        if(key === "high")tranlateValue = qsTr("Dynamic")
        else if(key === "middle") tranlateValue= qsTr("Balanced")
        else if(key === "low")tranlateValue = qsTr("Speedy")
        else if(key === "fast")tranlateValue = qsTr("Very Speedy")
        else
        {
            tranlateValue = key
        }

        return tranlateValue
    }

    font.family: Constants.labelFontFamily
    width : 220
    height: cmbHeight
    opacity: enabled?1 : 0.7
    delegate: ItemDelegate
    {
        width: popRectWidth//control.width
        height : popHeight
        contentItem: Rectangle
        {
            anchors.fill: parent
            Item
            {
                Text {
                    id:myText
                    x:10//popPadding
                    height: popHeight
                    width: leftItemWidth
                    text: control.textRole
                        ? (Array.isArray(control.model)
                            ? key[control.textRole]
                            : model[control.textRole])
                        : findTranslate(key)/**/
                    color: "#333333"
                    font: control.font
                    elide: Text.ElideRight
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Qt.AlignLeft
                }

                Text {
                    id:myText2
                    height: popHeight
                    width: popRectWidth-myText.width-25
                    text: control.textRole
                        ? (Array.isArray(control.model)
                            ? modelData[control.textRole]
                            : model[control.textRole])
                        : findTranslate(modelData)/**/
                    color: "#333333"
                    font: control.font
                    elide: Text.ElideRight
                    anchors.left: myText.right
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Qt.AlignRight
                }

            }

            color: (control.highlightedIndex === index)
                   ? control.itemHighlightColor
                   : control.itemNormalColor


        }
        hoverEnabled: control.hoverEnabled

    }
    indicator:
    Rectangle
    {
        y:1
        anchors.right: control.right
        anchors.rightMargin: 1
        width: control.height - 1
        height: control.height - 2
        radius: cmbRadius
        anchors.verticalCenter: control.verticalCenter
        color: "transparent"//control.pressed ? Constants.cmbIndicatorRectColor_pressed_basic : Constants.cmbIndicatorRectColor_basic
        Image {
            width: 7
            height: 5
            anchors.centerIn: parent
            source: mysource //"qrc:/qt-project.org/imports/QtQuick/Controls.2/images/double-arrow.png"
            opacity: enabled ? 1 : 0.3

        visible: control.width > 20 ? true : false
    }
   }
   contentItem: Item {
       Image {
        id : headImage
        x:6
        y:(control.height - headImage.height)/2
        height:sourceSize.height
        width: sourceSize.width
        visible: headImageSrc=="" ? false : true
        source: headImageSrc
        // anchors.left: control.left
        // anchors.leftMargin: 20//contentPadding
        // anchors.top: control.top
        // anchors.topMargin: (control.height - headImage.height)/2
        }
   
    Text {
        id:idContent
        anchors.left: headImage.right
        anchors.leftMargin: 10//contentPadding
        anchors.top: headImage.top
        anchors.right: control.indicator.left
        anchors.rightMargin: control.indicator.width + control.spacing
        height:headImage.height
        text: findTranslate(control.displayText)/**/
        font: control.font
        color: control.pressed ? "#17a81a" : textColor
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

   }
   
   
   background: Rectangle {
       border.color: control.pressed ? itemNormalColor :itemBorderColor
       border.width: control.visualFocus ? 2 : borderWidth
       color: control.backgroundColor
       radius: cmbRadius
   }
    //弹出
     popup: Popup {
         y: control.height
         width: popRectWidth//control.width
//         implicitHeight: (control.delegateModel.count<showCount
//                          ?contentItem.implicitHeight
//                          :5*control.height)

         implicitHeight: control.delegateModel ? (control.delegateModel.count<showCount
                                   ?contentItem.implicitHeight
                                   :showCount*popHeight) : popHeight
//         padding: 1

         contentItem:
          Rectangle
          {
             anchors.fill: parent
    //            color: "red"//Constants.cmbPopupColor
             ListView {
             id: listview
             clip: true
             width: popRectWidth//control.width
             implicitHeight: contentHeight > showCount*popHeight ? showCount*popHeight : contentHeight
             model: control.popup.visible ? control.delegateModel : null
             currentIndex: control.highlightedIndex
             snapMode: ListView.SnapToItem
             ScrollBar.vertical: ScrollBar {
             id :box_bar
              implicitWidth: 10
             visible: control.delegateModel ? (control.delegateModel.count > showCount) : false
             contentItem:
                 Rectangle
                    {
                     implicitWidth:10
                     radius: width/2
                     color: box_bar.pressed
                            ? Qt.rgba(0.6,0.6,0.6)
                            : Qt.rgba(0.6,0.6,0.6,0.5)
                    }
                 }
             }

          }
         background: Rectangle {
             border.color: itemBorderColor
             radius: 0
         }
     }
	 
	 onCurrentIndexChanged:
	 {
		comboBoxIndexChanged(keyStr, currentIndex)
	 }
}

