import QtQuick 2.0

import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
CheckBox {
    id:control
    property color textColor : Constants.textColor
    property color bgColor: checkState == Qt.Checked ? "#009cff" : "#3A3A3A" //Constants.rectBorderColor
    property color borderColor:"#cbcbcb"// "#434343"

    property real borderWidth: checkState == Qt.Checked ? 0 : 1
    property int pixelSize : 12
    property var indicatorSize: 18
    property int myspacing: 10
    padding: 0
    leftPadding: myspacing + indicatorSize
    spacing: myspacing

    implicitWidth: Math.max(30,implicitBackgroundWidth + leftInset + rightInset,
                            idContentText.contentWidth + leftPadding +rightPadding)

    contentItem:Item{
        clip: true
        Text {
            property bool needRuning: false
            property int hAlign: 0 //0： 默认值，字体左对齐 1： 字体居中 2:右对齐
            id: idContentText
            width: control.width - indicatorSize - myspacing
            x: hAlign == 1 ? Math.abs(idContentText.width - idContentText.contentWidth)/2 :
                             (hAlign == 0 ? 0:idContentText.width - idContentText.contentWidth)
            font.family: Constants.labelFontFamily
            font.weight: Constants.labelFontWeight
            font.pixelSize: pixelSize
            anchors.verticalCenter: parent.verticalCenter
            verticalAlignment: Qt.AlignVCenter
            text: control.text
            color: textColor
            //动画解释 1. 左对齐 2. 等待两秒 3.然后开始滚动 4. 等待一秒，归零 5. 等待3秒
            SequentialAnimation on x {//内容长度大于设置的text的长度才滚动
                running: idContentText.needRuning
                loops: Animation.Infinite
                PropertyAnimation { to: 0; duration: 0 }
                PropertyAnimation { duration: 2000 }
                PropertyAnimation { to: idContentText.width - idContentText.contentWidth + 0; duration: 5000 }
                PropertyAnimation { duration: 1000 }
                PropertyAnimation { to: 0; duration: 0 }
                PropertyAnimation { duration: 2000 }
            }

            onTextChanged: {
                width = width
                if(idContentText.contentWidth !== 0)
                    needRuning = control.width - indicatorSize - myspacing - idContentText.contentWidth < 0
            }

            Component.onCompleted:{
                needRuning = control.width - indicatorSize - myspacing - idContentText.contentWidth < 0
            }
        }
    }

    indicator: Item{
        implicitWidth: indicatorSize
        implicitHeight: control.height
        Rectangle {
            implicitWidth: indicatorSize
            implicitHeight: indicatorSize
            anchors.verticalCenter: parent.verticalCenter
            opacity: control.enabled ? 1 : 0.7
            border.color:  control.hovered ? "#009cff": borderColor //"#BDBEBF";
            border.width: borderWidth
            radius: 3
            color:  bgColor // "#5C6163" //"#BDBEBF"
            Canvas{
                anchors.fill: parent
                anchors.margins: 0
                visible: control.checked;
                onPaint: {
                    var ctx = getContext("2d");
                    ctx.save();
                    ctx.strokeStyle = "#FFFFFF"
                    ctx.lineWidth = 2;
                    ctx.beginPath();
                    ctx.moveTo(5,height/2);
                    ctx.lineTo(width/2 , height-5);
                    ctx.moveTo(width/2, height-5);
                    ctx.lineTo(width -5, 5);
                    ctx.stroke();
                    ctx.restore();
                }
            }
        }
    }
}
