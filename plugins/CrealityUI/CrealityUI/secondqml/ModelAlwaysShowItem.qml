import QtQuick 2.5
import QtQuick.Controls 2.12
import "qrc:/CrealityUI"
import ".."
import "../qml"

Button{
    id: idModelAlwaysBtnImage
    width: 83.6*screenScaleFactor
    height: 68.5*screenScaleFactor

    property var isTableShow: false
    signal sigButtonClicked(var isMoreBtn, var id, var name, var count, var author, var avtar, var ctime)

    property var btnNameText: "name"
    property var btnAuthorText: "author"
    property var btnModelImage: ""
    property var btnAvtarImage: ""
    property var modelGroupId: ""
    property var modelCount: 0
    property var modelTime: ""

    MouseArea
    {
        anchors.fill: parent
        hoverEnabled:true
        onClicked:
        {
            sigButtonClicked(isTableShow, modelGroupId, btnNameText, modelCount, btnAuthorText, btnAvtarImage, modelTime)
        }
    }

    background: Item{
        Rectangle{
            width: idModelAlwaysBtnImage.width
            height: idModelAlwaysBtnImage.height
            radius: 10
            border.color: hovered ? "#1E9BE2" : Constants.modelAlwaysItemBorderColor
            border.width: hovered ? 2 : 1
            color: isTableShow ? Constants.modelAlwaysMoreBtnBgColor : "transparent"
            BaseCircularImage{
                x: hovered ? 2 : 1
                y: hovered ? 2 : 1
                width: hovered ?  (parent.width - 4) : (parent.width - 2)
                height: hovered ? (parent.height - 4) : (parent.height - 2)
                radiusImg: 10
                visible: !isTableShow
                img_src: btnModelImage
                isPreserveAspectCrop: true
            }
            StyledLabel{
                x: 1*screenScaleFactor
                y: 1*screenScaleFactor
                width: parent.width - 2
                height: parent.height -2 
                visible: isTableShow
                font.pointSize: Constants.labelLargeFontPointSize
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment:Text.AlignHCenter
                color: "#333333"
                text: qsTr("More>>")
            }
        }
    }
}
