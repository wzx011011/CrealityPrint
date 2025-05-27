import QtQuick 2.0
import QtQuick 2.10
import QtQuick.Controls 2.12
//import QtQuick.Controls 2.0 as QQC2
//import QtQuick.Layouts 1.3
import QtQuick.Window 2.0
import QtGraphicalEffects 1.12

import ".."
import "../qml"
Window {
    id: eo_askDialog
    width: 398 + 10
    height: 138 + 10
    property string contentBackground: Constants.dialogContentBgColor
    property var bgImage: "qrc:/UI/photo/mode_library_share_bg.png"

    flags: Qt.FramelessWindowHint | Qt.Dialog
   // A model window prevents other windows from receiving input events. Possible values are Qt.NonModal (the default), Qt.WindowModal, and Qt.ApplicationModal.
    modality: Qt.ApplicationModal
    color:"transparent" //Constants.themeColor

  
    Image{
        width: parent.width
        height: parent.height  
        mipmap: true
        smooth: true
        cache: false
        asynchronous: true
        fillMode: Image.PreserveAspectFit
        source: bgImage
    }

    
}
