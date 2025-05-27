import QtQuick 2.0
import QtQuick.Controls 2.1
import CrealityUI 1.0
import "qrc:/CrealityUI"
ComboBox {
    id: control
    //像源码一样，定义一个全局的样式，然后取全局样式中对应的颜色
    //checked选中状态，down按下状态，hovered悬停状态
    property color textColor: "#333333"          //文字颜色
    property color backgroundTheme: "#FFFFFF"
    property color backgroundColor: enabled ? control.down
                                    ? backgroundTheme
                                    : control.hovered
                                      ? Qt.lighter(backgroundTheme)
                                      : backgroundTheme : "#F0F0F0"
    property color itemHighlightColor:"#42BDD8"
    property color itemNormalColor: "#E3EBEE"
    property  color  itemBorderColor: "#707070"
    property int radius: 0
    property int showCount: 5              //最多显示的item个数
    //
    property int cmbHeight: 32          //显示的cmb 高度
    property int popHeight: 30          //下拉出来的item 每项高度
    property var mysource : "qrc:/UI/photo/downBtn.png"
    property var popPadding: 20
    property var contentPadding: 10
    property alias contentFontPixelsize: idContent.font.pixelSize
    font.family: Constants.labelFontFamily
    width : 220
    height: cmbHeight
    function findTranslate(key)
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
    delegate: ItemDelegate
    {
        width: control.width
        height : popHeight
        contentItem: Rectangle
        {
            anchors.fill: parent
            Text {
                id:myText
                x:popPadding
                height: popHeight
                text: control.textRole
                      ? (Array.isArray(control.model)
                         ? modelData[control.textRole]
                         : model[control.textRole])
                      :findTranslate(modelData)
                color: textColor
                font: control.font
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
            }
            color: (control.highlightedIndex === index)
                   ? control.itemHighlightColor
                   : control.itemNormalColor

            Rectangle{
                height: 1
                width: parent.width
                anchors.bottom: parent.bottom
                color: Qt.lighter(itemNormalColor)
            }
        }
        hoverEnabled: control.hoverEnabled

    }
    //指示器
    indicator: Image {
      //  x: control.mirrored ? control.padding : control.width - width - control.padding - 10
      //  y: control.topPadding + (control.availableHeight - height) / 2
        width: 14
        height: 10
        anchors.right: control.right
        anchors.verticalCenter: control.verticalCenter
        anchors.rightMargin: 5
        source: mysource //"qrc:/qt-project.org/imports/QtQuick/Controls.2/images/double-arrow.png"
        opacity: enabled ? 1 : 0.3

        visible: control.width > 20 ? true : false
    }
   contentItem: Text {
       id:idContent
        anchors.left: control.left
        anchors.leftMargin: contentPadding
        anchors.right: control.indicator.left
        anchors.rightMargin: control.indicator.width + control.spacing
        text: findTranslate(control.displayText)
        font: control.font
        color: control.pressed ? "#17a81a" : textColor
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }


   background: Rectangle {
       border.color: enabled ? control.pressed ? itemNormalColor : "#707070" :"#F0F0F0"
       border.width: control.visualFocus ? 2 : 1
       color: control.backgroundColor
   }
    //弹出
     popup: Popup {
         y: control.height
         width: control.width
//         implicitHeight: (control.delegateModel.count<showCount
//                          ?contentItem.implicitHeight
//                          :5*control.height)

         implicitHeight: control.delegateModel ? (control.delegateModel.count<showCount
                                   ?contentItem.implicitHeight
                                   :showCount*popHeight) : popHeight
         padding: 1

         contentItem: ListView {
             id: listview
             clip: true
             implicitHeight: contentHeight
             model: control.popup.visible ? control.delegateModel : null
             currentIndex: control.highlightedIndex
             snapMode: ListView.SnapToItem //按行滚动
             ScrollBar.vertical: ScrollBar {
             id :box_bar
             implicitWidth: 10
             visible: control.delegateModel ? (control.delegateModel.count > showCount) : false
                             contentItem: Rectangle{
                                 implicitWidth:10
                                 radius: width/2
                                 color: box_bar.pressed
                                        ?Qt.rgba(0.6,0.6,0.6)
                                        : Qt.rgba(0.6,0.6,0.6,0.5)
                             }
             }
         }
         background: Rectangle {
             border.color: itemBorderColor
             radius: 2
         }
     }
}

