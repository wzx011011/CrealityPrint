import QtQuick 2.5
import QtQuick.Controls 2.12
import "qrc:/CrealityUI"
Button {
    id: cusButtonImage
    property bool isRound: false
    width: 24*screenScaleFactor
    height: 24*screenScaleFactor
    checkable: true
    //signal sigBtnClicked(var key)

    //property var keystr: 0
    property alias tipText: toolTip.text
    property alias tipItem: toolTip
    property alias tipVisible: toolTip.visible
    property alias tipDelay: toolTip.delay
    property alias tipTimeout: toolTip.timeout
    property bool selected: false
    property alias radius: backRec.radius
    property string btnImgNormal
    property string btnImgHovered
    property string btnImgPressed
    property string btnImgDisbaled
    property color defaultBtnBgColor:"#4b4b4d"
    property color hoveredBtnBgColor:"#68686b"
    property color selectedBtnBgColor:"#1e9be2"
    property alias sourceSize: backImage.sourceSize

    property string btnImgUrl: {
        if (!cusButtonImage.enabled) {
            return btnImgDisbaled
        } else if (cusButtonImage.pressed || selected) {
            return btnImgPressed
        } else if (cusButtonImage.hovered) {
            return btnImgHovered
        } else {
            return btnImgNormal
        }
    }

    background: Rectangle {
        id:backRec
        radius: isRound ? cusButtonImage.width/2 : 0
        color: cusButtonImage.checked ? selectedBtnBgColor : (cusButtonImage.hovered ? hoveredBtnBgColor : defaultBtnBgColor)
//        clip: true
        Image {
            id: backImage
            anchors.centerIn: parent
            source: btnImgUrl
            asynchronous: true
//            sourceSize.width: parent.width
//            sourceSize.height: parent.height
            mipmap: true
            smooth: true
            cache: false
            fillMode: Image.PreserveAspectFit
        }
    }
    ToolTip {
          id: toolTip
          visible: (cusButtonImage.hovered && String(tipText).length)
          delay: 500
          timeout: 500
          width: idStyledLabel.contentWidth + 20
          //height: 35
          background: Rectangle {
              anchors.fill: parent
              color: "transparent"
          }

          contentItem: Item{
              //visible: newStyle
              Canvas {
                  id: canvasId
                  x: -3
                  property color triangleColor: "#EDF8E8"
                  width: 14*screenScaleFactor
                  height: 8*screenScaleFactor
                  onPaint: {
                      var context = getContext("2d")
                      context.lineWidth = 1
                      context.strokeStyle = "#EDF8E8"
                      context.fillStyle = triangleColor
                      context.beginPath();
                      context.moveTo(0, canvasId.height)
                      context.lineTo(canvasId.width/2, 0);
                      context.lineTo(canvasId.width, canvasId.height);
                      context.lineTo(0, canvasId.height);
                      context.closePath();
                      context.fill()
                      context.stroke();
                  }
              }


              Rectangle {
                  id: tipContentItem
                  width: idStyledLabel.contentWidth + 20*screenScaleFactor
                  height: 30*screenScaleFactor
                  radius: 6
                  color: "#EDF8E8"
                  anchors.top : canvasId.bottom
                  anchors.horizontalCenter: canvasId.horizontalCenter
                  //anchors.leftMargin: idStyledLabel.width /2

                  StyledLabel {
                      id: idStyledLabel
                      x: 10*screenScaleFactor
                      color: "#000000"
                      text: tipText
                      //width:12
                      height: 25*screenScaleFactor
                      anchors.verticalCenter:tipContentItem.verticalCenter
                      verticalAlignment: Qt.AlignVCenter
                      horizontalAlignment: Qt.AlignHCenter
                  }
              }
          }
      }
}
