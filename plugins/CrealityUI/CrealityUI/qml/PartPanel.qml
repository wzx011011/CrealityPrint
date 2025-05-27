import QtQuick 2.10
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4
import "../secondqml"

BasicDialogNoWin_V4 {
    //    width: 190
    //    height: 100
    id : idRootRect
    width: 300* screenScaleFactor
    height: 364 * screenScaleFactor
    title: qsTr("Split")
    radius: 0
    color: Constants.itemBackgroundColor
    property var control
    
    onControlChanged:
    {
        if(control)
        {
            console.log("onControlChanged!!")
            control.onPositionChanged.disconnect(onPositionChange)
            control.onPositionChanged.connect(onPositionChange)

            control.onRotateAngleChanged.disconnect(onRotateAngleChange)
            control.onRotateAngleChanged.connect(onRotateAngleChange)
        }
    }

    function onPositionChange()
    {
        console.log("onPositionChanged")
        //signalsBlocked(true)
        cloader.item.x_pos.setRealValue(control.position.x)
        cloader.item.y_pos.setRealValue(control.position.y)
        cloader.item.z_pos.setRealValue(control.position.z)
        //signalsBlocked(false)
    }

    function onRotateAngleChange()
    {
        //        console.log("onRotateAngleChanged：")
        //signalsBlocked(true)
        cloader.item.x_rotate.setRealValue(control.dir.x)
        cloader.item.y_rotate.setRealValue(control.dir.y)
        cloader.item.z_rotate.setRealValue(control.dir.z)
        //signalsBlocked(false)
    }

    
    function resetPos(){
        cloader.item.x_pos.setRealValue(control.defauletPos.x)
        cloader.item.y_pos.setRealValue(control.defauletPos.y)
        cloader.item.z_pos.setRealValue(control.defauletPos.z)
    }

    function resetRotate(){
        cloader.item.x_rotate.setRealValue(control.defauletDir.x)
        cloader.item.y_rotate.setRealValue(control.defauletDir.y)
        cloader.item.z_rotate.setRealValue(control.defauletDir.z)
    }

    function lock()
    {
        if(control)
            control.blockSignalSplitChanged(true)
    }
    function unlock()
    {
        if(control)
            control.blockSignalSplitChanged(false)
    }
    function signalsBlocked(blocked)
    {
        cloader.item.x_pos.bSignalsBlcked = blocked
        cloader.item.y_pos.bSignalsBlcked = blocked
        cloader.item.z_pos.bSignalsBlcked = blocked

        cloader.item.x_rotate.bSignalsBlcked = blocked
        cloader.item.y_rotate.bSignalsBlcked = blocked
        cloader.item.z_rotate.bSignalsBlcked = blocked
    }

    bdContentItem:Rectangle {
        property alias x_pos: x_pos
        property alias y_pos: y_pos
        property alias z_pos: z_pos
        property alias x_rotate: x_rotate
        property alias y_rotate: y_rotate
        property alias zrotate: z_rotate
        color: Constants.lpw_bgColor
        Column
        {
            anchors.top: parent.top
            anchors.topMargin: 20* screenScaleFactor
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 5* screenScaleFactor
            StyledLabel {
                color: Constants.textColor
                text: qsTr("Position:")
                font.family: Constants.labelFontFamily
            }

            Row {
                spacing: 20* screenScaleFactor
                anchors.horizontalCenter: parent.horizontalCenter
                StyledLabel {
                    color: "#FB0202"
                    width: 15* screenScaleFactor
                    anchors.verticalCenter: parent.verticalCenter
                    text: "X:"
                    font.family: Constants.labelFontFamily
                    font.pointSize: 9
                }
                LeftToolSpinBox {
                    id:x_pos
                    width: 212* screenScaleFactor
                    height: 28* screenScaleFactor
                    anchors.verticalCenter: parent.verticalCenter
                    realStepSize:5
                    realFrom:0//-9999
                    realTo:9999
                    realValue:  control ? control.position.x : 0
                    font.pointSize: 9
                    onValueEdited:{
                        if(control)
                        {
                            console.log("x_pos")
                            if(isNaN(realValue)) return
                            lock()
                            control.position.x = x_pos.realValue
                            unlock()
                        }
                    }
                }

            }
            Row
            {
                spacing: 20* screenScaleFactor
                anchors.horizontalCenter: parent.horizontalCenter
                StyledLabel {
                    color: "#00FD00"
                    text: "Y:"
                    width: 15* screenScaleFactor
                    anchors.verticalCenter: parent.verticalCenter
                    font.family: Constants.labelFontFamily
                    font.pointSize: 9
                }
                LeftToolSpinBox {
                    id:y_pos
                    width: 212* screenScaleFactor
                    height: 28* screenScaleFactor
                    anchors.verticalCenter: parent.verticalCenter
                    realStepSize:5
                    realFrom:0//-9999
                    realTo:9999
                    font.pointSize: 9
                    realValue: control ? control.position.y : 0
                    onValueEdited:{
                        if(control)
                        {
                            console.log("y_pos")
                            if(isNaN(realValue)) return
                            lock()
                            control.position.y = y_pos.realValue
                            unlock()
                        }
                    }
                }
            }
            Row
            {
                spacing: 20* screenScaleFactor
                anchors.horizontalCenter: parent.horizontalCenter
                StyledLabel {
                    color: "#008FFD"
                    text: "Z:"
                    width: 15* screenScaleFactor
                    anchors.verticalCenter: parent.verticalCenter
                    font.pointSize: 9
                    font.family: Constants.labelFontFamily
                }
                LeftToolSpinBox {
                    id:z_pos
                    width: 212* screenScaleFactor
                    height: 28* screenScaleFactor
                    anchors.verticalCenter: parent.verticalCenter
                    realStepSize:5
                    realFrom:0//-9999
                    realTo:9999
                    font.pointSize: 9
                    realValue: control ? control.position.z : 0
                    onValueEdited:{
                        if(control)
                        {
                            console.log("z_pos")
                            if(isNaN(realValue)) return
                            lock()
                            control.position.z = z_pos.realValue
                            unlock()
                        }

                    }
                }
            }

            Item{
                width: 20* screenScaleFactor
                height: 10* screenScaleFactor
            }

            StyledLabel {
                color: Constants.textColor
                text: qsTr("Rotate:")
                font.pointSize: 9
                font.family: Constants.labelFontFamily
            }

            Row {
                spacing: 20* screenScaleFactor
                anchors.horizontalCenter: parent.horizontalCenter
                StyledLabel {
                    color: "#FB0202"
                    text: "X:"
                    width: 15* screenScaleFactor
                    anchors.verticalCenter: parent.verticalCenter
                    font.pointSize: 9
                    font.family: Constants.labelFontFamily
                }
                LeftToolSpinBox {
                    id:x_rotate
                    width: 212* screenScaleFactor
                    height: 28* screenScaleFactor
                    anchors.verticalCenter: parent.verticalCenter
                    realStepSize:5
                    realFrom:-9999
                    realTo:9999
                    font.pointSize: 9
                    realValue: control ? control.dir.x : 0
                    textValidator: RegExpValidator {
                        regExp:   /^([\+ \-]?(([1-9]\d*)|(0)))([.]\d{0,2})?$/
                    }
                    onValueEdited:{
                        if(control)
                        {
                            console.log("x_rotate")
                            if(isNaN(realValue)) return
                            lock()
                            control.dir.x = x_rotate.realValue
                            unlock()
                        }
                    }
                    unitchar: qsTr("(°)")
                }

            }
            Row
            {
                spacing: 20* screenScaleFactor
                anchors.horizontalCenter: parent.horizontalCenter
                StyledLabel {
                    color: "#00FD00"
                    text: "Y:"
                    width: 15* screenScaleFactor
                    anchors.verticalCenter: parent.verticalCenter
                    font.family: Constants.labelFontFamily
                    font.pointSize: 9
                }
                LeftToolSpinBox {
                    id:y_rotate
                    width: 212* screenScaleFactor
                    height: 28* screenScaleFactor
                    anchors.verticalCenter: parent.verticalCenter
                    realStepSize:5
                    realFrom:-9999
                    realTo:9999
                    font.pointSize: 9
                    realValue:control ? control.dir.y : 0
                    textValidator: RegExpValidator {
                        regExp:   /^([\+ \-]?(([1-9]\d*)|(0)))([.]\d{0,2})?$/
                    }
                    onValueEdited:{
                        if(control)
                        {
                            console.log("y_rotate")
                            if(isNaN(realValue)) return
                            lock()
                            control.dir.y = y_rotate.realValue
                            unlock()
                        }
                    }
                    unitchar: qsTr("(°)")
                }

            }
            Row
            {
                spacing: 20* screenScaleFactor
                anchors.horizontalCenter: parent.horizontalCenter
                StyledLabel {
                    color: "#008FFD"
                    text: "Z:"
                    width: 15* screenScaleFactor
                    anchors.verticalCenter: parent.verticalCenter
                    font.family: Constants.labelFontFamily
                    font.pointSize: 9
                }
                LeftToolSpinBox {
                    id:z_rotate
                    width: 212* screenScaleFactor
                    height: 28* screenScaleFactor
                    anchors.verticalCenter: parent.verticalCenter
                    realStepSize:5
                    realFrom:-9999
                    realTo:9999
                    font.pointSize: 9
                    realValue: control ? control.dir.z : 0
                    textValidator: RegExpValidator {
                        regExp:   /^([\+ \-]?(([1-9]\d*)|(0)))([.]\d{0,2})?$/
                    }
                    onValueEdited:{
                        if(control)
                        {
                            console.log("z_rotate")
                            if(isNaN(realValue)) return
                            lock()
                            control.dir.z = z_rotate.realValue
                            unlock()
                        }
                    }
                    unitchar: qsTr("(°)")
                }
            }

            Item{
                width: 20* screenScaleFactor
                height: 15* screenScaleFactor
            }

            Row
            {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 20* screenScaleFactor
                BasicButton {
                    id: reset
                    width: 124* screenScaleFactor
                    height: 28* screenScaleFactor
                    text : qsTr("Reset")
                    btnRadius:14
                    btnBorderW:1
                    pointSize: 9
                    borderColor: Constants.lpw_BtnBorderColor
                    borderHoverColor: Constants.lpw_BtnBorderHoverColor
                    defaultBtnBgColor: Constants.lpw_BtnColor
                    hoveredBtnBgColor: Constants.lpw_BtnHoverColor
                    onSigButtonClicked: {
                        resetPos();
                        resetRotate();
                        control.undo();//
                    }

                }

                BasicButton {
                    id: slice
                    width: 124* screenScaleFactor
                    height: 28* screenScaleFactor
                    text : qsTr("Start Split")
                    btnRadius:14
                    btnBorderW:1
                    pointSize: 9
                    borderColor: Constants.lpw_BtnBorderColor
                    borderHoverColor: Constants.lpw_BtnBorderHoverColor
                    defaultBtnBgColor: Constants.lpw_BtnColor
                    hoveredBtnBgColor: Constants.lpw_BtnHoverColor
                    onSigButtonClicked: {
                        control.split();
                    }
                }
            }
        }
    }
}
