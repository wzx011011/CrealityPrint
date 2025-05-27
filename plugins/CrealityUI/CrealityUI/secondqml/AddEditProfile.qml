import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.0
//import CrealityUI 1.0
//import "qrc:/CrealityUI"
import "../qml"
BasicDialog
{
    id: addEditProfiledlg

    width: 500*screenScaleFactor
    height: 330*screenScaleFactor
    titleHeight : 20*screenScaleFactor
    title: qsTr("New")

    property string profileName:qsTr("new")
    property string profileMachine: "Ender-3"
    property alias profileMaterial: editmaterial.currentText//"PLA"
    property string profileQuality: "PLA"

    property string newProfile: "new"
    property alias model: editprinter.model
    property alias modelprofile: editoldprofile.model
    property alias modelprofileIndex: editoldprofile.currentIndex
    property alias modelmaterial: editmaterial.model
    property alias modelmaterialIndex: editmaterial.currentIndex
    property alias currentIndex: editprinter.currentIndex
    property alias currentText: editprinter.currentText
    signal printerchanged(string currenttext)
    signal saveProfile(string newProfile,string printer,string material,string oldProfile)
    signal openEditMenu()
    signal confirmNewName(string newName)
    signal sigPreCreateFile(string fileName)
    signal openWarring()
    function qmlFunction(msg)
    {
        console.log("QML get message:",msg);
        if(msg === "new")
        {
            console.log("11111111111")
            parameterUI.onSaveProfile(newprofile.text,editprinter.textAt(currentIndex),editmaterial.currentText)

            addEditProfiledlg.close();
            openEditMenu()
        }
        else
        {
            openWarring()
        }

    }

    Item //Rectangle
    {
        id: rectangle
        x:5*screenScaleFactor
        y :10*screenScaleFactor + titleHeight
        width: parent.width
        height: parent.height-titleHeight-20
        //        rows:3
        //        spacing : 10
        Grid
        {
            y:40*screenScaleFactor
            width: 350*screenScaleFactor
            height : 120*screenScaleFactor
            spacing: 10
            rows: 5
            columns: 2
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.horizontalCenterOffset: -50
            //            anchors.centerIn: parent
            StyledLabel {
                id: element
                width: 160*screenScaleFactor
                height : 30*screenScaleFactor
                visible:true
                text: qsTr("New Profile:")
                verticalAlignment: Qt.AlignVCenter
                horizontalAlignment: Qt.AlignRight
            }

            BasicDialogTextField {
                id: newprofile
                width: 180*screenScaleFactor
                height: 30*screenScaleFactor
                text: addEditProfiledlg.newProfile
                visible:true
                cursorVisible: false
                validator: RegExpValidator { regExp: /^[^\s][\w\s-]+$/ }

                onTextChanged: {
                    profileName = newprofile.text
                    var res = paramSetUI.isRepeatFileName(text)
                    if(res){
                        basicComButton.enabled = false
                        errorMsg.visible = true
                        console.log("名称重复")
                    }else{
                        basicComButton.enabled = true
                        errorMsg.visible = false
                    }
                }
                //初始化之后增加文件按重复校验
                Component.onCompleted: {
                    var res = paramSetUI.isRepeatFileName(addEditProfiledlg.newProfile)
                    if(res){
                        basicComButton.enabled = false
                        errorMsg.visible = true
                        console.log("名称重复")
                    }else{
                        basicComButton.enabled = true
                        errorMsg.visible = false
                    }
                }
            }
            StyledLabel {
                id: element2
                text: qsTr("Printer:")
                width: 160*screenScaleFactor
                height : 30*screenScaleFactor
                verticalAlignment: Qt.AlignVCenter
                horizontalAlignment: Qt.AlignRight
            }

            BasicCombobox {
                id: editprinter
                width: 180*screenScaleFactor
                height: 30*screenScaleFactor
                onCurrentIndexChanged: {
                    //modelname.get(currentIndex).text
                    console.log(editprinter.textAt(currentIndex))
                    printerchanged(editprinter.textAt(currentIndex))

                    profileMachine = currentText
                }
            }

            StyledLabel {
                id: element3
                text: qsTr("Material:")
                width: 160*screenScaleFactor
                height : 30*screenScaleFactor
                verticalAlignment: Qt.AlignVCenter
                horizontalAlignment: Qt.AlignRight
            }

            BasicCombobox {
                id: editmaterial
                width: 180*screenScaleFactor
                height: 30*screenScaleFactor
                onCurrentIndexChanged: {
                    //    model.get(currentIndex).text
                    //                    addEditProfiledlg.profileMaterial = currentText
                }
            }

            StyledLabel {
                id: element4
                text: qsTr("Copy Profile From:")
                width: 160*screenScaleFactor
                height : 30*screenScaleFactor
                verticalAlignment: Qt.AlignVCenter
                horizontalAlignment: Qt.AlignRight
            }

            BasicCombobox {
                id: editoldprofile
                width: 180*screenScaleFactor
                height: 30*screenScaleFactor
                onCurrentIndexChanged: {
                    profileQuality = currentText
                }
            }

            Item{
                width: 160*screenScaleFactor
                height: 30*screenScaleFactor
            }

            CusText{
                id:errorMsg
                fontWidth: 200*screenScaleFactor
                visible: false
                fontColor: "red"
                Layout.columnSpan: 2
                fontText:qsTr("Name repeater, please rename!")
            }
        }
        BasicSeparator
        {
            x: 10*screenScaleFactor
            y:230*screenScaleFactor
            height: 2
            width: parent.width - 20
        }
        Grid
        {
            y: 250*screenScaleFactor
            width : 210*screenScaleFactor
            height: 30*screenScaleFactor
            columns: 2
            spacing: 10
            anchors.horizontalCenter: parent.horizontalCenter
            BasicDialogButton {
                id: basicComButton
                width: 100*screenScaleFactor
                height: 30*screenScaleFactor
                text: qsTr("Next")
                btnRadius:3
                btnBorderW:0
                defaultBtnBgColor: Constants.profileBtnColor
                hoveredBtnBgColor: Constants.profileBtnHoverColor
                onSigButtonClicked:
                {
                    if(!newprofile.text.match("^(?! +$).*$") || newprofile.text.length ===0)
                    {
                        messageDialog.show()
                        return;
                    }
                    qmlFunction("new")
                }
            }

            BasicDialogButton {
                id: basicComButton2
                width: 100*screenScaleFactor
                height: 32*screenScaleFactor
                text: qsTr("Cancel")
                btnRadius:3
                btnBorderW:0
                defaultBtnBgColor: Constants.profileBtnColor
                hoveredBtnBgColor: Constants.profileBtnHoverColor
                onSigButtonClicked:
                {
                    addEditProfiledlg.close();
                }
            }
        }
    }

    BasicDialog
    {
        id: messageDialog
        width: 400*screenScaleFactor
        height: 200*screenScaleFactor
        titleHeight : 30*screenScaleFactor
        title: qsTr("Message")

        Rectangle{
            anchors.centerIn: parent
            width: parent.width/2
            height: parent.height/2
            color: "transparent"
            Text {
                id: name
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
