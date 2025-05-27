import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import CrealityUI 1.0
import "qrc:/CrealityUI"
Rectangle
{
    id:idLeftTool
    property alias mymodel: gridview.model
    property var selectItem
    property var noSelect: ""//
    property int btnIndex: 0
    property bool showPop: true
    property var currentSource:""
    property var currentEnableIcon:"" //用于后面判断
    property int count:0

    width: 58 * screenScaleFactor
    height: 501 * screenScaleFactor
    radius: 5
    color: Constants.themeColor

    onVisibleChanged : {
        if(!visible)
        {
            content.source = ""
            focus = true
        }
    }
    MouseArea
    {
        anchors.fill: parent
        focus: true
    }

    function switchPopupMode(autoClose)
    {
        console.log("switchPopupMode =" + autoClose)
//        if(!autoClose)
//        {
//            contentWrapper.closePolicy = Popup.NoAutoClose//Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

//        }
//        else {
//            contentWrapper.closePolicy = Popup.CloseOnEscape | Popup.CloseOnReleaseOutsideParent
//        }
    }

    function switchPickMode()
    {
        var item = gridview.itemAtIndex(1)
        if(!item.bottonSelected)
        item.clicked()
    }

    function pButtomBtnClick()
    {
        var item = gridview.itemAtIndex(6)
        if(!item.bottonSelected) item.clicked()
    }

    function switchShowPopup(bShow)
    {
        if(selectItem && selectItem.tindex ==9)
        {
            if(!bShow && selectItem.bottonSelected)
            {
                content.source = ""
                selectItem.bottonSelected = false
            }
            else
            {
                content.source = currentSource
                selectItem.bottonSelected = true
            }


            return ;
        }

        if(!bShow && selectItem.bottonSelected)
        {
            content.source = ""
            selectItem.bottonSelected = false
        }
        else if(bShow && !selectItem.bottonSelected)
        {
            content.source = currentSource
            selectItem.bottonSelected = true
        }
    }

    function refreshPick()
    {
        content.source = ""
        content.source = currentSource
    }
    ListView{
        id:gridview
        anchors.top:parent.top
        anchors.topMargin: 4
        clip: true
        focus:true
        spacing: 1
        width: parent.width
        height: parent.height - anchors.topMargin * 2
        delegate: CusImglButton{

            anchors.horizontalCenter: parent.horizontalCenter
            width: 54 * screenScaleFactor
            height:index === 0 ? 0 :  54 * screenScaleFactor
            borderWidth : 0
            state : "imgTop"
            text: name
            enabledIconSource:enabledIcon
            pressedIconSource:pressedIcon
            highLightIconSource:pressedIcon
            defaultBtnBgColor: Constants.leftBtnBgColor_normal
            hoveredBtnBgColor: Constants.leftBtnBgColor_hovered
            selectedBtnBgColor: Constants.leftBtnBgColor_selected
            btnTextColor : Constants.leftTextColor
            btnTextColor_d : Constants.leftTextColor_d
            property var tindex
            property var bitem: item
            visible: !(index === 0)
            opacity: enabled ? 1:0.5
            onClicked:
            {
                Constants.leftShowType = 0
                //支撑按钮
                if(model.index === 9)
                {
                    supportUICpp.execute()
                    content.sourceComponent = supportCom
                }

                item.execute();
                if(enabledIcon == currentEnableIcon && currentSource==source && selectItem && selectItem.bottonSelected)
                {
                    content.source = ""
                    currentSource = ""
                    selectItem.bottonSelected = false
                    if(index !==1)
                    {
                        switchPickMode()
                    }
                    return
                }

                if(selectItem)
                    selectItem.bottonSelected = false

                selectItem=this
                selectItem.bottonSelected = true
                selectItem.tindex = index

                if(item.checkSelect())
                {
                    content.source = source
                }else{
                    content.source = noSelect
                }
                currentSource=source
                currentEnableIcon= enabledIcon
                if(source.length > 0)
                {
                    content.item.com = item
                    content.item.execute();
                }
            }
        }
    }
    Component{
        id:supportCom
        CusSupportPanel {
            onClosed: content.sourceComponent = null
        }
    }

    Loader {
        id: content
        x: 68 * screenScaleFactor
        visible: Constants.leftShowType === 0
        onLoaded: {
            content.item.com = selectItem.bitem
        }
    }
}

