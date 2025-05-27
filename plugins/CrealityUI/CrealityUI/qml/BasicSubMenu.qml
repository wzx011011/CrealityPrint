import QtQuick 2.0
import QtQuick.Controls 2.5
import QtGraphicalEffects 1.12
Menu {
    id:idMenu
    property alias mymodel: idMenuItems.model
    property var myItemObj
    property var separator
    property int  maxImplicitWidth: 200
    Column
    {
        Repeater
        {
            id : idMenuItems

            delegate:BasicMenuItemStyle
            {
                id : idMenuItem
                text: actionName
                icon.source: actionIcon
                height: 32//25
                width: maxImplicitWidth + 30
                separatorVisible: index ===0 ? false : actionSeparator
                checkable: false
                checked: actionChecked
                onTriggered:
                {
                    idClickVisible.onTriggered()
                    actionItem.execute();
                    if( model.count() === 1 /*text === qsTr("Clear History")*/)
                    {
                        width = 200
                    }
                }
            }
        }
    }

    BasicMenuItemStyle
   {
       id: idClickVisible
       text: "test"
       visible: false
       height: 1

   }
    background: Rectangle {
        implicitWidth:maxImplicitWidth + 20
        Rectangle {
            id: mainLayout
            anchors.fill: parent
            anchors.margins: 5
            color: Constants.menuStyleBgColor
            opacity: 1
        }

        DropShadow {
            anchors.fill: mainLayout
            horizontalOffset: 3
            verticalOffset: 3
            //                radius: 8
            samples:9// 17
            source: mainLayout
            color: Constants.dropShadowColor
        }
    }

    onOpened: {
        var max = getMaxWidth();
        console.log("MaxWidth ====== ", max)
        maxImplicitWidth = max
//        idMenu.width = max +  20;
    }

    function getMaxWidth()
    {
        var max = 200;
        for(var cIndex = 0; cIndex < idMenuItems.count; ++cIndex)
        {
            var obj = idMenuItems.itemAt(cIndex);
            max = Math.max(obj.implicitWidth,max);
        }
        return max;
    }
}
