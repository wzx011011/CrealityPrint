import QtQuick 2.10
import QtQuick.Controls 2.12
import CrealityUI 1.0
import "qrc:/CrealityUI"
BasicDialog
{
    id: idDialog
    width: 730//440
    height: 450 //mac中文字显示差异，需多留空白
    titleHeight : 30
    title: qsTr("Portions Copyright")
    property var fontSize:12
    Column
    {
        y:60//50
        width: parent.width
        height: parent.height-50
        spacing:20
        StyledLabel
        {
            width:600
            x:25
            horizontalAlignment: Text.LeftToRight
            text: qsTr("The license agreement for all of the following programs (libraries) is part of the Application License Agreement:")
            wrapMode: Label.WordWrap
            font.pixelSize: fontSize
            font.family: Constants.labelFontFamily
        }

        BasicScrollView {
            id: idBasicScrollView
            x:25
            width: 680//350
            height : 240//80/*120*/
            hpolicy: ScrollBar.AlwaysOff
            //vpolicy: ScrollBar.AsNeeded
            clip : true
            Column {
                spacing:5
                StyledLabel
                {
                    width:380
                    horizontalAlignment: Text.LeftToRight
                    text: "Openssl"
                    wrapMode: Label.WordWrap
                    font.pixelSize: Constants.panelLargeFontPixelSize//16
                    font.family: Constants.panelFontFamily
                    font.bold: true
                }
                StyledLabel
                {
                    width:600
                    horizontalAlignment: Text.LeftToRight
                    text: "Copyright© 1995-1998 Eric Young (eay@cryptsoft.com) All rights reserved."
                    wrapMode: Label.WordWrap
                    font.pixelSize:fontSize
                    font.family: Constants.labelFontFamily
                }

                Item{
                    width:600
                    height:5
                }

                StyledLabel
                {
                    width:380
                    horizontalAlignment: Text.LeftToRight
                    text: "Qhull"
                    wrapMode: Label.WordWrap
                    font.pixelSize: Constants.panelLargeFontPixelSize//16
                    font.family: Constants.panelFontFamily
                    font.bold: true
                }
                StyledLabel
                {
                    width:600
                    horizontalAlignment: Text.LeftToRight
                    text: "Copyright© 1993-2020  C.B. Barber, Arlington, MA "
                    wrapMode: Label.WordWrap
                    font.pixelSize:fontSize
                    font.family: Constants.labelFontFamily
                }

                Item{
                    width:600
                    height:5
                }

                StyledLabel
                {
                    width:380
                    horizontalAlignment: Text.LeftToRight
                    text: "Clipper"
                    wrapMode: Label.WordWrap
                    font.pixelSize: Constants.panelLargeFontPixelSize//16
                    font.family: Constants.panelFontFamily
                    font.bold: true
                }
                StyledLabel
                {
                    width:600
                    horizontalAlignment: Text.LeftToRight
                    text: "Copyright© 2010-2015 Angus Johnson"
                    wrapMode: Label.WordWrap
                    font.pixelSize:fontSize
                    font.family: Constants.labelFontFamily
                }

                Item{
                    width:600
                    height:5
                }

                StyledLabel
                {
                    width:380
                    horizontalAlignment: Text.LeftToRight
                    text: "Zlib"
                    wrapMode: Label.WordWrap
                    font.pixelSize: Constants.panelLargeFontPixelSize//16
                    font.family: Constants.panelFontFamily
                    font.bold: true
                }
                StyledLabel
                {
                    width:600
                    horizontalAlignment: Text.LeftToRight
                    text: "Copyright© 1995-2005 Jean-loup Gailly, Mark Adler"
                    wrapMode: Label.WordWrap
                    font.pixelSize:fontSize
                    font.family: Constants.labelFontFamily
                }

                Item{
                    width:600
                    height:5
                }

                StyledLabel
                {
                    width:380
                    horizontalAlignment: Text.LeftToRight
                    text: "Curl"
                    wrapMode: Label.WordWrap
                    font.pixelSize: Constants.panelLargeFontPixelSize//16
                    font.family: Constants.panelFontFamily
                    font.bold: true
                }
                StyledLabel
                {
                    width:600
                    horizontalAlignment: Text.LeftToRight
                    text: "Copyright© 1996 - 2022, Daniel Stenberg, daniel@haxx.se, and many contributors"
                    wrapMode: Label.WordWrap
                    font.pixelSize:fontSize
                    font.family: Constants.labelFontFamily
                }

                Item{
                    width:600
                    height:5
                }

                StyledLabel
                {
                    width:380
                    horizontalAlignment: Text.LeftToRight
                    text: "Boost"
                    wrapMode: Label.WordWrap
                    font.pixelSize: Constants.panelLargeFontPixelSize//16
                    font.family: Constants.panelFontFamily
                    font.bold: true
                }
                StyledLabel
                {
                    width:600
                    horizontalAlignment: Text.LeftToRight
                    text: "Copyright© 1998-2005 Beman Dawes, David Abrahams; 2004 - 2007 Rene Rivera"
                    wrapMode: Label.WordWrap
                    font.pixelSize:fontSize
                    font.family: Constants.labelFontFamily
                }

                Item{
                    width:600
                    height:5
                }

                StyledLabel
                {
                    width:380
                    horizontalAlignment: Text.LeftToRight
                    text: "Nlopt"
                    wrapMode: Label.WordWrap
                    font.pixelSize: Constants.panelLargeFontPixelSize//16
                    font.family: Constants.panelFontFamily
                    font.bold: true
                }
                StyledLabel
                {
                    width:600
                    horizontalAlignment: Text.LeftToRight
                    text: "Copyright© 2007-2014 Massachusetts Institute of Technology"
                    wrapMode: Label.WordWrap
                    font.pixelSize:fontSize
                    font.family: Constants.labelFontFamily
                }

                Item{
                    width:600
                    height:5
                }

                StyledLabel
                {
                    width:380
                    horizontalAlignment: Text.LeftToRight
                    text: "Opencv"
                    wrapMode: Label.WordWrap
                    font.pixelSize: Constants.panelLargeFontPixelSize//16
                    font.family: Constants.panelFontFamily
                    font.bold: true
                }
                StyledLabel
                {
                    width:600
                    horizontalAlignment: Text.LeftToRight
                    text: "Copyright© 2022 , OpenCV team"
                    wrapMode: Label.WordWrap
                    font.pixelSize:fontSize
                    font.family: Constants.labelFontFamily
                }

                Item{
                    width:600
                    height:5
                }

                StyledLabel
                {
                    width:380
                    horizontalAlignment: Text.LeftToRight
                    text: "Nest2d"
                    wrapMode: Label.WordWrap
                    font.pixelSize: Constants.panelLargeFontPixelSize//16
                    font.family: Constants.panelFontFamily
                    font.bold: true
                }
                StyledLabel
                {
                    width:600
                    horizontalAlignment: Text.LeftToRight
                    text: "Copyright© 2007 Free Software Foundation, Inc."
                    wrapMode: Label.WordWrap
                    font.pixelSize:fontSize
                    font.family: Constants.labelFontFamily
                }

                Item{
                    width:600
                    height:5
                }

                StyledLabel
                {
                    width:380
                    horizontalAlignment: Text.LeftToRight
                    text: "Eigen"
                    wrapMode: Label.WordWrap
                    font.pixelSize: Constants.panelLargeFontPixelSize//16
                    font.family: Constants.panelFontFamily
                    font.bold: true
                }
                StyledLabel
                {
                    width:600
                    horizontalAlignment: Text.LeftToRight
                    text: "Copyright© 2008 Gael Guennebaud <gael.guennebaud@inria.fr> \nCopyright© 2008 Benoit Jacob <jacob.benoit.1@gmail.com>"
                    wrapMode: Label.WordWrap
                    font.pixelSize:fontSize
                    font.family: Constants.labelFontFamily
                }

                Item{
                    width:600
                    height:5
                }

                StyledLabel
                {
                    width:380
                    horizontalAlignment: Text.LeftToRight
                    text: "Zip"
                    wrapMode: Label.WordWrap
                    font.pixelSize: Constants.panelLargeFontPixelSize//16
                    font.family: Constants.panelFontFamily
                    font.bold: true
                }
                StyledLabel
                {
                    width:600
                    horizontalAlignment: Text.LeftToRight
                    text: "Copyright© 1990-2008 Info-ZIP"
                    wrapMode: Label.WordWrap
                    font.pixelSize:fontSize
                    font.family: Constants.labelFontFamily
                }
            }
        }

        Item {
            width: idDialog.width - 14
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
            width: 395
            height: 30
            x: 290

            BasicButton {
                id: basicComButton1
                width: 125
                height: 28
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
}
