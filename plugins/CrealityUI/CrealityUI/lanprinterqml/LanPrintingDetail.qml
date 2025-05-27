import QtQuick 2.13
import QtQuick.Window 2.13
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.13
import QtQml 2.13
import QtMultimedia 5.12
//import "qrc:/CrealityUI"
//import CrealityUI 1.0
import QtQuick.Dialogs 1.3
import com.cxsw.SceneEditor3D 1.0
import QtGraphicalEffects 1.12
import QtQuick.Shapes 1.13
import QtCharts 2.13
import "../secondqml"
import "../qml"

Rectangle {
    id: basicDlg
    visible: true
    color: "#1C1C1D"
    width: parent.width
    height: parent.height

    property var deviceItem
    //property var deviceModel
    property string deviceID: ""

    function second2String(sec) {
        var hours = Math.floor(sec / 3600)
        var minutes = Math.floor(sec % 3600 / 60)
        var seconds = Math.floor(sec % 3600 % 60)
        return `${hours}h ${minutes}m ${seconds}s`
    }
    function startPlayVideo() {
        var ipAddr = deviceItem?deviceItem.pcIpAddr:""
        var hasCamera = deviceItem?deviceItem.pcHasCamera:false

        if(ipAddr !== "" && hasCamera)
        {
            var urlStr = "rtsp://" + ipAddr + "/ch0_0"
            cameraItem.showPopup = true
            idVideoPlayer.start(urlStr)
        }
    }
    function stopPlayVideo() {
        idVideoPlayer.stop()
    }
    function showChartView() {
        //Init
        idRefreshTimer.stop()
        idRefreshTimer.count = 0
        bedTempSeries.upperSeries.clear()
        nozzleTempSeries.upperSeries.clear()
        //Start
        axis_time.min = printerListModel.cSourceModel.getCurrentDateTime(true)
        axis_time.max = printerListModel.cSourceModel.getCurrentDateTime(false)
        idRefreshTimer.start()
    }
    function updateShowData()
    {
        //console.log("deviceID", deviceID)
        deviceItem = printerListModel.cSourceModel.getDeviceItem(deviceID)
        //deviceModel = deviceItem?deviceItem.gCodeFileList:[]
        showChartView()
    }

//    Connections {
//        target: printerListModel.cSourceModel
//        onDataChanged: if(visible) deviceItem = printerListModel.cSourceModel.getDeviceItem(deviceID)
//    }

BasicScrollView {
    clip: true
    width: basicDlg.width
    height: basicDlg.height
    anchors.centerIn: parent
    hpolicyVisible: contentWidth > width
    vpolicyVisible: contentHeight > height
    hpolicyindicator: Rectangle {
        color: "#737378"
        radius: height / 2
        implicitWidth: 630 * screenScaleFactor
        implicitHeight: 6 * screenScaleFactor
    }
    vpolicyindicator: Rectangle {
        color: "#737378"
        radius: width / 2
        implicitWidth: 6 * screenScaleFactor
        implicitHeight: 630 * screenScaleFactor
    }

    ColumnLayout {
        spacing: 0
        width: basicDlg.width
        height: basicDlg.height

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumHeight: 10 * screenScaleFactor
            Layout.maximumHeight: 20 * screenScaleFactor
        }

        RowLayout
        {
            spacing: 0

            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumWidth: 10 * screenScaleFactor
                Layout.maximumWidth: 20 * screenScaleFactor
            }

            ColumnLayout {
                spacing: 0
                Layout.alignment: Qt.AlignTop

                ColumnLayout {
                    spacing: 0
                    id: printerItem
                    property bool showPopup: true
                    property bool isPrinting: deviceItem?(deviceItem.pcPrinterState === 1):false
                    property bool isSuspending: deviceItem?(deviceItem.pcPrinterState === 5):false
                    property bool isWlanPrinting: deviceItem?(deviceItem.pcPrinterMethod === 1):false

                    CusRoundedBg {
                        leftTop: true
                        rightTop: true
                        clickedable: false
                        borderWidth: 1
                        borderColor: "#0F0F10"
                        color: "#37373B"
                        radius: 5

                        Layout.fillWidth: true
                        Layout.minimumWidth: 626 * screenScaleFactor
                        Layout.maximumWidth: 932 * screenScaleFactor
                        Layout.preferredHeight: 40 * screenScaleFactor

                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 20 * screenScaleFactor
                            anchors.rightMargin: 20 * screenScaleFactor
                            spacing: 10 * screenScaleFactor

                            Image {
                                Layout.preferredWidth: 14
                                Layout.preferredHeight: 14
                                source: "qrc:/UI/photo/printerIcon.png"
                            }

                            Text {
                                Layout.preferredWidth: contentWidth * screenScaleFactor
                                Layout.preferredHeight: 14 * screenScaleFactor

                                verticalAlignment: Text.AlignVCenter
                                font.weight: Font.Bold
                                font.family: Constants.mySystemFont.name
                                font.pointSize: Constants.labelFontPointSize
                                text: qsTr("Printing Infomation")
                                color: "white"
                            }

                            Item {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                            }

                            Rectangle {
                                radius: height / 2
                                opacity: enabled ? 1.0 : 0.7
                                enabled: printerItem.isWlanPrinting
                                color: btnStateArea.containsMouse?"#67676D":"#515157"
                                visible: printerItem.isPrinting || printerItem.isSuspending

                                Layout.preferredWidth: 120 * screenScaleFactor
                                Layout.preferredHeight: 24 * screenScaleFactor

                                Row {
                                    height: parent.height
                                    anchors.centerIn: parent
                                    spacing: 10 * screenScaleFactor

                                    Image {
                                        anchors.verticalCenter: parent.verticalCenter
                                        source: `qrc:/UI/photo/${printerItem.isSuspending?"continue":"pause"}_detail.png`
                                    }

                                    Text {
                                        anchors.verticalCenter: parent.verticalCenter
                                        verticalAlignment: Text.AlignVCenter
                                        font.weight: Font.Medium
                                        font.family: Constants.mySystemFont.name
                                        font.pointSize: Constants.labelFontPointSize
                                        text: printerItem.isSuspending?qsTr("Continue"):qsTr("Pause")
                                        color: "white"
                                    }
                                }

                                MouseArea {
                                    id: btnStateArea
                                    hoverEnabled: true
                                    anchors.fill: parent
                                    cursorShape: containsMouse?Qt.PointingHandCursor:Qt.ArrowCursor

                                    onClicked: {
                                        var ipAddr = deviceItem?deviceItem.pcIpAddr:""
                                        if(ipAddr !== "") {
                                            var command = printerItem.isSuspending?LanPrinterListLocal.PrintControlType.PRINT_CONTINUE:LanPrinterListLocal.PrintControlType.PRINT_PAUSE
                                            printerListModel.cSourceModel.sendUIControlCmdToDevice(ipAddr, command, " ")
                                        }
                                    }
                                }
                            }

                            Rectangle {
                                radius: height / 2
                                opacity: enabled ? 1.0 : 0.7
                                enabled: printerItem.isWlanPrinting
                                color: btnStopArea.containsMouse?"#67676D":"#515157"
                                visible: printerItem.isPrinting || printerItem.isSuspending

                                Layout.preferredWidth: 120 * screenScaleFactor
                                Layout.preferredHeight: 24 * screenScaleFactor

                                Row {
                                    height: parent.height
                                    anchors.centerIn: parent
                                    spacing: 10 * screenScaleFactor

                                    Image {
                                        anchors.verticalCenter: parent.verticalCenter
                                        source: "qrc:/UI/photo/stop_detail.png"
                                    }

                                    Text {
                                        anchors.verticalCenter: parent.verticalCenter
                                        verticalAlignment: Text.AlignVCenter
                                        font.weight: Font.Medium
                                        font.family: Constants.mySystemFont.name
                                        font.pointSize: Constants.labelFontPointSize
                                        text: qsTr("Stop")
                                        color: "white"
                                    }

                                }

                                MouseArea {
                                    id: btnStopArea
                                    hoverEnabled: true
                                    anchors.fill: parent
                                    cursorShape: containsMouse?Qt.PointingHandCursor:Qt.ArrowCursor

                                    onClicked: {
                                        var ipAddr = deviceItem?deviceItem.pcIpAddr:""
                                        if(ipAddr !== "") printerListModel.cSourceModel.sendUIControlCmdToDevice(ipAddr, LanPrinterListLocal.PrintControlType.PRINT_STOP, " ")
                                    }
                                }
                            }

                            Item {
                                Layout.preferredWidth: 20 * screenScaleFactor
                                Layout.preferredHeight: 12 * screenScaleFactor

                                Image {
                                    anchors.centerIn: parent
                                    source: `qrc:/UI/photo/${printerItem.showPopup?"upArrow":"downArrow"}.png`
                                }

                                MouseArea {
                                    hoverEnabled: true
                                    anchors.fill: parent
                                    cursorShape: containsMouse?Qt.PointingHandCursor:Qt.ArrowCursor

                                    onClicked: {
                                        printerItem.showPopup = !printerItem.showPopup
                                        printerPopup.visible = printerItem.showPopup
                                    }
                                }
                            }
                        }
                    }

                    CusRoundedBg {
                        id: printerPopup
                        visible: true
                        leftBottom: true
                        rightBottom: true
                        clickedable: false
                        borderWidth: 1
                        borderColor: "#0F0F10"
                        color: "#28282B"
                        radius: 5

                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.minimumWidth: 626 * screenScaleFactor
                        Layout.maximumWidth: 932 * screenScaleFactor
                        Layout.minimumHeight: 120 * screenScaleFactor
                        Layout.maximumHeight: 178 * screenScaleFactor

                        RowLayout {
                            spacing: 0
                            anchors.fill: parent
                            anchors.topMargin: 10 * screenScaleFactor
                            anchors.bottomMargin: 10 * screenScaleFactor

                            Item {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.minimumWidth: 10 * screenScaleFactor
                                Layout.maximumWidth: 20 * screenScaleFactor
                            }

                            Rectangle {
                                radius: 5
                                color: "#19191B"
                                property string printerID: deviceItem?deviceItem.pcPrinterID:" "

                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.minimumWidth: 96 * screenScaleFactor
                                Layout.maximumWidth: 200 * screenScaleFactor
                                Layout.minimumHeight: 96 * screenScaleFactor
                                Layout.maximumHeight: 135 * screenScaleFactor

                                Image {
                                    width: 96
                                    height: 96
                                    smooth: true
                                    anchors.centerIn: parent
                                    source: `image://preview/icons/${parent.printerID}`
                                    sourceSize: Qt.size(width, height)
                                }

                                Image{
                                    width: parent.width
                                    height: parent.height / 2
                                    anchors.bottom: parent.bottom
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    source: "qrc:/UI/photo/gridbg.png"
                                }
                            }

                            Item {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.minimumWidth: 10 * screenScaleFactor
                                Layout.maximumWidth: 20 * screenScaleFactor
                            }

                            ColumnLayout {
                                spacing: 20 * screenScaleFactor

                                RowLayout {
                                    spacing: 10 * screenScaleFactor

                                    Text {
                                        Layout.preferredWidth: contentWidth * screenScaleFactor
                                        Layout.preferredHeight: 14 * screenScaleFactor

                                        verticalAlignment: Text.AlignVCenter
                                        font.weight: Font.Medium
                                        font.family: Constants.mySystemFont.name
                                        font.pointSize: Constants.labelFontPointSize
                                        text: qsTr("File") + "："
                                        color: "#7F7F80"
                                    }

                                    Item {
                                        Layout.fillWidth: true
                                        Layout.minimumWidth: 150 * screenScaleFactor
                                        Layout.maximumWidth: 300 * screenScaleFactor
                                        Layout.preferredHeight: 14 * screenScaleFactor

                                        Text {
                                            id: idGcodeName
                                            anchors.fill: parent
                                            verticalAlignment: Text.AlignVCenter
                                            font.weight: Font.Medium
                                            font.family: Constants.mySystemFont.name
                                            font.pointSize: Constants.labelFontPointSize
                                            text: deviceItem?deviceItem.pcGCodeName:"undefined"
                                            elide: Text.ElideMiddle
                                            color: "#CBCBCC"

                                            MouseArea {
                                                id: idGcodeNameArea
                                                hoverEnabled: true
                                                anchors.fill: parent
                                                acceptedButtons: Qt.NoButton
                                            }
                                        }

                                        ToolTip.text: idGcodeName.text
                                        ToolTip.visible: idGcodeNameArea.containsMouse && idGcodeName.truncated//elide set
                                    }
                                }

                                RowLayout {
                                    spacing: 10 * screenScaleFactor

                                    Text {
                                        Layout.preferredWidth: contentWidth * screenScaleFactor
                                        Layout.preferredHeight: 14 * screenScaleFactor

                                        verticalAlignment: Text.AlignVCenter
                                        font.weight: Font.Medium
                                        font.family: Constants.mySystemFont.name
                                        font.pointSize: Constants.labelFontPointSize
                                        text: qsTr("Layer") + "："
                                        color: "#7F7F80"
                                    }

                                    Text {
                                        property int curLayer: deviceItem?deviceItem.pcCurPrintLayer:0
                                        property int totalLayer: deviceItem?deviceItem.pcTotalPrintLayer:0

                                        Layout.preferredWidth: contentWidth * screenScaleFactor
                                        Layout.preferredHeight: 14 * screenScaleFactor

                                        verticalAlignment: Text.AlignVCenter
                                        font.weight: Font.Medium
                                        font.family: Constants.mySystemFont.name
                                        font.pointSize: Constants.labelFontPointSize
                                        text: `${curLayer} / ${totalLayer}`
                                        color: "#CBCBCC"
                                    }
                                }

                                RowLayout {
                                    spacing: 10 * screenScaleFactor

                                    Rectangle {
                                        radius: 1
                                        color: "#404044"
                                        id: newProgressBar
                                        property real progress: deviceItem?deviceItem.pcCurPrintProgress:0

                                        Layout.fillWidth: true
                                        Layout.minimumWidth: 150 * screenScaleFactor
                                        Layout.maximumWidth: 300 * screenScaleFactor
                                        Layout.preferredHeight: 2 * screenScaleFactor

                                        Rectangle {
                                            width: parent.width * parent.progress / 100.0
                                            height: parent.height
                                            anchors.left: parent.left
                                            anchors.verticalCenter: parent.verticalCenter
                                            color: "#1E9BE2"
                                            radius: 1
                                        }
                                    }

                                    Text {
                                        Layout.preferredWidth: contentWidth * screenScaleFactor
                                        Layout.preferredHeight: 14 * screenScaleFactor

                                        verticalAlignment: Text.AlignVCenter
                                        font.weight: Font.Medium
                                        font.family: Constants.mySystemFont.name
                                        font.pointSize: Constants.labelFontPointSize
                                        text: `${newProgressBar.progress}%`
                                        color: "white"
                                    }
                                }
                            }

                            Item {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.minimumWidth: 10 * screenScaleFactor
                            }

                            Rectangle {
                                color: "#414145"
                                Layout.fillHeight: true
                                Layout.minimumHeight: 50
                                Layout.maximumHeight: 100
                                Layout.preferredWidth: 1 * screenScaleFactor
                            }

                            Item {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.minimumWidth: 10 * screenScaleFactor
                            }

                            ColumnLayout {
                                spacing: 20 * screenScaleFactor

                                RowLayout {
                                    spacing: 10 * screenScaleFactor

                                    Text {
                                        Layout.preferredWidth: contentWidth * screenScaleFactor
                                        Layout.preferredHeight: 14 * screenScaleFactor

                                        verticalAlignment: Text.AlignVCenter
                                        font.weight: Font.Medium
                                        font.family: Constants.mySystemFont.name
                                        font.pointSize: Constants.labelFontPointSize
                                        text: qsTr("Print time") + "："
                                        color: "#7F7F80"
                                    }

                                    Text {
                                        Layout.preferredWidth: contentWidth * screenScaleFactor
                                        Layout.preferredHeight: 14 * screenScaleFactor

                                        verticalAlignment: Text.AlignVCenter
                                        font.weight: Font.Medium
                                        font.family: Constants.mySystemFont.name
                                        font.pointSize: Constants.labelFontPointSize
                                        text: deviceItem?second2String(deviceItem.pcTotalPrintTime):"undefined"
                                        color: "#CBCBCC"
                                    }
                                }

                                RowLayout {
                                    spacing: 10 * screenScaleFactor

                                    Text {
                                        Layout.preferredWidth: contentWidth * screenScaleFactor
                                        Layout.preferredHeight: 14 * screenScaleFactor

                                        verticalAlignment: Text.AlignVCenter
                                        font.weight: Font.Medium
                                        font.family: Constants.mySystemFont.name
                                        font.pointSize: Constants.labelFontPointSize
                                        text: qsTr("Time left") + "："
                                        color: "#7F7F80"
                                    }

                                    Text {
                                        Layout.preferredWidth: contentWidth * screenScaleFactor
                                        Layout.preferredHeight: 14 * screenScaleFactor

                                        verticalAlignment: Text.AlignVCenter
                                        font.weight: Font.Medium
                                        font.family: Constants.mySystemFont.name
                                        font.pointSize: Constants.labelFontPointSize
                                        text: deviceItem?second2String(deviceItem.pcLeftTime):"undefined"
                                        color: "#CBCBCC"
                                    }
                                }
                            }

                            Item {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.minimumWidth: 10 * screenScaleFactor
                                Layout.maximumWidth: 20 * screenScaleFactor
                            }
                        }
                    }
                }

                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: 8 * screenScaleFactor
                    Layout.maximumHeight: 16 * screenScaleFactor
                }

                ColumnLayout {
                    spacing: 0
                    id: controlItem
                    property bool showPopup: true
                    property bool isWlanPrinting: deviceItem?(deviceItem.pcPrinterMethod === 1):false
                    property bool controlEnabled: deviceItem?(deviceItem.pcPrinterState !== 1 && deviceItem.pcPrinterState !== 5):true
                    property string currentUnit: "10"

                    CusRoundedBg {
                        leftTop: true
                        rightTop: true
                        clickedable: false
                        borderWidth: 1
                        borderColor: "#0F0F10"
                        color: "#37373B"
                        radius: 5

                        Layout.fillWidth: true
                        Layout.minimumWidth: 626 * screenScaleFactor
                        Layout.maximumWidth: 932 * screenScaleFactor
                        Layout.preferredHeight: 40 * screenScaleFactor

                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 20 * screenScaleFactor
                            anchors.rightMargin: 20 * screenScaleFactor
                            spacing: 10 * screenScaleFactor

                            Image {
                                Layout.preferredWidth: 14
                                Layout.preferredHeight: 14
                                source: "qrc:/UI/photo/controlIcon.png"
                            }

                            Text {
                                Layout.preferredWidth: contentWidth * screenScaleFactor
                                Layout.preferredHeight: 14 * screenScaleFactor

                                verticalAlignment: Text.AlignVCenter
                                font.weight: Font.Bold
                                font.family: Constants.mySystemFont.name
                                font.pointSize: Constants.labelFontPointSize
                                text: qsTr("Control")
                                color: "white"
                            }

                            Item {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                            }

                            Item {
                                Layout.preferredWidth: 20 * screenScaleFactor
                                Layout.preferredHeight: 12 * screenScaleFactor

                                Image {
                                    anchors.centerIn: parent
                                    source: `qrc:/UI/photo/${printerItem.showPopup?"upArrow":"downArrow"}.png`
                                }

                                MouseArea {
                                    hoverEnabled: true
                                    anchors.fill: parent
                                    cursorShape: containsMouse?Qt.PointingHandCursor:Qt.ArrowCursor

                                    onClicked: {
                                        controlItem.showPopup = !controlItem.showPopup
                                        controlPopup.visible = controlItem.showPopup
                                    }
                                }
                            }
                        }
                    }

                    CusRoundedBg {
                        id: controlPopup
                        visible: true
                        leftBottom: true
                        rightBottom: true
                        clickedable: false
                        borderWidth: 1
                        borderColor: "#0F0F10"
                        color: "#28282B"
                        radius: 5

                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.minimumWidth: 626 * screenScaleFactor
                        Layout.maximumWidth: 932 * screenScaleFactor
                        Layout.minimumHeight: 215 * screenScaleFactor
                        Layout.maximumHeight: 265 * screenScaleFactor

                        ColumnLayout {
                            anchors.fill: parent
                            spacing: 10 * screenScaleFactor
                            anchors.topMargin: 10 * screenScaleFactor
                            anchors.bottomMargin: 10 * screenScaleFactor

                            RowLayout {
                                spacing: 0

                                Item {
                                    Layout.fillHeight: true
                                    Layout.preferredWidth: 30 * screenScaleFactor
                                }

                                GridLayout {
                                    columns: 4
                                    rowSpacing: 10 * screenScaleFactor
                                    columnSpacing: 10 * screenScaleFactor

                                    Item {
                                        Layout.preferredWidth: 40 * screenScaleFactor
                                        Layout.preferredHeight: 40 * screenScaleFactor
                                    }

                                    Rectangle {
                                        radius: 5
                                        opacity: enabled ? 1 : 0.7
                                        enabled: controlItem.controlEnabled
                                        color: y_plusArea.containsMouse?"#1E9BE2":"#4F4F54"

                                        Layout.preferredWidth: 40 * screenScaleFactor
                                        Layout.preferredHeight: 40 * screenScaleFactor

                                        Column {
                                            width: parent.width
                                            anchors.centerIn: parent
                                            spacing: 5 * screenScaleFactor

                                            Image {
                                                anchors.horizontalCenter: parent.horizontalCenter
                                                source: "qrc:/UI/photo/axis_plusIcon.png"
                                            }

                                            Text {
                                                anchors.horizontalCenter: parent.horizontalCenter
                                                verticalAlignment: Text.AlignVCenter
                                                font.weight: Font.Bold
                                                font.family: Constants.mySystemFont.name
                                                font.pointSize: Constants.labelFontPointSize
                                                color: "#DBDBDC"
                                                text: "Y+"
                                            }
                                        }

                                        MouseArea {
                                            id: y_plusArea
                                            hoverEnabled: true
                                            anchors.fill: parent
                                            cursorShape: containsMouse?Qt.PointingHandCursor:Qt.ArrowCursor

                                            onClicked: {
                                                var ipAddr = deviceItem?deviceItem.pcIpAddr:""
                                                var postion = parseFloat(idYPostion.text) + parseFloat(controlItem.currentUnit)
                                                //console.log("postion Y+", postion)
                                                if(ipAddr !== "") printerListModel.cSourceModel.sendUIControlCmdToDevice(ipAddr, LanPrinterListLocal.PrintControlType.CONTROL_MOVE_Y, postion)
                                            }
                                        }
                                    }

                                    Item {
                                        Layout.preferredWidth: 40 * screenScaleFactor
                                        Layout.preferredHeight: 40 * screenScaleFactor
                                    }

                                    Rectangle {
                                        radius: 5
                                        opacity: enabled ? 1 : 0.7
                                        enabled: controlItem.controlEnabled
                                        color: z_plusArea.containsMouse?"#6F6F75":"#4F4F54"

                                        Layout.preferredWidth: 40 * screenScaleFactor
                                        Layout.preferredHeight: 40 * screenScaleFactor

                                        Column {
                                            width: parent.width
                                            anchors.centerIn: parent
                                            spacing: 5 * screenScaleFactor

                                            Image {
                                                anchors.horizontalCenter: parent.horizontalCenter
                                                source: "qrc:/UI/photo/axis_plusIcon.png"
                                            }

                                            Text {
                                                anchors.horizontalCenter: parent.horizontalCenter
                                                verticalAlignment: Text.AlignVCenter
                                                font.weight: Font.Bold
                                                font.family: Constants.mySystemFont.name
                                                font.pointSize: Constants.labelFontPointSize
                                                color: "#DBDBDC"
                                                text: "Z+"
                                            }
                                        }

                                        MouseArea {
                                            id: z_plusArea
                                            hoverEnabled: true
                                            anchors.fill: parent
                                            cursorShape: containsMouse?Qt.PointingHandCursor:Qt.ArrowCursor

                                            onClicked: {
                                                var ipAddr = deviceItem?deviceItem.pcIpAddr:""
                                                var postion = parseFloat(idZPostion.text) + parseFloat(controlItem.currentUnit)
                                                //console.log("postion Z+", postion)
                                                if(ipAddr !== "") printerListModel.cSourceModel.sendUIControlCmdToDevice(ipAddr, LanPrinterListLocal.PrintControlType.CONTROL_MOVE_Z, postion)
                                            }
                                        }
                                    }

                                    Rectangle {
                                        radius: 5
                                        opacity: enabled ? 1 : 0.7
                                        enabled: controlItem.controlEnabled
                                        color: x_minusArea.containsMouse?"#1E9BE2":"#4F4F54"

                                        Layout.preferredWidth: 40 * screenScaleFactor
                                        Layout.preferredHeight: 40 * screenScaleFactor

                                        Row {
                                            height: parent.height
                                            anchors.centerIn: parent
                                            spacing: 5 * screenScaleFactor

                                            Image {
                                                anchors.verticalCenter: parent.verticalCenter
                                                source: "qrc:/UI/photo/x_minusIcon.png"
                                            }

                                            Text {
                                                anchors.verticalCenter: parent.verticalCenter
                                                verticalAlignment: Text.AlignVCenter
                                                font.weight: Font.Bold
                                                font.family: Constants.mySystemFont.name
                                                font.pointSize: Constants.labelFontPointSize
                                                color: "#DBDBDC"
                                                text: "X-"
                                            }
                                        }

                                        MouseArea {
                                            id: x_minusArea
                                            hoverEnabled: true
                                            anchors.fill: parent
                                            cursorShape: containsMouse?Qt.PointingHandCursor:Qt.ArrowCursor

                                            onClicked: {
                                                var ipAddr = deviceItem?deviceItem.pcIpAddr:""
                                                var postion = parseFloat(idXPostion.text) - parseFloat(controlItem.currentUnit)
                                                //console.log("postion X-", postion)
                                                if(ipAddr !== "") printerListModel.cSourceModel.sendUIControlCmdToDevice(ipAddr, LanPrinterListLocal.PrintControlType.CONTROL_MOVE_X, postion)
                                            }
                                        }
                                    }

                                    Rectangle {
                                        radius: 5
                                        opacity: enabled ? 1 : 0.7
                                        enabled: controlItem.controlEnabled
                                        color: xy_homeArea.containsMouse?"#1E9BE2":"#4F4F54"

                                        Layout.preferredWidth: 40 * screenScaleFactor
                                        Layout.preferredHeight: 40 * screenScaleFactor

                                        Image {
                                            anchors.centerIn: parent
                                            source: "qrc:/UI/photo/axis_homeIcon.png"
                                        }

                                        MouseArea {
                                            id: xy_homeArea
                                            hoverEnabled: true
                                            anchors.fill: parent
                                            cursorShape: containsMouse?Qt.PointingHandCursor:Qt.ArrowCursor

                                            onClicked: {
                                                var ipAddr = deviceItem?deviceItem.pcIpAddr:""
                                                if(ipAddr !== "") printerListModel.cSourceModel.sendUIControlCmdToDevice(ipAddr, LanPrinterListLocal.PrintControlType.CONTROL_MOVE_XY0, " ")
                                            }
                                        }
                                    }

                                    Rectangle {
                                        radius: 5
                                        opacity: enabled ? 1 : 0.7
                                        enabled: controlItem.controlEnabled
                                        color: x_plusArea.containsMouse?"#1E9BE2":"#4F4F54"

                                        Layout.preferredWidth: 40 * screenScaleFactor
                                        Layout.preferredHeight: 40 * screenScaleFactor

                                        Row {
                                            spacing: 5
                                            height: parent.height
                                            anchors.centerIn: parent

                                            Image {
                                                anchors.verticalCenter: parent.verticalCenter
                                                source: "qrc:/UI/photo/x_plusIcon.png"
                                            }

                                            Text {
                                                anchors.verticalCenter: parent.verticalCenter
                                                verticalAlignment: Text.AlignVCenter
                                                font.weight: Font.Bold
                                                font.family: Constants.mySystemFont.name
                                                font.pointSize: Constants.labelFontPointSize
                                                color: "#DBDBDC"
                                                text: "X+"
                                            }
                                        }

                                        MouseArea {
                                            id: x_plusArea
                                            hoverEnabled: true
                                            anchors.fill: parent
                                            cursorShape: containsMouse?Qt.PointingHandCursor:Qt.ArrowCursor

                                            onClicked: {
                                                var ipAddr = deviceItem?deviceItem.pcIpAddr:""
                                                var postion = parseFloat(idXPostion.text) + parseFloat(controlItem.currentUnit)
                                                //console.log("postion X+", postion)
                                                if(ipAddr !== "") printerListModel.cSourceModel.sendUIControlCmdToDevice(ipAddr, LanPrinterListLocal.PrintControlType.CONTROL_MOVE_X, postion)
                                            }
                                        }
                                    }

                                    Rectangle {
                                        radius: 5
                                        opacity: enabled ? 1 : 0.7
                                        enabled: controlItem.controlEnabled
                                        color: z_homeArea.containsMouse?"#6F6F75":"#4F4F54"

                                        Layout.preferredWidth: 40 * screenScaleFactor
                                        Layout.preferredHeight: 40 * screenScaleFactor

                                        Image {
                                            anchors.centerIn: parent
                                            source: "qrc:/UI/photo/axis_homeIcon.png"
                                        }

                                        MouseArea {
                                            id: z_homeArea
                                            hoverEnabled: true
                                            anchors.fill: parent
                                            cursorShape: containsMouse?Qt.PointingHandCursor:Qt.ArrowCursor

                                            onClicked: {
                                                var ipAddr = deviceItem?deviceItem.pcIpAddr:""
                                                if(ipAddr !== "") printerListModel.cSourceModel.sendUIControlCmdToDevice(ipAddr, LanPrinterListLocal.PrintControlType.CONTROL_MOVE_Z0, " ")
                                            }
                                        }
                                    }

                                    Item {
                                        Layout.preferredWidth: 40 * screenScaleFactor
                                        Layout.preferredHeight: 40 * screenScaleFactor
                                    }

                                    Rectangle {
                                        radius: 5
                                        opacity: enabled ? 1 : 0.7
                                        enabled: controlItem.controlEnabled
                                        color: y_minusArea.containsMouse?"#1E9BE2":"#4F4F54"

                                        Layout.preferredWidth: 40 * screenScaleFactor
                                        Layout.preferredHeight: 40 * screenScaleFactor

                                        Column {
                                            width: parent.width
                                            anchors.centerIn: parent
                                            spacing: 5 * screenScaleFactor

                                            Image {
                                                anchors.horizontalCenter: parent.horizontalCenter
                                                source: "qrc:/UI/photo/axis_minusIcon.png"
                                            }

                                            Text {
                                                anchors.horizontalCenter: parent.horizontalCenter
                                                verticalAlignment: Text.AlignVCenter
                                                font.weight: Font.Bold
                                                font.family: Constants.mySystemFont.name
                                                font.pointSize: Constants.labelFontPointSize
                                                color: "#DBDBDC"
                                                text: "Y-"
                                            }
                                        }

                                        MouseArea {
                                            id: y_minusArea
                                            hoverEnabled: true
                                            anchors.fill: parent
                                            cursorShape: containsMouse?Qt.PointingHandCursor:Qt.ArrowCursor

                                            onClicked: {
                                                var ipAddr = deviceItem?deviceItem.pcIpAddr:""
                                                var postion = parseFloat(idYPostion.text) - parseFloat(controlItem.currentUnit)
                                                //console.log("postion Y-", postion)
                                                if(ipAddr !== "") printerListModel.cSourceModel.sendUIControlCmdToDevice(ipAddr, LanPrinterListLocal.PrintControlType.CONTROL_MOVE_Y, postion)
                                            }
                                        }
                                    }

                                    Item {
                                        Layout.preferredWidth: 40 * screenScaleFactor
                                        Layout.preferredHeight: 40 * screenScaleFactor
                                    }

                                    Rectangle {
                                        radius: 5
                                        opacity: enabled ? 1 : 0.7
                                        enabled: controlItem.controlEnabled
                                        color: z_minusArea.containsMouse?"#6F6F75":"#4F4F54"

                                        Layout.preferredWidth: 40 * screenScaleFactor
                                        Layout.preferredHeight: 40 * screenScaleFactor

                                        Column {
                                            width: parent.width
                                            anchors.centerIn: parent
                                            spacing: 5 * screenScaleFactor

                                            Image {
                                                anchors.horizontalCenter: parent.horizontalCenter
                                                source: "qrc:/UI/photo/axis_minusIcon.png"
                                            }

                                            Text {
                                                anchors.horizontalCenter: parent.horizontalCenter
                                                verticalAlignment: Text.AlignVCenter
                                                font.weight: Font.Bold
                                                font.family: Constants.mySystemFont.name
                                                font.pointSize: Constants.labelFontPointSize
                                                color: "#DBDBDC"
                                                text: "Z-"
                                            }
                                        }

                                        MouseArea {
                                            id: z_minusArea
                                            hoverEnabled: true
                                            anchors.fill: parent
                                            cursorShape: containsMouse?Qt.PointingHandCursor:Qt.ArrowCursor

                                            onClicked: {
                                                var ipAddr = deviceItem?deviceItem.pcIpAddr:""
                                                var postion = parseFloat(idZPostion.text) - parseFloat(controlItem.currentUnit)
                                                //console.log("postion Z-", postion)
                                                if(ipAddr !== "") printerListModel.cSourceModel.sendUIControlCmdToDevice(ipAddr, LanPrinterListLocal.PrintControlType.CONTROL_MOVE_Z, postion)
                                            }
                                        }
                                    }
                                }

                                Item {
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    Layout.minimumWidth: 20 * screenScaleFactor
                                    Layout.maximumWidth: 100 * screenScaleFactor
                                }

                                ColumnLayout {
                                    spacing: 10 * screenScaleFactor

                                    ColumnLayout {
                                        spacing: 3 * screenScaleFactor

                                        Rectangle {
                                            radius: 5
                                            border.width: 1
                                            border.color: "#4F4F54"
                                            color: "#28282B"

                                            Layout.preferredWidth: 270 * screenScaleFactor
                                            Layout.preferredHeight: 28 * screenScaleFactor

                                            Text {
                                                anchors.left: parent.left
                                                anchors.leftMargin: 11 * screenScaleFactor
                                                anchors.verticalCenter: parent.verticalCenter

                                                verticalAlignment: Text.AlignVCenter
                                                font.weight: Font.Bold
                                                font.family: Constants.mySystemFont.name
                                                font.pointSize: Constants.labelFontPointSize
                                                color: "white"
                                                text: "X："
                                            }

                                            Text {
                                                id: idXPostion
                                                anchors.right: parent.right
                                                anchors.rightMargin: 11 * screenScaleFactor
                                                anchors.verticalCenter: parent.verticalCenter

                                                verticalAlignment: Text.AlignVCenter
                                                font.weight: Font.Bold
                                                font.family: Constants.mySystemFont.name
                                                font.pointSize: Constants.labelFontPointSize
                                                color: "white"
                                                text: deviceItem?(deviceItem.pcX).toFixed(1):"undefined"
                                            }
                                        }

                                        Rectangle {
                                            radius: 5
                                            border.width: 1
                                            border.color: "#4F4F54"
                                            color: "#28282B"

                                            Layout.preferredWidth: 270 * screenScaleFactor
                                            Layout.preferredHeight: 28 * screenScaleFactor

                                            Text {
                                                anchors.left: parent.left
                                                anchors.leftMargin: 11 * screenScaleFactor
                                                anchors.verticalCenter: parent.verticalCenter

                                                verticalAlignment: Text.AlignVCenter
                                                font.weight: Font.Bold
                                                font.family: Constants.mySystemFont.name
                                                font.pointSize: Constants.labelFontPointSize
                                                color: "white"
                                                text: "Y："
                                            }

                                            Text {
                                                id: idYPostion
                                                anchors.right: parent.right
                                                anchors.rightMargin: 11 * screenScaleFactor
                                                anchors.verticalCenter: parent.verticalCenter

                                                verticalAlignment: Text.AlignVCenter
                                                font.weight: Font.Bold
                                                font.family: Constants.mySystemFont.name
                                                font.pointSize: Constants.labelFontPointSize
                                                color: "white"
                                                text: deviceItem?(deviceItem.pcY).toFixed(1):"undefined"
                                            }
                                        }

                                        Rectangle {
                                            radius: 5
                                            border.width: 1
                                            border.color: "#4F4F54"
                                            color: "#28282B"

                                            Layout.preferredWidth: 270 * screenScaleFactor
                                            Layout.preferredHeight: 28 * screenScaleFactor

                                            Text {
                                                anchors.left: parent.left
                                                anchors.leftMargin: 11 * screenScaleFactor
                                                anchors.verticalCenter: parent.verticalCenter
                                                verticalAlignment: Text.AlignVCenter

                                                font.weight: Font.Bold
                                                font.family: Constants.mySystemFont.name
                                                font.pointSize: Constants.labelFontPointSize
                                                color: "white"
                                                text: "Z："
                                            }

                                            Text {
                                                id: idZPostion
                                                anchors.right: parent.right
                                                anchors.rightMargin: 11 * screenScaleFactor
                                                anchors.verticalCenter: parent.verticalCenter

                                                verticalAlignment: Text.AlignVCenter
                                                font.weight: Font.Bold
                                                font.family: Constants.mySystemFont.name
                                                font.pointSize: Constants.labelFontPointSize
                                                color: "white"
                                                text: deviceItem?(deviceItem.pcZ).toFixed(1):"undefined"
                                            }
                                        }
                                    }

                                    RowLayout {
                                        spacing: 10 * screenScaleFactor

                                        Repeater {
                                            model: ["0.1", "1", "10", "100"]
                                            delegate: RadioButton {
                                                checkable: true
                                                indicator: null
                                                opacity: enabled ? 1 : 0.7
                                                enabled: controlItem.controlEnabled

                                                Layout.preferredWidth: 60 * screenScaleFactor
                                                Layout.preferredHeight: 40 * screenScaleFactor

                                                background: Rectangle {
                                                    radius: 5
                                                    anchors.fill: parent
                                                    color: parent.checked?"#1E9BE2":(parent.hovered?"#6F6F75":"#4F4F54")
                                                }

                                                Text {
                                                    anchors.centerIn: parent
                                                    verticalAlignment: Text.AlignVCenter
                                                    font.weight: Font.Bold
                                                    font.family: Constants.mySystemFont.name
                                                    font.pointSize: Constants.labelFontPointSize
                                                    text: `${modelData}mm`
                                                    color: "white"
                                                }

                                                Component.onCompleted: if(index == 2) checked = true//default
                                                onCheckedChanged: if(checked) controlItem.currentUnit = modelData
                                            }
                                        }
                                    }
                                }
                            }

                            RowLayout {
                                spacing: 0

                                Item {
                                    Layout.fillHeight: true
                                    Layout.preferredWidth: 30 * screenScaleFactor
                                }

                                RowLayout {
                                    spacing: 10 * screenScaleFactor

                                    Text {
                                        Layout.preferredWidth: contentWidth * screenScaleFactor
                                        Layout.preferredHeight: 14 * screenScaleFactor

                                        verticalAlignment: Text.AlignVCenter
                                        font.weight: Font.Bold
                                        font.family: Constants.mySystemFont.name
                                        font.pointSize: Constants.labelFontPointSize
                                        text: qsTr("Fan switch")
                                        color: "#CBCBCC"
                                    }

                                    Button {
                                        id: fanSwitch
                                        checkable: true
                                        background: null
                                        opacity: enabled ? 1.0 : 0.7
                                        enabled: controlItem.isWlanPrinting

                                        property var delayShow: false
                                        property bool fan_checked: deviceItem?deviceItem.fanOpened:false

                                        Layout.preferredWidth: 50 * screenScaleFactor
                                        Layout.preferredHeight: 32 * screenScaleFactor

                                        Binding on checked {
                                            when: !fanSwitch.delayShow
                                            value: fanSwitch.fan_checked
                                        }

                                        Timer {
                                            repeat: false
                                            interval: 10000
                                            id: fanSwitchDelayShow
                                            onTriggered: fanSwitch.delayShow = false
                                        }

                                        onClicked: {
                                            var value = checked ? "1" : "0"
                                            var ipAddr = deviceItem?deviceItem.pcIpAddr:""
                                            if(ipAddr !== "") {
                                                fanSwitch.delayShow = true
                                                fanSwitchDelayShow.start()
                                                //console.log("fanSwitch", value)
                                                printerListModel.cSourceModel.sendUIControlCmdToDevice(ipAddr, LanPrinterListLocal.PrintControlType.CONTROL_CMD_FAN, value)
                                            }
                                        }

                                        indicator: Rectangle{
                                            color: "#424246"
                                            radius: height / 2
                                            anchors.fill: parent

                                            Rectangle {
                                                radius: height / 2
                                                width: 26 * screenScaleFactor
                                                height: 26 * screenScaleFactor
                                                anchors.verticalCenter: parent.verticalCenter

                                                color: fanSwitch.checked ? "#00A3FF" : "#28282B"
                                                x: fanSwitch.checked ? (parent.width - width - 3 * screenScaleFactor) : 3 * screenScaleFactor

                                                Behavior on x {
                                                    NumberAnimation { duration: 100 }
                                                }
                                            }
                                        }
                                    }
                                }

                                Item {
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    Layout.minimumWidth: 30 * screenScaleFactor
                                    Layout.maximumWidth: 60 * screenScaleFactor
                                }

                                RowLayout {
                                    spacing: 10 * screenScaleFactor

                                    Text {
                                        Layout.preferredWidth: contentWidth * screenScaleFactor
                                        Layout.preferredHeight: 14 * screenScaleFactor

                                        verticalAlignment: Text.AlignVCenter
                                        font.weight: Font.Bold
                                        font.family: Constants.mySystemFont.name
                                        font.pointSize: Constants.labelFontPointSize
                                        text: qsTr("Led switch")
                                        color: "#CBCBCC"
                                    }

                                    Button {
                                        id: ledSwitch
                                        checkable: true
                                        background: null
                                        opacity: enabled ? 1.0 : 0.7
                                        enabled: controlItem.isWlanPrinting

                                        property var delayShow: false
                                        property bool led_checked: deviceItem?deviceItem.ledOpened:false

                                        Layout.preferredWidth: 50 * screenScaleFactor
                                        Layout.preferredHeight: 32 * screenScaleFactor

                                        Binding on checked {
                                            when: !ledSwitch.delayShow
                                            value: ledSwitch.led_checked
                                        }

                                        Timer {
                                            repeat: false
                                            interval: 10000
                                            id: ledSwitchDelayShow
                                            onTriggered: ledSwitch.delayShow = false
                                        }

                                        onClicked:{
                                            var value = checked ? "1" : "0"
                                            var ipAddr = deviceItem?deviceItem.pcIpAddr:""
                                            if(ipAddr !== "") {
                                                ledSwitch.delayShow = true
                                                ledSwitchDelayShow.start()
                                                //console.log("ledSwitch", value)
                                                printerListModel.cSourceModel.sendUIControlCmdToDevice(ipAddr, LanPrinterListLocal.PrintControlType.CONTROL_CMD_LED, value)
                                            }
                                        }

                                        indicator: Rectangle{
                                            color: "#424246"
                                            radius: height / 2
                                            anchors.fill: parent

                                            Rectangle {
                                                radius: height / 2
                                                width: 26 * screenScaleFactor
                                                height: 26 * screenScaleFactor
                                                anchors.verticalCenter: parent.verticalCenter

                                                color: ledSwitch.checked ? "#00A3FF" : "#28282B"
                                                x: ledSwitch.checked ? (parent.width - width - 3 * screenScaleFactor) : 3 * screenScaleFactor

                                                Behavior on x {
                                                    NumberAnimation { duration: 100 }
                                                }
                                            }
                                        }
                                    }
                                }

                                Item {
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    Layout.minimumWidth: 30 * screenScaleFactor
                                    Layout.maximumWidth: 60 * screenScaleFactor
                                }

                                RowLayout {
                                    spacing: 10 * screenScaleFactor

                                    Text {
                                        Layout.preferredWidth: contentWidth * screenScaleFactor
                                        Layout.preferredHeight: 28 * screenScaleFactor

                                        verticalAlignment: Text.AlignVCenter
                                        font.weight: Font.Normal
                                        font.family: Constants.mySystemFont.name
                                        font.pointSize: Constants.labelFontPointSize
                                        text: qsTr("Printing speed") + "："
                                        color: "#CBCBCC"
                                    }

                                    Rectangle {
                                        id: _printSpeedBox
                                        opacity: enabled ? 1.0 : 0.7
                                        enabled: controlItem.isWlanPrinting

                                        property int minValue: 10
                                        property int maxValue: 300
                                        property var delayShow: false
                                        property var printSpeed: deviceItem?deviceItem.pcPrintSpeed:"undefined"
                                        property alias boxValue: _editPrintSpeedBox.text

                                        function specifyPcPrintSpeed() {
                                            var ipAddr = deviceItem?deviceItem.pcIpAddr:""
                                            if(ipAddr !== "")
                                            {
                                                delayShow = true
                                                printSpeedDelayShow.start()
                                                printerListModel.cSourceModel.sendUIControlCmdToDevice(ipAddr, LanPrinterListLocal.PrintControlType.CONTROL_CMD_FEEDRATEPCT, boxValue)
                                            }
                                        }
                                        function decrease() {
                                            if(boxValue != "" && boxValue > minValue)
                                            {
                                                var value = parseInt(boxValue) - 1
                                                boxValue = value
                                            }
                                        }
                                        function increase() {
                                            if(boxValue != "" && boxValue < maxValue)
                                            {
                                                var value = parseInt(boxValue) + 1
                                                boxValue = value
                                            }
                                        }

                                        Layout.preferredWidth: 120 * screenScaleFactor
                                        Layout.preferredHeight: 28 * screenScaleFactor

                                        color: "#28282B"
                                        border.color: "#4F4F54"
                                        border.width: 1
                                        radius: 5

                                        Binding on boxValue {
                                            when: !_editPrintSpeedBox.activeFocus && !_printSpeedBox.delayShow
                                            value: _printSpeedBox.printSpeed
                                        }

                                        Timer {
                                            repeat: false
                                            interval: 10000
                                            id: printSpeedDelayShow
                                            onTriggered: _printSpeedBox.delayShow = false
                                        }

                                        TextField {
                                            id: _editPrintSpeedBox
                                            width: parent.width
                                            height: parent.height
                                            anchors.left: parent.left
                                            anchors.verticalCenter: parent.verticalCenter

                                            validator: RegExpValidator{regExp: new RegExp("[1-9][\\d]|[1-2][\\d][\\d]|300")}
                                            selectByMouse: true
                                            selectionColor: "#1E9BE2"
                                            selectedTextColor: color
                                            topPadding: 0
                                            bottomPadding: 0
                                            leftPadding: 6 * screenScaleFactor
                                            rightPadding: 4 * screenScaleFactor
                                            font.weight: Font.Medium
                                            font.family: Constants.mySystemFont.name
                                            font.pointSize: Constants.labelFontPointSize
                                            verticalAlignment: TextInput.AlignVCenter
                                            horizontalAlignment: TextInput.AlignLeft
                                            background: null
                                            color: "white"

                                            Keys.onUpPressed: _printSpeedBox.increase()
                                            Keys.onDownPressed: _printSpeedBox.decrease()
                                            Keys.onEnterPressed: parent.forceActiveFocus()
                                            Keys.onReturnPressed: parent.forceActiveFocus()
                                            //onActiveFocusChanged: mItem.pcPrintSpeedFocus = activeFocus
                                            onEditingFinished: _printSpeedBox.specifyPcPrintSpeed()
                                        }

                                        Text {
                                            anchors.left: parent.left
                                            anchors.verticalCenter: parent.verticalCenter
                                            anchors.leftMargin: _editPrintSpeedBox.contentWidth + _editPrintSpeedBox.leftPadding + 4 * screenScaleFactor

                                            font: _editPrintSpeedBox.font
                                            color: _editPrintSpeedBox.color
                                            verticalAlignment: Text.AlignVCenter
                                            text: "%"
                                        }

                                        Image {
                                            property string upBtnImage: upButtonArea_0.containsMouse?"upBtn_d":"upBtn"
                                            anchors.right: parent.right
                                            anchors.top: parent.top
                                            anchors.rightMargin: 8 * screenScaleFactor
                                            anchors.topMargin: 8 * screenScaleFactor
                                            visible: _editPrintSpeedBox.activeFocus
                                            source: `qrc:/UI/photo/${upBtnImage}.png`

                                            MouseArea {
                                                id: upButtonArea_0
                                                anchors.fill: parent
                                                hoverEnabled: true
                                                onClicked: _printSpeedBox.increase()
                                            }
                                        }

                                        Image {
                                            property string downBtnImg: downButtonArea_0.containsMouse?"downBtn_d":"downBtn"
                                            anchors.right: parent.right
                                            anchors.bottom: parent.bottom
                                            anchors.rightMargin: 8 * screenScaleFactor
                                            anchors.bottomMargin: 7 * screenScaleFactor
                                            visible: _editPrintSpeedBox.activeFocus
                                            source: `qrc:/UI/photo/${downBtnImg}.png`

                                            MouseArea {
                                                id: downButtonArea_0
                                                anchors.fill: parent
                                                hoverEnabled: true
                                                onClicked: _printSpeedBox.decrease()
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: 8 * screenScaleFactor
                    Layout.maximumHeight: 16 * screenScaleFactor
                }

                ColumnLayout {
                    spacing: 0
                    id: gFileItem
                    property var currentDevice: ""
                    property var currentFilePath: ""

                    property int selectCount: 0
                    property int curImportProgress: deviceItem?deviceItem.gCodeImportProgress:0

                    property bool showPopup: true
                    property bool controlEnabled: deviceItem?(deviceItem.pcPrinterState !== 1 && deviceItem.pcPrinterState !== 5):false

                    CusRoundedBg {
                        leftTop: true
                        rightTop: true
                        clickedable: false
                        borderWidth: 1
                        borderColor: "#0F0F10"
                        color: "#37373B"
                        radius: 5

                        Layout.fillWidth: true
                        Layout.minimumWidth: 626 * screenScaleFactor
                        Layout.maximumWidth: 932 * screenScaleFactor
                        Layout.preferredHeight: 40 * screenScaleFactor

                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 20 * screenScaleFactor
                            anchors.rightMargin: 20 * screenScaleFactor
                            spacing: 10 * screenScaleFactor

                            Image {
                                Layout.preferredWidth: 11
                                Layout.preferredHeight: 14
                                source: "qrc:/UI/photo/gFileIcon.png"
                            }

                            RadioButton {
                                checkable: true
                                indicator: null

                                Layout.preferredWidth: 80 * screenScaleFactor
                                Layout.preferredHeight: 32 * screenScaleFactor

                                background: Rectangle {
                                    radius: 5
                                    anchors.fill: parent
                                    color: parent.checked?"#444449":(parent.hovered?"#515157":"transparent")
                                }

                                Text {
                                    anchors.centerIn: parent
                                    verticalAlignment: Text.AlignVCenter
                                    font.weight: Font.Bold
                                    font.family: Constants.mySystemFont.name
                                    font.pointSize: Constants.labelFontPointSize
                                    color: parent.checked?"white":"#CBCBCC"
                                    text: qsTr("File")
                                }

                                Component.onCompleted: checked = true
                            }

                            RadioButton {
                                visible: false
                                checkable: true
                                indicator: null

                                Layout.preferredWidth: 80 * screenScaleFactor
                                Layout.preferredHeight: 32 * screenScaleFactor

                                background: Rectangle {
                                    radius: 5
                                    anchors.fill: parent
                                    color: parent.checked?"#444449":(parent.hovered?"#515157":"transparent")
                                }

                                Text {
                                    anchors.centerIn: parent
                                    verticalAlignment: Text.AlignVCenter
                                    font.weight: Font.Bold
                                    font.family: Constants.mySystemFont.name
                                    font.pointSize: Constants.labelFontPointSize
                                    color: parent.checked?"white":"#CBCBCC"
                                    text: qsTr("Records")
                                }
                            }

                            Item {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                            }

                            RowLayout {
                                id: importProgress
                                visible: (gFileItem.curImportProgress != 0)
                                spacing: 10 * screenScaleFactor

                                Rectangle {
                                    color: "#28282B"
                                    radius: height / 2

                                    Layout.preferredWidth: 200 * screenScaleFactor
                                    Layout.preferredHeight: 2 * screenScaleFactor

                                    Rectangle {
                                        anchors.left: parent.left
                                        anchors.verticalCenter: parent.verticalCenter
                                        width: parent.width * gFileItem.curImportProgress / 100.0
                                        height: parent.height
                                        radius: parent.radius
                                        color: "#1E9BE2"
                                    }
                                }

                                Text {
                                    property var showImportText: (gFileItem.curImportProgress < 0) ? qsTr("Upload Failed") : `${gFileItem.curImportProgress}%`
                                    Layout.preferredWidth: contentWidth * screenScaleFactor
                                    Layout.preferredHeight: 14 * screenScaleFactor

                                    verticalAlignment: Text.AlignVCenter
                                    font.weight: Font.Medium
                                    font.family: Constants.mySystemFont.name
                                    font.pointSize: Constants.labelFontPointSize
                                    text: showImportText
                                    color: "white"
                                }
                            }

                            Item {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                            }

                            Rectangle {
                                radius: height / 2
                                opacity: enabled ? 1.0 : 0.7
                                enabled: gFileItem.controlEnabled && (gFileItem.curImportProgress <= 0)
                                color: btnImportArea.containsMouse?"#67676D":"#515157"

                                Layout.preferredWidth: 120 * screenScaleFactor
                                Layout.preferredHeight: 24 * screenScaleFactor

                                Row {
                                    height: parent.height
                                    anchors.centerIn: parent
                                    spacing: 10 * screenScaleFactor

                                    Image {
                                        anchors.verticalCenter: parent.verticalCenter
                                        source: "qrc:/UI/photo/add_printer.png"
                                    }

                                    Text {
                                        anchors.verticalCenter: parent.verticalCenter
                                        verticalAlignment: Text.AlignVCenter
                                        font.weight: Font.Medium
                                        font.family: Constants.mySystemFont.name
                                        font.pointSize: Constants.labelFontPointSize
                                        text: qsTr("Import")
                                        color: "white"
                                    }
                                }

                                MouseArea {
                                    id: btnImportArea
                                    hoverEnabled: true
                                    anchors.fill: parent
                                    cursorShape: containsMouse?Qt.PointingHandCursor:Qt.ArrowCursor
                                    onClicked: fileDialog.open()
                                }
                            }

                            Item {
                                Layout.preferredWidth: 20 * screenScaleFactor
                                Layout.preferredHeight: 12 * screenScaleFactor

                                Image {
                                    anchors.centerIn: parent
                                    source: `qrc:/UI/photo/${printerItem.showPopup?"upArrow":"downArrow"}.png`
                                }

                                MouseArea {
                                    hoverEnabled: true
                                    anchors.fill: parent
                                    cursorShape: containsMouse?Qt.PointingHandCursor:Qt.ArrowCursor
                                    anchors.centerIn: parent

                                    onClicked: {
                                        gFileItem.showPopup = !gFileItem.showPopup
                                        gFilePopup.visible = gFileItem.showPopup
                                    }
                                }
                            }
                        }

                        FileDialog {
                            id: fileDialog
                            title: qsTr("Open File")
                            folder: shortcuts.desktop
                            selectMultiple: false
                            nameFilters: ["*.gcode"]

                            onAccepted: {
                                var filePath = fileDialog.fileUrls
                                if(deviceID !== "" && filePath !== "")
                                {
                                    printerListModel.cSourceModel.importFile(deviceID, filePath)
                                    //console.log("deviceID", deviceID, "filePath", filePath)
                                }
                            }
                        }
                    }

                    CusRoundedBg {
                        id: gFilePopup
                        visible: true
                        leftBottom: true
                        rightBottom: true
                        clickedable: false
                        borderWidth: 1
                        borderColor: "#0F0F10"
                        color: "#28282B"
                        radius: 5

                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.minimumWidth: 626 * screenScaleFactor
                        Layout.maximumWidth: 932 * screenScaleFactor
                        Layout.minimumHeight: 200 * screenScaleFactor
                        Layout.maximumHeight: 370 * screenScaleFactor

                        ColumnLayout {
                            spacing: 0
                            anchors.fill: parent

                            Rectangle {
                                id: header
                                border.width: 0
                                color: "#28282B"

                                Layout.alignment: Qt.AlignHCenter
                                Layout.preferredWidth: parent.width - 2 * screenScaleFactor
                                Layout.preferredHeight: (42 - 1) * screenScaleFactor

                                Row {
                                    spacing: 0
                                    Repeater {
                                        model: [qsTr("File Name"), qsTr("File Size"), qsTr("File Time")]
                                        delegate: Rectangle {
                                            width: {
                                                var w = 0
                                                switch(index) {
                                                case 0:
                                                    w = 300 * screenScaleFactor;
                                                    break;
                                                case 1:
                                                    w = 100 * screenScaleFactor;
                                                    break;
                                                case 2:
                                                    w = 100 * screenScaleFactor;
                                                    break;
                                                }
                                                return w
                                            }
                                            height: header.height
                                            color: "#28282B"
                                            border.width: 0

                                            Text {
                                                anchors.centerIn: parent
                                                verticalAlignment: Text.AlignVCenter
                                                horizontalAlignment: Text.AlignHCenter
                                                font.weight: Font.Medium
                                                font.family: Constants.mySystemFont.name
                                                font.pointSize: Constants.labelFontPointSize
                                                text: modelData
                                                color: "#CBCBCC"
                                            }
                                        }
                                    }
                                }
                            }

                            ListView {
                                spacing: 0
                                clip: true
                                id: idListView
                                Layout.preferredWidth: parent.width - 2 * screenScaleFactor
                                Layout.preferredHeight: parent.height - header.height - 2 * screenScaleFactor

                                ScrollBar.vertical: ScrollBar {
                                    visible: idListView.contentHeight > idListView.height
                                    contentItem: Rectangle {
                                        color: "#7E7E84"
                                        radius: width / 2
                                        implicitWidth: 6 * screenScaleFactor
                                        implicitHeight: 180 * screenScaleFactor
                                    }
                                }
                                model: deviceItem?deviceItem.gCodeFileList:[]

                                delegate: Item {
                                    width: parent.width
                                    height: 42 * screenScaleFactor

                                    enabled: checkedEnabled
                                    opacity: enabled ? 1.0 : 0.7
                                    anchors.horizontalCenter: parent.horizontalCenter

                                    property var fileInfos: modelData?modelData.split("#"):[]
                                    property bool checkedEnabled: gFileItem.controlEnabled && (btnChecked || gFileItem.selectCount == 0)
                                    property bool btnChecked: false

                                    MouseArea {
                                        hoverEnabled: true
                                        anchors.fill: parent
                                        acceptedButtons: Qt.LeftButton | Qt.RightButton

                                        onClicked: {
                                            if(mouse.button === Qt.LeftButton)
                                            {
                                                btnChecked = !btnChecked
                                                gFileItem.selectCount += btnChecked?1:-1
                                                gFileItem.currentDevice = btnChecked?deviceID:""
                                                gFileItem.currentFilePath = btnChecked?idFileName.text:""
                                            }
                                            else {
                                                if(btnChecked) _contentMenu.openMenu(mouseX, mouseY)
                                            }
                                        }
                                    }

                                    Popup {
                                        id: _contentMenu
                                        background: null
                                        width: 160 * screenScaleFactor
                                        height: 56 * screenScaleFactor

                                        function openMenu(mouseX, mouseY){
                                            x = mouseX
                                            y = mouseY - height
                                            open()
                                        }

                                        Column {
                                            spacing: 0

                                            Rectangle {
                                                width: 160 * screenScaleFactor
                                                height: 28 * screenScaleFactor

                                                color: "white"
                                                opacity: enabled ? 1.0 : 0.7
                                                enabled: gFileItem.controlEnabled

                                                Text {
                                                    elide: Text.ElideRight
                                                    leftPadding: 8 * screenScaleFactor
                                                    verticalAlignment: Text.AlignVCenter
                                                    anchors.verticalCenter: parent.verticalCenter
                                                    font.weight: Font.Medium
                                                    font.family: Constants.mySystemFont.name
                                                    font.pointSize: Constants.labelFontPointSize
                                                    text: qsTr("Start Print")
                                                    color: "#414143"
                                                }

                                                MouseArea {
                                                    hoverEnabled: true
                                                    anchors.fill: parent
                                                    onContainsMouseChanged: parent.color = containsMouse?"#A5DBF9":"white"

                                                    onClicked: {
                                                        _contentMenu.close()
                                                        btnChecked = false; --gFileItem.selectCount
                                                        var ipAddr = deviceItem?deviceItem.pcIpAddr:""
                                                        var printerPath = gFileItem.currentFilePath

                                                        if(ipAddr !== "" && printerPath !== "")
                                                        {
                                                            var filePath = "/media/mmcblk0p1/creality/gztemp/" + printerPath
                                                            printerListModel.cSourceModel.sendUIControlCmdToDevice(ipAddr, LanPrinterListLocal.PrintControlType.PRINT_START, filePath)
                                                        }
                                                    }
                                                }
                                            }

                                            Rectangle {
                                                width: 160 * screenScaleFactor
                                                height: 28 * screenScaleFactor

                                                color: "white"
                                                opacity: enabled ? 1.0 : 0.7
                                                enabled: gFileItem.controlEnabled

                                                Text {
                                                    elide: Text.ElideRight
                                                    leftPadding: 8 * screenScaleFactor
                                                    verticalAlignment: Text.AlignVCenter
                                                    anchors.verticalCenter: parent.verticalCenter
                                                    font.weight: Font.Medium
                                                    font.family: Constants.mySystemFont.name
                                                    font.pointSize: Constants.labelFontPointSize
                                                    text: qsTr("Delete")
                                                    color: "#414143"
                                                }

                                                MouseArea {
                                                    hoverEnabled: true
                                                    anchors.fill: parent
                                                    onContainsMouseChanged: parent.color = containsMouse?"#A5DBF9":"white"

                                                    onClicked: {
                                                        btnChecked = false
                                                        _contentMenu.close()
                                                        btnChecked = false; --gFileItem.selectCount
                                                        var printerID = gFileItem.currentDevice
                                                        var printerPath = gFileItem.currentFilePath

                                                        if(printerID !== "" && printerPath !== "")
                                                        {
                                                            //--gFileItem.selectCount
                                                            printerListModel.cSourceModel.deleteFile(printerID, printerPath)
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }

                                    Rectangle {
                                        width: parent.width
                                        height: 1 * screenScaleFactor
                                        color: btnChecked?"#1E9BE2":"#414145"
                                    }

                                    Rectangle {
                                        width: parent.width
                                        height: 1 * screenScaleFactor
                                        y: (40 + 1)  * screenScaleFactor

                                        color: btnChecked?"#1E9BE2":"#414145"
                                        visible: btnChecked
                                    }

                                    Rectangle {
                                        width: 1 * screenScaleFactor
                                        height: parent.height

                                        color: btnChecked?"#1E9BE2":"#414145"
                                        visible: btnChecked
                                    }

                                    Rectangle {
                                        width: 1 * screenScaleFactor
                                        height: parent.height
                                        x: parent.width - 1 * screenScaleFactor

                                        color: btnChecked?"#1E9BE2":"#414145"
                                        visible: btnChecked
                                    }

                                    Row {
                                        spacing: 0
                                        y: 1 * screenScaleFactor

                                        Item {
                                            width: 300 * screenScaleFactor
                                            height: 40 * screenScaleFactor

                                            Rectangle {
                                                width: 14
                                                height: 14

                                                anchors.left: parent.left
                                                anchors.leftMargin: 20 * screenScaleFactor
                                                anchors.verticalCenter: parent.verticalCenter

                                                radius: 3
                                                border.width: 1
                                                border.color: "#48484C"
                                                color: btnChecked?"#1E9BE2":"#28282B"

                                                Image {
                                                    width: 9
                                                    height: 6
                                                    visible: btnChecked

                                                    anchors.left: parent.left
                                                    anchors.top: parent.top
                                                    anchors.leftMargin: 3
                                                    anchors.topMargin: 4
                                                    source: "qrc:/UI/photo/checkIcon.png"
                                                }
                                            }

                                            Text {
                                                id: idFileName
                                                height: parent.height
                                                width: parent.width - anchors.leftMargin

                                                anchors.left: parent.left
                                                anchors.leftMargin: 64 * screenScaleFactor
                                                anchors.verticalCenter: parent.verticalCenter

                                                verticalAlignment: Text.AlignVCenter
                                                horizontalAlignment: Text.AlignLeft
                                                font.weight: Font.Medium
                                                font.family: Constants.mySystemFont.name
                                                font.pointSize: Constants.labelFontPointSize
                                                text: (fileInfos[0] !== undefined)?fileInfos[0]:" "
                                                elide: Text.ElideMiddle
                                                color: "#CBCBCC"

                                                MouseArea {
                                                    id: idFileNameArea
                                                    hoverEnabled: true
                                                    anchors.fill: parent
                                                    acceptedButtons: Qt.NoButton
                                                }
                                            }

                                            ToolTip.text: idFileName.text
                                            ToolTip.visible: idFileNameArea.containsMouse && idFileName.truncated//elide set
                                        }

                                        Rectangle {
                                            width: 100 * screenScaleFactor
                                            height: 40 * screenScaleFactor
                                            color: "transparent"

                                            Text {
                                                width: parent.width
                                                anchors.centerIn: parent
                                                verticalAlignment: Text.AlignVCenter
                                                horizontalAlignment: Text.AlignHCenter
                                                font.weight: Font.Medium
                                                font.family: Constants.mySystemFont.name
                                                font.pointSize: Constants.labelFontPointSize
                                                text: (fileInfos[1] !== undefined)?fileInfos[1]:" "
                                                elide: Text.ElideMiddle
                                                color: "#CBCBCC"
                                            }
                                        }

                                        Rectangle {
                                            width: 100 * screenScaleFactor
                                            height: 40 * screenScaleFactor
                                            color: "transparent"

                                            Text {
                                                width: parent.width
                                                anchors.centerIn: parent
                                                verticalAlignment: Text.AlignVCenter
                                                horizontalAlignment: Text.AlignHCenter
                                                font.weight: Font.Medium
                                                font.family: Constants.mySystemFont.name
                                                font.pointSize: Constants.labelFontPointSize
                                                text: (fileInfos[2] !== undefined)?fileInfos[2]:" "
                                                elide: Text.ElideMiddle
                                                color: "#CBCBCC"
                                            }
                                        }
                                   }
                                }
                            }
                        }
                    }
                }
            }

            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumWidth: 8 * screenScaleFactor
                Layout.maximumWidth: 16 * screenScaleFactor
            }

            ColumnLayout {
                Layout.alignment: Qt.AlignTop
                spacing: 16 * screenScaleFactor

                ColumnLayout {
                    spacing: 0
                    id: cameraItem
                    property bool showPopup: false

                    CusRoundedBg {
                        leftTop: true
                        rightTop: true
                        clickedable: false
                        borderWidth: 1
                        borderColor: "#0F0F10"
                        color: "#37373B"
                        radius: 5

                        Layout.fillWidth: true
                        Layout.minimumWidth: 626 * screenScaleFactor
                        Layout.maximumWidth: 932 * screenScaleFactor
                        Layout.preferredHeight: 40 * screenScaleFactor

                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 20 * screenScaleFactor
                            anchors.rightMargin: 20 * screenScaleFactor
                            spacing: 10 * screenScaleFactor

                            Image {
                                Layout.preferredWidth: 11
                                Layout.preferredHeight: 14
                                source: "qrc:/UI/photo/cameraIcon.png"
                            }

                            Text {
                                Layout.preferredWidth: contentWidth * screenScaleFactor
                                Layout.preferredHeight: 14 * screenScaleFactor

                                verticalAlignment: Text.AlignVCenter
                                font.weight: Font.Bold
                                font.family: Constants.mySystemFont.name
                                font.pointSize: Constants.labelFontPointSize
                                text: qsTr("Camera")
                                color: "white"
                            }

                            Item {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                            }

                            Item {
                                Layout.preferredWidth: 20 * screenScaleFactor
                                Layout.preferredHeight: 12 * screenScaleFactor

                                Image {
                                    anchors.centerIn: parent
                                    source: `qrc:/UI/photo/${printerItem.showPopup?"upArrow":"downArrow"}.png`
                                }

                                MouseArea {
                                    hoverEnabled: true
                                    anchors.fill: parent
                                    cursorShape: containsMouse?Qt.PointingHandCursor:Qt.ArrowCursor

                                    onClicked: cameraItem.showPopup = !cameraItem.showPopup
                                }
                            }
                        }
                    }

                    CusRoundedBg {
                        id: cameraPopup
                        visible: parent.showPopup
                        leftBottom: true
                        rightBottom: true
                        clickedable: false
                        borderWidth: 1
                        borderColor: "#0F0F10"
                        color: "#28282B"
                        radius: 5

                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.minimumWidth: 626 * screenScaleFactor
                        Layout.maximumWidth: 932 * screenScaleFactor
                        Layout.minimumHeight: 350 * screenScaleFactor
                        Layout.maximumHeight: 700 * screenScaleFactor

                        Rectangle {
                            radius: 5
                            color: "#19191B"
                            anchors.fill: parent
                            anchors.margins: 20 * screenScaleFactor

                            QMLPlayer {
                                visible: true
                                id: idVideoPlayer
                                anchors.fill: parent
                                anchors.margins: 10 * screenScaleFactor

                                Component.onDestruction: stop()
                            }
                        }
                    }
                }

                ColumnLayout {
                    spacing: 0
                    id: temperatureItem
                    property bool showPopup: true
                    property bool isWlanPrinting: deviceItem?(deviceItem.pcPrinterMethod === 1):false

                    CusRoundedBg {
                        leftTop: true
                        rightTop: true
                        clickedable: false
                        borderWidth: 1
                        borderColor: "#0F0F10"
                        color: "#37373B"
                        radius: 5

                        Layout.fillWidth: true
                        Layout.minimumWidth: 626 * screenScaleFactor
                        Layout.maximumWidth: 932 * screenScaleFactor
                        Layout.preferredHeight: 40 * screenScaleFactor

                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 20 * screenScaleFactor
                            anchors.rightMargin: 20 * screenScaleFactor
                            spacing: 10 * screenScaleFactor

                            Image {
                                Layout.preferredWidth: 12
                                Layout.preferredHeight: 14
                                source: "qrc:/UI/photo/white_temperatureIcon.png"
                            }

                            Text {
                                Layout.preferredWidth: contentWidth * screenScaleFactor
                                Layout.preferredHeight: 14 * screenScaleFactor

                                verticalAlignment: Text.AlignVCenter
                                font.weight: Font.Bold
                                font.family: Constants.mySystemFont.name
                                font.pointSize: Constants.labelFontPointSize
                                text: qsTr("Temperature")
                                color: "white"
                            }

                            Item {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                            }

                            Item {
                                Layout.preferredWidth: 20 * screenScaleFactor
                                Layout.preferredHeight: 12 * screenScaleFactor

                                Image {
                                    anchors.centerIn: parent
                                    source: `qrc:/UI/photo/${printerItem.showPopup?"upArrow":"downArrow"}.png`
                                }

                                MouseArea {
                                    hoverEnabled: true
                                    anchors.fill: parent
                                    cursorShape: containsMouse?Qt.PointingHandCursor:Qt.ArrowCursor

                                    onClicked: {
                                        temperatureItem.showPopup = !temperatureItem.showPopup
                                        temperaturePopup.visible = temperatureItem.showPopup
                                    }
                                }
                            }
                        }
                    }

                    CusRoundedBg {
                        id: temperaturePopup
                        visible: true
                        leftBottom: true
                        rightBottom: true
                        clickedable: false
                        borderWidth: 1
                        borderColor: "#0F0F10"
                        color: "#28282B"
                        radius: 5

                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.minimumWidth: 626 * screenScaleFactor
                        Layout.maximumWidth: 932 * screenScaleFactor
                        Layout.minimumHeight: 320 * screenScaleFactor
                        Layout.maximumHeight: 500 * screenScaleFactor

                        ColumnLayout {
                            spacing: 0
                            anchors.fill: parent

                            Rectangle {
                                color: "transparent"
                                Layout.preferredWidth: parent.width - 2 * screenScaleFactor
                                Layout.preferredHeight: 40 * screenScaleFactor

                                RowLayout {
                                    anchors.fill: parent
                                    spacing: 10 * screenScaleFactor
                                    anchors.leftMargin: 20 * screenScaleFactor
                                    anchors.rightMargin: 20 * screenScaleFactor

                                    Item {
                                        Layout.preferredWidth: 12
                                        Layout.preferredHeight: 14
                                    }

                                    Text {
                                        Layout.preferredWidth: contentWidth * screenScaleFactor
                                        Layout.preferredHeight: 14 * screenScaleFactor

                                        verticalAlignment: Text.AlignVCenter
                                        font.weight: Font.Medium
                                        font.family: Constants.mySystemFont.name
                                        font.pointSize: Constants.labelFontPointSize
                                        text: qsTr("Project")
                                        color: "#CBCBCC"
                                    }

                                    Item {
                                        Layout.fillWidth: true
                                        Layout.fillHeight: true
                                        Layout.minimumWidth: 100 * screenScaleFactor
                                        Layout.maximumWidth: 570 * screenScaleFactor
                                    }

                                    RowLayout {
                                        spacing: 80 * screenScaleFactor

                                        Text {
                                            Layout.preferredWidth: contentWidth * screenScaleFactor
                                            Layout.preferredHeight: 14 * screenScaleFactor

                                            verticalAlignment: Text.AlignVCenter
                                            font.weight: Font.Medium
                                            font.family: Constants.mySystemFont.name
                                            font.pointSize: Constants.labelFontPointSize
                                            text: qsTr("Current")
                                            color: "#CBCBCC"
                                        }

                                        Text {
                                            Layout.preferredWidth: contentWidth * screenScaleFactor
                                            Layout.preferredHeight: 14 * screenScaleFactor

                                            verticalAlignment: Text.AlignVCenter
                                            font.weight: Font.Medium
                                            font.family: Constants.mySystemFont.name
                                            font.pointSize: Constants.labelFontPointSize
                                            text: qsTr("Target")
                                            color: "#CBCBCC"
                                        }
                                    }
                                }
                            }

                            Rectangle {
                                color: "#414145"
                                Layout.preferredWidth: parent.width - 2 * screenScaleFactor
                                Layout.preferredHeight: 1 * screenScaleFactor
                            }

                            Rectangle {
                                color: "transparent"
                                Layout.preferredWidth: parent.width - 2 * screenScaleFactor
                                Layout.preferredHeight: 40 * screenScaleFactor

                                RowLayout {
                                    anchors.fill: parent
                                    spacing: 10 * screenScaleFactor
                                    anchors.leftMargin: 20 * screenScaleFactor
                                    anchors.rightMargin: 20 * screenScaleFactor

                                    Image {
                                        Layout.preferredWidth: 12
                                        Layout.preferredHeight: 14
                                        source: "qrc:/UI/photo/bedDst_temperatureIcon.png"
                                    }

                                    Text {
                                        Layout.preferredWidth: contentWidth * screenScaleFactor
                                        Layout.preferredHeight: 14 * screenScaleFactor

                                        verticalAlignment: Text.AlignVCenter
                                        font.weight: Font.Medium
                                        font.family: Constants.mySystemFont.name
                                        font.pointSize: Constants.labelFontPointSize
                                        text: qsTr("Hot Bed")
                                        color: "#CBCBCC"
                                    }

                                    Item {
                                        Layout.fillWidth: true
                                        Layout.fillHeight: true
                                        Layout.minimumWidth: 100 * screenScaleFactor
                                        Layout.maximumWidth: 570 * screenScaleFactor
                                    }

                                    RowLayout {
                                        spacing: 40 * screenScaleFactor

                                        Text {
                                            Layout.preferredWidth: contentWidth * screenScaleFactor
                                            Layout.preferredHeight: 14 * screenScaleFactor

                                            verticalAlignment: Text.AlignVCenter
                                            font.weight: Font.Medium
                                            font.family: Constants.mySystemFont.name
                                            font.pointSize: Constants.labelFontPointSize
                                            text: `${deviceItem?deviceItem.pcBedTemp:"undefined"}°C`
                                            color: "white"
                                        }

                                        Text {
                                            Layout.preferredWidth: contentWidth * screenScaleFactor
                                            Layout.preferredHeight: 14 * screenScaleFactor

                                            verticalAlignment: Text.AlignVCenter
                                            font.weight: Font.Medium
                                            font.family: Constants.mySystemFont.name
                                            font.pointSize: Constants.labelFontPointSize
                                            color: "#CBCBCC"
                                            text: "/"
                                        }

                                        Rectangle {
                                            id: __bedDstTempBox
                                            property int minValue: 0
                                            property int maxValue: 200
                                            property var delayShow: false
                                            property var bedDstTemp: deviceItem?deviceItem.pcBedDstTemp:"undefined"
                                            property alias boxValue: __editBedDstTempBox.text

                                            function decrease() {
                                                if(boxValue != "" && boxValue > minValue)
                                                {
                                                    var value = parseInt(boxValue) - 1
                                                    boxValue = value
                                                }
                                            }
                                            function increase() {
                                                if(boxValue != "" && boxValue < maxValue)
                                                {
                                                    var value = parseInt(boxValue) + 1
                                                    boxValue = value
                                                }
                                            }
                                            function specifyPcBedDstTemp() {
                                                var ipAddr = deviceItem?deviceItem.pcIpAddr:""
                                                if(ipAddr !== "")
                                                {
                                                    delayShow = true
                                                    __bedDstTempDelayShow.start()
                                                    printerListModel.cSourceModel.sendUIControlCmdToDevice(ipAddr, LanPrinterListLocal.PrintControlType.CONTROL_CMD_BEDTEMP, boxValue)
                                                }
                                            }

                                            Layout.preferredWidth: 120 * screenScaleFactor
                                            Layout.preferredHeight: 36 * screenScaleFactor

                                            color: "#28282B"
                                            opacity: enabled ? 1.0 : 0.7
                                            enabled: temperatureItem.isWlanPrinting
                                            border.color: __editBedDstTempBox.activeFocus?"#3699FF":"#4F4F54"
                                            border.width: 1
                                            radius: 5

                                            Binding on boxValue {
                                                when: !__editBedDstTempBox.activeFocus && !__bedDstTempBox.delayShow
                                                value: __bedDstTempBox.bedDstTemp
                                            }

                                            Timer {
                                                repeat: false
                                                interval: 10000
                                                id: __bedDstTempDelayShow
                                                onTriggered: __bedDstTempBox.delayShow = false
                                            }

                                            TextField {
                                                id: __editBedDstTempBox
                                                width: parent.width
                                                height: parent.height
                                                anchors.left: parent.left
                                                anchors.verticalCenter: parent.verticalCenter

                                                validator: RegExpValidator{regExp: new RegExp("[\\d]|[1-9][\\d]|[1][\\d][\\d]|200")}
                                                selectByMouse: true
                                                selectionColor: "#1E9BE2"
                                                selectedTextColor : color
                                                topPadding: 0
                                                bottomPadding: 0
                                                leftPadding: 10 * screenScaleFactor
                                                rightPadding: 4 * screenScaleFactor
                                                font.weight: Font.Medium
                                                font.family: Constants.mySystemFont.name
                                                font.pointSize: Constants.labelFontPointSize
                                                verticalAlignment: TextInput.AlignVCenter
                                                horizontalAlignment: TextInput.AlignLeft
                                                background: null
                                                color: "white"

                                                Keys.onUpPressed: __bedDstTempBox.increase()
                                                Keys.onDownPressed: __bedDstTempBox.decrease()
                                                Keys.onEnterPressed: parent.forceActiveFocus()
                                                Keys.onReturnPressed: parent.forceActiveFocus()
                                                onEditingFinished: __bedDstTempBox.specifyPcBedDstTemp()

                                                Text {
                                                    anchors.right: parent.right
                                                    anchors.rightMargin: 10 * screenScaleFactor
                                                    anchors.verticalCenter: parent.verticalCenter
                                                    verticalAlignment: Text.AlignVCenter
                                                    font.weight: Font.Medium
                                                    font.family: Constants.mySystemFont.name
                                                    font.pointSize: Constants.labelFontPointSize
                                                    color: "#CBCBCC"
                                                    text: "°C"
                                                }
                                            }
                                        }
                                    }
                                }
                            }

                            Rectangle {
                                color: "#414145"
                                Layout.preferredWidth: parent.width - 2 * screenScaleFactor
                                Layout.preferredHeight: 1 * screenScaleFactor
                            }

                            Rectangle {
                                color: "transparent"
                                Layout.preferredWidth: parent.width - 2 * screenScaleFactor
                                Layout.preferredHeight: 40 * screenScaleFactor

                                RowLayout {
                                    anchors.fill: parent
                                    spacing: 10 * screenScaleFactor
                                    anchors.leftMargin: 20 * screenScaleFactor
                                    anchors.rightMargin: 20 * screenScaleFactor

                                    Image {
                                        Layout.preferredWidth: 12
                                        Layout.preferredHeight: 14
                                        source: "qrc:/UI/photo/nozzleDst_temperatureIcon.png"
                                    }

                                    Text {
                                        Layout.preferredWidth: contentWidth * screenScaleFactor
                                        Layout.preferredHeight: 14 * screenScaleFactor

                                        verticalAlignment: Text.AlignVCenter
                                        font.weight: Font.Medium
                                        font.family: Constants.mySystemFont.name
                                        font.pointSize: Constants.labelFontPointSize
                                        text: qsTr("Nozzle")
                                        color: "#CBCBCC"
                                    }

                                    Item {
                                        Layout.fillWidth: true
                                        Layout.fillHeight: true
                                        Layout.minimumWidth: 100 * screenScaleFactor
                                        Layout.maximumWidth: 570 * screenScaleFactor
                                    }

                                    RowLayout {
                                        spacing: 40 * screenScaleFactor

                                        Text {
                                            Layout.preferredWidth: contentWidth * screenScaleFactor
                                            Layout.preferredHeight: 14 * screenScaleFactor

                                            verticalAlignment: Text.AlignVCenter
                                            font.weight: Font.Medium
                                            font.family: Constants.mySystemFont.name
                                            font.pointSize: Constants.labelFontPointSize
                                            text: `${deviceItem?deviceItem.pcNozzleTemp:"undefined"}°C`
                                            color: "white"
                                        }

                                        Text {
                                            Layout.preferredWidth: contentWidth * screenScaleFactor
                                            Layout.preferredHeight: 14 * screenScaleFactor

                                            verticalAlignment: Text.AlignVCenter
                                            font.weight: Font.Medium
                                            font.family: Constants.mySystemFont.name
                                            font.pointSize: Constants.labelFontPointSize
                                            color: "#CBCBCC"
                                            text: "/"
                                        }

                                        Rectangle {
                                            id: __nozzleDstTempBox
                                            property int minValue: 0
                                            property int maxValue: 260
                                            property var delayShow: false
                                            property var nozzleDstTemp: deviceItem?deviceItem.pcNozzleDstTemp:"undefined"
                                            property alias boxValue: __editNozzleDstTempBox.text

                                            function decrease() {
                                                if(boxValue != "" && boxValue > minValue)
                                                {
                                                    var value = parseInt(boxValue) - 1
                                                    boxValue = value
                                                }
                                            }
                                            function increase() {
                                                if(boxValue != "" && boxValue < maxValue)
                                                {
                                                    var value = parseInt(boxValue) + 1
                                                    boxValue = value
                                                }
                                            }
                                            function specifyPcNozzleDstTemp() {
                                                var ipAddr = deviceItem?deviceItem.pcIpAddr:""
                                                if(ipAddr !== "")
                                                {
                                                    delayShow = true
                                                    __nozzleDstTempDelayShow.start()
                                                    printerListModel.cSourceModel.sendUIControlCmdToDevice(ipAddr, LanPrinterListLocal.PrintControlType.CONTROL_CMD_NOZZLETEMP, boxValue)
                                                }
                                            }

                                            Layout.preferredWidth: 120 * screenScaleFactor
                                            Layout.preferredHeight: 36 * screenScaleFactor

                                            color: "#28282B"
                                            opacity: enabled ? 1.0 : 0.7
                                            enabled: temperatureItem.isWlanPrinting
                                            border.color: __editNozzleDstTempBox.activeFocus?"#3699FF":"#4F4F54"
                                            border.width: 1
                                            radius: 5

                                            Binding on boxValue {
                                                when: !__editNozzleDstTempBox.activeFocus && !__nozzleDstTempBox.delayShow
                                                value: __nozzleDstTempBox.nozzleDstTemp
                                            }

                                            Timer {
                                                repeat: false
                                                interval: 10000
                                                id: __nozzleDstTempDelayShow
                                                onTriggered: __nozzleDstTempBox.delayShow = false
                                            }

                                            TextField {
                                                id: __editNozzleDstTempBox
                                                width: parent.width
                                                height: parent.height
                                                anchors.left: parent.left
                                                anchors.verticalCenter: parent.verticalCenter

                                                validator: RegExpValidator{regExp: new RegExp("[\\d]|[1-9][\\d]|[1-2][0-5][\\d]|260")}
                                                selectByMouse: true
                                                selectionColor: "#1E9BE2"
                                                selectedTextColor : color
                                                topPadding: 0
                                                bottomPadding: 0
                                                leftPadding: 10 * screenScaleFactor
                                                rightPadding: 4 * screenScaleFactor
                                                font.weight: Font.Medium
                                                font.family: Constants.mySystemFont.name
                                                font.pointSize: Constants.labelFontPointSize
                                                verticalAlignment: TextInput.AlignVCenter
                                                horizontalAlignment: TextInput.AlignLeft
                                                background: null
                                                color: "white"

                                                Keys.onUpPressed: __nozzleDstTempBox.increase()
                                                Keys.onDownPressed: __nozzleDstTempBox.decrease()
                                                Keys.onEnterPressed: parent.forceActiveFocus()
                                                Keys.onReturnPressed: parent.forceActiveFocus()
                                                onEditingFinished: __nozzleDstTempBox.specifyPcNozzleDstTemp()

                                                Text {
                                                    anchors.right: parent.right
                                                    anchors.rightMargin: 10 * screenScaleFactor
                                                    anchors.verticalCenter: parent.verticalCenter

                                                    verticalAlignment: Text.AlignVCenter
                                                    font.weight: Font.Medium
                                                    font.family: Constants.mySystemFont.name
                                                    font.pointSize: Constants.labelFontPointSize
                                                    color: "#CBCBCC"
                                                    text: "°C"
                                                }
                                            }
                                        }
                                    }
                                }
                            }

                            Rectangle {
                                color: "#414145"
                                Layout.preferredWidth: parent.width - 2 * screenScaleFactor
                                Layout.preferredHeight: 1 * screenScaleFactor
                            }

                            ChartView {
                                id: chartView
                                property bool viewVisible: basicDlg.visible

                                Layout.alignment: Qt.AlignHCenter
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.minimumWidth: 600 * screenScaleFactor
                                Layout.maximumWidth: 900 * screenScaleFactor
                                Layout.minimumHeight: 200 * screenScaleFactor
                                Layout.maximumHeight: 300 * screenScaleFactor

                                antialiasing: true
                                legend.visible: false
                                dropShadowEnabled: true
                                animationDuration: 1000
                                animationOptions: ChartView.SeriesAnimations
                                titleColor: "#CBCBCC"
                                titleFont.weight: Font.Medium
                                titleFont.family: Constants.mySystemFont.name
                                titleFont.pointSize: Constants.labelFontPointSize
                                //title: qsTr("Temperature")+"°C"
                                backgroundColor: "#28282B"

                                Timer {
                                    repeat: true
                                    interval: 1000
                                    id: idRefreshTimer
                                    triggeredOnStart: true
                                    property int count: 0

                                    onTriggered: {
                                        if(count++ < 60)
                                        {
                                            var currentBedTemp = deviceItem?deviceItem.pcBedTemp:"undefined"
                                            var currentNozzleTemp = deviceItem?deviceItem.pcNozzleTemp:"undefined"
                                            var currentUtcTime = printerListModel.cSourceModel.getCurrentUtcTime()

                                            if(currentBedTemp !== "undefined") bedTempSeries.upperSeries.append(currentUtcTime, currentBedTemp)
                                            if(currentNozzleTemp !== "undefined") nozzleTempSeries.upperSeries.append(currentUtcTime, currentNozzleTemp)
                                        }
                                        else {
                                            count = 0
                                            axis_time.min = printerListModel.cSourceModel.getCurrentDateTime(true)
                                            axis_time.max = printerListModel.cSourceModel.getCurrentDateTime(false)
                                        }
                                    }
                                }

                                DateTimeAxis {
                                    id: axis_time
                                    tickCount: 8
                                    format: "hh:mm:ss"
                                    labelsColor: "#CBCBCC"
                                    gridLineColor: "#414145"
                                    labelsFont.weight: Font.Medium
                                    labelsFont.family: Constants.mySystemFont.name
                                    labelsFont.pointSize: Constants.labelFontPointSize
                                }

                                ValueAxis {
                                    id: axis_value
                                    min: 0
                                    max: 300
                                    tickCount: 6
                                    labelFormat: "%.0f"
                                    labelsColor: "#CBCBCC"
                                    gridLineColor: "#414145"
                                    labelsFont.weight: Font.Medium
                                    labelsFont.family: Constants.mySystemFont.name
                                    labelsFont.pointSize: Constants.labelFontPointSize
                                }

                                AreaSeries {
                                    id: bedTempSeries
                                    axisX: axis_time
                                    axisY: axis_value
                                    useOpenGL: true
                                    color: Qt.rgba(40/255, 154/255, 218/255, 0.08)
                                    borderColor: "#289ADA"
                                    borderWidth: 2 * screenScaleFactor
                                    upperSeries: LineSeries{}
                                }

                                AreaSeries {
                                    id: nozzleTempSeries
                                    axisX: axis_time
                                    axisY: axis_value
                                    useOpenGL: true
                                    color: Qt.rgba(165/255, 87/255, 87/255, 0.08)
                                    borderColor: "#A55757"
                                    borderWidth: 2 * screenScaleFactor
                                    upperSeries: LineSeries{}
                                }
                            }
                        }
                    }
                }
            }

            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumWidth: 10 * screenScaleFactor
                Layout.maximumWidth: 20 * screenScaleFactor
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumHeight: 10 * screenScaleFactor
            Layout.maximumHeight: 20 * screenScaleFactor
        }
    }
}
}
