import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.12

Rectangle{
    property alias imgUrl: img.source
    property alias title: titleTxt.fontText
    property bool isShadow: false
    property color shadowColor : "#19191B"

    property int radiusClipAlign: 0 //0: 不需要切; 1: top 两个角；2：bottom 两个角
    width: 278
    height: 32
    color:"#4b4b4d"

    Rectangle
    {
        width: 5
        height : 5
        anchors.left: parent.left
        color:"#4b4b4d"
        visible: radiusClipAlign !==0
        Component.onCompleted:
        {
            if(radiusClipAlign ===1)
            {
                anchors.bottom = parent.bottom
            }
            else if(radiusClipAlign ===2)
            {
                anchors.top = parent.top
            }
        }
    }

    Rectangle
    {
        width: 5
        height : 5
        anchors.right: parent.right
        color:"#4a4a4f"
        visible: radiusClipAlign !==0
        Component.onCompleted:
        {
            if(radiusClipAlign ===1)
            {
                anchors.bottom = parent.bottom
            }
            else if(radiusClipAlign ===2)
            {
                anchors.top = parent.top
            }
        }
    }

    Image{
        id:img
        source: ""
        sourceSize.width: 12
        sourceSize.height: 12
        anchors.left: parent.left
        anchors.leftMargin: 9
        anchors.verticalCenter: parent.verticalCenter
    }

    CusText{
        id:titleTxt
        anchors.left: img.right
        anchors.leftMargin: 8
        anchors.verticalCenter: parent.verticalCenter
        fontPixelSize: 14
        hAlign: 0
        fontColor: "#ffffff"
        fontWeight: Font.Medium
    }
    layer.enabled: isShadow
    layer.effect:
        DropShadow {
        transparentBorder: true
        verticalOffset: 4
//        samples: 9
        color: shadowColor
    }
}
