import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.13
ComboBox {
    id: control

    font.family: Constants.labelFontFamily
    width : 260
    height: cmbHeight
    opacity: enabled?1 : 0.7

    //像源码一样，定义一个全局的样式，然后取全局样式中对应的颜色
    //checked选中状态，down按下状态，hovered悬停状态
    property color textColor: "white" //"#333333"          //文字颜色
    property color treeItemBgHoverColor : Constants.treeItemColor_pressed
    property color treeItemBgSelectColor : "#525252" // Constants.treeItemColor_pressed
    property color treeItemBgColor : "#525252"

    property color backgroundTheme: Constants.dialogItemRectBgColor//"#FFFFFF"
    property color backgroundColor: control.down
                                    ? backgroundTheme
                                    : control.hovered
                                      ? Qt.lighter(backgroundTheme)
                                      : backgroundTheme
    property color itemHighlightColor: "#3f3f44"//Constants.cmbPopupColor_pressed//"#42BDD8"
    property color itemNormalColor: "#262629"
    //    property color indicatorColor: "#383C3E"     //勾选的指示器的颜色
    property  color  itemBorderColor: "#009cff"//Constants.rectBorderColor
    //    property int cmbRadius: 3
    property int showCount: 5              //最多显示的item个数
    //
    property int cmbHeight: 28
    property int popHeight: 28
    property var mysource : "qrc:/Photo/downBtn.png"
    property var mysource_d : "qrc:/Photo/downBtn.png"
    property var popPadding: 20
    property var contentPadding: 10
    property alias contentFontPixelsize: idContent.font.pixelSize

    signal currentContentChanged(var ctext)
    signal addMaterialClick()
    signal delModelData(var dataName)
    delegate: ItemDelegate
    {
        width: control.width
        height : popHeight
        contentItem: Rectangle
        {
            anchors.fill: parent
            Text {
                id:myText
                x:5//popPadding
                height: popHeight
                text: control.textRole
                      ? (Array.isArray(control.model)
                         ? modelData[control.textRole]
                         : model[control.textRole])
                      : modelData
                color: textColor
                font: control.font
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
            }
            BasicImageButton {
                id: namea
                visible: control.highlightedIndex === model.index
                anchors.right: parent.right
                anchors.rightMargin: 20
                width: 20
                height: 20
                anchors.verticalCenter: parent.verticalCenter
                btnImgNormal:"qrc:/Photo/cmb_delete.png"
                btnImgHovered:"qrc:/Photo/cmb_delete.png"
                btnImgPressed:"qrc:/Photo/cmb_delete.png"
                btnImgNormalColor : "transparent"

                onClicked:
                {
                    console.log("BasicImageButton modelData");
                    delModelData(modelData)
                }
            }
            color: (control.highlightedIndex === index)
                   ? control.itemHighlightColor
                   : control.itemNormalColor
        }
        hoverEnabled: control.hoverEnabled
    }
    indicator:
        Item
    {
        anchors.right: control.right
        anchors.rightMargin: 1
        implicitHeight: 16
        implicitWidth: 16
        //        radius: 3
        anchors.verticalCenter: control.verticalCenter
        //        color: control.pressed ? Constants.cmbIndicatorRectColor_pressed_basic : Constants.cmbIndicatorRectColor_basic
        Image {
            width: sourceSize.width
            height: sourceSize.height
            anchors.centerIn: parent
            source:control.pressed ? mysource_d :  mysource //"qrc:/qt-project.org/imports/QtQuick/Controls.2/images/double-arrow.png"
            opacity: enabled ? 1 : 0.3
            fillMode: Image.Pad

            visible: control.width > 20 ? true : false
        }
    }
    contentItem: Text {
        id:idContent
        anchors.left: control.left
        anchors.leftMargin: 10//contentPadding
        anchors.right: control.indicator.left
        anchors.rightMargin: control.indicator.width + control.spacing
        text: control.displayText
        font: control.font
        color: textColor
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
        onTextChanged: {
            currentContentChanged(text)
        }
    }
    background: Rectangle {
        border.color: control.pressed ? "#009CFF"  :  "#6c6c70"//itemBorderColor
        border.width:   1
        color: "transparent"
        radius: 5
    }
    //    //弹出
    popup: Popup {
        y: control.height
        width: control.width
        padding: 5
        //         topPadding: 0
        implicitHeight:  control.delegateModel ? (control.delegateModel.count<showCount
                                                  ? control.delegateModel.count * popHeight + popHeight + 2 + 5
                                                  : showCount*popHeight + popHeight + 2 + 5) : popHeight + 5
        contentItem:Item{
            Column {
                width: parent.width
                spacing: 2
                ListView {
                    id: listview
                    clip: true
                    width: parent.width
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
                            //                     radius: width/2
                            color: box_bar.pressed
                                   ? Qt.rgba(0.6,0.6,0.6)
                                   : Qt.rgba(0.6,0.6,0.6,0.5)
                        }
                    }
                }

                Row{
                    spacing: 2
                    CusButton {
                        id:addBtn
                        width: 125
                        height : 24
                        radius: 5
                        txtContent: qsTr("Add")
                        enabled: true
                        onClicked:
                        {
                            control.popup.visible = false
                        }
                    }

                    CusButton {
                        id:manageBtn
                        width: 125
                        height : 24
                        radius: 5
                        txtContent: qsTr("Manage")
                        enabled: true
                        onClicked:
                        {
                            control.popup.visible = false
                        }
                    }
                }
            }
        }
        background: Rectangle {
            border.width: 1
            border.color: itemBorderColor
            color: control.itemNormalColor
            radius: 5
        }
    }
}

