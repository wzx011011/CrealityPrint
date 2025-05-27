import QtQuick 2.0
import QtQuick.Window 2.1
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.12
Window {
    id: splash
    color: "transparent"
    title: qsTr("Splash Window")
    modality: Qt.ApplicationModal
    flags: Qt.SplashScreen
    property int timeoutInterval: 2000
    signal timeout
    x: (Screen.width - splash.width) / 2
    y: (Screen.height - splash.height) / 2

    width: 876 * screenScaleFactor//splashImage.width
    height: 480 * screenScaleFactor//splashImage.height
    //property alias versionText: idVersion.text

    Rectangle{
        id: mainWindowRext
        color: "transparent"
        anchors.fill: parent
        Image {
            id: splashImage
            source: "qrc:/UI/images/splash.png"
            width: 876 * screenScaleFactor
            height: 480 * screenScaleFactor
            // MouseArea {
            //     anchors.horizontalCenter: parent.horizontalCenter
            //     anchors.verticalCenter: parent.verticalCenter
            //     onClicked: Qt.quit()
            // }

            TextEdit {//by TCJ
                anchors.left: parent.left
                anchors.leftMargin: 20* screenScaleFactor
                anchors.top: parent.top
                anchors.topMargin: 200* screenScaleFactor
                id: idTextLine4
                text: qsTr("Shenzhen Creality 3D Technology Co., Ltd. is an internationally renowned 3D printer manufacturer, focusing on the R & D and production of 3D printers.")
                color: "#a3c2d4"
                font.pointSize:Constants.labelFontPointSize
                font.family: "Microsoft YaHei UI"//"Source Han Sans CN"
                width: 174 * screenScaleFactor
                height: 60 * screenScaleFactor
                wrapMode: TextEdit.Wrap
                textFormat: TextEdit.AutoText
            }
        }
    }

    // Rectangle
    // {
    //     id: mainWindowRext
    //     color: "transparent"
    //     anchors.fill: parent

    //     anchors.margins: 5
    //     radius:15

    //     Row{
    //         anchors.fill: parent
    //         Rectangle
    //         {
    //             id: idRect
    //             height: 480
    //             width:260
    //             color: "#0066A0"

    //             Image{
    //                 id: idSplashLogo
    //                 width: 63
    //                 height: 63
    //                 anchors.top: parent.top
    //                 anchors.topMargin: 20
    //                 anchors.left:parent.left
    //                 anchors.leftMargin: 20
    //                 source: "qrc:/UI/images/splashLogo.png"

    //             }

    //             Label {
    //                 id: idTextLine1
    //                 text: qsTr("Creality 3D Technology Co.,Ltd.")
    //                 color: "#a3c2d4"
    //                 font.pixelSize: 14
    //                 font.family: "Microsoft YaHei UI"//"Source Han Sans CN"
    //                 width: 201
    //                 anchors.top: idSplashLogo.bottom
    //                 anchors.topMargin: 13/*10*/
    //                 anchors.left: idSplashLogo.left
    //             }

    //             Label {
    //                 id: idTextLine2
    //                 text: qsTr("Creality Print")
    //                 color: "#FFFFFF"
    //                 font.pixelSize: 24
    //                 font.family: "Microsoft YaHei UI"//"Source Han Sans CN"
    // //                font.weight: Font.Regular
    //                 width: 121
    //                 anchors.top: idTextLine1.bottom
    //                 anchors.topMargin: 5/*10*/
    //                 anchors.left: idSplashLogo.left
    //             }

    //             Rectangle
    //             {
    //                 anchors.top: idTextLine2.bottom
    //                 anchors.topMargin: 15
    //                 anchors.left: idSplashLogo.left
    //                 width: 26
    //                 height: 2
    //                 color: "#4d85a6"
    //             }

    //             Label {
    //                 id: idTextLine3
    //                 text: qsTr("Loading...")
    //                 color: "#FFFFFF"
    //                 font.pixelSize: 14
    //                 font.family: "Microsoft YaHei UI"//"Source Han Sans CN"
    //                 width: 81
    //                 anchors.bottom: idTextLine4.top
    //                 anchors.bottomMargin: 5
    //                 anchors.left: idSplashLogo.left
    //             }

    //             TextEdit {//by TCJ
    //                 id: idTextLine4
    //                 text: qsTr("Shenzhen Creality 3D Technology Co., Ltd. is an internationally renowned 3D printer manufacturer, focusing on the R & D and production of 3D printers.")
    //                 color: "#a3c2d4"
    //                 font.pixelSize: 10
    //                 font.family: "Microsoft YaHei UI"//"Source Han Sans CN"
    //                 width: 220/*241*/
    //                 height: 44/*80*/
    //                 // anchors.top: idTextLine3.bottom
    //                 // anchors.topMargin: 15
    //                 // anchors.left: idSplashLogo.left
    //                 anchors.bottomMargin: 140
    //                 anchors.bottom: idSplashCloudLogo.top
    //                 anchors.left: idSplashLogo.left
    //                 wrapMode: TextEdit.Wrap
    //                 textFormat: TextEdit.AutoText
    //             }

    //             Image{
    //                 id: idSplashCloudLogo
    //                 width: 111
    //                 height: 38
    //                 anchors.bottom: idTextLine5.top
    //                 anchors.bottomMargin: 5
    //                 anchors.leftMargin: 5
    //                 anchors.left: idSplashLogo.left
    //                 source: "qrc:/UI/images/splashCloudLogo.png"

    //             }

    //             Label {
    //                 id: idTextLine5
    //                 text: qsTr("www.crealitycloud.com")
    //                 color: "#a3c2d4"
    //                 font.pixelSize: 14
    //                 font.family: "Microsoft YaHei UI"//"Source Han Sans CN"
    //                 width: 81

    //                 anchors.bottomMargin: 20
    //                 anchors.bottom: parent.bottom
    //                 anchors.left: idSplashLogo.left
    //             }

    //             // StyledLabel {
    //             //     id: idVersion
    //             //     text: qsTr("V0.3.4.1 Beta")
    //             //     color: "#666666"
    //             //     font.pixelSize: 12
    //             //     font.family: "Microsoft YaHei UI"//"Source Han Sans CN"
    //             //     width: 68
    //             //     anchors.bottomMargin: 23
    //             //     anchors.bottom: parent.bottom
    //             //     anchors.left: idSplashLogo.left
    //             // }
    //         }

    //         Image {
    //             id: splashImage
    //             source: "qrc:/UI/images/splash.png"
    //             width: 616//splashImage.width
    //             height: 480//splashImage.height
    //             MouseArea {
    //                 //anchors.fill: parent
    //                 anchors.horizontalCenter: parent.horizontalCenter
    //                 anchors.verticalCenter: parent.verticalCenter
    //                 onClicked: Qt.quit()
    //             }
    //         }
    //     }
    // }

    // DropShadow {
    //     anchors.fill: mainWindowRext
    //     radius: 12
    //     samples: 25
    //     source: mainWindowRext
    //     color:  "#14201D"//"#cecece"
    // }
    //Timer {
    //    interval: timeoutInterval; running: true; repeat: false
    //    onTriggered: {
    //        //visible = false
    //        splash.timeout()
    //    }
    //}
    Component.onCompleted: visible = true
}
