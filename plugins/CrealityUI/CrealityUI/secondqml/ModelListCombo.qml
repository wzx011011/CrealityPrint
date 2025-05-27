import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.13

import ".."
import "../qml"

Item {
    id: mlRec
    width: 58 * screenScaleFactor
    height: 58 * screenScaleFactor
    property real sizeX
    property real sizeY
    property real sizeZ
    property var checkedModels: []
    property bool selected: mouseOptPopup.opened
    property bool isCheckAll: modelListData.itemCount() === checkedModels.length
    property alias modelNumber:  modelListView.count
    CusImglButton {
        id: mouseButton
        width: mlRec.width
        height: mlRec.height
        visible: true
        anchors.centerIn: mlRec
        borderWidth: 0
        enabledIconSource:"qrc:/UI/photo/modelLIstIcon.png"
        pressedIconSource:"qrc:/UI/photo/modelLIstIcon.png"
        defaultBtnBgColor: Constants.leftBtnBgColor_normal
        hoveredBtnBgColor: Constants.leftBtnBgColor_hovered
        selectedBtnBgColor: Constants.leftBtnBgColor_selected
        bottonSelected: mlRec.selected
        onClicked: mlRec.selected ? mouseOptPopup.close() : mouseOptPopup.open()
        text : qsTr("List")
        state: "imgTop"
    }

    Rectangle{
        width: 20
        height: 20
        radius: 10
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.rightMargin: -10
        opacity: mouseButton.hovered ? 0.7 : 1
        visible: !selected
        Text {
            id:modelNum
            text: modelListView.count
            anchors.centerIn: parent
        }
    }

    Popup {
        padding: 0
        margins: 0
        background: null
        id: mouseOptPopup
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
        x: (mouseButton.x + mouseButton.width + 10)/* * screenScaleFactor*/
        y: (mouseButton.y) //* screenScaleFactor
        Rectangle {
            visible: true
            id: mouseOptRect

            color: Constants.lpw_bgColor
            radius: 5
            width: 280 * screenScaleFactor
            height: 180 * screenScaleFactor

            CusPopViewTitle {
                id: closeBtn
                width: parent.width
                height: 24 * screenScaleFactor
                color: Constants.lpw_titleColor
                borderColor: Constants.lpw_titleColor
                closeBtnNColor: "transparent"//color
                closeBtnWidth: 8 * screenScaleFactor
                closeBtnHeight: 8 * screenScaleFactor
                anchors.top: parent.top
                anchors.right: parent.right
                radius: 5
                leftTop: true
                rightTop: true
                clickedable: false
                //closeBtnVis: false
                maxBtnVis: false
                onCloseClicked: mouseOptPopup.close()
            }

            Item{
                anchors.top: closeBtn.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                width: (mouseOptRect.width - 22* screenScaleFactor)
                height: (mouseOptRect.height - 52* screenScaleFactor)
                Column{
                    anchors.fill: parent
                    spacing: 9
                    Item{
                        width: parent.width
                        height: 35* screenScaleFactor
                        RowLayout{
                            anchors.fill: parent
                            CustomCheckBox{
                                text:qsTr("All")
                                indWidth: 14 * screenScaleFactor
                                indHeight: 14 * screenScaleFactor
                                checked: false//modelListData.itemCount() === checkedModels.length ? Qt.Checked : Qt.Unchecked
                                onCheckStateChanged:{
                                    console.log("curState==== ", checkState)

                                    checkedModels = []
                                    for(var index = 0; index < modelListData.rowCount(); ++index)
                                    {
                                        checkedModels.push(index)
                                    }
                                    if(checkState == 0)
                                        sliceUI.cancelSelect(checkedModels)
                                    else if(checkState == 2)
                                        sliceUI.selectModel(checkedModels)
                                }
                            }

                            Item{
                                Layout.fillWidth: true
                            }


                            CusButton{
                                id:addBtn
                                Layout.preferredWidth: 24* screenScaleFactor
                                Layout.preferredHeight: 18* screenScaleFactor
                                radius: 5
                                normalColor:  "transparent"
                                hoveredColor: Constants.lpw_BtnBorderHoverColor
                                pressedColor: Constants.lpw_BtnBorderHoverColor
                                borderWidth: 0
                                Image{
                                    anchors.centerIn: parent
                                    sourceSize.width: closeBtn.width - 2
                                    sourceSize.height: closeBtn.height - 2
                                    source: addBtn.isHovered ? Constants.addModel : Constants.addModel
                                }

                                onClicked: {
                                    actionCommands.getOpt("Open File").execute()
                                }
                            }

                            CusButton{
                                id:delBtn
                                Layout.preferredWidth: 24* screenScaleFactor
                                Layout.preferredHeight: 18* screenScaleFactor
                                radius: 5
                                normalColor:  "transparent"
                                hoveredColor: Constants.lpw_BtnBorderHoverColor
                                pressedColor: Constants.lpw_BtnBorderHoverColor
                                borderWidth: 0
                                Image{
                                    anchors.centerIn: parent
                                    sourceSize.width: closeBtn.width - 2
                                    sourceSize.height: closeBtn.height - 2
                                    source: delBtn.isHovered ? Constants.delModel : Constants.delModel
                                }

                                onClicked: {
                                    sliceUI.deleteModel(checkedModels)
                                }
                            }
                        }
                    }

                    Rectangle{
                        width: 258 * screenScaleFactor
                        height: 85 * screenScaleFactor
                        border.width: 1
                        border.color:Constants.lpw_BtnBorderColor
                        radius: 5
                        color:Constants.lpw_bgColor
                        anchors.right : parent.right
                        //anchors.horizontalCenter: parent.horizontalCenter
                        CusListView{
                            id: modelListView
                            width: parent.width -2
                            height: parent.height -2
                            anchors.centerIn: parent
                            clip: true
                            model: modelListData
                            delegate: listViewCom

                            Connections{
                                target: modelListView
                                onCheckOne: {
                                    checkedModels = []
                                    {
                                        checkedModels.push(modelListView.currentIndex)
                                    }
                                    console.log("====chose indexOne===", checkedModels)
                                    sliceUI.selectModel(checkedModels)
                                    //                                modelListData.choseChanged(checkedModels)
                                }
                                onCheckMul:{
                                    checkedModels = []
                                    var isChecked =false;
                                    if(idx > modelListView.mulBegin){
                                        isChecked = (modelListView.currentIndex>=modelListView.mulBegin&&modelListView.currentIndex<=idx);
                                    }else{
                                        isChecked = (currentIndex<=modelListView.mulBegin&&currentIndex>=idx);
                                    }
                                    if(isChecked/*item_delegate.checked*/)
                                    {
                                        checkedModels.push(modelListView.currentIndex)
                                    }
                                    console.log("====chose indexMutil===", checkedModels)
                                    sliceUI.selectModel(checkedModels)
                                    modelListData.choseChanged(checkedModels)
                                }
                            }
                        }
                    }

                    Text{
                        id:sizeTxt
                        font.pointSize: Constants.labelFontPointSize
                        color: Constants.themeTextColor
                        text:qsTr("Size") + ": " +sizeX.toFixed(1) + " x " + sizeY.toFixed(1) + " x " + sizeZ.toFixed(1) + " mm"
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }
    }

    Connections{
        target: infoDispatch
        onSigSendSize:{
            mlRec.sizeX = cSize.x
            mlRec.sizeY = cSize.y
            mlRec.sizeZ = cSize.z
        }
    }

    Component{
        id:listViewCom
        Rectangle{
            id:item_delegate
            //            property bool checked: fileChecked
            property bool isHovered: false
            property string normalColor: isHovered ? "#353539" : "transparent"
            width: parent.width - 2
            height: 26
            color: fileChecked ? "#009cff" : normalColor
            Image{
                id:img1
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                source: fileChecked ? "qrc:/UI/photo/right_fileLogo_d.png" : "qrc:/UI/photo/right_fileLogo.png"
            }

            Text{
                anchors.left: img1.right
                anchors.leftMargin: 7
                anchors.verticalCenter: parent.verticalCenter
                text: fileName
                color: Constants.themeTextColor
            }

            //            Image{
            //                //property bool isvisible: true
            //                id:modelVisBtn
            //                anchors.right: parent.right
            //                anchors.rightMargin: 10
            //                anchors.verticalCenter: parent.verticalCenter
            //                source: fileVisible ? "qrc:/UI/photo/modelVis_normal.png" : "qrc:/UI/photo/modelNoVis_normal.png"
            //                visible: false
            //                MouseArea{
            //                    anchors.fill: parent
            //                    onClicked: {
            //                        //modelVisBtn.isvisible = !modelVisBtn.isvisible
            //                        modelListData.visibleChanged(model.index, !fileVisible)
            //                        sliceUI.setModelVisible(model.index, !fileVisible)
            //                    }
            //                }
            //            }

            //            Image{
            //                id:img2
            //                property bool isOpen: fileVisible
            //                property bool isHovered: false
            //                property string eyeOpen: isHovered ? "qrc:/UI/photo/eye_open_d.png" : "qrc:/UI/photo/eye_open.png"
            //                property string eyeClose: isHovered ? "qrc:/UI/photo/eye_close_d.png" : "qrc:/UI/photo/eye_close.png"
            //                anchors.right: parent.right
            //                anchors.rightMargin: 10
            //                anchors.verticalCenter: parent.verticalCenter
            //                source: isOpen ? eyeOpen : eyeClose
            //                MouseArea{
            //                    hoverEnabled: true
            //                    anchors.fill: parent
            //                    onClicked: {
            //                        img2.isOpen = !img2.isOpen
            //                        //HalotContext.obj("ModelListController").modelHide(index, img2.isOpen)
            //                    }

            //                    onEntered: {
            //                        img2.isHovered = true
            //                    }

            //                    onExited: {
            //                        img2.isHovered = false
            //                    }
            //                }
            //            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                propagateComposedEvents: true
                onEntered: {
                    parent.isHovered = true
                }
                onExited: {
                    parent.isHovered = false
                }
                onClicked: {
                    modelListView.currentIndex = index
                    mouse.accepted = false
                    switch(mouse.modifiers){
                    case Qt.ControlModifier:
                        item_delegate.checked
                                =!item_delegate.checked
                        console.log("Qt.ControlModifier index =",index)
                        break;
                    case Qt.ShiftModifier:
                        modelListView.checkMul(index);
                        console.log("Qt.ShiftModifier index =",index)
                        break;
                    default:
                        modelListView.checkOne(index)
                        modelListView.mulBegin = index;
                        break;
                    }
                    //console.log("onClickedonClicked =" + index)
                }
            }
        }
    }
}
