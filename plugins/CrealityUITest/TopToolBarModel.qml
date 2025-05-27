import QtQuick 2.0

ListModel {
    id: topToolbarlist
    property var actions : {
            "New": function(){ console.log("New clicked!"); },
            "Open": function(){ console.log("Open clicked!"); }
        }
    ListElement {
        name: "New"
        enabledIcon: "qrc:/UI/images/new.png"
        pressedIcon: "qrc:/UI/images/new.png"
        //hander:toolbarhander

    }
    ListElement {
        name: "Open"
        enabledIcon: "qrc:/UI/images/open.png"
        pressedIcon: "qrc:/UI/images/open.png"
        //hander:toolbarhander
    }
    ListElement {
        name: "Save"
        enabledIcon: "qrc:/UI/images/save.png"
        pressedIcon: "qrc:/UI/images/save.png"
        //hander:toolbarhander

    }
    ListElement {
        name: "Delete"
        enabledIcon: "qrc:/UI/images/delete.png"
        pressedIcon: "qrc:/UI/images/delete.png"
        //hander:toolbarhander

    }
//    ListElement {
//        name: "Undo"
//        enabledIcon: "qrc:/UI/images/undo.png"
//        pressedIcon: "qrc:/UI/images/undo.png"
//        //hander:toolbarhander

//    }
//    ListElement {
//        name: "Redo"
//        enabledIcon: "qrc:/UI/images/redo.png"
//        pressedIcon: "qrc:/UI/images/redo.png"
//        //hander:toolbarhander
//    }
//    ListElement {
//        name: "Fix"
//        enabledIcon: "qrc:/UI/images/fix.png"
//        pressedIcon: "qrc:/UI/images/fix.png"
//        //hander:toolbarhander
//    }
//    ListElement {
//        name: "Website"
//        enabledIcon: "qrc:/UI/images/website.png"
//        pressedIcon: "qrc:/UI/images/website.png"
//        //hander:toolbarhander
//    }
}
