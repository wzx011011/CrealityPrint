import QtQuick 2.10
import QtQuick.Controls 2.0
import "qrc:/CrealityUI"
import "../secondqml"

BasicDialogNoWin_V4 {//by TCJ
    id:control
    width: 300* screenScaleFactor
    height: 269* screenScaleFactor
    title:qsTr("Rotate")
    property var mrotate

    visible: mrotate.message

    onMrotateChanged: {
        if(mrotate)
        {//console.log("-------onMrotateChanged555----aaaaa---")
            lock()
            signalsBlocked(true)
            cloader.item.x_angle.setRealValue(mrotate.rotate.x)
            cloader.item.y_angle.setRealValue(mrotate.rotate.y)
            cloader.item.z_angle.setRealValue(mrotate.rotate.z)
            signalsBlocked(false)
            unlock()

            mrotate.rotateChanged.disconnect(onRotateChange)
            mrotate.rotateChanged.connect(onRotateChange)
        }
    }

    function lock()
    {
        mrotate.blockSignalScaleChanged(true)
    }
    function unlock()
    {
        mrotate.blockSignalScaleChanged(false)
    }
    function signalsBlocked(blocked)
    {
        cloader.item.x_angle.bSignalsBlcked = blocked
        cloader.item.y_angle.bSignalsBlcked = blocked
        cloader.item.z_angle.bSignalsBlcked = blocked
    }


    function onRotateChange (){
        if(mrotate)
        {//console.log("onRotateChange aaaaa")
            signalsBlocked(true)
            cloader.item.x_angle.setRealValue(mrotate.rotate.x)
            cloader.item.y_angle.setRealValue(mrotate.rotate.y)
            cloader.item.z_angle.setRealValue(mrotate.rotate.z)
            signalsBlocked(false)
        }
    }




    readonly property  int  g_nButHeight: 32
    signal valueXChanged()
    signal valueYChanged()
    signal valueZChanged()
    signal resetValue()
    bdContentItem:Rectangle {
        property alias x_angle: x_angle
        property alias y_angle: y_angle
        property alias z_angle: z_angle
        color: Constants.lpw_bgColor
        Column{
            spacing:20* screenScaleFactor
            anchors.top: parent.top
            anchors.topMargin: 20* screenScaleFactor
            anchors.horizontalCenter: parent.horizontalCenter

            Row {
                spacing:10* screenScaleFactor

                BasicButton{
                    width: 56* screenScaleFactor
                    height: 28* screenScaleFactor
                    text : qsTr("+45°")
                    pointSize: 9
                    btnRadius:14
                    btnBorderW:1
                    borderColor: Constants.lpw_BtnBorderColor
                    borderHoverColor: Constants.lpw_BtnBorderHoverColor
                    defaultBtnBgColor: Constants.lpw_BtnColor
                    hoveredBtnBgColor: Constants.lpw_BtnHoverColor
                    onSigButtonClicked:
                    {
                        mrotate.rotate.x += 45
                    }
                }

                StyledLabel {
                    color: "#FB0202"
                    text: "X:"
                    width:12* screenScaleFactor
                    height: 28* screenScaleFactor
                    font.family: Constants.labelFontFamily
                    font.pointSize: 9
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignHCenter
                }

                LeftToolSpinBox {
                    id:x_angle
                    width: 115 * screenScaleFactor
                    height: 28 * screenScaleFactor
                    realStepSize:5
                    realFrom:-9999
                    realTo:9999
                    textValidator: RegExpValidator {
                        regExp:   /^([\+ \-]?(([1-9]\d*)|(0)))([.]\d{0,2})?$/
                    }
                    //                       realValue:  mrotate.rotate.x
                    unitchar: qsTr("° ")

                    onTextPressed: {console.log("X onTextPressed aaaaa")
                        mrotate.startRotate()
                    }
                    onValueEdited:{
                        if(!mrotate)return
                        if(isNaN(realValue)) return
                        if(parseFloat(realValue).toFixed(2) === mrotate.rotate.x.toFixed(2))return
                        lock()
                        console.log("mrotate.rotate =" + mrotate.rotate )
                        mrotate.rotate = Qt.vector3d(x_angle.realValue,mrotate.rotate.y,mrotate.rotate.z)
                        unlock()
                    }
                }

                BasicButton{
                    width: 56* screenScaleFactor
                    height: 28* screenScaleFactor
                    text : "-45°"
                    btnRadius:14
                    btnBorderW:1
                    pointSize: 9
                    borderColor: Constants.lpw_BtnBorderColor
                    borderHoverColor: Constants.lpw_BtnBorderHoverColor
                    defaultBtnBgColor: Constants.lpw_BtnColor
                    hoveredBtnBgColor: Constants.lpw_BtnHoverColor
                    onSigButtonClicked:
                    {
                        mrotate.rotate.x -= 45
                    }
                }
            }
            Row {
                spacing:10* screenScaleFactor
                BasicButton{
                    width: 56* screenScaleFactor
                    height: 28* screenScaleFactor
                    text : qsTr("+45°")
                    btnRadius:14
                    btnBorderW:1
                    pointSize: 9
                    borderColor: Constants.lpw_BtnBorderColor
                    borderHoverColor: Constants.lpw_BtnBorderHoverColor
                    defaultBtnBgColor: Constants.lpw_BtnColor
                    hoveredBtnBgColor: Constants.lpw_BtnHoverColor
                    onSigButtonClicked:
                    {
                        mrotate.rotate.y += 45
                    }
                }

                StyledLabel {
                    color: "#00FD00"
                    text: "Y:"
                    width:12* screenScaleFactor
                    height: 28* screenScaleFactor
                    font.family: Constants.labelFontFamily
                    font.pointSize: 9
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignHCenter
                }
                LeftToolSpinBox {
                    id:y_angle
                    width:115* screenScaleFactor
                    height: 28 * screenScaleFactor
                    realStepSize:5
                    realFrom:-9999
                    realTo:9999
                    textValidator: RegExpValidator {
                        regExp:   /^([\+ \-]?(([1-9]\d*)|(0)))([.]\d{0,2})?$/
                    }
                    //                       realValue: mrotate.rotate.y
                    unitchar: qsTr("° ")

                    onTextPressed: {
                        mrotate.startRotate()
                    }
                    onValueEdited:{
                        if(!mrotate)return
                        if(isNaN(realValue)) return
                        if(parseFloat(realValue).toFixed(2) === mrotate.rotate.y.toFixed(2))return
                        lock()
                        mrotate.rotate = Qt.vector3d(mrotate.rotate.x,y_angle.realValue,mrotate.rotate.z)
                        unlock()
                    }
                }

                BasicButton{
                    width: 56* screenScaleFactor
                    height: 28* screenScaleFactor
                    text : "-45°"
                    btnRadius:14
                    btnBorderW:1
                    pointSize: 9
                    borderColor: Constants.lpw_BtnBorderColor
                    borderHoverColor: Constants.lpw_BtnBorderHoverColor
                    defaultBtnBgColor: Constants.lpw_BtnColor
                    hoveredBtnBgColor: Constants.lpw_BtnHoverColor
                    onSigButtonClicked:
                    {
                        mrotate.rotate.y -= 45
                    }
                }
            }
            Row {
                spacing:10* screenScaleFactor
                BasicButton{
                    width: 56* screenScaleFactor
                    height: 28* screenScaleFactor
                    text : qsTr("+45°")
                    btnRadius:14
                    btnBorderW:1
                    pointSize: 9
                    borderColor: Constants.lpw_BtnBorderColor
                    borderHoverColor: Constants.lpw_BtnBorderHoverColor
                    defaultBtnBgColor: Constants.lpw_BtnColor
                    hoveredBtnBgColor: Constants.lpw_BtnHoverColor
                    onSigButtonClicked:
                    {
                        mrotate.rotate.z += 45
                    }
                }

                StyledLabel {
                    color: "#008FFD"
                    text: "Z:"
                    width:12* screenScaleFactor
                    height: 28* screenScaleFactor
                    font.pointSize: 9
                    font.family: Constants.labelFontFamily
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignHCenter
                }
                LeftToolSpinBox {
                    id:z_angle
                    width:115* screenScaleFactor
                    height: 28 * screenScaleFactor
                    realStepSize:5
                    realFrom:-9999
                    realTo:9999
                    textValidator: RegExpValidator {
                        regExp:   /^([\+ \-]?(([1-9]\d*)|(0)))([.]\d{0,2})?$/
                    }
                    //                       realValue: mrotate.rotate.z
                    unitchar: qsTr("° ")

                    onTextPressed: {
                        mrotate.startRotate()
                    }
                    onValueEdited:{
                        if(!mrotate)return
                        if(isNaN(realValue)) return
                        if(parseFloat(realValue).toFixed(2) === mrotate.rotate.z.toFixed(2))return
                        lock()
                        mrotate.rotate = Qt.vector3d(mrotate.rotate.x,mrotate.rotate.y,z_angle.realValue)
                        unlock()
                    }
                }

                BasicButton{
                    width: 56* screenScaleFactor
                    height: 28* screenScaleFactor
                    text : "-45°"
                    btnRadius:14
                    btnBorderW:1
                    pointSize: 9
                    borderColor: Constants.lpw_BtnBorderColor
                    borderHoverColor: Constants.lpw_BtnBorderHoverColor
                    defaultBtnBgColor: Constants.lpw_BtnColor
                    hoveredBtnBgColor: Constants.lpw_BtnHoverColor
                    onSigButtonClicked:
                    {
                        mrotate.rotate.z -= 45
                    }
                }
            }
            BasicButton{
                width: 258* screenScaleFactor
                height: 28* screenScaleFactor
                text : qsTr("Reset")
                btnRadius:14
                btnBorderW:1
                pointSize: 9
                anchors.horizontalCenter: parent.horizontalCenter
                borderColor: Constants.lpw_BtnBorderColor
                borderHoverColor: Constants.lpw_BtnBorderHoverColor
                defaultBtnBgColor: Constants.lpw_BtnColor
                hoveredBtnBgColor: Constants.lpw_BtnHoverColor
                onSigButtonClicked:
                {
                    lock()
                    signalsBlocked(true)
                    mrotate.reset()
                    x_angle.setRealValue(0)
                    y_angle.setRealValue(0)
                    z_angle.setRealValue(0)
                    signalsBlocked(false)
                    unlock()
                }
            }
        }
    }
}
