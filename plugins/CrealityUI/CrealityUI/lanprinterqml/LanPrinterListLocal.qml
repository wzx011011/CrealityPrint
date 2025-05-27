import QtQml 2.13
import QtQuick 2.13
import QtQuick.Window 2.13
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.13
import QtGraphicalEffects 1.12
import "../secondqml"
import "../qml"

Rectangle {
    id: basicDlg
    width: parent.width
    height: parent.height
    color: sourceTheme.background_color

    property int themeType: -1
    property bool realEntry: false
    property bool editGroupReadOnly: true

    property var sourceTheme
    property var deviceIDArray: []
    property string gCCodePath: ""
    signal clickDetail(var printerId,var printerName)

    function updateDeviceID(checked, deviceID) {
        var index = deviceIDArray.indexOf(deviceID)
        if(checked && index < 0) deviceIDArray.push(deviceID)
        else if(!checked && index >= 0) deviceIDArray.splice(index, 1)
        //console.log(deviceIDArray)
    }
    function setRealEntry(value) {
        realEntry = value
        stateControl.currentIndex = realEntry ? 0 : 1
    }
    function isDeviceIDEmpty() {
        return (deviceIDArray.length === 0);
    }

    enum PrintControlType
    {
        PRINT_START,
        PRINT_PAUSE,
        PRINT_CONTINUE,
        PRINT_STOP,
        CONTROL_MOVE_X,
        CONTROL_MOVE_Y,
        CONTROL_MOVE_Z,
        CONTROL_MOVE_XY0,
        CONTROL_MOVE_Z0,
        CONTROL_CMD_FAN,
        CONTROL_CMD_LED,
        CONTROL_CMD_BEDTEMP,
        CONTROL_CMD_NOZZLETEMP,
        CONTROL_CMD_FEEDRATEPCT,
        CONTROL_CMD_AUTOHOME
    }

    ListModel {
        id: themeModel
        ListElement {
            //Dark Theme
            background_color: "#363638"

            dialog_title_color: "#6E6E73"
            dialog_shadow_color: "#333333"
            dialog_border_color: "#262626"
            dialog_background_color: "#4B4B4D"

            btn_border_color: "transparent"
            btn_default_color: "#59595D"
            btn_hovered_color: "#6E6E73"

            innerBtn_default_color: "#59595D"
            innerBtn_hovered_color: "#6E6E73"

            text_light_color: "#FFFFFF"
            text_weight_color: "#FFFFFF"
            text_method_color: "#1E9BE2"
            text_special_color: "#7E7E85"

            box_default_color: "#363638"
            box_highlight_color: "#1E9BE2"
            box_border_default: "#6E6E72"
            box_border_highlight: "#1E9BE2"

            scrollbar_color: "#7E7E84"
            progressbar_color: "#343435"
            textedit_selection_color: "#1E9BE2"

            add_printer_border: "#59595D"
            add_printer_border_bg: "#6E6E72"
            add_printer_background: "#4B4B4D"

            right_menu_background: "#1E9BE2"
            right_submenu_text_color: "#414143"
            right_submenu_background: "#FFFFFF"
            right_submenu_selection: "#A5DBF9"

            item_border_real: "#414143"
            item_border_color: "#626266"
            item_checked_color: "#1E9BE2"
            item_checked_border: "#89898D"
            item_crossline_color: "#515153"
            item_deviceImg_color: "#6C6C6F"
            item_deviceImg_border: "transparent"
            item_background_color: "#414143"

            img_downBtn: "qrc:/UI/photo/downBtn.png"
            img_downBtn_d: "qrc:/UI/photo/downBtn_d.png"
            img_checkIcon: "qrc:/UI/photo/checkIcon.png"
            img_edit_device: "qrc:/UI/photo/editDevice.png"
            img_add_printer: "qrc:/UI/photo/add_printer.png"
            img_camera_status: "qrc:/UI/photo/camera_%1.png"
            img_submenu_white: "qrc:/UI/photo/submenu_white.png"
            img_refresh_device: "qrc:/UI/photo/refresh_device.png"

            img_print_time: "qrc:/UI/photo/print_total_time.png"
            imt_print_leftTime: "qrc:/UI/photo/print_left_time.png"
            img_print_progress: "qrc:/UI/photo/print_progress.png"

            img_print_speed: "qrc:/UI/photo/print_speed.png"
            img_bed_temperature: "qrc:/UI/photo/bed_temperature.png"
            img_nozzle_temperature: "qrc:/UI/photo/nozzle_temperature.png"

            img_warning: "qrc:/UI/photo/addPrinter_warning.png"
            img_infomation: "qrc:/UI/photo/addPrinter_infomation.png"
            img_successful: "qrc:/UI/photo/addPrinter_successful.png"
        }
        ListElement {
            //Light Theme
            background_color: "#F2F2F5"

            dialog_title_color: "#FFFFFF"
            dialog_shadow_color: "#BBBBBB"
            dialog_border_color: "transparent"
            dialog_background_color: "#FFFFFF"

            btn_border_color: "#D6D6DC"
            btn_default_color: "#FFFFFF"
            btn_hovered_color: "#E8E8ED"

            innerBtn_default_color: "#ECECEC"
            innerBtn_hovered_color: "#C2C2C5"

            text_light_color: "#666666"
            text_weight_color: "#333333"
            text_method_color: "#333333"
            text_special_color: "#999999"

            box_default_color: "#FFFFFF"
            box_highlight_color: "#98DAFF"
            box_border_default: "#D6D6DC"
            box_border_highlight: "#98DAFF"

            scrollbar_color: "#C7C7CE"
            progressbar_color: "#E5E5E5"
            textedit_selection_color: "#98DAFF"

            add_printer_border: "#CBCBCC"
            add_printer_border_bg: "#CBCBCC"
            add_printer_background: "#FFFFFF"

            right_menu_background: "#1E9BE2"
            right_submenu_text_color: "#333333"
            right_submenu_background: "#FFFFFF"
            right_submenu_selection: "#A5DBF9"

            item_border_real: "#D6D6DC"
            item_border_color: "#D6D6DC"
            item_checked_color: "#1E9BE2"
            item_checked_border: "#D6D6DC"
            item_crossline_color: "#E5E5E5"
            item_deviceImg_color: "#EEEEEE"
            item_deviceImg_border: "#D6D6DC"
            item_background_color: "#FFFFFF"

            img_downBtn: "qrc:/UI/photo/downBtn.png"
            img_downBtn_d: "qrc:/UI/photo/downBtn.png"
            img_checkIcon: "qrc:/UI/photo/checkIcon.png"
            img_edit_device: "qrc:/UI/photo/editDevice.png"
            img_add_printer: "qrc:/UI/photo/add_printer.png"
            img_camera_status: "qrc:/UI/photo/camera_%1.png"
            img_submenu_white: "qrc:/UI/photo/submenu_white.png"
            img_refresh_device: "qrc:/UI/photo/refresh_device.png"

            img_print_time: "qrc:/UI/photo/print_total_time.png"
            imt_print_leftTime: "qrc:/UI/photo/print_left_time.png"
            img_print_progress: "qrc:/UI/photo/print_progress.png"

            img_print_speed: "qrc:/UI/photo/print_speed.png"
            img_bed_temperature: "qrc:/UI/photo/bed_temperature.png"
            img_nozzle_temperature: "qrc:/UI/photo/nozzle_temperature.png"

            img_warning: "qrc:/UI/photo/addPrinter_warning.png"
            img_infomation: "qrc:/UI/photo/addPrinter_infomation.png"
            img_successful: "qrc:/UI/photo/addPrinter_successful.png"
        }
    }

    Binding on themeType {
        when: visible
        value: Constants.currentTheme
    }

    onVisibleChanged: if(visible) printerListModel.cSourceModel.findDeviceList()//发现设备

    onThemeTypeChanged: sourceTheme = themeModel.get(themeType)//切换主题

    Rectangle {
        id: btnRefresh
        width: 120 * screenScaleFactor
        height: 28 * screenScaleFactor
        radius: height / 2
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 20 * screenScaleFactor
        anchors.topMargin: 20 * screenScaleFactor

        border.width: 1
        border.color: sourceTheme.btn_border_color
        color: btnRefreshArea.containsMouse ? sourceTheme.btn_hovered_color : sourceTheme.btn_default_color

        Image {
            id: refreshImg
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: 28 * screenScaleFactor
            source: sourceTheme.img_refresh_device
        }

        Text {
            anchors.left: refreshImg.right
            anchors.leftMargin: 10 * screenScaleFactor
            anchors.verticalCenter: parent.verticalCenter
            verticalAlignment: Text.AlignVCenter
            font.weight: Font.Medium
            font.family: Constants.mySystemFont.name
            font.pointSize: Constants.labelFontPointSize
            text: qsTr("Refresh")
            color: sourceTheme.text_weight_color
        }

        MouseArea {
            id: btnRefreshArea
            hoverEnabled: true
            anchors.fill: parent
            cursorShape: containsMouse?Qt.PointingHandCursor:Qt.ArrowCursor
            onClicked: printerListModel.cSourceModel.refresh()
        }
    }

    Text {
        id: deviceGroupText
        anchors.leftMargin: 20 * screenScaleFactor
        anchors.left: btnRefresh.right
        anchors.verticalCenter: btnRefresh.verticalCenter
        verticalAlignment: Text.AlignVCenter
        font.weight: Font.Normal
        font.family: Constants.mySystemFont.name
        font.pointSize: Constants.labelFontPointSize
        text: qsTr("Device group") + ":"
        color: sourceTheme.text_weight_color
    }

    ComboBox {
        id: deviceControl
        property int viewPadding: 5
        property int extraMargin: 2
        property int showMaxCount: 10//3

        width: 160 * screenScaleFactor
        height: 28 * screenScaleFactor
        anchors.left: deviceGroupText.right
        anchors.verticalCenter: deviceGroupText.verticalCenter
        anchors.leftMargin: 3 * screenScaleFactor

        onCurrentIndexChanged: {
            if(currentIndex != -1) printerListModel.multiConditionFilter(`${stateControl.currentIndex}`, `${currentIndex+1}`)
        }
        Component.onCompleted: {
            model = printerListModel.cSourceModel.getGroups()
        }

        delegate: ItemDelegate {//Item
            width: deviceControl.width
            height: deviceControl.height
            contentItem: Rectangle {
                //radius: 5
                anchors.fill: parent
                color: (deviceControl.highlightedIndex == index) ? sourceTheme.box_highlight_color : sourceTheme.box_default_color
                Text {
                    padding: 8
                    height: parent.height
                    font.weight: Font.Medium
                    font.family: Constants.mySystemFont.name
                    font.pointSize: Constants.labelFontPointSize
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                    text: modelData
                    color: sourceTheme.text_weight_color
                }
            }
        }

        background: Rectangle {
            color: sourceTheme.box_default_color
            border.color: deviceControl.popup.visible ? sourceTheme.box_highlight_color : sourceTheme.box_border_default
            border.width: 1
            radius: 5
        }

        indicator: Item {
            width: 7 * screenScaleFactor
            height: 4 * screenScaleFactor
            visible: editGroupReadOnly
            anchors.right: deviceControl.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.rightMargin: 12

            Image {
                anchors.fill: parent
                source: controlPopupArea.containsMouse ? sourceTheme.img_downBtn_d : sourceTheme.img_downBtn

                MouseArea {
                    id: controlPopupArea
                    anchors.fill: parent
                    hoverEnabled: true
                    acceptedButtons: Qt.NoButton
                }
            }
        }

        contentItem: TextField {
            id: editGroupName
            anchors.fill: parent
            clip: true
            leftPadding: 8.3
            rightPadding: 8.3
            maximumLength: 16
            readOnly: editGroupReadOnly
            enabled: !editGroupReadOnly
            selectByMouse: !editGroupReadOnly
            activeFocusOnPress: !editGroupReadOnly
            selectionColor: sourceTheme.textedit_selection_color
            selectedTextColor: color
            verticalAlignment: TextInput.AlignVCenter
            //horizontalAlignment: Text.AlignHCenter
            color: sourceTheme.text_weight_color
            font.weight: Font.Normal
            font.family: Constants.mySystemFont.name
            font.pointSize: Constants.labelFontPointSize
            text: enabled ? parent.editText : parent.displayText
            background: null

            Keys.onEnterPressed: editGroupReadOnly = true
            Keys.onReturnPressed: editGroupReadOnly = true
            onActiveFocusChanged: if(!activeFocus) editGroupReadOnly = true
            onEditingFinished: {
                if(text !== "" && text !== parent.currentText)
                {
                    var groupNames = parent.model
                    var currentIndex = parent.currentIndex
                    var elementIndex = groupNames.indexOf(text)
                    var newGroupName = (elementIndex < 0) ? text : `${text}(1)`

                    //console.log("newGroupName", newGroupName)
                    groupNames.splice(currentIndex, 1, newGroupName)
                    parent.model = groupNames;parent.currentIndex = currentIndex
                    printerListModel.cSourceModel.editGroupName(currentIndex, newGroupName)
                }
            }
        }

        popup: Popup {
            property alias itemHeight: deviceControl.height
            property alias itemPadding: deviceControl.viewPadding
            property alias btnExtraMargin: deviceControl.extraMargin
            property alias itemMaxCount: deviceControl.showMaxCount

            property int itemCount: deviceControl.delegateModel.count
            property int count: itemCount < itemMaxCount ? itemCount : itemMaxCount

            id: devPopup
            width: deviceControl.width
            y: deviceControl.height + 1// offset
            leftPadding: 1; rightPadding: 1
            bottomPadding: 0; topPadding: itemPadding

            implicitHeight: deviceControl.delegateModel?
                                count*itemHeight + btnExtraMargin + itemHeight + itemPadding*2
                              :itemHeight + itemPadding*2

            contentItem: Item {
                Column {
                    width: parent.width
                    spacing: 2
                    ListView {
                        clip: true
                        width: parent.width
                        implicitHeight: contentHeight < devPopup.itemMaxCount*devPopup.itemHeight?
                                            contentHeight : devPopup.itemMaxCount*devPopup.itemHeight
                        //snapMode: ListView.SnapToItem
                        model: deviceControl.popup.visible?deviceControl.delegateModel:null
                        ScrollBar.vertical: ScrollBar {
                            //id: deviceBar
                            visible: deviceControl.delegateModel ? (devPopup.itemCount > devPopup.itemMaxCount) : false
                        }
                    }
                    Rectangle {
                        radius: 5
                        width: 155 * screenScaleFactor
                        height: devPopup.itemHeight
                        anchors.left: parent.left
                        anchors.leftMargin: 2

                        border.width: 1
                        border.color: sourceTheme.btn_border_color
                        color: btnNewGroupArea.containsMouse ? sourceTheme.innerBtn_hovered_color : sourceTheme.innerBtn_default_color

                        Text {
                            anchors.centerIn: parent
                            verticalAlignment: Text.AlignVCenter
                            font.weight: Font.Medium
                            font.family: Constants.mySystemFont.name
                            font.pointSize: Constants.labelFontPointSize
                            elide: Text.ElideMiddle
                            text: qsTr("New group")
                            color: sourceTheme.text_weight_color
                        }

                        MouseArea {
                            id: btnNewGroupArea
                            hoverEnabled: true
                            anchors.fill: parent
                            cursorShape: containsMouse?Qt.PointingHandCursor:Qt.ArrowCursor

                            onClicked: {
                                var groupNames = deviceControl.model
                                var itemCount = groupNames.length + 1
                                var currentIndex = deviceControl.currentIndex

                                if(itemCount <= devPopup.itemMaxCount)
                                {
                                    var addGroupName = `Group${itemCount}`
                                    groupNames.push(addGroupName)
                                    deviceControl.model = groupNames
                                    deviceControl.currentIndex = currentIndex
                                    printerListModel.cSourceModel.addGroup(addGroupName)
                                }

                                deviceControl.popup.visible = false
                            }
                        }
                    }
                }
            }

            background: Rectangle {
                color: sourceTheme.box_default_color
                border.color: deviceControl.popup.visible ? sourceTheme.box_highlight_color : sourceTheme.box_border_default
                border.width: 1
                radius: 5
            }
        }
    }

    Rectangle {
        id: editGroupRect
        color: "transparent"
        width: 14 * screenScaleFactor
        height: 14 * screenScaleFactor

        anchors.left: deviceControl.right
        anchors.leftMargin: 10 * screenScaleFactor
        anchors.verticalCenter: deviceControl.verticalCenter

        Image {
            anchors.centerIn: parent
            source: sourceTheme.img_edit_device
        }

        MouseArea {
            id: editGroupArea
            hoverEnabled: true
            anchors.fill: parent
            cursorShape: containsMouse?Qt.PointingHandCursor:Qt.ArrowCursor
            onClicked: {
                if(deviceControl.model && editGroupReadOnly) {
                    editGroupReadOnly = false
                    editGroupName.forceActiveFocus()
                }
            }
        }
    }

    Text {
        id: stateGroupText
        anchors.left: editGroupRect.right
        anchors.verticalCenter: editGroupRect.verticalCenter
        anchors.leftMargin: 22 * screenScaleFactor
        verticalAlignment: Text.AlignVCenter
        font.weight: Font.Normal
        font.family: Constants.mySystemFont.name
        font.pointSize: Constants.labelFontPointSize
        text: qsTr("State") + "："
        color: sourceTheme.text_weight_color
    }

    ComboBox {
        id: stateControl
        property int viewPadding: 5
        //property int extraMargin: 2
        property int showMaxCount: 3

        width: 160 * screenScaleFactor
        height: 28 * screenScaleFactor
        anchors.left: stateGroupText.right
        anchors.verticalCenter: stateGroupText.verticalCenter
        anchors.leftMargin: 3 * screenScaleFactor

        onCurrentIndexChanged: {
            if(currentIndex != -1) printerListModel.multiConditionFilter(`${currentIndex}`, `${deviceControl.currentIndex+1}`)
        }

        model: [qsTr("Idle"), qsTr("Printing")]

        delegate: ItemDelegate {//Item
            width: stateControl.width
            height: stateControl.height
            contentItem: Rectangle {
                //radius: 5
                anchors.fill: parent
                color: (stateControl.highlightedIndex == index) ? sourceTheme.box_highlight_color : sourceTheme.box_default_color
                Text {
                    padding: 8
                    height: parent.height
                    font.weight: Font.Medium
                    font.family: Constants.mySystemFont.name
                    font.pointSize: Constants.labelFontPointSize
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                    text: modelData
                    color: sourceTheme.text_weight_color
                }
            }
        }

        background: Rectangle {
            color: sourceTheme.box_default_color
            border.color: stateControl.popup.visible ? sourceTheme.box_highlight_color : sourceTheme.box_border_default
            border.width: 1
            radius: 5
        }

        indicator: Item {
            width: 7 * screenScaleFactor
            height: 4 * screenScaleFactor
            anchors.right: stateControl.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.rightMargin: 12

            Image {
                anchors.fill: parent
                source: statePopupArea.containsMouse ? sourceTheme.img_downBtn_d : sourceTheme.img_downBtn

                MouseArea {
                    id: statePopupArea
                    anchors.fill: parent
                    hoverEnabled: true
                    acceptedButtons: Qt.NoButton
                }
            }
        }

        contentItem: Text {//lineEdit
            padding: 8
            font.weight: Font.Normal
            font.family: Constants.mySystemFont.name
            font.pointSize: Constants.labelFontPointSize
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
            text: stateControl.displayText
            color: sourceTheme.text_weight_color
        }

        popup: Popup {
            property alias itemHeight: stateControl.height
            property alias itemPadding: stateControl.viewPadding
            //property alias btnExtraMargin: stateControl.extraMargin
            property alias itemMaxCount: stateControl.showMaxCount
            property int itemCount: stateControl.delegateModel.count

            id: staPopup
            width: stateControl.width
            y: stateControl.height + 1//offset
            leftPadding: 1; rightPadding: 1
            bottomPadding: 0; topPadding: itemPadding

            implicitHeight: itemCount*itemHeight + itemPadding*2

            contentItem: Item {
                Column {
                    width: parent.width
                    spacing: 0
                    ListView {
                        clip: true
                        width: parent.width
                        implicitHeight: contentHeight < staPopup.itemMaxCount*staPopup.itemHeight?
                                            contentHeight : staPopup.itemMaxCount*staPopup.itemHeight
                        //snapMode: ListView.SnapToItem
                        model: stateControl.popup.visible?stateControl.delegateModel:null
                        ScrollBar.vertical: ScrollBar {
                            //id: deviceBar
                            visible: stateControl.delegateModel ? (staPopup.itemCount > staPopup.itemMaxCount) : false
                        }
                    }
                }
            }

            background: Rectangle {
                color: sourceTheme.box_default_color
                border.color: stateControl.popup.visible ? sourceTheme.box_highlight_color : sourceTheme.box_border_default
                border.width: 1
                radius: 5
            }
        }
    }

    BasicDialog_V4 {
        visible: false
        id: idAddPrinterDlg
        title: qsTr("Add printer")
        width: 500 * screenScaleFactor
        height: 152 * screenScaleFactor
        shadowColor: sourceTheme.dialog_shadow_color
        borderColor: sourceTheme.dialog_border_color
        titleBgColor: sourceTheme.dialog_title_color
        titleFtColor: sourceTheme.text_weight_color
        backgroundColor: sourceTheme.dialog_background_color

        function resetStatus() {
            cloader.item.resetAllPage = true
        }

        onClosing: resetStatus()

        bdContentItem: Item {
            property bool resetAllPage: false
            property string curConnectIp: ""

            onResetAllPageChanged: {
                if(resetAllPage)
                {
                    resetAllPage = false
                    idAddPrinterConnect.visible = false
                    idAddPrinterTimeout.visible = false
                    idAddPrinterSuccess.visible = false
                    if(checkTimer.running) checkTimer.stop()
                    idAddPrinterStack.pop(idAddPrinterStack.initialItem)
                }
            }

            StackView {
                clip: true
                anchors.fill: parent
                id: idAddPrinterStack
                initialItem: idAddPrinterMain

                Timer {
                    id: checkTimer
                    interval: 5000
                    onTriggered: {
                        idAddPrinterTimeout.visible = true
                        idAddPrinterStack.push(idAddPrinterTimeout)
                    }
                }

                Connections {
                    target: printerListModel.cSourceModel
                    onSigConnectedIpSuccessed: {
                        if(!checkTimer.running) return

                        checkTimer.stop()
                        idAddPrinterSuccess.visible = true
                        idAddPrinterStack.push(idAddPrinterSuccess)
                    }
                }

                Item {
                    visible: false
                    id: idAddPrinterMain

                    Text {
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.leftMargin: 67.5 * screenScaleFactor
                        anchors.topMargin: 14 * screenScaleFactor
                        verticalAlignment: Text.AlignVCenter
                        font.weight: Font.Medium
                        font.family: Constants.mySystemFont.name
                        font.pointSize: Constants.labelFontPointSize
                        text: qsTr("Ip address") + "："
                        color: sourceTheme.text_weight_color
                    }

                    Row {
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.leftMargin: 67.5 * screenScaleFactor
                        anchors.topMargin: 34 * screenScaleFactor
                        spacing: 11 * screenScaleFactor

                        Repeater {
                            id: editRepeater
                            function getIpAddressText() {
                                var result = ""
                                for(var index=0; index<model.count; ++index)
                                {
                                    var data = model.get(index).data
                                    var split = model.get(index).split
                                    if(data !== "") {result += `${data}${split}`;continue}

                                    return undefined
                                }
                                return result
                            }

                            model: ListModel {
                                ListElement {data: ""; split: "."}
                                ListElement {data: ""; split: "."}
                                ListElement {data: ""; split: "."}
                                ListElement {data: ""; split: ""}
                            }

                            delegate: Rectangle {
                                width: 83 * screenScaleFactor
                                height: 28 * screenScaleFactor
                                radius: 5

                                color: sourceTheme.add_printer_background
                                border.color: sourceTheme.add_printer_border_bg
                                border.width: 1

                                property string addrRegExp: "[\\d]{1,2}|[1][\\d][\\d]|[2][0-4][\\d]|[2][5][0-5]"
                                //property string portRegExp: "[0-9]|[1-9][\\d]{1,3}|[1-5][\\d]{4}|[6][0-5]{2}[0-3][0-5]"

                                TextField {
                                    id: editIpAddress
                                    anchors.fill: parent
                                    selectByMouse: true
                                    activeFocusOnTab: true
                                    activeFocusOnPress: true
                                    selectionColor: sourceTheme.textedit_selection_color
                                    selectedTextColor : color
                                    verticalAlignment: TextInput.AlignVCenter
                                    horizontalAlignment: TextInput.AlignHCenter
                                    color: sourceTheme.text_weight_color
                                    font.weight: Font.Normal
                                    font.family: Constants.mySystemFont.name
                                    font.pointSize: Constants.labelFontPointSize
                                    validator: RegExpValidator{regExp: new RegExp(addrRegExp)}
                                    onEditingFinished: focus = false
                                    onTextChanged: editRepeater.model.setProperty(index, "data", text)
                                    background: null
                                }

                                Text {
                                    anchors.leftMargin: 4 * screenScaleFactor
                                    anchors.left: editIpAddress.right
                                    anchors.verticalCenter: editIpAddress.verticalCenter
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignHCenter
                                    font: editIpAddress.font
                                    color: editIpAddress.color
                                    text: split
                                }
                            }
                        }
                    }

                    Rectangle {
                        radius: height / 2
                        width: 124 * screenScaleFactor
                        height: 28 * screenScaleFactor
                        anchors.topMargin: 76 * screenScaleFactor
                        anchors.top: parent.top
                        anchors.horizontalCenter: parent.horizontalCenter

                        border.width: 1
                        border.color: sourceTheme.add_printer_border
                        color: addComfirmArea.containsMouse ? sourceTheme.innerBtn_hovered_color : sourceTheme.innerBtn_default_color

                        Text {
                            anchors.centerIn: parent
                            verticalAlignment: Text.AlignVCenter
                            font.weight: Font.Medium
                            font.family: Constants.mySystemFont.name
                            font.pointSize: Constants.labelFontPointSize
                            text: qsTr("Add")
                            color: sourceTheme.text_weight_color
                        }

                        MouseArea {
                            id: addComfirmArea
                            hoverEnabled: true
                            anchors.fill: parent
                            cursorShape: containsMouse?Qt.PointingHandCursor:Qt.ArrowCursor
                            onClicked: {
                                var ipAddr = editRepeater.getIpAddressText()
                                if(ipAddr !== undefined)
                                {
                                    curConnectIp = ipAddr
                                    parent.forceActiveFocus()
                                    idAddPrinterConnect.visible = true
                                    idAddPrinterStack.push(idAddPrinterConnect)
                                }
                            }
                        }
                    }
                }

                Item {
                    visible: false
                    id: idAddPrinterConnect

                    onVisibleChanged: {
                        if(visible)
                        {
                            printerListModel.cSourceModel.createConnect(curConnectIp)
                            checkTimer.start()
                        }
                    }

                    Image {
                        anchors.right: idConnectText.left
                        anchors.rightMargin: 10 * screenScaleFactor
                        anchors.verticalCenter: idConnectText.verticalCenter
                        source: sourceTheme.img_infomation
                    }

                    Text {
                        id: idConnectText
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        verticalAlignment: Text.AlignVCenter
                        font.weight: Font.Normal
                        font.family: Constants.mySystemFont.name
                        font.pointSize: Constants.labelFontPointSize
                        text: qsTr("Connecting") + "..."
                        color: sourceTheme.text_weight_color
                    }
                }

                Item {
                    visible: false
                    id: idAddPrinterTimeout

                    Image {
                        anchors.right: idWarningText.left
                        anchors.rightMargin: 10 * screenScaleFactor
                        anchors.verticalCenter: idWarningText.verticalCenter
                        source: sourceTheme.img_warning
                    }

                    Text {
                        id: idWarningText
                        anchors.top: parent.top
                        anchors.topMargin: 28 * screenScaleFactor
                        anchors.horizontalCenter: parent.horizontalCenter
                        verticalAlignment: Text.AlignVCenter
                        font.weight: Font.Normal
                        font.family: Constants.mySystemFont.name
                        font.pointSize: Constants.labelFontPointSize
                        text: qsTr("The printer could not be found") + "!"
                        color: sourceTheme.text_weight_color
                    }

                    Rectangle {
                        radius: height / 2
                        width: 124 * screenScaleFactor
                        height: 28 * screenScaleFactor
                        anchors.topMargin: 63 * screenScaleFactor
                        anchors.top: parent.top
                        anchors.horizontalCenter: parent.horizontalCenter

                        border.width: 1
                        border.color: sourceTheme.add_printer_border
                        color: warningComfirmArea.containsMouse ? sourceTheme.innerBtn_hovered_color : sourceTheme.innerBtn_default_color

                        Text {
                            anchors.centerIn: parent
                            verticalAlignment: Text.AlignVCenter
                            font.weight: Font.Medium
                            font.family: Constants.mySystemFont.name
                            font.pointSize: Constants.labelFontPointSize
                            text: qsTr("OK")
                            color: sourceTheme.text_weight_color
                        }

                        MouseArea {
                            id: warningComfirmArea
                            hoverEnabled: true
                            anchors.fill: parent
                            cursorShape: containsMouse?Qt.PointingHandCursor:Qt.ArrowCursor
                            onClicked: {
                                //idAddPrinterTimeout.visible = false
                                //idAddPrinterStack.pop(idAddPrinterStack.initialItem)
                                idAddPrinterDlg.close()
                                idAddPrinterDlg.resetStatus()
                            }
                        }
                    }
                }

                Item {
                    visible: false
                    id: idAddPrinterSuccess

                    Image {
                        anchors.right: idSuccessText.left
                        anchors.rightMargin: 10 * screenScaleFactor
                        anchors.verticalCenter: idSuccessText.verticalCenter
                        source: sourceTheme.img_successful
                    }

                    Text {
                        id: idSuccessText
                        anchors.top: parent.top
                        anchors.topMargin: 28 * screenScaleFactor
                        anchors.horizontalCenter: parent.horizontalCenter
                        verticalAlignment: Text.AlignVCenter
                        font.weight: Font.Normal
                        font.family: Constants.mySystemFont.name
                        font.pointSize: Constants.labelFontPointSize
                        text: qsTr("The printer added successfully") + "!"
                        color: sourceTheme.text_weight_color
                    }

                    Rectangle {
                        radius: height / 2
                        width: 124 * screenScaleFactor
                        height: 28 * screenScaleFactor
                        anchors.topMargin: 63 * screenScaleFactor
                        anchors.top: parent.top
                        anchors.horizontalCenter: parent.horizontalCenter

                        border.width: 1
                        border.color: sourceTheme.add_printer_border
                        color: successComfirmArea.containsMouse ? sourceTheme.innerBtn_hovered_color : sourceTheme.innerBtn_default_color

                        Text {
                            anchors.centerIn: parent
                            verticalAlignment: Text.AlignVCenter
                            font.weight: Font.Medium
                            font.family: Constants.mySystemFont.name
                            font.pointSize: Constants.labelFontPointSize
                            text: qsTr("OK")
                            color: sourceTheme.text_weight_color
                        }

                        MouseArea {
                            id: successComfirmArea
                            hoverEnabled: true
                            anchors.fill: parent
                            cursorShape: containsMouse?Qt.PointingHandCursor:Qt.ArrowCursor
                            onClicked: {
                                //idAddPrinterSuccess.visible = false
                                //idAddPrinterStack.pop(idAddPrinterStack.initialItem)
                                idAddPrinterDlg.close()
                                idAddPrinterDlg.resetStatus()
                            }
                        }
                    }
                }
            }
        }
    }

    Rectangle {
        width: 100 * screenScaleFactor
        height: 28 * screenScaleFactor
        radius: height / 2
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 20 * screenScaleFactor
        anchors.topMargin: 20 * screenScaleFactor

        border.width: 1
        border.color: sourceTheme.btn_border_color
        color: addPrinterArea.containsMouse ? sourceTheme.btn_hovered_color : sourceTheme.btn_default_color

        Image {
            id: addPrinterImg
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: 24 * screenScaleFactor
            source: sourceTheme.img_add_printer
        }

        Text {
            anchors.left: addPrinterImg.right
            anchors.verticalCenter: addPrinterImg.verticalCenter
            anchors.leftMargin: 10 * screenScaleFactor
            verticalAlignment: Text.AlignVCenter
            font.weight: Font.Medium
            font.family: Constants.mySystemFont.name
            font.pointSize: Constants.labelFontPointSize
            text: qsTr("New")
            color: sourceTheme.text_weight_color
        }

        MouseArea {
            id: addPrinterArea
            hoverEnabled: true
            anchors.fill: parent
            cursorShape: containsMouse?Qt.PointingHandCursor:Qt.ArrowCursor
            onClicked: {
                idAddPrinterDlg.show()
            }
        }
    }

    Rectangle {
        color: "transparent"
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 60 * screenScaleFactor
        anchors.leftMargin: 20 * screenScaleFactor
        anchors.rightMargin: 20 * screenScaleFactor
        anchors.bottomMargin: 170 * screenScaleFactor
        width: parent.width - anchors.leftMargin - anchors.rightMargin
        height: parent.height - anchors.topMargin - anchors.bottomMargin

        BasicScrollView {
            clip: true
            id: idScrollView
            anchors.fill: parent
            //hpolicy: ScrollBar.AsNeeded
            //vpolicy: ScrollBar.AsNeeded
            hpolicyVisible: contentWidth > width
            vpolicyVisible: contentHeight > height
            hpolicyindicator: Rectangle {
                radius: height / 2
                color: sourceTheme.scrollbar_color
                implicitWidth: 180 * screenScaleFactor
                implicitHeight: 6 * screenScaleFactor
            }
            vpolicyindicator: Rectangle {
                radius: width / 2
                color: sourceTheme.scrollbar_color
                implicitWidth: 6 * screenScaleFactor
                implicitHeight: 180 * screenScaleFactor
            }

            Column {
                id: columnLayout
                anchors.fill: parent
                spacing: 10 * screenScaleFactor

                Repeater {
                    model: printerListModel
                    delegate: Rectangle {
                        id: rootItem
                        width: idScrollView.width
                        height: 138 * screenScaleFactor
                        opacity: enabled ? 1.0 : 0.7
                        visible: (printerStatus == 1) //在线状态
                        enabled: curTransProgress <= 0

                        color: sourceTheme.item_background_color
                        border.color: curBtnSelect ? checkedColor : sourceTheme.item_border_real
                        border.width: 1
                        radius: 5

                        property int currentState: printerState
                        property int curBtnSelect: mItem.pcHasSelect
                        property int curTransProgress: mItem.gCodeTransProgress

                        //property bool btnChecked: false
                        property bool editDevReadOnly: true
                        property bool controlEnabled: !stateControl.currentIndex || (stateControl.currentIndex && printerMethod == 1)//空闲 or 局域网打印中

                        readonly property string checkedColor: sourceTheme.item_checked_color
                        readonly property string cameraImagePath: sourceTheme.img_camera_status

                        function second2String(sec) {
                            var hours = Math.floor(sec / 3600)
                            var minutes = Math.floor(sec % 3600 / 60)
                            var seconds = Math.floor(sec % 3600 % 60)
                            return `${hours}h ${minutes}m ${seconds}s`
                        }
                        function getPrinterMethodText(method) {
                            var methodText = " "
                            switch(method) {
                            case 0:
                                methodText = qsTr("TF Card Printing")
                                break;
                            case 1:
                                methodText = qsTr("WLAN Printing")
                                break;
                            case 2:
                                methodText = qsTr("CrealityCloud Printing")
                                break;
                            default:
                                break;
                            }
                            return methodText
                        }

                        onCurBtnSelectChanged: updateDeviceID(curBtnSelect, printerID)
                        onCurrentStateChanged: if(curTransProgress == 100 && currentState == 1) mItem.gCodeTransProgress = 0

                        MouseArea {
                            id: itemArea
                            anchors.fill: parent
                            acceptedButtons: Qt.LeftButton | Qt.RightButton
                            onClicked: {
                                if(mouse.button === Qt.LeftButton)
                                {
                                    rootItem.forceActiveFocus()
                                }
                                else {
                                    if(curBtnSelect) contentMenu.openMenu(mouseX, mouseY)
                                }
                            }
                        }

                        Popup {
                            id: contentMenu
                            background: null
                            function openMenu(mouseX, mouseY){
                                x = mouseX
                                y = mouseY
                                open()
                            }

                            Rectangle {
                                //radius: 5
                                width: 160 * screenScaleFactor
                                height: 28 * screenScaleFactor
                                color: sourceTheme.right_menu_background

                                Text {
                                    leftPadding: 8 * screenScaleFactor
                                    elide: Text.ElideRight
                                    verticalAlignment: Text.AlignVCenter
                                    anchors.verticalCenter: parent.verticalCenter
                                    font.weight: Font.Medium
                                    font.family: Constants.mySystemFont.name
                                    font.pointSize: Constants.labelFontPointSize
                                    text: qsTr("Move to")
                                    color: sourceTheme.text_weight_color
                                }

                                Image {
                                    anchors.right: parent.right
                                    anchors.rightMargin: 10 * screenScaleFactor
                                    anchors.verticalCenter: parent.verticalCenter
                                    source: sourceTheme.img_submenu_white
                                }

                                MouseArea {
                                    hoverEnabled: true
                                    anchors.fill: parent
                                    acceptedButtons: Qt.NoButton
                                    onContainsMouseChanged: if(containsMouse) subMenu.open()
                                }
                            }

                            Menu {
                                id: subMenu
                                y: 0
                                x: 160 * screenScaleFactor
                                enabled: groupsInstantiator.count > 0

                                background: Rectangle {
                                    implicitWidth: 160 * screenScaleFactor
                                    implicitHeight: groupsInstantiator.count * 28 * screenScaleFactor
                                    color: "transparent"
                                    border.color: "transparent"
                                    border.width: 0
                                    //radius: 5
                                }

                                Instantiator {
                                    id: groupsInstantiator
                                    model: deviceControl.model
                                    onObjectRemoved: subMenu.removeItem(object)
                                    onObjectAdded: subMenu.insertItem(index, object)

                                    delegate: MenuItem {
                                        id: menuItem
                                        enabled: groupNo != (index+1)
                                        implicitWidth: 160 * screenScaleFactor
                                        implicitHeight: 28 * screenScaleFactor

                                        onTriggered: {
                                            printerListModel.cSourceModel.editDeviceGroup(printerID, index+1)
                                            contentMenu.close()
                                        }

                                        //用于定制选项文本
                                        contentItem: Text {
                                            leftPadding: 8 * screenScaleFactor
                                            elide: Text.ElideRight
                                            opacity: enabled ? 1.0 : 0.7
                                            verticalAlignment: Text.AlignVCenter
                                            font.weight: Font.Medium
                                            font.family: Constants.mySystemFont.name
                                            font.pointSize: Constants.labelFontPointSize
                                            text: modelData
                                            color: sourceTheme.right_submenu_text_color
                                        }

                                        //用于定制选项背景
                                        background: Rectangle {
                                            //radius: 5
                                            color: menuItem.highlighted ? sourceTheme.right_submenu_selection : "white"
                                            border.color: "transparent"
                                            border.width: 0
                                        }
                                    }
                                }
                            }
                        }

                        RowLayout {
                            anchors.fill: parent
                            spacing: 5 * screenScaleFactor

                            Item {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.minimumWidth: 10 * screenScaleFactor - parent.spacing
                                Layout.maximumWidth: 20 * screenScaleFactor - parent.spacing
                            }

                            Rectangle {
                                id: checkButton
                                enabled: controlEnabled
                                opacity: enabled ? 1.0 : 0.7

                                Layout.preferredWidth: 14 * screenScaleFactor
                                Layout.preferredHeight: 14 * screenScaleFactor

                                radius: 3
                                border.width: 1
                                border.color: sourceTheme.item_checked_border
                                color: curBtnSelect ? checkedColor : sourceTheme.item_background_color

                                Image {
                                    visible: curBtnSelect
                                    width: 9 * screenScaleFactor
                                    height: 6 * screenScaleFactor
                                    anchors.top: checkButton.top
                                    anchors.left: checkButton.left
                                    anchors.leftMargin: 3 * screenScaleFactor
                                    anchors.topMargin: 4 * screenScaleFactor
                                    source: sourceTheme.img_checkIcon
                                }

                                MouseArea {
                                    hoverEnabled: true
                                    anchors.fill: parent
                                    cursorShape: containsMouse?Qt.PointingHandCursor:Qt.ArrowCursor
                                    onClicked: {
                                        mItem.pcHasSelect = !mItem.pcHasSelect
                                        parent.forceActiveFocus()
                                    }
                                }
                            }

                            Item {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.minimumWidth: 0
                                Layout.maximumWidth: 20 * screenScaleFactor - parent.spacing * 2
                            }

                            Rectangle {
                                radius: 5
                                id: deviceImg
                                Layout.preferredWidth: 96 * screenScaleFactor
                                Layout.preferredHeight: 96 * screenScaleFactor

                                color: sourceTheme.item_deviceImg_color
                                border.color: sourceTheme.item_deviceImg_border
                                border.width: 1

                                Image {
                                    anchors.centerIn: parent
                                    width: 64 * screenScaleFactor
                                    height: 70 * screenScaleFactor
                                    source: (printerModel != "")?`qrc:/UI/photo/machineImage_${printerModel}.png`:"qrc:/UI/photo/crealityIcon.png"
                                }
                            }

                            Item {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.minimumWidth: 0
                                Layout.maximumWidth: 20 * screenScaleFactor - parent.spacing * 2
                            }

                            ColumnLayout {
                                spacing: 15 * screenScaleFactor

                                RowLayout {
                                    spacing: 20 * screenScaleFactor

                                    Rectangle {
                                        color: "transparent"
                                        Layout.preferredWidth: 11 * screenScaleFactor
                                        Layout.preferredHeight: 14 * screenScaleFactor

                                        Image {
                                            id: cameraImage
                                            anchors.centerIn: parent
                                            source: cameraImagePath.arg(hasCamera?"online":"offline")
                                        }

                                        ToolTip {
                                            x: -10 * screenScaleFactor
                                            y: -implicitHeight - 6 * screenScaleFactor
                                            margins: 6 * screenScaleFactor
                                            padding: 6 * screenScaleFactor

                                            contentItem: Text {
                                                verticalAlignment: Text.AlignVCenter
                                                font.weight: Font.Medium
                                                font.family: Constants.mySystemFont.name
                                                font.pointSize: Constants.labelFontPointSize
                                                text: qsTr("Camera connected")
                                                color: sourceTheme.text_weight_color
                                            }

                                            background: Rectangle {
                                                radius: 5
                                                width: 124 * screenScaleFactor
                                                height: 28 * screenScaleFactor

                                                color: sourceTheme.item_background_color
                                                border.color: sourceTheme.item_border_color
                                                border.width: 1
                                            }

                                            visible: hasCamera && cameraArea.containsMouse
                                        }

                                        MouseArea {
                                            id: cameraArea
                                            hoverEnabled: true
                                            anchors.fill: parent
                                        }
                                    }

                                    Rectangle {
                                        visible: stateControl.currentIndex
                                        color: sourceTheme.item_crossline_color
                                        Layout.preferredWidth: 1 * screenScaleFactor
                                        Layout.preferredHeight: 14 * screenScaleFactor
                                    }

                                    Text {
                                        visible: stateControl.currentIndex
                                        Layout.preferredWidth: 150 * screenScaleFactor
                                        Layout.preferredHeight: 14 * screenScaleFactor

                                        verticalAlignment: Text.AlignVCenter
                                        font.weight: Font.Medium
                                        font.family: Constants.mySystemFont.name
                                        font.pointSize: Constants.labelFontPointSize
                                        text: getPrinterMethodText(printerMethod)
                                        color: sourceTheme.text_method_color
                                    }
                                }

                                RowLayout {
                                    spacing: 10 * screenScaleFactor

                                    Text {
                                        id: deviceText
                                        Layout.preferredWidth: 80 * screenScaleFactor
                                        Layout.preferredHeight: 14 * screenScaleFactor
                                        verticalAlignment: Text.AlignVCenter
                                        font.weight: Font.Medium
                                        font.family: Constants.mySystemFont.name
                                        font.pointSize: Constants.labelFontPointSize
                                        text: qsTr("Device Name") + ":"
                                        color: sourceTheme.text_light_color
                                    }

                                    Rectangle {
                                        Layout.minimumWidth: 100 * screenScaleFactor
                                        Layout.maximumWidth: 200 * screenScaleFactor
                                        Layout.preferredHeight: 28 * screenScaleFactor
                                        Layout.fillWidth: true

                                        color: sourceTheme.item_background_color
                                        border.color: sourceTheme.item_border_color
                                        border.width: 1
                                        radius: 5

                                        TextField {
                                            id: editDeviceName
                                            anchors.fill: parent
                                            property var delayShow: false
                                            property bool widthExceed: contentWidth > (width - leftPadding - rightPadding)

                                            Binding on text {
                                                when: !editDeviceName.activeFocus && !editDeviceName.delayShow
                                                value: mItem.pcDeviceName
                                            }

                                            Timer {
                                                repeat: false
                                                interval: 10000
                                                id: deviceNameDelayShow
                                                onTriggered: editDeviceName.delayShow = false
                                            }

                                            leftPadding: 8.3 * screenScaleFactor
                                            rightPadding: 8.3 * screenScaleFactor
                                            maximumLength: 48
                                            readOnly: editDevReadOnly
                                            enabled: !editDevReadOnly
                                            selectByMouse: !editDevReadOnly
                                            activeFocusOnPress: !editDevReadOnly
                                            selectionColor: sourceTheme.textedit_selection_color
                                            selectedTextColor: color
                                            verticalAlignment: TextInput.AlignVCenter
                                            //horizontalAlignment: Text.AlignHCenter
                                            color: sourceTheme.text_weight_color
                                            font.weight: Font.Medium
                                            font.family: Constants.mySystemFont.name
                                            font.pointSize: Constants.labelFontPointSize
                                            background: null

                                            Keys.onEnterPressed: rootItem.forceActiveFocus()
                                            Keys.onReturnPressed: rootItem.forceActiveFocus()
                                            onActiveFocusChanged: if(!activeFocus) editDevReadOnly = true
                                            onEditingFinished: {
                                                editDeviceName.delayShow = true; deviceNameDelayShow.start()
                                                printerListModel.cSourceModel.editDeviceName(printerID, displayText)
                                            }
                                        }

                                        MouseArea {
                                            id: hoverArea
                                            hoverEnabled: true
                                            anchors.fill: parent
                                            acceptedButtons: Qt.NoButton
                                        }

                                        ToolTip.text: editDeviceName.displayText
                                        ToolTip.visible: hoverArea.containsMouse && editDeviceName.widthExceed && !editDeviceName.activeFocus
                                    }

                                    Rectangle {
                                        color: "transparent"
                                        Layout.preferredWidth: 14 * screenScaleFactor
                                        Layout.preferredHeight: 14 * screenScaleFactor

                                        Image {
                                            anchors.centerIn: parent
                                            source: sourceTheme.img_edit_device
                                        }

                                        MouseArea {
                                            id: editButtonArea
                                            hoverEnabled: true
                                            anchors.fill: parent
                                            cursorShape: containsMouse?Qt.PointingHandCursor:Qt.ArrowCursor
                                            onClicked: {
                                                if(editDevReadOnly) {
                                                    editDevReadOnly = false
                                                    editDeviceName.forceActiveFocus()
                                                }
                                            }
                                        }
                                    }
                                }

                                RowLayout {
                                    spacing: 10 * screenScaleFactor

                                    Text {
                                        Layout.preferredWidth: 80 * screenScaleFactor
                                        Layout.preferredHeight: 14 * screenScaleFactor
                                        verticalAlignment: Text.AlignVCenter
                                        font.weight: Font.Medium
                                        font.family: Constants.mySystemFont.name
                                        font.pointSize: Constants.labelFontPointSize
                                        text: qsTr("Ip address") + " : "
                                        color: sourceTheme.text_light_color
                                    }

                                    Text {
                                        Layout.preferredWidth: 120 * screenScaleFactor
                                        Layout.preferredHeight: 14 * screenScaleFactor
                                        verticalAlignment: Text.AlignVCenter
                                        font.weight: Font.Medium
                                        font.family: Constants.mySystemFont.name
                                        font.pointSize: Constants.labelFontPointSize
                                        color: sourceTheme.text_weight_color
                                        text: ipAddr
                                    }
                                }
                            }

                            Item {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.minimumWidth: 0
                                Layout.maximumWidth: 50 * screenScaleFactor - parent.spacing * 2
                            }

                            Rectangle {
                                color: sourceTheme.item_crossline_color
                                Layout.preferredWidth: 1 * screenScaleFactor
                                Layout.preferredHeight: 96 * screenScaleFactor
                            }

                            Item {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.minimumWidth: 0
                                Layout.maximumWidth: 50 * screenScaleFactor - parent.spacing * 2
                            }

                            Rectangle {
                                id: gcodeImg
                                Layout.preferredWidth: 62 * screenScaleFactor
                                Layout.preferredHeight: 62 * screenScaleFactor
                                radius: 5

                                color: sourceTheme.item_background_color
                                border.color: sourceTheme.item_border_color
                                border.width: 1

                                Image {
                                    smooth: true
                                    anchors.centerIn: parent
                                    width: 48 * screenScaleFactor
                                    height: 48 * screenScaleFactor
                                    visible: stateControl.currentIndex
                                    source: `image://preview/icons/${printerID}`
                                }
                            }

                            ColumnLayout {
                                spacing: 15 * screenScaleFactor

                                Text {
                                    function getModelName(absPath) {
                                        var modelName = " "
                                        if(absPath !== " ") {
                                            var array = absPath.split("/")
                                            if(array.length) modelName = array[array.length - 1]
                                        }

                                        return modelName
                                    }
                                    property var realGcodeName: stateControl.currentIndex ? gCodeName : idleModelName
                                    property var idleModelName: (curTransProgress != 100) ? " " : getModelName(gCCodePath)

                                    Layout.minimumWidth: 70 * screenScaleFactor
                                    Layout.maximumWidth: 210 * screenScaleFactor
                                    Layout.preferredHeight: contentHeight
                                    Layout.fillWidth: true
                                    verticalAlignment: Text.AlignVCenter
                                    font.weight: Font.Medium
                                    font.family: Constants.mySystemFont.name
                                    font.pointSize: Constants.labelFontPointSize
                                    lineHeight: 18 * screenScaleFactor
                                    lineHeightMode: Text.FixedHeight
                                    wrapMode: Text.WrapAnywhere//explicit width
                                    text: qsTr("G-code") + ":" + (realGcodeName !== " " ? ("\n" + realGcodeName) : realGcodeName)
                                    color: sourceTheme.text_light_color
                                }

                                RowLayout {
                                    spacing: 10 * screenScaleFactor
                                    visible: (curTransProgress != 0) && !stateControl.currentIndex//空闲页面传输中

                                    Rectangle {
                                        radius: 1
                                        color: sourceTheme.progressbar_color

                                        Layout.fillWidth: true
                                        Layout.minimumWidth: 72 * screenScaleFactor
                                        Layout.maximumWidth: 120 * screenScaleFactor
                                        Layout.preferredHeight: 2 * screenScaleFactor

                                        Rectangle {
                                            width: parent.width * curTransProgress / 100.0
                                            height: 2 * screenScaleFactor
                                            anchors.left: parent.left
                                            anchors.top: parent.top
                                            color: "#1E9BE2"
                                            radius: 1
                                        }
                                    }

                                    Text {
                                        property var showTransText: (curTransProgress < 0) ? qsTr("Upload Failed") : `${curTransProgress}%`

                                        Layout.preferredWidth: 100 * screenScaleFactor
                                        Layout.preferredHeight: 14 * screenScaleFactor
                                        verticalAlignment: Text.AlignVCenter
                                        font.weight: Font.Medium
                                        font.family: Constants.mySystemFont.name
                                        font.pointSize: Constants.labelFontPointSize
                                        text: showTransText
                                        color: sourceTheme.text_weight_color
                                    }
                                }
                            }

                            Item {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.minimumWidth: 0
                                Layout.maximumWidth: 50 * screenScaleFactor - parent.spacing * 2
                            }

                            Rectangle {
                                color: sourceTheme.item_crossline_color
                                Layout.preferredWidth: 1 * screenScaleFactor
                                Layout.preferredHeight: 96 * screenScaleFactor
                            }

                            Item {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.minimumWidth: 0
                                Layout.maximumWidth: 50 * screenScaleFactor - parent.spacing * 2
                            }

                            ColumnLayout {
                                spacing: 15 * screenScaleFactor

                                RowLayout {
                                    spacing: 6 * screenScaleFactor

                                    Image {
                                        Layout.preferredWidth: 14
                                        Layout.preferredHeight: 14
                                        source: sourceTheme.img_print_time
                                    }

                                    Text {
                                        Layout.preferredWidth: 70 * screenScaleFactor
                                        Layout.preferredHeight: 14 * screenScaleFactor

                                        verticalAlignment: Text.AlignVCenter
                                        font.weight: Font.Medium
                                        font.family: Constants.mySystemFont.name
                                        font.pointSize: Constants.labelFontPointSize
                                        text: qsTr("Print time") + ": "
                                        color: sourceTheme.text_light_color
                                    }

                                    Text {
                                        Layout.preferredWidth: 120 * screenScaleFactor
                                        Layout.preferredHeight: 14 * screenScaleFactor

                                        verticalAlignment: Text.AlignVCenter
                                        font.weight: Font.Medium
                                        font.family: Constants.mySystemFont.name
                                        font.pointSize: Constants.labelFontPointSize
                                        text: second2String(totalPrintTime)
                                        color: sourceTheme.text_weight_color
                                    }
                                }

                                RowLayout {
                                    spacing: 7 * screenScaleFactor

                                    Image {
                                        Layout.leftMargin: 1
                                        Layout.preferredWidth: 12
                                        Layout.preferredHeight: 14
                                        source: sourceTheme.imt_print_leftTime
                                    }

                                    Text {
                                        Layout.preferredWidth: 70 * screenScaleFactor
                                        Layout.preferredHeight: 14 * screenScaleFactor
                                        verticalAlignment: Text.AlignVCenter
                                        font.weight: Font.Medium
                                        font.family: Constants.mySystemFont.name
                                        font.pointSize: Constants.labelFontPointSize
                                        text: qsTr("Time left") + ": "
                                        color: sourceTheme.text_light_color
                                    }

                                    Text {
                                        Layout.preferredWidth: 120 * screenScaleFactor
                                        Layout.preferredHeight: 14 * screenScaleFactor
                                        verticalAlignment: Text.AlignVCenter
                                        font.weight: Font.Medium
                                        font.family: Constants.mySystemFont.name
                                        font.pointSize: Constants.labelFontPointSize
                                        text: second2String(leftTime)
                                        color: sourceTheme.text_weight_color
                                    }
                                }

                                RowLayout {
                                    spacing: 6 * screenScaleFactor

                                    Image {
                                        Layout.preferredWidth: 14
                                        Layout.preferredHeight: 14
                                        source: sourceTheme.img_print_progress
                                    }

                                    Text {
                                        Layout.preferredWidth: 90 * screenScaleFactor
                                        Layout.preferredHeight: 14 * screenScaleFactor

                                        verticalAlignment: Text.AlignVCenter
                                        font.weight: Font.Medium
                                        font.family: Constants.mySystemFont.name
                                        font.pointSize: Constants.labelFontPointSize
                                        text: qsTr("Print progress")
                                        color: sourceTheme.text_light_color
                                    }

                                    ColumnLayout {
                                        spacing: 7 * screenScaleFactor

                                        Text {
                                            Layout.fillWidth: true
                                            Layout.minimumWidth: 72 * screenScaleFactor
                                            Layout.maximumWidth: 120 * screenScaleFactor
                                            Layout.preferredHeight: 10 * screenScaleFactor

                                            font.weight: Font.Medium
                                            font.family: Constants.mySystemFont.name
                                            font.pointSize: Constants.labelFontPointSize
                                            verticalAlignment: Text.AlignVCenter
                                            horizontalAlignment: Text.AlignHCenter
                                            text: curPrintProgress + "%"
                                            color: sourceTheme.text_light_color
                                        }

                                        Rectangle {
                                            radius: 1
                                            color: sourceTheme.progressbar_color

                                            Layout.fillWidth: true
                                            Layout.minimumWidth: 72 * screenScaleFactor
                                            Layout.maximumWidth: 120 * screenScaleFactor
                                            Layout.preferredHeight: 2 * screenScaleFactor

                                            Rectangle {
                                                width: parent.width * curPrintProgress / 100.0
                                                height: 2 * screenScaleFactor
                                                anchors.left: parent.left
                                                anchors.top: parent.top
                                                color: "#1E9BE2"
                                                radius: 1
                                            }
                                        }
                                    }
                                }
                            }

                            Item {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.minimumWidth: 0
                                Layout.maximumWidth: 50 * screenScaleFactor - parent.spacing * 2
                            }

                            Rectangle {
                                color: sourceTheme.item_crossline_color
                                Layout.preferredWidth: 1 * screenScaleFactor
                                Layout.preferredHeight: 96 * screenScaleFactor
                            }

                            Item {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.minimumWidth: 0
                                Layout.maximumWidth: 50 * screenScaleFactor - parent.spacing * 2
                            }

                            ColumnLayout {
                                spacing: 6 * screenScaleFactor

                                RowLayout {
                                    spacing: 6 * screenScaleFactor

                                    Image {
                                        Layout.preferredWidth: 14
                                        Layout.preferredHeight: 14
                                        source: sourceTheme.img_print_speed
                                    }

                                    Text {
                                        Layout.preferredWidth: 140 * screenScaleFactor
                                        Layout.preferredHeight: 14 * screenScaleFactor
                                        verticalAlignment: Text.AlignVCenter
                                        font.weight: Font.Medium
                                        font.family: Constants.mySystemFont.name
                                        font.pointSize: Constants.labelFontPointSize
                                        text: qsTr("Printing speed") + "："
                                        color: sourceTheme.text_light_color
                                    }

                                    Rectangle {
                                        id: printSpeedBox
                                        enabled: controlEnabled
                                        opacity: enabled ? 1.0 : 0.7
                                        //enabled: (printerState == 1)//打印中才允许操作

                                        property int minValue: 10
                                        property int maxValue: 300
                                        property var delayShow: false
                                        property alias boxValue: editPrintSpeedBox.text

                                        function specifyPcPrintSpeed() {
                                            delayShow = true
                                            printSpeedDelayShow.start()
                                            printerListModel.cSourceModel.sendUIControlCmdToDevice(ipAddr, LanPrinterListLocal.PrintControlType.CONTROL_CMD_FEEDRATEPCT, boxValue)
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

                                        Layout.fillWidth: true
                                        Layout.minimumWidth: 90 * screenScaleFactor
                                        Layout.maximumWidth: 140 * screenScaleFactor
                                        Layout.preferredHeight: 28 * screenScaleFactor

                                        color: sourceTheme.item_background_color
                                        border.color: sourceTheme.item_border_color
                                        border.width: 1
                                        radius: 5

                                        Binding on boxValue {
                                            when: !editPrintSpeedBox.activeFocus && !printSpeedBox.delayShow
                                            value: mItem.pcPrintSpeed
                                        }

                                        Timer {
                                            repeat: false
                                            interval: 10000
                                            id: printSpeedDelayShow
                                            onTriggered: printSpeedBox.delayShow = false
                                        }

                                        TextField {
                                            id: editPrintSpeedBox
                                            width: parent.width
                                            height: parent.height
                                            anchors.left: parent.left
                                            anchors.verticalCenter: parent.verticalCenter

                                            validator: RegExpValidator{regExp: new RegExp("[1-9][\\d]|[1-2][\\d][\\d]|300")}
                                            selectByMouse: true
                                            selectionColor: sourceTheme.textedit_selection_color
                                            selectedTextColor: color
                                            leftPadding: 6 * screenScaleFactor
                                            rightPadding: 4 * screenScaleFactor
                                            verticalAlignment: TextInput.AlignVCenter
                                            horizontalAlignment: TextInput.AlignLeft
                                            color: sourceTheme.text_weight_color
                                            font.weight: Font.Medium
                                            font.family: Constants.mySystemFont.name
                                            font.pointSize: Constants.labelFontPointSize
                                            background: null

                                            Keys.onUpPressed: printSpeedBox.increase()
                                            Keys.onDownPressed: printSpeedBox.decrease()
                                            Keys.onEnterPressed: rootItem.forceActiveFocus()
                                            Keys.onReturnPressed: rootItem.forceActiveFocus()
                                            //onActiveFocusChanged: mItem.pcPrintSpeedFocus = activeFocus
                                            onEditingFinished: printSpeedBox.specifyPcPrintSpeed()
                                        }

                                        Text {
                                            anchors.left: parent.left
                                            anchors.verticalCenter: parent.verticalCenter
                                            anchors.leftMargin: editPrintSpeedBox.contentWidth + editPrintSpeedBox.leftPadding + 4 * screenScaleFactor

                                            font: editPrintSpeedBox.font
                                            color: editPrintSpeedBox.color
                                            verticalAlignment: Text.AlignVCenter
                                            text: "%"
                                        }

                                        Image {
                                            property string upBtnImage: upButtonArea_0.containsMouse?"upBtn_d":"upBtn"
                                            anchors.right: parent.right
                                            anchors.top: parent.top
                                            anchors.rightMargin: 8 * screenScaleFactor
                                            anchors.topMargin: 8 * screenScaleFactor
                                            visible: editPrintSpeedBox.activeFocus
                                            source: `qrc:/UI/photo/${upBtnImage}.png`

                                            MouseArea {
                                                id: upButtonArea_0
                                                anchors.fill: parent
                                                hoverEnabled: true
                                                onClicked: printSpeedBox.increase()
                                            }
                                        }

                                        Image {
                                            property string downBtnImg: downButtonArea_0.containsMouse?"downBtn_d":"downBtn"
                                            anchors.right: parent.right
                                            anchors.bottom: parent.bottom
                                            anchors.rightMargin: 8 * screenScaleFactor
                                            anchors.bottomMargin: 7 * screenScaleFactor
                                            visible: editPrintSpeedBox.activeFocus
                                            source: `qrc:/UI/photo/${downBtnImg}.png`

                                            MouseArea {
                                                id: downButtonArea_0
                                                anchors.fill: parent
                                                hoverEnabled: true
                                                onClicked: printSpeedBox.decrease()
                                            }
                                        }
                                    }

                                    Item {
                                        Layout.preferredWidth: 60 * screenScaleFactor
                                        Layout.preferredHeight: 14 * screenScaleFactor
                                    }
                                }

                                RowLayout {
                                    spacing: 6 * screenScaleFactor

                                    Item {
                                        Layout.preferredWidth: 14
                                        Layout.preferredHeight: 14

                                        Image {
                                            anchors.centerIn: parent
                                            source: sourceTheme.img_bed_temperature
                                        }
                                    }

                                    Text {
                                        Layout.preferredWidth: 140 * screenScaleFactor
                                        Layout.preferredHeight: 14 * screenScaleFactor
                                        verticalAlignment: Text.AlignVCenter
                                        font.weight: Font.Medium
                                        font.family: Constants.mySystemFont.name
                                        font.pointSize: Constants.labelFontPointSize
                                        text: qsTr("Hot bed temperature") + "："
                                        color: sourceTheme.text_light_color
                                    }

                                    Rectangle {
                                        id: bedDstTempBox
                                        enabled: controlEnabled
                                        opacity: enabled ? 1.0 : 0.7

                                        property int minValue: 0
                                        property int maxValue: 200
                                        property var delayShow: false
                                        property alias boxValue: editBedDstTempBox.text

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
                                            delayShow = true
                                            bedDstTempDelayShow.start()
                                            printerListModel.cSourceModel.sendUIControlCmdToDevice(ipAddr, LanPrinterListLocal.PrintControlType.CONTROL_CMD_BEDTEMP, boxValue)
                                        }

                                        Layout.fillWidth: true
                                        Layout.minimumWidth: 90 * screenScaleFactor
                                        Layout.maximumWidth: 140 * screenScaleFactor
                                        Layout.preferredHeight: 28 * screenScaleFactor

                                        color: sourceTheme.item_background_color
                                        border.color: sourceTheme.item_border_color
                                        border.width: 1
                                        radius: 5

                                        Binding on boxValue {
                                            when: !editBedDstTempBox.activeFocus && !bedDstTempBox.delayShow
                                            value: mItem.pcBedDstTemp
                                        }

                                        Timer {
                                            repeat: false
                                            interval: 10000
                                            id: bedDstTempDelayShow
                                            onTriggered: bedDstTempBox.delayShow = false
                                        }

                                        TextField {
                                            id: editBedDstTempBox
                                            width: parent.width
                                            height: parent.height
                                            anchors.left: parent.left
                                            anchors.verticalCenter: parent.verticalCenter

                                            validator: RegExpValidator{regExp: new RegExp("[\\d]|[1-9][\\d]|[1][\\d][\\d]|200")}
                                            selectByMouse: true
                                            selectionColor: sourceTheme.textedit_selection_color
                                            selectedTextColor: color
                                            leftPadding: 6 * screenScaleFactor
                                            rightPadding: 4 * screenScaleFactor
                                            verticalAlignment: TextInput.AlignVCenter
                                            horizontalAlignment: TextInput.AlignLeft
                                            color: sourceTheme.text_weight_color
                                            font.weight: Font.Medium
                                            font.family: Constants.mySystemFont.name
                                            font.pointSize: Constants.labelFontPointSize
                                            background: null

                                            Keys.onUpPressed: bedDstTempBox.increase()
                                            Keys.onDownPressed: bedDstTempBox.decrease()
                                            Keys.onEnterPressed: rootItem.forceActiveFocus()
                                            Keys.onReturnPressed: rootItem.forceActiveFocus()
                                            //onActiveFocusChanged: mItem.pcBedDstTempFocus = activeFocus
                                            onEditingFinished: bedDstTempBox.specifyPcBedDstTemp()
                                        }

                                        Text {
                                            anchors.left: parent.left
                                            anchors.verticalCenter: parent.verticalCenter
                                            anchors.leftMargin: editBedDstTempBox.contentWidth + editBedDstTempBox.leftPadding + 4 * screenScaleFactor

                                            verticalAlignment: Text.AlignVCenter
                                            font: editBedDstTempBox.font
                                            color: editBedDstTempBox.color
                                            text: "°C"
                                        }

                                        Image {
                                            property string upBtnImage: upButtonArea_1.containsMouse?"upBtn_d":"upBtn"
                                            anchors.right: parent.right
                                            anchors.top: parent.top
                                            anchors.rightMargin: 8 * screenScaleFactor
                                            anchors.topMargin: 8 * screenScaleFactor
                                            visible: editBedDstTempBox.activeFocus
                                            source: `qrc:/UI/photo/${upBtnImage}.png`

                                            MouseArea {
                                                id: upButtonArea_1
                                                anchors.fill: parent
                                                hoverEnabled: true
                                                onClicked: bedDstTempBox.increase()
                                            }
                                        }

                                        Image {
                                            property string downBtnImg: downButtonArea_1.containsMouse?"downBtn_d":"downBtn"
                                            anchors.right: parent.right
                                            anchors.bottom: parent.bottom
                                            anchors.rightMargin: 8 * screenScaleFactor
                                            anchors.bottomMargin: 7 * screenScaleFactor
                                            visible: editBedDstTempBox.activeFocus
                                            source: `qrc:/UI/photo/${downBtnImg}.png`

                                            MouseArea {
                                                id: downButtonArea_1
                                                anchors.fill: parent
                                                hoverEnabled: true
                                                onClicked: bedDstTempBox.decrease()
                                            }
                                        }
                                    }

                                    Text {
                                        Layout.preferredWidth: 60 * screenScaleFactor
                                        Layout.preferredHeight: 14 * screenScaleFactor
                                        verticalAlignment: Text.AlignVCenter
                                        font.weight: Font.Medium
                                        font.family: Constants.mySystemFont.name
                                        font.pointSize: Constants.labelFontPointSize
                                        text: `/ ${bedTemp} °C`
                                        color: sourceTheme.text_light_color
                                    }
                                }

                                RowLayout {
                                    spacing: 6 * screenScaleFactor

                                    Item {
                                        Layout.preferredWidth: 14
                                        Layout.preferredHeight: 14

                                        Image {
                                            anchors.centerIn: parent
                                            source: sourceTheme.img_nozzle_temperature
                                        }
                                    }

                                    Text {
                                        Layout.preferredWidth: 140 * screenScaleFactor
                                        Layout.preferredHeight: 14 * screenScaleFactor
                                        verticalAlignment: Text.AlignVCenter
                                        font.weight: Font.Medium
                                        font.family: Constants.mySystemFont.name
                                        font.pointSize: Constants.labelFontPointSize
                                        text: qsTr("Nozzle temperature") + "："
                                        color: sourceTheme.text_light_color
                                    }

                                    Rectangle {
                                        id: nozzleDstTempBox
                                        enabled: controlEnabled
                                        opacity: enabled ? 1.0 : 0.7

                                        property int minValue: 0
                                        property int maxValue: 260
                                        property var delayShow: false
                                        property alias boxValue: editNozzleDstTempBox.text

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
                                            delayShow = true
                                            nozzleDstTempDelayShow.start()
                                            printerListModel.cSourceModel.sendUIControlCmdToDevice(ipAddr, LanPrinterListLocal.PrintControlType.CONTROL_CMD_NOZZLETEMP, boxValue)
                                        }

                                        Layout.fillWidth: true
                                        Layout.minimumWidth: 90 * screenScaleFactor
                                        Layout.maximumWidth: 140 * screenScaleFactor
                                        Layout.preferredHeight: 28 * screenScaleFactor

                                        color: sourceTheme.item_background_color
                                        border.color: sourceTheme.item_border_color
                                        border.width: 1
                                        radius: 5

                                        Binding on boxValue {
                                            when: !editNozzleDstTempBox.activeFocus && !nozzleDstTempBox.delayShow
                                            value: mItem.pcNozzleDstTemp
                                        }

                                        Timer {
                                            repeat: false
                                            interval: 10000
                                            id: nozzleDstTempDelayShow
                                            onTriggered: nozzleDstTempBox.delayShow = false
                                        }

                                        TextField {
                                            id: editNozzleDstTempBox
                                            width: parent.width
                                            height: parent.height
                                            anchors.left: parent.left
                                            anchors.verticalCenter: parent.verticalCenter

                                            validator: RegExpValidator{regExp: new RegExp("[\\d]|[1-9][\\d]|[1-2][0-5][\\d]|260")}
                                            selectByMouse: true
                                            selectionColor: sourceTheme.textedit_selection_color
                                            selectedTextColor: color
                                            leftPadding: 6 * screenScaleFactor
                                            rightPadding: 4 * screenScaleFactor
                                            horizontalAlignment: TextInput.AlignLeft
                                            verticalAlignment: TextInput.AlignVCenter
                                            color: sourceTheme.text_weight_color
                                            font.weight: Font.Medium
                                            font.family: Constants.mySystemFont.name
                                            font.pointSize: Constants.labelFontPointSize
                                            background: null

                                            Keys.onUpPressed: nozzleDstTempBox.increase()
                                            Keys.onDownPressed: nozzleDstTempBox.decrease()
                                            Keys.onEnterPressed: rootItem.forceActiveFocus()
                                            Keys.onReturnPressed: rootItem.forceActiveFocus()
                                            //onActiveFocusChanged: mItem.pcNozzleDstTempFocus = activeFocus
                                            onEditingFinished: nozzleDstTempBox.specifyPcNozzleDstTemp()
                                        }

                                        Text {
                                            anchors.left: parent.left
                                            anchors.verticalCenter: parent.verticalCenter
                                            anchors.leftMargin: editNozzleDstTempBox.contentWidth + editNozzleDstTempBox.leftPadding + 4 * screenScaleFactor

                                            font: editNozzleDstTempBox.font
                                            color: editNozzleDstTempBox.color
                                            verticalAlignment: Text.AlignVCenter
                                            text: "°C"
                                        }

                                        Image {
                                            property string upBtnImage: upButtonArea_2.containsMouse?"upBtn_d":"upBtn"
                                            anchors.right: parent.right
                                            anchors.top: parent.top
                                            anchors.rightMargin: 8 * screenScaleFactor
                                            anchors.topMargin: 8 * screenScaleFactor
                                            visible: editNozzleDstTempBox.activeFocus
                                            source: `qrc:/UI/photo/${upBtnImage}.png`

                                            MouseArea {
                                                id: upButtonArea_2
                                                anchors.fill: parent
                                                hoverEnabled: true
                                                onClicked: nozzleDstTempBox.increase()
                                            }
                                        }

                                        Image {
                                            property string downBtnImg: downButtonArea_2.containsMouse?"downBtn_d":"downBtn"
                                            anchors.right: parent.right
                                            anchors.bottom: parent.bottom
                                            anchors.rightMargin: 8 * screenScaleFactor
                                            anchors.bottomMargin: 7 * screenScaleFactor
                                            visible: editNozzleDstTempBox.activeFocus
                                            source: `qrc:/UI/photo/${downBtnImg}.png`

                                            MouseArea {
                                                id: downButtonArea_2
                                                anchors.fill: parent
                                                hoverEnabled: true
                                                onClicked: nozzleDstTempBox.decrease()
                                            }
                                        }
                                    }

                                    Text {
                                        Layout.preferredWidth: 60 * screenScaleFactor
                                        Layout.preferredHeight: 14 * screenScaleFactor
                                        verticalAlignment: Text.AlignVCenter
                                        font.weight: Font.Medium
                                        font.family: Constants.mySystemFont.name
                                        font.pointSize: Constants.labelFontPointSize
                                        text: `/ ${nozzleTemp} °C`
                                        color: sourceTheme.text_light_color
                                    }
                                }
                            }

                            Item {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.minimumWidth: 0
                                Layout.maximumWidth: 50 * screenScaleFactor - parent.spacing * 2
                            }

                            Rectangle {
                                color: sourceTheme.item_crossline_color
                                Layout.preferredWidth: 1 * screenScaleFactor
                                Layout.preferredHeight: 96 * screenScaleFactor
                            }

                            Item {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.minimumWidth: 0
                                Layout.maximumWidth: 50 * screenScaleFactor - parent.spacing * 2
                            }

                            Rectangle {
                                Layout.fillWidth: true
                                Layout.minimumWidth: 80 * screenScaleFactor
                                Layout.maximumWidth: 100 * screenScaleFactor
                                Layout.preferredHeight: 40 * screenScaleFactor

                                //enabled: controlEnabled
                                opacity: enabled ? 1 : 0.7
                                radius: 20 * screenScaleFactor

                                border.width: 1
                                border.color: sourceTheme.btn_border_color
                                color: skipButtonArea.containsMouse ? sourceTheme.btn_hovered_color : sourceTheme.btn_default_color

                                Text {
                                    anchors.centerIn: parent
                                    verticalAlignment: Text.AlignVCenter
                                    font.weight: Font.Medium
                                    font.family: Constants.mySystemFont.name
                                    font.pointSize: Constants.labelFontPointSize
                                    text: qsTr("Details")
                                    color: sourceTheme.text_weight_color
                                }

                                MouseArea {
                                    id: skipButtonArea
                                    hoverEnabled: true
                                    anchors.fill: parent
                                    cursorShape: containsMouse?Qt.PointingHandCursor:Qt.ArrowCursor
                                    onClicked: clickDetail(printerID,editDeviceName.text)
                                }
                            }

                            Item {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                            }
                        }
                    }
                }
            }
        }
    }

    Row {
        spacing: 10 * screenScaleFactor
        anchors.bottomMargin: 60 * screenScaleFactor
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        visible: stateControl.currentIndex

        Image {
            anchors.verticalCenter: parent.verticalCenter
            source: sourceTheme.img_infomation
        }

        Text {
            anchors.verticalCenter: parent.verticalCenter
            verticalAlignment: Text.AlignVCenter
            font.family: Constants.mySystemFont.name
            font.weight: Font.Medium
            font.pointSize: Constants.labelFontPointSize
            text: qsTr("You cannot choose or view the printers which are printing with a TF card or the Creality Cloud app")
            color: sourceTheme.text_special_color
        }
    }

    Row {
        spacing: 10 * screenScaleFactor
        anchors.bottomMargin: 20 * screenScaleFactor
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter

        Rectangle {
            radius: height / 2
            width: 160 * screenScaleFactor
            height: 28 * screenScaleFactor

            border.width: 1
            border.color: sourceTheme.btn_border_color
            color: leftButtonArea.containsMouse ? sourceTheme.btn_hovered_color : sourceTheme.btn_default_color

            Text {
                anchors.centerIn: parent
                verticalAlignment: Text.AlignVCenter
                font.weight: Font.Medium
                font.family: Constants.mySystemFont.name
                font.pointSize: Constants.labelFontPointSize
                text: stateControl.currentIndex?qsTr("Pause"):qsTr("Preheating")
                color: sourceTheme.text_weight_color
            }

            MouseArea {
                id: leftButtonArea
                hoverEnabled: true
                anchors.fill: parent
                cursorShape: containsMouse?Qt.PointingHandCursor:Qt.ArrowCursor
                onClicked: {
                    //console.log(stateControl.currentIndex?"pause":"warmUp")
                    if(isDeviceIDEmpty()) return;

                    var copyArray = deviceIDArray
                    if(stateControl.currentIndex)
                        printerListModel.cSourceModel.batch_pause(copyArray)
                    else
                        printerListModel.cSourceModel.batch_warmUp(copyArray)
                }
            }
        }

        Rectangle {
            radius: height / 2
            width: 160 * screenScaleFactor
            height: 28 * screenScaleFactor
            visible: realEntry || stateControl.currentIndex

            border.width: 1
            border.color: sourceTheme.btn_border_color
            color: rightButtonArea.containsMouse ? sourceTheme.btn_hovered_color : sourceTheme.btn_default_color

            Text {
                anchors.centerIn: parent
                verticalAlignment: Text.AlignVCenter
                font.weight: Font.Medium
                font.family: Constants.mySystemFont.name
                font.pointSize: Constants.labelFontPointSize
                text: stateControl.currentIndex?qsTr("Stop"):qsTr("Start Printing")
                color: sourceTheme.text_weight_color
            }

            MouseArea {
                id: rightButtonArea
                hoverEnabled: true
                anchors.fill: parent
                cursorShape: containsMouse?Qt.PointingHandCursor:Qt.ArrowCursor
                onClicked: {
                    //console.log(stateControl.currentIndex?"stop":"start")
                    if(isDeviceIDEmpty()) return;

                    var copyArray = deviceIDArray
                    if(stateControl.currentIndex)
                        printerListModel.cSourceModel.batch_stop(copyArray)
                    else
                        printerListModel.cSourceModel.batch_start(copyArray, gCCodePath)
                }
            }
        }
    }
}
