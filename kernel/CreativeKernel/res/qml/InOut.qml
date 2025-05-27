import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 1.3
import Qt.labs.platform 1.1
import Qt.labs.settings 1.1

 Item {
 
    objectName:inOutObject
    function  requestSaveFileDialog(receiver)
    {
        console.log("enter  requestSaveFileDialog!");
        if(!receiver)
        {
            console.log("receiver is null !");
            return
        }
        console.log("test ok!");
        inOutfileDialog.receiver = receiver
		inOutfileDialog.nameFilters = receiver.nameFilters()
		inOutfileDialog.title = receiver.title()
        inOutfileDialog.fileMode = FileDialog.SaveFile
        inOutfileDialog.open()
    }
    function  requestOpenFileDialog(receiver)
    {
        if(!receiver)
        {
                console.log("receiver is null !");
                return
        }
        console.log("test ok!");
        inOutfileDialog.receiver = receiver
		inOutfileDialog.nameFilters = receiver.nameFilters()
		inOutfileDialog.title = receiver.title()
        inOutfileDialog.open()
    }

    FileDialog
    {
        id: inOutfileDialog
		property var receiver
        onAccepted:
        {
            console.log("on accepted ")
            receiver.filesOpen(file)
			receiver = null
        }
    }
}
