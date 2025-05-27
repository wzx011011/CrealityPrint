import QtGraphicalEffects 1.12
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import "qrc:/CrealityUI/qml"
import "qrc:/CrealityUI/secondqml"

import com.cxsw.SceneEditor3D 1.0
Rectangle {
    id: root
    property real originWidth: 1420* screenScaleFactor
    property real recScale: width/originWidth
    property real buttonWidth: 40 * screenScaleFactor
//    property bool needScale: false

    property var topLeftButton: null
    property var selectItem: ""
    property int count: 0
    property int btnIndex: 0
    property bool showPop: true
    property alias mainmodel: mainlistView.model
    property alias othermodel: otherlistView.model
    property alias drawmodel: drawlistView.model
    property alias leftLoader: content
    property string noSelect: ""
    property string currentSource: ""
    property string currentEnableIcon: "" //用于后面判断
    property string dividing_line: Constants.currentTheme ? "#AAAAB0" : "#454548"
    property bool panelVisible: false
    property bool printerSelected: false
    property bool machineTabClose: false
    property var toolSelect: ""

    property bool enabledFps: false
    enum DrawListIndex
    {
        Item_Support =0,
        Item_Z,
        Item_Adaptive,
        Item_Color,
        Item_Relief

    }

    function setOperatePanelVisible(vis) {
        panelVisible = vis;
    }

    function checkDrawItemStatus()
    {
        console.log("checkDrawItemStatus")
        if (kernel_parameter_manager.curExtruderCount() > 1)
            drawlistView.itemAt(CToolBar.DrawListIndex.Item_Color).visible = true;
        else
            drawlistView.itemAt(CToolBar.DrawListIndex.Item_Color).visible = false;
    }

    function switchModeById(btnIndex) {
        let listView;
        let offset = 0;
        if (btnIndex < 10) {
            listView = mainlistView;
            offset = 0;
        } else if (btnIndex >= 10 && btnIndex < 30) {
            listView = otherlistView;
            offset = 10;
        } else {
            listView = drawlistView;
            offset = 30;
        }
        var item = listView.itemAt(btnIndex - offset);
        //        if (selectItem === item)
        //            return ;

        if (!item.bottonSelected)
            item.clicked();

    }

    function switchMakeAdaptiveLayer()
    {
        var item = drawlistView.itemAt(CToolBar.DrawListIndex.Item_Adaptive);
        item.clicked();
    }

    function switchMakeColor() {
        var item = drawlistView.itemAt(CToolBar.DrawListIndex.Item_Color);
        item.clicked();
    }

    function switchMakeZ() {
        var item = drawlistView.itemAt(CToolBar.DrawListIndex.Item_Z);
        item.clicked();
    }

    function switchMakeSupport() {
        var item = drawlistView.itemAt(CToolBar.DrawListIndex.Item_Support);
        item.clicked();
    }

    //c++调用
    function switchPickMode() {
        kernel_kernelui.commandIndex = -1;
    }

    //c++调用
    function pButtomBtnClick() {
        var item = listView.itemAtIndex(5);
        if (!item.bottonSelected)
            item.clicked();
    }

    function switchToolMode(listView, btnIndex) {
        var item = listView.itemAt(btnIndex);
        //        if (selectItem === item)
        //            return ;

        if (!item.bottonSelected && item.enabled){
            item.clicked();
            toolSelect = item;
        }
    }

    height: 60* screenScaleFactor
    color: Constants.currentTheme ? Qt.rgba(255, 255, 255, 0.5) : Qt.rgba(0, 0, 0, 0.5)

    GlobalShortcut {
        funcItem: !kernel_kernel.currentPhase ? root : null
        shortcutType: 0
        listView: mainlistView
    }

    GlobalShortcut {
        funcItem: !kernel_kernel.currentPhase ? root : null
        shortcutType: 1
        listView: otherlistView
    }

    Item {
        id: setEscShortcut

        Shortcut {
            context: Qt.WindowShortcut
            sequence: "ESC"
            onActivated: {
                for(var i=0;i<mainlistView.count;i++){
                    if(mainlistView.itemAt(i).bottonSelected) mainlistView.itemAt(i).clicked()
                }
                for(var i=0;i<otherlistView.count;i++){
                    if(otherlistView.itemAt(i).bottonSelected) otherlistView.itemAt(i).clicked()
                }
                for(var i=0;i<drawlistView.count;i++){
                    if(drawlistView.itemAt(i).bottonSelected) drawlistView.itemAt(i).clicked()
                }

            }
        }

    }

    onVisibleChanged: function() {
        if (!visible) {
            focus = true
        }
    }

    Component.onCompleted: function() {
        //idMachineManager.open()
        root.originWidth = root.width
    }

    state: "disabled"
    states: [
        State {
            name: "disabled"
        },
        State {
            //other

            name: "selected"

            PropertyChanges {
                target: drawlistView
                enabled: true
            }

            PropertyChanges {
                target: mainlistView.itemAt(0)
                isEnabled: true
            }

            PropertyChanges {
                target: mainlistView.itemAt(1)
                isEnabled: true
            }

            PropertyChanges {
                target: mainlistView.itemAt(2)
                isEnabled: true
            }

            PropertyChanges {
                target: mainlistView.itemAt(3)
                isEnabled: true
            }

            PropertyChanges {
                target: otherlistView.itemAt(0)
                isEnabled: true
            }

            PropertyChanges {
                target: otherlistView.itemAt(1)
                isEnabled: true
            }

            PropertyChanges {
                target: otherlistView.itemAt(2)
                isEnabled: true
            }

            PropertyChanges {
                target: otherlistView.itemAt(3)
                isEnabled: kernel_model_selector.selectedObjectsNum >= 1
            }

            PropertyChanges {
                target: otherlistView.itemAt(4)
                isEnabled: kernel_model_selector.selectedObjectsNum >= 1
            }

            PropertyChanges {
                target: otherlistView.itemAt(5)
                isEnabled: kernel_model_selector.selectedObjectsNum >= 1
            }

            PropertyChanges {
                target: otherlistView.itemAt(6)
                isEnabled: kernel_model_selector.selectedObjectsNum >= 1
            }

            PropertyChanges {
                target: otherlistView.itemAt(7)
                isEnabled: true
            }

            PropertyChanges {
                target: otherlistView.itemAt(8)
                isEnabled: true
            }

            PropertyChanges {
                target: otherlistView.itemAt(9)
                isEnabled: true
            }

            PropertyChanges {
                target: otherlistView.itemAt(10)
                isEnabled: true
            }
            //draw

            PropertyChanges {
                target: drawlistView.itemAt(CToolBar.DrawListIndex.Item_Support)
                isEnabled: kernel_model_selector.selectedObjectsNum == 1
            }

            PropertyChanges {
                target: drawlistView.itemAt(CToolBar.DrawListIndex.Item_Z)
                isEnabled: kernel_model_selector.selectedObjectsNum == 1
            }

            PropertyChanges {
                target: drawlistView.itemAt(CToolBar.DrawListIndex.Item_Adaptive)
                isEnabled: kernel_model_selector.selectedGroupsNum == 1
            }

            PropertyChanges {
                target: drawlistView.itemAt(CToolBar.DrawListIndex.Item_Color)
                isEnabled: kernel_model_selector.selectedObjectsNum == 1
            }

            PropertyChanges {
                target: drawlistView.itemAt(CToolBar.DrawListIndex.Item_Relief)
                isEnabled: true
            }
        },
        State {
            //other

            name: "unselected"

            PropertyChanges {
                target: mainlistView.itemAt(2)
                isEnabled: true
            }

            PropertyChanges {
                target: mainlistView.itemAt(3)
                isEnabled: true
            }

            PropertyChanges {
                target: otherlistView.itemAt(0)
                isEnabled: false
            }

            PropertyChanges {
                target: otherlistView.itemAt(1)
                isEnabled: true
            }

            PropertyChanges {
                target: otherlistView.itemAt(2)
                isEnabled: true
            }

            PropertyChanges {
                target: otherlistView.itemAt(3)
                isEnabled: true
            }

            PropertyChanges {
                target: otherlistView.itemAt(4)
                isEnabled: true
            }

            PropertyChanges {
                target: otherlistView.itemAt(5)
                isEnabled: true
            }

            PropertyChanges {
                target: otherlistView.itemAt(6)
                isEnabled: true
            }

            PropertyChanges {
                target: otherlistView.itemAt(7)
                isEnabled: true
            }

            PropertyChanges {
                target: otherlistView.itemAt(8)
                isEnabled: false
            }

            PropertyChanges {
                target: otherlistView.itemAt(9)
                isEnabled: false
            }

            PropertyChanges {
                target: otherlistView.itemAt(10)
                isEnabled: false
            }
            //draw

            PropertyChanges {
                target: drawlistView.itemAt(CToolBar.DrawListIndex.Item_Support)
                isEnabled: false
            }

            PropertyChanges {
                target: drawlistView.itemAt(CToolBar.DrawListIndex.Item_Z)
                isEnabled: false
            }

            PropertyChanges {
                target: drawlistView.itemAt(CToolBar.DrawListIndex.Item_Adaptive)
                isEnabled: false
            }

            PropertyChanges {
                target: drawlistView.itemAt(CToolBar.DrawListIndex.Item_Color)
                isEnabled: false
            }

            PropertyChanges {
                target: drawlistView.itemAt(CToolBar.DrawListIndex.Item_Relief)
                isEnabled: true
            }
        }
    ]

    MouseArea {
        focus: true
        anchors.fill: parent
    }

    RowLayout {
        id: button_background
        anchors.fill: parent
        spacing: 10 * screenScaleFactor * recScale

        property real imageWidth: buttonWidth * recScale
        property real imageHeight: buttonWidth * recScale


        FPSText {
            visible:enabledFps
            Layout.preferredWidth: buttonWidth
            Layout.preferredHeight: buttonWidth

            Text {
                anchors.centerIn: parent
                text: parent.fps.toFixed(2)
            }
        }
        CusButtonRec{
            id: printerBtn
            property var tindex: -2
            ButtonGroup.group: button_group
            Layout.preferredWidth: btnImg.sourceSize.width + 6
            Layout.preferredHeight: btnImg.sourceSize.width + 6

            Layout.leftMargin: 10* screenScaleFactor* recScale
            bottonSelected: idMachineManager.visible
            btnImg.sourceSize.width: button_background.imageWidth
            btnImg.sourceSize.height: button_background.imageHeight
            btnImg.source: isEnabled ? (Constants.currentTheme ? "qrc:/UI/photo/cToolBar/printer_light_default.svg"
                                                               : "qrc:/UI/photo/cToolBar/printer_dark_default.svg")
                                     : Constants.right_panel_save_disabled_image

            toolTipPosition: BasicTooltip.Position.BOTTOM
            toolTipText:qsTr("Printer")
            radius: 5
            color: bottonSelected ? Constants.themeGreenColor : "transparent"
            border.width: bottonSelected ? 0 : (isHovered ? 2 : 1)
            border.color: isHovered ? Constants.themeGreenColor : (Constants.currentTheme ? "#AAAAB0": "#505052")
            onClicked: {
                kernel_kernelui.commandIndex = -1
                kernel_kernelui.otherCommandIdex = -1
                idMachineManager.open()
                root.machineTabClose = false
            }
            onDoubleClicked: {
                //enabledFps = !enabledFps;
            }
            Component.onCompleted: {
                kernel_kernelui.commandIndex = -1

                if (kernel_parameter_manager.curExtruderCount() > 1)
                    drawlistView.itemAt(CToolBar.DrawListIndex.Item_Color).visible = true;
                else
                    drawlistView.itemAt(CToolBar.DrawListIndex.Item_Color).visible = false;
            }
        }

        Rectangle{
            Layout.preferredWidth: 1 * screenScaleFactor
            Layout.preferredHeight: 20 * screenScaleFactor
            color: root.dividing_line
        }

        CusButtonRec{
            //            ButtonGroup.group: button_group
            radius: 5

            normalColor: "transparent"
            hoveredColor: "transparent"
            pressedColor: Constants.themeGreenColor
            disabledColor: "transparent"

            border.width: bottonSelected ? 0 : (isHovered ? 2 : 1)
            border.color: isHovered ? Constants.themeGreenColor : (Constants.currentTheme ? "#AAAAB0": "#505052")

            Layout.preferredWidth: btnImg.sourceSize.width + 6
            Layout.preferredHeight: btnImg.sourceSize.width + 6

            btnImg.sourceSize.width: button_background.imageWidth
            btnImg.sourceSize.height: button_background.imageHeight
            btnImg.source: isEnabled ? (Constants.currentTheme ? "qrc:/UI/photo/cToolBar/openFile_light_default.svg"
                                                               : "qrc:/UI/photo/cToolBar/openFile_dark_default.svg")
                                     : Constants.right_panel_save_disabled_image

            toolTipPosition: BasicTooltip.Position.BOTTOM
            toolTipText:qsTr("Import Model")

            onClicked: kernel_kernel.openFile()
        }

        CusButtonRec{
            id: addPlateBtn
            radius: 5

            normalColor: "transparent"
            hoveredColor: "transparent"
            pressedColor: Constants.themeGreenColor
            disabledColor: "transparent"

            border.width: bottonSelected ? 0 : (isHovered ? 2 : 1)
            border.color: isHovered ? Constants.themeGreenColor : (Constants.currentTheme ? "#AAAAB0": "#505052")

            Layout.preferredWidth: btnImg.sourceSize.width + 6
            Layout.preferredHeight: btnImg.sourceSize.width + 6

            btnImg.sourceSize.width: button_background.imageWidth
            btnImg.sourceSize.height: button_background.imageHeight
            btnImg.source: isEnabled ? (Constants.currentTheme ? "qrc:/UI/photo/cToolBar/addPlate_light_default.svg"
                                                               : "qrc:/UI/photo/cToolBar/addPlate_dark_default.svg")
                                     : Constants.right_panel_save_disabled_image

            toolTipPosition: BasicTooltip.Position.BOTTOM
            toolTipText:qsTr("Add Plate")

            onClicked: kernel_kernel.addPrinter();
        }

        Rectangle{
            Layout.preferredWidth: 1 * screenScaleFactor
            Layout.preferredHeight: 20 * screenScaleFactor * recScale
            color: root.dividing_line
        }

        Repeater {
            id: mainlistView

            delegate: CusButtonRec{
                property var tindex: index
                property var bitem: item
                radius: 5
                isEnabled: {
                    if (index == 2 || index == 3) {
                        return true;
                    } else {
                        return false;
                    }
                }
                bottonSelected: kernel_kernelui.commandIndex === tindex

                normalColor: "transparent"
                hoveredColor: "transparent"
                pressedColor: Constants.themeGreenColor
                disabledColor: "transparent"

                border.width: bottonSelected && isEnabled ? 0 : (isHovered && isEnabled ? 2 : 1)
                border.color: isHovered && isEnabled ? Constants.themeGreenColor
                                                     : (Constants.currentTheme ? "#AAAAB0": "#505052")

                Layout.preferredWidth: btnImg.sourceSize.width + 6
                Layout.preferredHeight: btnImg.sourceSize.width + 6

                btnImg.sourceSize.width: button_background.imageWidth
                btnImg.sourceSize.height: button_background.imageHeight
                btnImg.source: isEnabled ? (bottonSelected ? model.pressedIcon : model.enabledIcon) : model.disabledIcon

                toolTipPosition: BasicTooltip.Position.BOTTOM
                toolTipText: name

                onClicked: {
                    root.selectItem = this
                    printerBtn.bottonSelected =  false

                    if (bottonSelected === false) {
                        kernel_kernelui.commandIndex = index;
                        //初始化item
                        item.execute();
                        content.source = source;
                        content.item.com = item;
                        root.panelVisible = true;
                        if (source.length > 0) {
                            content.item.com = item;
                            content.item.execute();
                        }
                    } else {
                        kernel_kernelui.commandIndex = -1;
                    }
                }

                Connections {
                    target: kernel_kernelui
                    function onCommandIndexChanged() {
                        if (tindex === kernel_kernelui.commandIndex) {
                        } else {
                            if (bottonSelected && kernel_kernelui.commandIndex == -1)
                                root.panelVisible = false;

                        }
                    }
                }
            }
        }

        Rectangle{
            Layout.preferredWidth: 1 * screenScaleFactor
            Layout.preferredHeight: 20 * screenScaleFactor * recScale
            color: root.dividing_line
        }

        Repeater {
            id: otherlistView

            delegate: CusButtonRec{
                property var tindex: index + 10
                property var bitem: item
                radius: 5
                isEnabled: {
                    if (index == 1 || index == 2 || index == 3 ||
                            index == 4 || index == 5 || index == 6 || index == 7) {
                        return true;
                    } else {
                        return false;
                    }
                }
                bottonSelected: kernel_kernelui.commandIndex === tindex

                normalColor: "transparent"
                hoveredColor: "transparent"
                pressedColor: Constants.themeGreenColor
                disabledColor: "transparent"

                border.width: bottonSelected && isEnabled ? 0 : (isHovered && isEnabled ? 2 : 1)
                border.color: isHovered && isEnabled ? Constants.themeGreenColor
                                                     : (Constants.currentTheme ? "#AAAAB0": "#505052")

                Layout.preferredWidth: btnImg.sourceSize.width + 6
                Layout.preferredHeight: btnImg.sourceSize.width + 6

                btnImg.sourceSize.width: button_background.imageWidth
                btnImg.sourceSize.height: button_background.imageHeight
                btnImg.source: isEnabled ? (bottonSelected ? model.pressedIcon : model.enabledIcon) : model.disabledIcon

                toolTipPosition: BasicTooltip.Position.BOTTOM
                toolTipText: name

                onClicked: {
                    root.selectItem = this
                    printerBtn.bottonSelected =  false

                    if (bottonSelected == false) {
                        //bottonSelected = true;
                        kernel_kernelui.commandIndex = index + 10;
                        //初始化item
                        item.execute();
                        content.source = source;
                        content.item.com = item;
                        root.panelVisible = true;
                        if (source.length > 0) {
                            content.item.com = item;
                            content.item.execute();
                        }
                    } else {
                        //bottonSelected = false;
                        kernel_kernelui.commandIndex = -1;
                    }
                }

                Connections {
                    //bottonSelected = false;

                    function onCommandIndexChanged() {
                        if (tindex === kernel_kernelui.commandIndex) {
                        } else {
                            if (bottonSelected && kernel_kernelui.commandIndex == -1)
                                root.panelVisible = false;

                        }
                    }

                    target: kernel_kernelui
                }
            }

            onItemAdded: {
                console.log("otherlistView add " + index);
                root.state = "disabled"
                root.state = "unselected"
            }
        }

        Rectangle{
            Layout.preferredWidth: 1 * screenScaleFactor
            Layout.preferredHeight: 20 * screenScaleFactor* recScale
            color: root.dividing_line
        }

        Repeater {
            id: drawlistView

            delegate: CusButtonRec{
                property var tindex: index + 30
                property var bitem: item
                radius: 5
                isEnabled: false
                bottonSelected: kernel_kernelui.commandIndex === tindex

                normalColor: "transparent"
                hoveredColor: "transparent"
                pressedColor: Constants.themeGreenColor
                disabledColor: "transparent"

                border.width: bottonSelected && isEnabled ? 0 : (isHovered && isEnabled ? 2 : 1)
                border.color: isHovered && isEnabled ? Constants.themeGreenColor
                                                     : (Constants.currentTheme ? "#AAAAB0": "#505052")

                Layout.preferredWidth: btnImg.sourceSize.width + 6
                Layout.preferredHeight: btnImg.sourceSize.width + 6

                btnImg.sourceSize.width: button_background.imageWidth
                btnImg.sourceSize.height: button_background.imageHeight
                btnImg.source: isEnabled ? (bottonSelected ? model.pressedIcon : model.enabledIcon) : model.disabledIcon

                toolTipPosition: BasicTooltip.Position.BOTTOM
                toolTipText: name

                onClicked: {
                    //                            idMachineManager.y = Qt.binding(function() { return root.height })
                    root.selectItem = this
                    if(model.index === CToolBar.DrawListIndex.Item_Support ||
                            model.index  === CToolBar.DrawListIndex.Item_Adaptive){
                        printerBtn.bottonSelected = false
                    }else{
                        // printerBtn.bottonSelected =  Qt.binding(function() {
                        //     return kernel_kernel.isDefaultVisScene && kernel_kernel.currentPhase === 0 &&
                        //             kernel_parameter_manager.functionType === 0})
                    }

                    if (bottonSelected == false) {
                        kernel_kernelui.commandIndex = index + 30;
                        //初始化item
                        item.execute();
                        content.source = source;
                        content.item.com = item;
                        root.panelVisible = true;
                        if (source.length > 0) {
                            content.item.com = item;
                            content.item.execute();
                        }
                    } else {
                        //bottonSelected = false;
                        kernel_kernelui.commandIndex = -1;
                    }
                }

                Connections {
                    //bottonSelected = false;

                    function onCommandIndexChanged() {
                        if (tindex === kernel_kernelui.commandIndex) {
                        } else {
                            if (bottonSelected && kernel_kernelui.commandIndex == -1)
                                root.panelVisible = false;

                            content.source = "";
                        }
                    }

                    target: kernel_kernelui
                }

                Component.onCompleted: {
                    if (model.index === CToolBar.DrawListIndex.Item_Relief) {
                        enabled = Qt.binding(_ => kernel_model_selector.selectedNum <= 1)
                    }
                }
            }

            onItemAdded: {
                root.state = "disabled"
                root.state = "unselected"
            }
        }

        Item {
            Layout.fillWidth: true
//            onWidthChanged: {
//                if(width < 60){
//                    needScale = true
//                    originWidth = root.width
//                    console.log("22222222222222222")
//                }else if(root.height >= 60){
//                    needScale = false
//                    originWidth = 1420
//                    console.log("33333333333333333")
//                }
//            }
        }
    }

    Connections {
        //otherBtn.checked = false;
        //2.当前选项清空
        //drawlistView.currentIndex = -1;
        //mainlistView.currentIndex = -1;
        //otherlistView.currentIndex = -1;
        //1.弹窗置空

        target: kernel_modelspace
        onSigAddModel: {
            if (kernel_model_selector.selectedNum == 0)
                root.state = "unselected";

        }
        onSigRemoveModel: {
            if (kernel_modelspace.modelNNum === 0)
                root.state = "unselected";

        }
    }

    Connections {
        target: kernel_model_selector
        onSelectedNumChanged: {
            root.state = kernel_model_selector.selectedNum > 0 ? "selected" : "unselected"

            if (selectItem === drawlistView.itemAt(CToolBar.DrawListIndex.Item_Adaptive)) {
                if (selectItem.bottonSelected && kernel_model_selector.selectedNum != 1) {
                    selectItem.clicked()
                }
            } else if (selectItem === drawlistView.itemAt(CToolBar.DrawListIndex.Item_Relief)) {
                if (selectItem.bottonSelected && kernel_model_selector.selectedNum > 1) {
                    selectItem.clicked()
                }
            }
        }
    }

    Connections {
        target: kernel_parameter_manager
        onCurrentColorsChanged: {
            if (kernel_parameter_manager.curExtruderCount() > 1)
                drawlistView.itemAt(CToolBar.DrawListIndex.Item_Color).visible = true;
            else
                drawlistView.itemAt(CToolBar.DrawListIndex.Item_Color).visible = false;
        }
    }

    Loader {
        id: content
        parent: c3droot
        anchors.top: parent.top
        anchors.topMargin: 60 * screenScaleFactor
        anchors.left: parent.left
        anchors.leftMargin: 10 * screenScaleFactor
        visible: root.panelVisible && kernel_kernel.currentPhase === 0
        //换料塔不需要控制这个 enbaled
        onLoaded: {
            if (selectItem && selectItem.bitem && content.item.com)
                content.item.com = selectItem.bitem;

        }

        onVisibleChanged: {
        }
        onYChanged: {
        }
    }

    CMachineTab {
        id: idMachineManager
        visible:  !root.machineTabClose && kernel_kernelui.commandIndex === -1 && kernel_kernel.currentPhase === 0
        width: 296 * screenScaleFactor
        height: 140 * screenScaleFactor
        x: 0
        y: parent.height
        onVisibleChanged: printerBtn.bottonSelected = visible
        onClosing: root.machineTabClose = true
    }

    onWidthChanged: {
        recScale = (width - 70* screenScaleFactor)/20/(buttonWidth+6+10* screenScaleFactor)*1.0;
        //var lastItem = drawlistView.itemAt(drawlistView.count - 1);
        //var lastPos = lastItem ? lastItem.x + lastItem.width : 0;
        if(recScale * buttonWidth > height*0.8){
            recScale = 1.0//height/buttonWidth*0.85
        }
    }
}
