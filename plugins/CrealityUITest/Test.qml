import QtQuick 2.0
import QtQuick.Controls 2.5
Rectangle
{

    width: 200
    height: 200

    anchors.centerIn: parent


    color: "blue"
    visible: true
    function showWifiDevice()
    {

        console.log("lsg showWifiDevice test")
        __dlg.show()
    }

    Component.onCompleted:
    {
        console.log("lsg dlg test")
//        __dlg.show()
    }
    DlgTest
    {
        id: __dlg
    }
//    DlgTest
//    {
//        id : __dlg

//    }
}
