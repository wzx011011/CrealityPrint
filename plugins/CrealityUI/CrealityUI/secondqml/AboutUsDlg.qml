import QtQuick 2.10
import QtQuick.Controls 2.12
import CrealityUI 1.0
import "qrc:/CrealityUI"
BasicDialog
{
    id: idDialog
    width: 730 * screenScaleFactor
    height: (Constants.languageType == 0 ? 450 : 400) *  screenScaleFactor //mac中文字显示差异，需多留空白
    titleHeight : 30 * screenScaleFactor
    property int spacing: 5
    title: qsTr("About Us")
    property string version:""
    property string website: ""
    property string telnumber: "+86 755-8523 4565"
    property string email: "teamcloud@creality.com"

    onVisibleChanged:
    {
        console.log("Constants.languageType: " + Constants.languageType)
    }
    
    Column
    {
        y:60//50
        width: parent.width
        height: parent.height-20
        spacing:20
        Item
        {
    //        color: "transparent"
            width: parent.width
            height: 28
            Row
            {
                id:idIconRect
                width: 35 + idLogoName.width
                height: 28
                leftPadding: 25//(parent.width - idIconRect.width )/2
                spacing: 5//10
                Image {
                    id: idImage
                    width: 25
                    height: 28
                    source: "qrc:/UI/photo/LOGO.png"
                }
                StyledLabel {
                    id: idLogoName
                    text: "Creality Print"
                    width: contentWidth
                    height: 28
                    verticalAlignment: Qt.AlignVCenter
                    font.family: Constants.labelFontFamily
                }
            }
        }

        StyledLabel {
            id: textEdit
            x:25
            text: qsTr("Founded in 2014, Shenzhen Creality 3D Technology Co., Ltd. is the first R & D manufacturer in China to enter the 3D printing industry and is committed to building a full range of 3D printing solutions, whose business field mainly focuses on the R & D, manufacturing and sales of 3D printers, extending to 3D printing accessories, 3D printing filament, 3D education curriculum system, 3D printing services, etc.  At present, it has become a leading high-tech enterprise in China's 3D printing industry, with its R & D strength, production, quality and services all leading the industry.")
            height: contentHeight + 20
            width:idDialog.width - 55
            wrapMode: TextEdit.Wrap
        }

        Column
        {
            topPadding: -10
            leftPadding: 25//(idDialog.width - 380 ) /2
            spacing: 8//10

            StyledLabel
            {
               id: idVersion
               width:380 * screenScaleFactor
               horizontalAlignment: Text.LeftToRight
               text: "Creality Print " + version

               wrapMode: Label.WordWrap
                font.family: Constants.labelFontFamily
               //font.pixelSize:Constants.labelFontPixelSize  // panelFontSize 14
            }


            StyledLabel
            {
               id: idCopyRight
               width:600 * screenScaleFactor//380
               horizontalAlignment: Text.LeftToRight
               //版权归深圳市创想三维科技有限公司，保留所有权利
               text: qsTr("Copyright©2014-2024 Shenzhen Creality 3D technology company, LTD. All rights reserved.")
               wrapMode: Text.WordWrap
//               color: "black"
               //font.pixelSize:Constants.labelFontPixelSize  // panelFontSize
               font.family: Constants.labelFontFamily
            }

            StyledLabel
            {
               id: idWebsite
               width:380 * screenScaleFactor
               horizontalAlignment: Text.LeftToRight
               text: qsTr("Website:") + "<a href=' " +website +"'>" + website +  "</a> "
               wrapMode: Label.WordWrap
//               color:  "black"
               //font.pixelSize:Constants.labelFontPixelSize  // panelFontSize
                font.family: Constants.labelFontFamily
               MouseArea
               {
                   id:idMouse
                    anchors.fill: parent
                    anchors.leftMargin: 30
                    onClicked:
                    {
                        Qt.openUrlExternally(website)
                    }
               }
            }

            StyledLabel
            {
               id: idTel
               width:380 * screenScaleFactor
               horizontalAlignment: Text.LeftToRight
               text: qsTr("Tel:")  + telnumber
               wrapMode: Label.WordWrap
               //font.pixelSize:Constants.labelFontPixelSize  // panelFontSize
               font.family: Constants.labelFontFamily
            }
            StyledLabel
            {
               id: idEMail
               width:380
               horizontalAlignment: Text.LeftToRight
               text: qsTr("Email:")  + email
               wrapMode: Label.WordWrap
               //font.pixelSize:Constants.labelFontPixelSize
               font.family: Constants.labelFontFamily
            }
        }

        Item {
            width: (idDialog.width - 14) * screenScaleFactor
            height : 2
            
            Rectangle {
                // anchors.left: idCol.left
                // anchors.leftMargin: -10
                x: 7
                width:parent.width > parent.height ?  parent.width : 2
                height: parent.width > parent.height ?  2 : parent.height
                color: Constants.splitLineColor
                Rectangle {
                    width: parent.width > parent.height ? parent.width : 1
                    height: parent.width > parent.height ? 1 : parent.height
                    color: Constants.splitLineColorDark
                }
            }
        }

        Row
        {
            id: idBtnrow
            spacing: 10
            width: 395 * screenScaleFactor
            height: 30 * screenScaleFactor
            x: 230
            BasicButton {
                id: basicComButton
                width: 125 * screenScaleFactor
                height: 28 * screenScaleFactor
                btnRadius:3
                btnBorderW:0
                defaultBtnBgColor: Constants.profileBtnColor
                hoveredBtnBgColor: Constants.profileBtnHoverColor
                text: qsTr("Portions Copyright")
                onSigButtonClicked:
                {
                    idcopyrihgt.show()
                }
            }

            BasicButton {
                id: basicComButton1
                width: 125 * screenScaleFactor
                height: 28 * screenScaleFactor
                btnRadius:3
                btnBorderW:0
                defaultBtnBgColor: Constants.profileBtnColor
                hoveredBtnBgColor: Constants.profileBtnHoverColor
                text: qsTr("Close")
                onSigButtonClicked:
                {
                    idDialog.close()
                }
            }
        }

    }

    CopyRightDlg{
        id: idcopyrihgt
    }
}
