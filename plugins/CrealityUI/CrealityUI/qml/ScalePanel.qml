import QtQuick 2.10
import QtQuick.Controls 2.0
import "../secondqml"

BasicDialogNoWin_V4 {
    id:control
    width: 300* screenScaleFactor
    height: 240* screenScaleFactor
    title: qsTr("Scale")
    property var msale
    property var size:msale ? msale.orgSize : ""

    enum XYZType
    {
        X_Type =0,
        Y_Type,
        Z_Type
    }

    visible: msale.message

    onMsaleChanged: {
        if(msale)
        {console.log("onMsaleChanged aaaaa!")
            lock()
            signalsBlocked(true)
            cloader.item.x_scale.setRealValue(msale.scale.x * 100)
            cloader.item.y_scale.setRealValue(msale.scale.y * 100)
            cloader.item.z_scale.setRealValue(msale.scale.z * 100)
            cloader.item.x_length.setRealValue(msale.size.x)
            cloader.item.y_length.setRealValue(msale.size.y)
            cloader.item.z_length.setRealValue(msale.size.z)
            signalsBlocked(false)
            unlock()

            msale.scaleChanged.disconnect(onScaleChange)
            msale.scaleChanged.connect(onScaleChange)
        }
    }

    function lock()
    {
        msale.blockSignalScaleChanged(true)
    }

    function unlock()
    {
        msale.blockSignalScaleChanged(false)
    }

    function signalsBlocked(blocked)
    {
        cloader.item.x_scale.bSignalsBlcked = blocked
        cloader.item.y_scale.bSignalsBlcked = blocked
        cloader.item.z_scale.bSignalsBlcked = blocked
        cloader.item.x_length.bSignalsBlcked = blocked
        cloader.item.y_length.bSignalsBlcked = blocked
        cloader.item.z_length.bSignalsBlcked = blocked
    }


    function onScaleChange(){
        
        if(msale)
        {console.log("onScaleChange sss aaaaa!!!")
            signalsBlocked(true)
            cloader.item.x_scale.setRealValue(msale.scale.x * 100)
            cloader.item.y_scale.setRealValue(msale.scale.y * 100)
            cloader.item.z_scale.setRealValue(msale.scale.z * 100)
            cloader.item.x_length.setRealValue(msale.size.x)
            cloader.item.y_length.setRealValue(msale.size.y)
            cloader.item.z_length.setRealValue(msale.size.z)
            signalsBlocked(false)
            //console.log("z_length.value +" + z_length.value)
        }
    }

    bdContentItem:Rectangle {
        property alias x_scale: x_scale
        property alias y_scale: y_scale
        property alias z_scale: z_scale
        property alias x_length: x_length
        property alias y_length: y_length
        property alias z_length: z_length
        color: Constants.lpw_bgColor
        Column{
            anchors.top: parent.top
            anchors.topMargin: 20* screenScaleFactor
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 10* screenScaleFactor

            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 10* screenScaleFactor
                StyledLabel {
                    anchors.verticalCenter: parent.verticalCenter
                    width: 15* screenScaleFactor
                    leftPadding: 0/*10*/
                    rightPadding: 10//
                    color: "#FB0202"
                    text: "X:"
                    font.pointSize: 9
                }
                LeftToolSpinBox {
                    id:x_length
                    anchors.verticalCenter: parent.verticalCenter
                    width: 115* screenScaleFactor
                    height: 28* screenScaleFactor
                    realStepSize: 5
                    // realFrom:(size.x/10000) < 0.01 ? 0.01 : size.x/10000
                    realFrom:(size.x === undefined) ? 0.001 : size.x * 0.00107
                    realTo:(size.x === undefined) ? 9999 : size.x*100
                    realValue: msale ? msale.size.x.toFixed(2) : 1
                    textValidator: RegExpValidator {
                        regExp:   /(\d{1,4})([.,]\d{1,2})?$/
                    }
                    onValueEdited:{
                        console.log("x_length")
                        if(!msale)return
                        if(isNaN(realValue)) return
                        if(realValue === 0 || size === "")return;
                        if(parseFloat(realValue).toFixed(2) === msale.size.x.toFixed(2))return
                        lock()
                        signalsBlocked(true)
                        if(uniformScalingCheckbox.checked)
                        {
                            //                               myText
                            var factor = realValue/size.x
                            msale.scale.x = factor
                            msale.scale.y = factor
                            msale.scale.z = factor
                            msale.size.x = realValue
                            msale.size.y = msale.scale.y * size.y
                            msale.size.z = msale.scale.z * size.z
                            //                               //updata qml show data

                            x_scale.setRealValue(msale.scale.x *100)
                            y_scale.setRealValue(msale.scale.y * 100)
                            z_scale.setRealValue(msale.scale.z * 100)
                            y_length.setRealValue(msale.size.y)
                            z_length.setRealValue(msale.size.z)
                        }
                        else{
                            msale.size.x = realValue
                            msale.scale.x = realValue/size.x
                            x_scale.setRealValue(msale.scale.x * 100)
                        }
                        signalsBlocked(false)
                        unlock()
                    }
                }
                LeftToolSpinBox {
                    width: 115* screenScaleFactor
                    height: 28* screenScaleFactor
                    anchors.verticalCenter: parent.verticalCenter
                    id:x_scale
                    realStepSize:0.5
                    realFrom:0.1
                    realTo:9999
                    realValue: msale ? msale.scale.x * 100 : 100
                    unitchar : "%"
                    textValidator: RegExpValidator {
                        regExp:   /(\d{1,4})([.,]\d{1,2})?$/
                    }
                    onValueEdited:{
                        console.log("x_scale")
                        if(!msale)return
                        if(isNaN(realValue)) return
                        if(realValue === 0 || size === "")return;
                        if(parseFloat(realValue/100).toFixed(3) === msale.scale.x.toFixed(3))return
                        //lock C++ signal
                        lock()
                        //lock other SpinBox signal
                        signalsBlocked(true)

                        if(msale)
                        {
                            if(uniformScalingCheckbox.checked)
                            {
                                msale.scale.x = x_scale.realValue/100
                                msale.scale.y = x_scale.realValue/100
                                msale.scale.z = x_scale.realValue/100

                                msale.size.x = msale.scale.x * size.x
                                msale.size.y = msale.scale.y * size.y
                                msale.size.z = msale.scale.z * size.z

                                y_scale.setRealValue(msale.scale.y * 100)
                                z_scale.setRealValue(msale.scale.z * 100)
                                x_length.setRealValue(msale.size.x)
                                y_length.setRealValue(msale.size.y)
                                z_length.setRealValue(msale.size.z)
                            }else{
                                msale.scale.x = x_scale.realValue/100
                                msale.size.x = msale.scale.x * size.x
                                x_length.setRealValue(msale.size.x)
                            }
                        }

                        signalsBlocked(false)
                        unlock()
                    }
                }
            }
            Row {
                spacing: 10* screenScaleFactor
                anchors.horizontalCenter: parent.horizontalCenter
                StyledLabel {
                    anchors.verticalCenter: parent.verticalCenter
                    width: 15* screenScaleFactor
                    leftPadding: 0/*10*/
                    rightPadding: 10//
                    color: "#00FD00"
                    text: "Y:"
                    font.pointSize: 9
                }
                LeftToolSpinBox {
                    id:y_length
                    anchors.verticalCenter: parent.verticalCenter
                    width: 115* screenScaleFactor
                    height: 28* screenScaleFactor
                    realStepSize: 5
                    // realFrom:(size.y/10000) < 0.01 ? 0.01 : size.y/10000
                    realFrom:(size.y === undefined) ? 0.001 : size.y * 0.00107
                    realTo:(size.y === undefined) ? 9999 : size.y*100
                    realValue: msale ? msale.size.y.toFixed(2) : 1
                    textValidator: RegExpValidator {
                        regExp:   /(\d{1,4})([.,]\d{1,2})?$/
                    }
                    onValueEdited:{
                        console.log("y_length")
                        if(!msale)return
                        if(isNaN(realValue)) return
                        if(realValue === 0 || size === "")return;
                        if(parseFloat(realValue).toFixed(2) === msale.size.y.toFixed(2))return
                        lock()
                        signalsBlocked(true)

                        if(uniformScalingCheckbox.checked)
                        {
                            var factor =  y_length.realValue/size.y
                            msale.scale.x = factor
                            msale.scale.y = factor
                            msale.scale.z = factor

                            msale.size.x = msale.scale.x * size.x
                            msale.size.y = realValue
                            msale.size.z = msale.scale.z * size.z

                            //
                            //updata qml show data
                            x_scale.setRealValue(msale.scale.x * 100)
                            y_scale.setRealValue(msale.scale.y * 100)
                            z_scale.setRealValue(msale.scale.z * 100)
                            x_length.setRealValue(msale.size.x)
                            //                               y_length.setRealValue(msale.scale.y * size.y)
                            z_length.setRealValue(msale.size.z)

                        }else{
                            msale.size.y = realValue
                            msale.scale.y = parseFloat(realValue/size.y).toFixed(4)
                            y_scale.setRealValue(msale.scale.y * 100)
                        }


                        signalsBlocked(false)
                        unlock()
                    }
                }
                LeftToolSpinBox {
                    width: 115* screenScaleFactor
                    height: 28* screenScaleFactor
                    anchors.verticalCenter: parent.verticalCenter
                    id:y_scale
                    realStepSize:0.5
                    realFrom:0.1
                    realTo:9999
                    realValue: msale ? msale.scale.y * 100: 100
                    unitchar : "%"
                    textValidator: RegExpValidator {
                        regExp:   /(\d{1,4})([.,]\d{1,2})?$/
                    }
                    onValueEdited:{
                        console.log("y_scale")
                        if(!msale)return
                        if(isNaN(realValue)) return
                        if(realValue === 0 || size === "")return;
                        if(parseFloat(realValue/100).toFixed(3) === msale.scale.y.toFixed(3))return
                        lock()
                        signalsBlocked(true)

                        if(msale)
                        {
                            if(uniformScalingCheckbox.checked)
                            {
                                msale.scale.x = y_scale.realValue/100
                                msale.scale.y = y_scale.realValue/100
                                msale.scale.z = y_scale.realValue/100

                                msale.size.x = msale.scale.x * size.x
                                msale.size.y = msale.scale.y * size.y
                                msale.size.z = msale.scale.z * size.z

                                //updata qml show data
                                x_scale.setRealValue(msale.scale.x * 100)
                                //                                  y_scale.setRealValue(msale.scale.y * 100)
                                z_scale.setRealValue(msale.scale.z * 100)
                                x_length.setRealValue(msale.size.x)
                                y_length.setRealValue(msale.size.y)
                                z_length.setRealValue(msale.size.z)
                            }
                            else
                            {
                                msale.scale.y = y_scale.realValue/100
                                msale.size.y = msale.scale.y * size.y
                                y_length.setRealValue(msale.size.y)
                            }
                        }

                        signalsBlocked(false)
                        unlock()
                    }
                }
            }
            Row {
                spacing: 10* screenScaleFactor
                anchors.horizontalCenter: parent.horizontalCenter
                StyledLabel {
                    width: 15* screenScaleFactor
                    anchors.verticalCenter: parent.verticalCenter
                    leftPadding: 0/*10*/
                    rightPadding: 10//
                    color: "#008FFD"
                    text: "Z:"
                    font.pointSize: 9
                }
                LeftToolSpinBox {
                    id:z_length
                    width: 115* screenScaleFactor
                    height: 28* screenScaleFactor
                    realStepSize: 5
                    // realFrom:(size.z/10000) < 0.01 ? 0.01 : size.z/10000
                    realFrom:(size.z === undefined) ? 0.001 : size.z * 0.00107
                    realTo:(size.z === undefined) ? 9999 : size.z*100
                    realValue: msale ? msale.size.z : 1
                    textValidator: RegExpValidator {
                        regExp:   /(\d{1,4})([.,]\d{1,2})?$/
                    }
                    onValueEdited:{
                        console.log("z_length")
                        if(!msale)return
                        if(isNaN(realValue)) return
                        if(realValue === 0 || size === "")return;
                        if(parseFloat(realValue).toFixed(2) === msale.size.z.toFixed(2))return
                        lock()
                        signalsBlocked(true)

                        if(uniformScalingCheckbox.checked)
                        {
                            var factor =  z_length.realValue/size.z;
                            msale.scale.x = factor
                            msale.scale.y = factor
                            msale.scale.z = factor

                            msale.size.x = msale.scale.x * size.x
                            msale.size.y = msale.scale.y * size.y
                            msale.size.z = realValue

                            x_scale.setRealValue(msale.scale.x * 100)
                            y_scale.setRealValue(msale.scale.y * 100)
                            z_scale.setRealValue(msale.scale.z * 100)
                            x_length.setRealValue(msale.size.x)
                            y_length.setRealValue(msale.size.y)
                            //                               z_length.setRealValue(msale.scale.z * size.z)

                        }
                        else
                        {
                            msale.size.z = realValue
                            msale.scale.z = z_length.realValue/size.z

                            z_scale.setRealValue(msale.scale.z * 100)
                        }

                        signalsBlocked(false)
                        unlock()
                    }
                }
                LeftToolSpinBox {
                    width: 115* screenScaleFactor
                    height: 28* screenScaleFactor
                    id:z_scale
                    realStepSize:0.5
                    realFrom:0.1
                    realTo:9999
                    realValue: msale ? msale.scale.z * 100 : 100
                    unitchar : "%"
                    textValidator: RegExpValidator {
                        regExp:   /(\d{1,4})([.,]\d{1,2})?$/
                    }
                    onValueEdited:{
                        console.log("z_scale")
                        if(!msale)return
                        if(isNaN(realValue)) return
                        if(realValue === 0 || size === "")return;
                        if(parseFloat(realValue/100).toFixed(3) === msale.scale.z.toFixed(3))return
                        lock()
                        signalsBlocked(true)

                        if(msale)
                        {
                            if(uniformScalingCheckbox.checked)
                            {
                                msale.scale.x = z_scale.realValue/100//parseFloat(z_scale.realValue/100).toFixed(2)
                                msale.scale.y = z_scale.realValue/100//parseFloat(z_scale.realValue/100).toFixed(2)
                                msale.scale.z = z_scale.realValue/100//parseFloat(z_scale.realValue/100).toFixed(2)

                                msale.size.x = msale.scale.x * size.x
                                msale.size.y = msale.scale.y * size.y
                                msale.size.y = msale.scale.z * size.z
                                //                                  onScaleChange()

                                x_scale.setRealValue(msale.scale.x * 100)
                                y_scale.setRealValue(msale.scale.y * 100)
                                //                                  z_scale.setRealValue(msale.scale.z * 100)
                                x_length.setRealValue(msale.size.x)
                                y_length.setRealValue(msale.size.y)
                                z_length.setRealValue(msale.size.z)
                            }
                            else
                            {
                                msale.scale.z = z_scale.realValue/100
                                msale.size.z = msale.scale.z * size.z
                                z_length.setRealValue(msale.size.z)
                            }
                        }

                        signalsBlocked(false)
                        unlock()
                    }
                }
            }
            StyleCheckBox {
                id: uniformScalingCheckbox
                checked: msale ? msale.uniformCheck : false
                fontSize:12
                text: qsTr("Uniform")

                onCheckedChanged:
                {
                    console.log("checked = " + checked)
                    msale.uniformCheck = checked
                }

            }
            BasicButton{
                width: 258* screenScaleFactor
                height: 30* screenScaleFactor
                text : qsTr("Reset")
                btnRadius:3
                btnBorderW:1
                anchors.horizontalCenter: parent.horizontalCenter
                borderColor: Constants.lpw_BtnBorderColor
                borderHoverColor: Constants.lpw_BtnBorderHoverColor
                defaultBtnBgColor: Constants.lpw_BtnColor
                hoveredBtnBgColor: Constants.lpw_BtnHoverColor
                onSigButtonClicked:
                {
                    msale.scale.x = 1
                    msale.scale.y = 1
                    msale.scale.z = 1

                    msale.size.x = msale.scale.x * size.x
                    msale.size.y = msale.scale.y * size.y
                    msale.size.z = msale.scale.z * size.z

                    x_scale.setRealValue(100)
                    y_scale.setRealValue(100)
                    z_scale.setRealValue(100)
                    x_length.setRealValue(msale.size.x)
                    y_length.setRealValue(msale.size.y)
                    z_length.setRealValue(msale.size.z)

                }
            }
        }
    }
}
