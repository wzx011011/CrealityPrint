import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.2
import CrealityUI 1.0
import "qrc:/CrealityUI"
import "../qml"
BasicDialog
{

    signal sigFileName(var filename)
    signal sigViewMyUpload()
    signal sigCloudPrinting(var id)
    property var progressValue: 0
    property var uploadSuccessGcodeId: ""

    onDialogClosed:
    {
        idDialog.height = 263*screenScaleFactor
        idLogoImageColumn.visible = true
        idUploadFile.visible = true
        idprogressBar.visible = false
        idUploadSuccess.visible = false
        progressValue = 0
    }

    function clusterUploadGcodeShow()
    {
        idDialog.height = 223*screenScaleFactor
        idLogoImageColumn.visible = false
        idUploadFile.visible = false
        idprogressBar.visible = true
        idUploadSuccess.visible = false
    }

    function initDialogQMLData()
    {
        idDialog.height = 263*screenScaleFactor
        idLogoImageColumn.visible = true
        idUploadFile.visible = true
        idprogressBar.visible = false
        idUploadSuccess.visible = false
        progressValue = 0
    }

    function setFileName(name)
    {
        idInput.text = name
    }

    function updaloadGcodeFailed()
    {
        idDialog.height = 263*screenScaleFactor
        idLogoImageColumn.visible = true
        idUploadFile.visible = true
        idprogressBar.visible = false
        idUploadSuccess.visible = false
        progressValue = 0
    }

    function uploadGcodeSuccess(strjson)
    {
        idUploadFile.visible = false
        idprogressBar.visible = false
        idUploadSuccess.visible = true

        var objectArray = JSON.parse(strjson);
        var objResult = objectArray.result.list;
        console.log("objResult[0].id = " + objResult[0].id)
        uploadSuccessGcodeId = objResult[0].id
    }

    id: idDialog
    width: 600*screenScaleFactor
    height: 263*screenScaleFactor
    titleHeight : 30*screenScaleFactor
    title: qsTr("Upload Gcode")

    Column{
        id: idLogoImageColumn
        y: 30*screenScaleFactor
        Rectangle {
            id:logoRect
            x: (idDialog.width - logoRect.width)/2
            width: idDialog.width
            height: 74*screenScaleFactor
            color: "transparent"
            Row
            {
                width: logoImage.width + idText.contentWidth
                anchors{
                    horizontalCenter: parent.horizontalCenter
                    verticalCenter: parent.verticalCenter
                }
                spacing: 10
                Image {
                    id : logoImage
                    height:sourceSize.height
                    width: sourceSize.width
                    source: "qrc:/UI/photo/CloudLogo.png"
                }
                StyledLabel
                {
                    id:idText
                    height:logoImage.sourceSize.height
                    text: qsTr("Creality Cloud")
                    font.pointSize:Constants.labelFontPointSize
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignLeft
                }
            }
        }
        Item {
            id: name
            x: 5*screenScaleFactor
            width:idDialog.width - 10*screenScaleFactor
            height: 2
            BasicSeparator
            {
                anchors.fill: parent
            }
        }
    }

    Column{
        id: idUploadFile
        visible: true
        x: (idDialog.width - idUploadFile.width)/2
        y: 60*screenScaleFactor + logoRect.height 
        spacing: 25
        width: idInput.width + idFileLabel.contentWidth+10
        Row{
            spacing: 10
            StyledLabel{
                id:idFileLabel
                height:28*screenScaleFactor
                text: qsTr("Name")
                font.pointSize:Constants.labelFontPointSize
                verticalAlignment: Qt.AlignVCenter
                horizontalAlignment: Qt.AlignLeft
            }
            BasicDialogTextField{
                id: idInput
                width: 408*screenScaleFactor
                height: 28*screenScaleFactor
                font.pointSize:Constants.labelFontPointSize
                //placeholderText: qsTr("Please enter the file name")
                //baseValidator:RegExpValidator { regExp: /^\S{100}$/ }
                baseValidator:RegExpValidator { regExp: /^.{100}$/ }
            }
        }
        Row{
            anchors{
                horizontalCenter: parent.horizontalCenter
                //verticalCenter: parent.verticalCenter
            }
            spacing: 10
            BasicButton{
                width: 125*screenScaleFactor
                height: 28*screenScaleFactor
                btnRadius:3
                btnBorderW:0
                defaultBtnBgColor: Constants.profileBtnColor
                hoveredBtnBgColor: Constants.profileBtnHoverColor
                enabled: idInput.text.trim() != ""
                text: qsTr("Upload")
                onSigButtonClicked:
                {
                    idDialog.height = 223*screenScaleFactor
                    idLogoImageColumn.visible = false
                    idUploadFile.visible = false
                    idprogressBar.visible = true
                    idUploadSuccess.visible = false
                    sigFileName(idInput.text.trim())
                }
            }
            BasicButton{
                width: 125*screenScaleFactor
                height: 28*screenScaleFactor
                btnRadius:3
                btnBorderW:0
                defaultBtnBgColor: Constants.profileBtnColor
                hoveredBtnBgColor: Constants.profileBtnHoverColor
                text: qsTr("Cancel")
                onSigButtonClicked:
                {
                    idDialog.close()
                }
            }
        }
    }
    Column{
        id: idprogressBar
        x: 96*screenScaleFactor
        y: 108*screenScaleFactor
        visible: false
        spacing: 10
        StyledLabel{
            anchors{
                horizontalCenter: parent.horizontalCenter
                //verticalCenter: parent.verticalCenter
            }
            text: progressValue + "%"
            font.pointSize:Constants.labelFontPointSize
        }
        ProgressBar{
            id: progressBar
            from: 0
            to:100
            value: progressValue
            width: 408*screenScaleFactor
            height: 3*screenScaleFactor

            background: Rectangle {
                implicitWidth: progressBar.width
                implicitHeight: progressBar.height
                color: "#303030"
            }

            contentItem: Item {
                Rectangle {
                    width: progressBar.visualPosition * progressBar.width
                    height: progressBar.height
                    color: "#1E9BE2"
                }
            }
        }
    }
    Rectangle{
        id: idUploadSuccess
        visible: false
        y: 35*screenScaleFactor
        width: idDialog.width
        height: idDialog.height - titleHeight
        color: "transparent"
        Column{
            anchors{
                horizontalCenter: parent.horizontalCenter
                verticalCenter: parent.verticalCenter
            }
            spacing: 27
            Row{
                anchors{
                    horizontalCenter: parent.horizontalCenter
                    //verticalCenter: parent.verticalCenter
                }
                spacing: 10
                Image{
                    id: idFinishImage
                    height:sourceSize.height
                    width: sourceSize.width
                    source: "qrc:/UI/photo/upload_success_image.png"
                }
                StyledLabel
                {
                    id:idFinishText
                    height:logoImage.sourceSize.height
                    text: qsTr("Uploaded Successfully")
                    font.pointSize:Constants.labelLargeFontPointSize
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignLeft
                }
            }
            Row{
                anchors{
                    horizontalCenter: parent.horizontalCenter
                    //verticalCenter: parent.verticalCenter
                }
                spacing: 10
                BasicButton{
                    width: 125*screenScaleFactor
                    height: 28*screenScaleFactor
                    btnRadius:0
                    btnBorderW:0
                    pointSize:Constants.labelFontPointSize
                    text: qsTr("View My Uploads")
                    onSigButtonClicked:
                    {
                        sigViewMyUpload()
                        initDialogQMLData()
                    }
                }
                BasicButton{
                    width: 125*screenScaleFactor
                    height: 28*screenScaleFactor
                    btnRadius:0
                    btnBorderW:0
                    pointSize:Constants.labelFontPointSize
                    text: qsTr("Cloud Printing")
                    onSigButtonClicked:
                    {
                        sigCloudPrinting(uploadSuccessGcodeId)
                        initDialogQMLData()
                    }
                }
            }
        }
    }
}
