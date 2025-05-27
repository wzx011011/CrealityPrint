import QtQuick 2.12
import QtQml 2.3
import QtQuick.Controls 2.5


ListView{
    id: control
    property int mulBegin: 0
    signal checkOne(int idx)
    signal checkMul(int idx)


    Connections{
        target: control
        onCheckOne: control.mulBegin = idx;
    }

    clip:  true
    boundsBehavior: Flickable.StopAtBounds
    spacing: 2
    model: 20
    ScrollBar.vertical: ScrollBar{

    }
}
