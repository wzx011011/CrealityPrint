import QtQuick 2.12
import QtQuick.Controls 2.12

SpinBox {
      id: control
      editable: true

      property var unitchar: "mm"

      property int decimals: 2
      property real factor: Math.pow(10, decimals)
      property var realValue: 1
      property real realStepSize: 1.0
      property real realFrom: 0.0
      property real realTo: 100.0
      property real orgValue:1
      property alias  textObj: numbTxt
      property var wheelEnable: true


      //lisugui 2020-10-14 control signal block
      property bool bSignalsBlcked: false


      signal valueEdited
      signal textPressed
      //height: 30
      implicitWidth: 180
      implicitHeight: 28
      value: realValue*factor
      stepSize: realStepSize*factor
      to : realTo*factor
      from : realFrom*factor

      function setRealValue(v)
      {
          console.log("v=" + parseFloat(v).toFixed(2))
          realValue = parseFloat(v).toFixed(2)
        //   value = realValue*factor
      }
      property bool bZeroInputEanbled: true
      property alias textValidator: numbTxt.validator

      property color bgColor: Constants.dialogItemRectBgColor
      property color borderColor: Constants.dialogItemRectBgBorderColor
      property var bgRadius : 0
      property color indicatorColor: Constants.cmbIndicatorRectColor_basic
      property color textColor: Constants.textColor
      contentItem: TextInput {
          id:numbTxt
          text: control.textFromValue(realValue, control.locale)
          anchors.left: control.left
          anchors.leftMargin: 10
          anchors.right: idUnitchar.right
          anchors.rightMargin: idUnitchar.width
          anchors.top: control.top
          anchors.topMargin: 0
          anchors.bottom: control.bottom
          anchors.bottomMargin: 0
//          anchors.horizontalCenter: control.horizontalCenter
//          anchors.verticalCenter: control.verticalCenter
          width: control.width - idUp.width
          height: control.height
          font: control.font
          color: textColor//"white"/*"#21be2b"*/
          selectionColor: "#21be2b"
          selectedTextColor: "#ffffff"
          horizontalAlignment: Qt.AlignLeft
          verticalAlignment: Qt.AlignVCenter

//          readOnly: !control.editable
          inputMethodHints: Qt.ImhFormattedNumbersOnly
//          validator: DoubleValidator {
//                      bottom: Math.min(control.from, control.to)*control.factor
//                      top:  Math.max(control.from, control.to)*control.factor
//                  }
        //   validator: RegExpValidator {
        //               regExp:   /^([\+ \-]?(([1-9]\d*)|(0)))([.]\d{0,2})?$/  ///^([\+ \-]?(\d{1,4})([.,]\d{1,3})?$/
        //           }
        validator: RegExpValidator {
            regExp:   /(\d{1,4})([.,]\d{1,2})?$/
         }
          DoubleValidator {
              notation:DoubleValidator.StandardNotation
                      bottom: Math.min(control.realFrom, control.realTo)
                      top:  Math.max(control.realFrom, control.realTo)
         }

          MouseArea{
            enabled: wheelEnable
            anchors.fill: parent
            onClicked: {
                numbTxt.selectAll()
                numbTxt.forceActiveFocus()
                textPressed()
            }
            onWheel: {
                var datl = wheel.angleDelta.y/120;
                console.log("control.value:" + control.value + " stepSize:" + stepSize)
                console.log("control.realValue:" + control.realValue + " realStepSize:" + realStepSize)
                if(datl>0){
                    if(control.realValue + realStepSize <= control.realTo)
                    {
                        control.realValue += realStepSize
                        //control.value+=stepSize
                    }
                }else{
                    if(control.realValue - realStepSize >= control.realFrom)
                    {
                        control.realValue -= realStepSize
                        //control.value-=stepSize
                    }
                }
                 
                orgValue = realValue
                //  realValue =control.textFromValue(control.value, control.locale)// control.value/factor
                valueEdited()
            }
           }

           onAccepted:
           {
                if(text <realFrom)realValue =control.textFromValue(control.realFrom, control.locale)
                else if(text > realTo) realValue =control.textFromValue(control.realTo, control.locale)
                var locale_lang = Qt.locale("zh_CN");
                realValue = Number.fromLocaleString(locale_lang, text)// control.value/factor
                orgValue = realValue
    //              text = Qt.binding(function(){ return control.textFromValue(control.value, control.locale)})
                valueEdited()
           }
           onActiveFocusChanged:
           {
                if(!activeFocus)
                {
                    if(text <realFrom)realValue =control.textFromValue(control.realFrom, control.locale)
                    else if(text > realTo) realValue =control.textFromValue(control.realTo, control.locale)
                    if(orgValue == realValue)return
                    else
                    {
                        var locale_lang = Qt.locale("zh_CN");
                        realValue = Number.fromLocaleString(locale_lang, text)// control.value/factor
                        orgValue = realValue
                        valueEdited()
                    }
                }
           }
//           onTextChanged:
//           {
// //              console.log("onTextChanged finish = text =" + text)
//               if(text === "")return
//               if(text >= realFrom)
//               {
// //                  console.log("orgValue =" + orgValue)
// //                  console.log("realValue =" + realValue)
//                   if(!bSignalsBlcked  && orgValue !== realValue)
//                   {
//                         valueEdited()
//                   }
//                    orgValue = realValue
// //                  console.log("realValue =" + realValue)
// //                  console.log("value =" + value)
//               }
//           }


      }

      StyledLabel
      {
          id: idUnitchar
        text: unitchar
        width: contentWidth
        color: "#999999"
        visible: unitchar.length > 0 ? true : false
        anchors.bottom: control.bottom
        anchors.bottomMargin: 8//4
        anchors.right: control.right
        anchors.rightMargin: 20
        font.pixelSize: 12
      }
        onValueModified: {
            console.log("onValueModified vlaue =" +value)
            orgValue = realValue
            realValue = control.value/factor
            valueEdited()
        }
      up.indicator: Rectangle {
          id:idUp
          x: control.mirrored ? 0 : parent.width - width - 1
          y:1
          height: parent.height/2 - 1
          implicitWidth: 16
          implicitHeight: parent.height/2 - 1
          color: indicatorColor//"#3F3F3F"//control.up.pressed ? "transparent" : "#f6f6f6"
          z:2
          Image {
              width: 7
              height: 5
              source: control.up.pressed || control.up.hovered? "qrc:/UI/photo/upBtn_d.png":"qrc:/UI/photo/upBtn.png"
              anchors.centerIn: parent
              horizontalAlignment: Text.AlignHCenter
              verticalAlignment: Text.AlignVCenter
          }
      }

      down.indicator: Rectangle {
          x: control.mirrored ? 0:parent.width - width - 1
          y:parent.height/2
          height: parent.height/2 - 1
          implicitWidth: 16
          implicitHeight: parent.height/2 - 1
          color: indicatorColor//"#3F3F3F"//control.down.pressed ? "transparent" : "#f6f6f6"
          z:2
          Image {
              width: 7
              height: 5
              source: control.down.pressed || control.down.hovered ? "qrc:/UI/photo/downBtn_d.png" :"qrc:/UI/photo/downBtn.png"
              horizontalAlignment: Text.AlignHCenter
              verticalAlignment: Text.AlignVCenter
              anchors.centerIn: parent
          }
      }

      background: Rectangle {
          implicitWidth: control.width
          border.color: control.hovered ? "lightblue" : borderColor
          color: bgColor //Constants.dialogItemRectBgColor//"#4B4B4B"
          radius: bgRadius
      }
//      onValueModified: {
//          console.log("vlaue =" +value)
//          orgValue = realValue
//          realValue = control.value/factor
//          valueEdited()
//      }
      validator: DoubleValidator {
                  bottom: Math.min(control.from, control.to)*control.factor
                  top:  Math.max(control.from, control.to)*control.factor
              }

      textFromValue: function(value, locale) {
                  //return parseFloat(value*1.0/factor).toFixed(decimals);
                  return parseFloat(value).toFixed(decimals);
              }
      valueFromText: function(text, locale) {
        var locale_lang = Qt.locale("zh_CN");
          realValue = Number.fromLocaleString(locale_lang, text)
          return realValue*factor;
      }
  }
