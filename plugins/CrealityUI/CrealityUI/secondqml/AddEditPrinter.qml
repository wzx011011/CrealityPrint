import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.0
//import CrealityUI 1.0
//import "qrc:/CrealityUI"
import ".."
import "../qml"
BasicDialog
{
    id: addEditprinterdlg

    width: 500
    height: 300
    titleHeight : 20

    property string newPrinter: "new"
    property alias newprintertext: newprinter.text
    property alias model_printer: editprinter.model
    property alias currentIndex_printer: editprinter.currentIndex
    property alias currentText_printer: editprinter.currentText

    signal printeradd()
    title: qsTr("New/Copy Printer")
    Item {
        id: rectangle
        x:5
        y :10 + titleHeight
        width: parent.width-100
        height: parent.height-titleHeight-20
//        radius: 5
        StyledLabel {
            id: element
            x: 48
            y: 40
            text: qsTr("New printer:")
//            font.pixelSize: 18
        }

        BasicDialogTextField {
            id: newprinter
            x: 188
            y: 35
            width: 228
            height: 30
            text: newPrinter
            cursorVisible: false
//            font.pixelSize: 18
        }

        StyledLabel {
            id: element1
            x: 48
            y: 101
            text: qsTr("Copy the selected slice parameter profile")
//            font.pixelSize: 18
        }

        StyledLabel {
            id: element2
            x: 48
            y: 159
            text: qsTr("Printer:")
//            font.pixelSize: 18
        }

        BasicCombobox
        {
            id: editprinter
            x: 185
            y: 159
            width: 220
            height: 30
//            font.pointSize: 18
            onCurrentIndexChanged: {
                    //modelname.get(currentIndex).text
                console.log(editprinter.textAt(currentIndex))
                //printerchanged(editprinter.textAt(currentIndex))
            }
        }

        MessageDiagbasic
        {
            id:warringdlg
            text: qsTr("The printer already exists! please enter again.")
            onAccepted: {
                warringdlg.close()
            }
            visible: false
        }
    }
    BasicDialogButton {
        id: basicComButton
        x: 180
        y: 247
        width: 110
        height: 32
        text: qsTr("Next")
//        btnRadius:6
//        Layout.preferredHeight: 32
//        Layout.preferredWidth: 53
        onSigButtonClicked:
        {
           if(!newprinter.text.match("^[a-zA-Z0-9_\u4e00-\u9fa5]+$"))
           {
               messageDialog.show()
               return
           }
            //confirmNewName(newprofile.text)
            for ( var i in editprinter.model)
            {
                //console.log(editprinter.model[i])
                if(editprinter.model[i] === newprinter.text )
                {
                    warringdlg.visible=true
                    break
                }
            }

            if(!warringdlg.visible)
            {
                printeradd()
                addEditprinterdlg.close()
            }
        }
    }

    BasicDialogButton {
        id: basicComButton2
        x: 310
        y: 247
        width: 110
        height: 32
        text: qsTr("Cancel")
//        btnRadius:6
//        Layout.preferredHeight: 32
//        Layout.preferredWidth: 53
        onSigButtonClicked:
        {
            addEditprinterdlg.close();
        }
    }

    MessageDiagbasic {
        id: messageDialog
        //title: "Warring"
        text: qsTr("The profile can not empty!!")
        onAccepted: {
            messageDialog.close()
        }
        visible: false
    }
}
