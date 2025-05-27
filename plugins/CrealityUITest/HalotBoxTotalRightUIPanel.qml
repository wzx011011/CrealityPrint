import QtQuick 2.13
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQml 2.13
import QtGraphicalEffects 1.12
import "./BasicComponent"


BasicShadowRectangle{
    id:rootRec
    width: 280 * screenScaleFactor
    height: 439
    color: "#4b4b4d"
    radius: 5
    border.width: 1
    border.color: "#151517"

    property var printerController: HalotContext.obj("PrinterController")
    property string curProfileName: ""
    property bool curProfileIsDefault: true

    Rectangle{
        width: parent.width
        height: 21
        radius: 5
        color:"#1c1c1d"
        x: 0
        y: 4
    }

    ColumnLayout{
        anchors.fill: parent
        anchors.margins: 1
        spacing: 10
        PanelComponent{
            //            Layout.fillWidth: true
            imgUrl: "qrc:/Photo/printerPanel.png"
            title: qsTr("Printer")
            radiusClipAlign : 1
            isShadow: false
            radius: 5
        }

        BasicCombobox_Metarial
        {
            id : idConsumables
            Layout.preferredWidth: 260
            Layout.preferredHeight: 28
            Layout.alignment: Qt.AlignHCenter
            clip: true
            model: 5
            currentIndex : printerController.currentVisualIndex
            onAddMaterialClick:
            {
            }
            onDelModelData:
            {

            }
            onCurrentTextChanged:
            {

            }
        }

        Rectangle{
            Layout.preferredWidth: 278
            Layout.preferredHeight: 1
            color: "#3b3b3d"
        }

        PanelComponent{
            id:modelListC
            imgUrl: "qrc:/Photo/materialPanel.png"
            title: qsTr("Material")
            isShadow: false
        }

        BasicCombobox_Metarial
        {
            id : metarialComb
            Layout.preferredWidth: 260
            Layout.preferredHeight: 28
            Layout.alignment: Qt.AlignHCenter
            clip: true
            model: printerController.visualPrinters
            currentIndex : printerController.currentVisualIndex
            onAddMaterialClick:
            {
            }
            onDelModelData:
            {

            }
            onCurrentTextChanged:
            {

            }
        }

        Rectangle{
            Layout.preferredWidth: 278
            Layout.preferredHeight: 1
            color: "#3b3b3d"
        }

        PanelComponent{
            id:paramCfgC
            imgUrl: "qrc:/Photo/paramPanel.png"
            title: qsTr("Parameter config")
            isShadow: false

            CusImglButton
            {
                id: importBtn
                width: 28
                height: 28
                anchors.right: parent.right
                anchors.rightMargin: 53
                anchors.verticalCenter: parent.verticalCenter

                defaultBtnBgColor: "#4b4b4d"
                hoveredBtnBgColor: "#3f3f41"
                selectedBtnBgColor: "#3f3f41"
                font.family: Constants.labelFontFamily
                font.weight: Constants.labelFontWeight
                enabledIconSource: "qrc:/Photo/right_import.png"
                highLightIconSource: "qrc:/Photo/right_import.png"
                pressedIconSource: "qrc:/Photo/right_import.png"
                btnRadius:5
                borderWidth :0
                onClicked:
                {
                    printerController.importProfile()
                }
            }

            CusImglButton
            {
                id: exportBtn
                width: 28
                height: 28
                anchors.right: parent.right
                anchors.rightMargin: 20
                anchors.verticalCenter: parent.verticalCenter

                defaultBtnBgColor: "#4b4b4d"
                hoveredBtnBgColor: "#3f3f41"
                selectedBtnBgColor: "#3f3f41"
                font.family: Constants.labelFontFamily
                font.weight: Constants.labelFontWeight
                enabledIconSource: "qrc:/Photo/right_export.png"
                highLightIconSource: "qrc:/Photo/right_export.png"
                pressedIconSource: "qrc:/Photo/right_export.png"
                btnRadius:5
                btnTipType : 0
                onClicked:
                {
                    printerController.exportProfile()
                }
            }
        }

        Rectangle{
            Layout.preferredWidth: 258
            Layout.preferredHeight: 106
            border.color: "#262626"
            border.width: 1
            color:"transparent"
            Layout.alignment: Qt.AlignHCenter
            radius: 5
            clip: true
            CusListView{
                id:paramCfgListView
                anchors.fill: parent
                anchors.margins: 2
                spacing: 0
                focus: true
                model:  printerController.profiles
                delegate:paramCfgCom
                onCurrentIndexChanged: {
                    printerController.changeCurrentProfile(model[currentIndex])
                    rootRec.curProfileIsDefault = printerController.isProfileDefault(model[currentIndex])
                }
            }
        }

        function add(){

        }
        function edit(){

        }

        Row{
            Layout.alignment: Qt.AlignHCenter
            spacing: 6
            Repeater{
                model:ListModel{
                    Component.onCompleted: {
                        append({"imgSource": "qrc:/UI/photo/right_add.png", "hoverImgSource": "qrc:/UI/photo/right_add_d.png", "toolTip": qsTr("Add"), "cEnabled": true})
                        append({"imgSource": "qrc:/UI/photo/right_edit.png", "hoverImgSource": "qrc:/UI/photo/right_edit_d.png", "toolTip": qsTr("Edit"), "cEnabled": true})
                        append({"imgSource": "qrc:/UI/photo/right_delete.png", "hoverImgSource": "qrc:/UI/photo/right_delete_d.png", "toolTip": qsTr("Delete"), "cEnabled": rootRec.curProfileIsDefault})
                    }
                }
                delegate: imgBtnProfileCom
            }
        }

        Rectangle{
            Layout.preferredWidth: 278
            Layout.preferredHeight: 1
            color: "#3b3b3d"
        }

        CusButton{
            width: 258
            height: 48
            Layout.alignment: Qt.AlignHCenter
            txtContent: qsTr("Slice")
            txtColor: "#ffffff"
            normalColor: "#00a97e"
            hoveredColor: "#08c192"
            pressedColor: "#08c192"
            radius: 5
            enabled : HalotContext.obj("SpaceManager").inModelNumber > 0
            shadowEnabled : true
            onClicked:
            {
                HalotContext.obj("Command.SliceFlow").slice()
            }
        }

        Item{
            Layout.fillHeight: true
        }
    }

    Loader{
        id:slicerPg
        onLoaded: {
            slicerPg.item.show()
        }
    }

    Component{
        id:imgBtnCom
        CusImglButton
        {
            id: control
            width: 58
            height: 28
            defaultBtnBgColor: "#4a4a4f"
            hoveredBtnBgColor: "#65656c"
            selectedBtnBgColor: "#65656c"
            font.family: Constants.labelFontFamily
            font.weight: Constants.labelFontWeight
            enabledIconSource: imgSource
            highLightIconSource: hoverImgSource
            pressedIconSource: hoverImgSource
            btnRadius:5
            enabled : !(model.index ===1 || model.index ===2)
            shadowEnabled : true
            onClicked:
            {
                if(model.index === 0)
                {
                    HalotContext.obj("FileOSManager").qOpen()
                }
                else if(model.index === 1)
                {

                }
                else if(model.index === 2)
                {

                }
                else if(model.index === 3)
                {
                    HalotContext.obj("ModelListController").remove()
                }
            }
        }
    }
    Component{
        id:imgBtnProfileCom
        CusButton
        {
            id: control
            width: 82
            height: 24
//            enabled: {
//                true
//            }
            shadowEnabled : false
            normalColor: "#474749"
            hoveredColor: "#474749"
            pressedColor: "#474749"

            normalBdColor: "#6c6c70"
            hoveredBdColor: "#1e9be2"
            pressedBdColor: "#1e9be2"

            txtContent: toolTip
            onClicked:
            {
            console.log("click = ", toolTip)
            }
        }
    }
    Component{
        id:listViewCom
        Rectangle{
            id:item_delegate
            property bool checked: isChecked
            property bool isHovered: false
            property string normalColor: isHovered ? "#353539" : "transparent"
            width: 258
            height: 26
            color: checked ? "#009cff" : normalColor
            Image{
                id:img1
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                source: checked ? "qrc:/UI/photo/right_fileLogo_d.png" : "qrc:/UI/photo/right_fileLogo.png"
            }

            CusText{
                anchors.left: img1.right
                anchors.leftMargin: 7
                anchors.verticalCenter: parent.verticalCenter
                fontText: modelName
                fontColor: "#ffffff"
                hAlign: 0
            }

            Connections{
                target: modelListView
                onCheckOne: {

                    item_delegate.checked =(idx === index)
                    isChecked = checked

                }
                onCheckMul:{
                    if(idx > modelListView.mulBegin){
                        item_delegate.checked
                                =(index>=modelListView.mulBegin&&index<=idx);
                    }else{
                        item_delegate.checked
                                = (index<=modelListView.mulBegin&&index>=idx);
                    }
                    isChecked = checked
                }
            }

            MouseArea{
                anchors.fill: parent
                hoverEnabled: true
                propagateComposedEvents: true
                onEntered: {
                    parent.isHovered = true
                }
                onExited: {
                    parent.isHovered = false
                }
                onClicked: {
                    modelListView.currentIndex = index
                    mouse.accepted = false
                    //                    isChecked = true
                    switch(mouse.modifiers){
                    case Qt.ControlModifier:
                        item_delegate.checked
                                =!item_delegate.checked
                        isChecked = checked
                        console.log("Qt.ControlModifier index =",index)
                        break;
                    case Qt.ShiftModifier:
                        modelListView.checkMul(index);
                        console.log("Qt.ShiftModifier index =",index)
                        break;
                    default:
                        modelListView.checkOne(index)
                        modelListView.mulBegin = index;
                        break;
                    }

                    console.log("onClickedonClicked =" + index)

                    HalotContext.obj("ModelListController").selects()
                }

                Image{
                    id:img2
                    property bool isOpen: isVisible
                    property bool isHovered: false
                    property string eyeOpen: isHovered ? "qrc:/UI/photo/eye_open_d.png" : "qrc:/UI/photo/eye_open.png"
                    property string eyeClose: isHovered ? "qrc:/UI/photo/eye_close_d.png" : "qrc:/UI/photo/eye_close.png"
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    source: isOpen ? eyeOpen : eyeClose
                    MouseArea{
                        hoverEnabled: true
                        anchors.fill: parent
                        onClicked: {
                            img2.isOpen = !img2.isOpen
                            HalotContext.obj("ModelListController").modelHide(index, img2.isOpen)
                        }

                        onEntered: {
                            img2.isHovered = true
                        }

                        onExited: {
                            img2.isHovered = false
                        }
                    }
                }
            }
        }
    }

    Component{
        id:paramCfgCom
        Rectangle{
            property bool isHovered: false
            property string normalColor: isHovered ? "#353539" : "#414143"
            width: paramCfgListView.width
            height: 20
            color: ListView.isCurrentItem ? "#009cff" : normalColor
            Image{
                id:img1
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                source: paramCfgListView.currentIndex === model.index ? "qrc:/UI/photo/right_fileLogo_d.png" : "qrc:/UI/photo/right_fileLogo.png"
            }

            CusText{
                anchors.left: img1.right
                anchors.leftMargin: 7
                anchors.verticalCenter: parent.verticalCenter
                fontText: modelData
                fontColor: "#ffffff"
                hAlign: 0
            }

            MouseArea{
                anchors.fill: parent
                hoverEnabled: true
                propagateComposedEvents: true
                onEntered: {
                    parent.isHovered = true
                }
                onExited: {
                    parent.isHovered = false
                }
                onClicked: {
                    paramCfgListView.currentIndex = index
                    mouse.accepted = false
                }
            }
        }
    }
}
