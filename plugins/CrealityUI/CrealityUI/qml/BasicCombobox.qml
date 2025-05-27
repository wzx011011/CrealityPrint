import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Controls 2.0 as QQC2
import QtQuick.Controls 2.5
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
    property  color  itemBorderColor_H: Constants.textRectBgHoveredColor  //"#1e9be2"
    property int cmbRadius: 0
    property int showCount: 5              //最多显示的item个数
    //
    property int cmbHeight: 28
    property int popHeight: 25
    property var mysource : Constants.downBtnImg
    property var popPadding: 20
    property var contentPadding: 10
    property alias contentFontPixelsize: idContent.font.pixelSize

    property var strToolTip: ""
    property var popRectWidth: control.width
    property var keyStr: ""
    signal comboBoxIndexChanged(var key, var value)
    font.pointSize: 9
    QQC2.ToolTip {
        id: tipCtrl
        visible: hovered&&strToolTip ? true : false
        //timeout: 2000
        delay: 100
        width: 400
        implicitHeight: idTextArea.contentHeight + 40

        background: Rectangle {
            anchors.fill: parent
            color: "transparent"
        }

        contentItem: QQC2.TextArea{
            id: idTextArea
            text: strToolTip
            wrapMode: TextEdit.WordWrap
            color: Constants.textColor
            font.family: Constants.panelFontFamily
            font.weight: Constants.labelFontWeight
            font.pointSize: control.font.pointSize
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
        else if(key === "best")tranlateValue = qsTr("Best quality")
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
            Text {
                id:myText
                x:10//popPadding
                height: popHeight
                text: control.textRole
                      ? (Array.isArray(control.model)
                         ? modelData[control.textRole]
                         : model[control.textRole])
                      : findTranslate(modelData)/**/
                color: "#333333"
                font: control.font
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
            }
            color: (control.highlightedIndex === index)
                   ? control.itemHighlightColor
                   : control.itemNormalColor

            //            Rectangle{
            //                height: 1
            //                width: parent.width
            //                anchors.bottom: parent.bottom
            //                color: Qt.lighter(itemNormalColor)
            //            }
        }
        hoverEnabled: control.hoverEnabled

        /*ToolTip{
            visible: control.highlightedIndex === index
            contentItem:Image{
                source: "qrc:/UI/photo/config_quality_middle_checked.png"
            }
        }*/
    }
    indicator:
        Rectangle
    {
        y:1
        anchors.right: control.right
        anchors.rightMargin: 1
        width: control.height - 1
        height: control.height - 2
        radius: 0
        anchors.verticalCenter: control.verticalCenter
        color: control.pressed ? Constants.cmbIndicatorRectColor_pressed_basic : Constants.cmbIndicatorRectColor_basic
        Image {
            width: 7
            height: 5
            anchors.centerIn: parent
            source: mysource //"qrc:/qt-project.org/imports/QtQuick/Controls.2/images/double-arrow.png"
            opacity: enabled ? 1 : 0.3

            visible: control.width > 20 ? true : false
        }
    }
    contentItem: Text {
        id:idContent
        anchors.left: control.left
        anchors.leftMargin: 10//contentPadding
        anchors.right: control.indicator.left
        anchors.rightMargin: control.indicator.width + control.spacing
        text: findTranslate(control.displayText)/**/
        font: control.font
        color: control.pressed ? "#17a81a" : textColor
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }
    background: Rectangle {
        border.color: control.pressed ? itemNormalColor : (control.hovered ? itemBorderColor_H : itemBorderColor)
        border.width: control.visualFocus ? 2 : 1
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
            radius: 2
        }
    }

    onCurrentIndexChanged:
    {
        comboBoxIndexChanged(keyStr, currentIndex)
    }
}

