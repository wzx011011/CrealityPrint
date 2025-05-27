import QtQuick 2.0

import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 2.0 as QQC2
CheckBox {
    id:control
    property color textColor : Constants.textColor
    property var keyStr: ""
    property var strToolTip: ""
    property var fontSize: Constants.labelFontPointSize
    property var isGroupPrintUsed: false

    signal styleCheckClicked(var key, var value)
    signal styleCheckedChanged(var key, var value)

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
            font.family: Constants.labelFontFamily
            font.weight: Constants.labelFontWeight
            font.pointSize: Constants.labelFontPointSize
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

    style: CheckBoxStyle {

        label: Text{
            color: enabled ? textColor : "gray"
            text: control.text;
            /*font: Constants.labelFontFamily*/
            font.family: Constants.labelFontFamily
            //font.pixelSize: fontSize//by TCJ
            font.pointSize: fontSize
        }
        indicator: Rectangle {
            implicitWidth: 16* screenScaleFactor
            implicitHeight: 16* screenScaleFactor
            radius: 3
            opacity: control.enabled ? 1 : 0.7
            border.color:  control.hovered ?  Constants.textRectBgHoveredColor : isGroupPrintUsed ? "#828790" : Constants.checkBoxBorderColor
            border.width: 1
            color:  isGroupPrintUsed ? "#F9F9F9" : Constants.checkBoxBgColor //"#BDBEBF"
            /*Canvas{
                  anchors.fill: parent;
                  anchors.margins: 3;
                  visible: control.checked;
                  onPaint: {
                      var ctx = getContext("2d");
                      ctx.save();
                      ctx.strokeStyle = "#FFFFFF"//"#42BDD8"
                      ctx.lineWidth = 2;
                      ctx.beginPath();
                      ctx.moveTo(2,height/2);
                      ctx.lineTo(width/2 , height-2);
                      ctx.moveTo(width/2 , height-2);
                      ctx.lineTo(width-2 , 2);
                      ctx.stroke();
                      ctx.restore();
                  }
              }*/
            Image {
                width: 9* screenScaleFactor
                height: 6* screenScaleFactor
                anchors.centerIn: parent
                source: isGroupPrintUsed ? "qrc:/UI/images/check3.png" : Constants.checkBtnImg
                visible: control.checked
            }
        }
    }

    onClicked:
    {
        styleCheckClicked(keyStr, checked.toString())
    }
    onCheckedChanged:
    {
        styleCheckedChanged(keyStr, checked.toString())
    }
}
