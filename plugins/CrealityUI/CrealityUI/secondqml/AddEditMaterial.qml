import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.0
//import CrealityUI 1.0
//import "qrc:/CrealityUI"
import "../qml"
BasicDialog
{
    id: addEditmaterialdlg
    title: qsTr("New Material")

    width: 400
    height: 250
    //titleHeight : 20

    property string newtext: "new"
    property alias newmaterialtext: newmaterial.text
    property alias model_material: editmaterial.model
    property alias currentIndex_printer: editmaterial.currentIndex
    property alias currentText_printer: editmaterial.currentText

    signal materialadd()

    Item {
        id: rectangle
        x:5
        y :10 + titleHeight
        width: parent.width-50
        height: parent.height-titleHeight-20

        StyledLabel {
            id: element
            x: 7
            y: 19
            text: qsTr("New Material:")
        }

        BasicDialogTextField {
            id: newmaterial
            x: 121
            y: 11
            width: 238
            height: 30
            text: newtext
			validator: RegExpValidator { regExp: /^[^\s][\w\s-]+$/ }
            cursorVisible: false
        }

        StyledLabel {
            id: element1
            x: 8
            y: 66 - 10
            text: qsTr("Copy The Selected Slice Parameter Material")
            font.pixelSize: 14
        }

        StyledLabel {
            id: element2
            x: 11
            y: 110//123
            text: qsTr("Matrerial:")
        }

        BasicCombobox {
            id: editmaterial
            x: 121
            y: 110 - 15
            width: 238
            height: 35
            showCount: 3

            onCurrentIndexChanged: {
                    //modelname.get(currentIndex).text
                console.log(editmaterial.textAt(currentIndex))
            }
        }

        BasicDialog
        {
            id: warringdlg
            width: 400
            height: 200
            titleHeight : 30
            title: qsTr("Message")
            visible: false
            Rectangle{
                anchors.centerIn: parent
                width: parent.width/2
                height: parent.height/2
                color: "transparent"
                Text {
                    id: warringdlgname
                    anchors.centerIn: parent
                    font.family: Constants.labelFontFamily
                    font.weight: Constants.labelFontWeight
                    text: qsTr("The input can't be empty!!")
                    font.pixelSize: Constants.labelFontPixelSize
                    color:Constants.textColor
                }
            }
        }
    }
    BasicDialogButton {
        id: basicComButton
        x: 151 - 50
        y: 210 - 20
        width: 98
        height: 32
        text: qsTr("Next")
        btnRadius:3
        btnBorderW:0
        defaultBtnBgColor: Constants.profileBtnColor
        hoveredBtnBgColor: Constants.profileBtnHoverColor
        Layout.preferredHeight: 32
        Layout.preferredWidth: 53
        onSigButtonClicked:
        {
//            if(!newmaterial.text.match("^[a-zA-Z0-9_\u4e00-\u9fa5]+$"))
//            {
//                warringdlg.visible = true
//                warringdlg.text = qsTr("The profile can not empty!!")
//                return
//            }
            if(!newmaterial.text.match("^(?! +$).*$") || newmaterial.text.length ===0)
            {
                //can not is noly space or empty
                warringdlg.visible = true
                warringdlgname.text = qsTr("The profile can not empty!!")
                return
            }

            for ( var i in editmaterial.model)//by TCJ
            {
                if(editmaterial.model[i] === newmaterial.text )
                {
                    warringdlgname.text = qsTr("The material already exists! Please enter again.")
                    warringdlg.visible=true
                    break
                }
            }

            if(!warringdlg.visible)
            {
                materialadd()
                addEditmaterialdlg.close()
            }
        }
    }

    BasicDialogButton {
        id: basicComButton2
        x: 270 - 50
        y: 210 - 20
        width: 98
        height: 32
        text: qsTr("Cancel")
        btnRadius:3
        btnBorderW:0
        defaultBtnBgColor: Constants.profileBtnColor
        hoveredBtnBgColor: Constants.profileBtnHoverColor
        Layout.preferredHeight: 32
        Layout.preferredWidth: 53
        onSigButtonClicked:
        {
            addEditmaterialdlg.close();
        }
    }
}
