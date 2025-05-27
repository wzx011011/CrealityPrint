import QtQuick 2.10
import QtQuick.Controls 2.0
//import ".."
import "../secondqml"

BasicDialogNoWin_V4 {
    id: control
    width: 300* screenScaleFactor
    height: 213* screenScaleFactor
    title:  qsTr("Move")
    visible: mtranslate.message
    //    border.width: 1

    property var mtranslate
    property int labelWidth: 15* screenScaleFactor

    function lock()
    {
        if(mtranslate)
            mtranslate.blockSignalMoveChanged(true)
    }

    function unlock()
    {
        if(mtranslate)
            mtranslate.blockSignalMoveChanged(false)
    }

    function signalsBlocked(blocked)
    {
        cloader.item.x_pos.bSignalsBlcked = blocked
        cloader.item.y_pos.bSignalsBlcked = blocked
        cloader.item.z_pos.bSignalsBlcked = blocked

        console.log("4444444444444= ", cloader.item)
    }

    function onPositionChange(){
        if(!mtranslate)return
        //console.log("onPositionChange aaaaa")
        signalsBlocked(true)

        cloader.item.x_pos.setRealValue(mtranslate.position.x)
        cloader.item.y_pos.setRealValue(mtranslate.position.y)
        cloader.item.z_pos.setRealValue(mtranslate.position.z)
        //        x_pos.realValue = mtranslate.position.x
        //        y_pos.realValue = mtranslate.position.y
        //        z_pos.realValue = mtranslate.position.z
        signalsBlocked(false)
    }

    onMtranslateChanged: {
        if(mtranslate)
        {
            lock()
            signalsBlocked(true)
            cloader.item.x_pos.setRealValue(mtranslate.position.x)
            cloader.item.y_pos.setRealValue(mtranslate.position.y)
            cloader.item.z_pos.setRealValue(mtranslate.position.z)
            signalsBlocked(false)
            unlock()
            mtranslate.positionChanged.disconnect(onPositionChange)
            mtranslate.positionChanged.connect(onPositionChange)
        }
    }

    bdContentItem:Rectangle {
        property alias x_pos: x_pos
        property alias y_pos: y_pos
        property alias z_pos: z_pos
        color: Constants.lpw_bgColor
        Column {
            anchors.top: parent.top
            anchors.topMargin: 20* screenScaleFactor
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 5* screenScaleFactor

            Row {
                spacing: 10//
                Label {
                    width:labelWidth* screenScaleFactor
                    color: "#FB0202"
                    text: "X:"
                    font.family: Constants.labelFontFamily
                    font.pointSize: 9
                    anchors.verticalCenter: parent.verticalCenter
                }
                LeftToolSpinBox {
                    id:x_pos
                    width: 239* screenScaleFactor
                    height: 28* screenScaleFactor
                    realStepSize:5
                    realFrom:-9999
                    realTo:9999
                    anchors.verticalCenter: parent.verticalCenter
                    realValue: mtranslate ? mtranslate.position.x.toFixed(2) : 0
                    textValidator: RegExpValidator {
                        regExp:   /^([\+ \-]?(([1-9]\d*)|(0)))([.]\d{0,2})?$/
                    }
                    onValueEdited:{
                        console.log("x_posx_pos realValue:"+realValue)
                        if(!mtranslate)return
                        if(isNaN(realValue)) return
                        if(parseFloat(realValue).toFixed(2) === mtranslate.position.x.toFixed(2))return
                        lock()
                        mtranslate.position.x = x_pos.realValue
                        unlock()
                    }
                }
            }
            Row {
                spacing: 10//
                Label {
                    width:labelWidth* screenScaleFactor
                    color: "#00FD00"
                    text: "Y:"
                    font.family: Constants.labelFontFamily
                    font.pointSize: 9
                    anchors.verticalCenter: parent.verticalCenter
                }
                LeftToolSpinBox {
                    id:y_pos
                    width: 239* screenScaleFactor
                    height: 28* screenScaleFactor
                    realStepSize:5
                    realFrom:-9999
                    realTo:9999
                    anchors.verticalCenter: parent.verticalCenter
                    realValue: mtranslate ? mtranslate.position.y.toFixed(2) : 0
                    textValidator: RegExpValidator {
                        regExp:   /^([\+ \-]?(([1-9]\d*)|(0)))([.]\d{0,2})?$/
                    }
                    onValueEdited:{
                        if(!mtranslate)return
                        if(isNaN(realValue)) return
                        if(parseFloat(realValue).toFixed(2) === mtranslate.position.y.toFixed(2))return
                        lock()
                        mtranslate.position.y = y_pos.realValue
                        unlock()
                    }
                }
            }
            Row {
                spacing: 10//
                Label {
                    width:labelWidth* screenScaleFactor
                    color: "#008FFD"
                    text: "Z:"
                    font.pointSize: 9
                    font.family: Constants.labelFontFamily
                    anchors.verticalCenter: parent.verticalCenter
                }
                LeftToolSpinBox {
                    id:z_pos
                    width: 239* screenScaleFactor
                    height: 28* screenScaleFactor
                    realStepSize:5
                    realFrom:-9999
                    realTo:9999
                    anchors.verticalCenter: parent.verticalCenter
                    realValue: mtranslate ? mtranslate.position.z.toFixed(2) : 0
                    textValidator: RegExpValidator {
                        regExp:   /^([\+ \-]?(([1-9]\d*)|(0)))([.]\d{0,2})?$/
                    }
                    onValueEdited:{
                        //                           console.log("z_posz_pos")
                        if(!mtranslate)return
                        if(isNaN(realValue)) return
                        if(parseFloat(realValue).toFixed(2) === mtranslate.position.z.toFixed(2))return
                        lock()
                        mtranslate.position.z = z_pos.realValue
                        unlock()
                    }
                }
            }

            Item{
                width: 20
                height: 15
            }

            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 10
                BasicButton {
                    width: 82* screenScaleFactor
                    height: 28* screenScaleFactor
                    text : qsTr("Center")
                    btnRadius:14
                    btnBorderW:1
                    borderColor: Constants.lpw_BtnBorderColor
                    borderHoverColor: Constants.lpw_BtnBorderHoverColor
                    defaultBtnBgColor: Constants.lpw_BtnColor
                    hoveredBtnBgColor: Constants.lpw_BtnHoverColor
                    onSigButtonClicked:
                    {
                        mtranslate.center()
                    }
                }

                BasicButton {
                    width: 82* screenScaleFactor
                    height: 28* screenScaleFactor
                    text : qsTr("Bottom")
                    btnRadius:14
                    btnBorderW:1
                    borderColor: Constants.lpw_BtnBorderColor
                    borderHoverColor: Constants.lpw_BtnBorderHoverColor
                    defaultBtnBgColor: Constants.lpw_BtnColor
                    hoveredBtnBgColor: Constants.lpw_BtnHoverColor
                    onSigButtonClicked:
                    {
                        mtranslate.bottom()
                    }
                }

                BasicButton {
                    width: 82* screenScaleFactor
                    height: 28* screenScaleFactor
                    text : qsTr("Reset")
                    btnRadius:14
                    btnBorderW:1
                    borderColor: Constants.lpw_BtnBorderColor
                    borderHoverColor: Constants.lpw_BtnBorderHoverColor
                    defaultBtnBgColor: Constants.lpw_BtnColor
                    hoveredBtnBgColor: Constants.lpw_BtnHoverColor
                    onSigButtonClicked:
                    {
                        mtranslate.reset()
                    }
                }
            }
        }
    }
}
