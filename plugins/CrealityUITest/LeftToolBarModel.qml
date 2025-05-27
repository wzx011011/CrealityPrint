import QtQuick 2.0

ListModel {
    id: toolbarlist
    ListElement {
        name: "Move"
        enabledIcon: "qrc:/UI/photo/move.png";//"qrc:/UI/images/move.png"
        pressedIcon: "qrc:/UI/photo/move.png";//"qrc:/UI/images/move_d.png"
        source: "MovePanel.qml"
    }
    ListElement {
        name: "Rotate"
        enabledIcon: "qrc:/UI/photo/rotate.png";
        pressedIcon: "qrc:/UI/photo/rotate.png";
        source: "RotatePanel.qml"
    }
    ListElement {
        name: "Scale"
        enabledIcon: "qrc:/UI/photo/scale.png";//"qrc:/UI/images/scale.png"
        pressedIcon: "qrc:/UI/photo/scale.png";//"qrc:/UI/images/scale_d.png"
        source: "ScalePanel.qml"
    }

    ListElement {
        name: "Clone"
        enabledIcon: "qrc:/UI/photo/clone.png";//"qrc:/UI/images/clone.png"
        pressedIcon: "qrc:/UI/photo/clone.png";//"qrc:/UI/images/clone_d.png"
        source: "ClonePanel.qml"
    }

}
