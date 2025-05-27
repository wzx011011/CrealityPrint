import QtQuick 2.10
import QtQuick.Controls 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0 as QQC2
import CrealityUI 1.0

import com.cxsw.SceneEditor3D 1.0
import QtMultimedia 5.12

// import QtGraphicalEffects 1.12

import "qrc:/CrealityUI"
import ".."
import "../qml"

Rectangle
{
    id: idVideoItem
    width: parent.width
    height: parent.height
    anchors.top : parent.top
    anchors.left : parent.left
    signal initMaxP2P()
    signal uninitMaxP2P()
    signal updatePreview(var productName)
    signal maxBtnClicked()
    signal minBtnClicked()
    signal playUserVideo()

    property var selectMode: 0
    property var imagesource: ""
    property var rate:0
    property var playUrl: ""
    property var isMaxShow: false
    property var xyStrDID: ""
    property var xyStrLicense: ""
    property var deviceID: ""
    property var hasVidew: false
    property var isPreviewShow: false
    property var linkTimes: 0
    property var isShowSwitch: true

    function setImage(url)
    {
        imagesource = url
    }

    function setProgress(val)
    {
        rate = val
    }

    function initPreViewShow()
    {
        updatePreview(deviceID)
        showTimer.start()
    }

    function endPreViewShow()
    {
        showTimer.stop()
    }

    function loadVideoUrl(urlStr)
    {
        playUrl = urlStr
        // idvideoItem.start(urlStr)
    }

    function stopVideo()
    {
        idvideoItem.stop()
    }

    function restartVideo()
    {
        console.log("restartVideo")
        stopVideo()
        restartTimer.start()
    }

    color: "#333333"

    // width: 400
    // height: 450

    QMLPlayer {
        id: idvideoItem
        anchors.top : idVideoItem.top
        anchors.topMargin: 0
        anchors.left : idVideoItem.left
        anchors.leftMargin: 0
        width: idVideoItem.width
        height: idVideoItem.height
        visible: selectMode == 1 ? true : false
    }

    Rectangle
    {
        id: idCustomerVideo
        anchors.top : idVideoItem.top
        anchors.topMargin: 0
        anchors.left : idVideoItem.left
        anchors.leftMargin: 0
        width: idVideoItem.width
        height: idVideoItem.height
        color: "#333333"
        visible: !hasVidew
        StyledLabel {
            //color: "#FB0202"
            id: idMessage
            text: qsTr("No camera detected, unable to use this function")
            width:parent.width - 20
            height: 28
            color:"#d0d0d0"
            font.family: Constants.labelFontFamily
            font.pixelSize: 14
            anchors.top : parent.top
            anchors.topMargin: 90
            anchors.horizontalCenter:parent.horizontalCenter
            verticalAlignment: Qt.AlignVCenter
            horizontalAlignment: Qt.AlignHCenter
        }
        BasicButton{
            id: idaddButton
            width: 125
            height: 28
            text : qsTr("Add")
            btnRadius:3
            btnBorderW:0
            defaultBtnBgColor: Constants.profileBtnColor
            hoveredBtnBgColor: Constants.profileBtnHoverColor
            anchors.top : idMessage.bottom
            anchors.topMargin: 10
            anchors.horizontalCenter:parent.horizontalCenter
            onSigButtonClicked:
            {
                idAddVideoDlg.visible = true
            }
        }
    }

    Rectangle
    {
        id : idimageRect
        anchors.top : idVideoItem.top
        anchors.topMargin: 0
        anchors.left : idVideoItem.left
        anchors.leftMargin: 0
        width: idVideoItem.width
        height: idVideoItem.height
        color: "#333333"
        visible: selectMode == 0 ? true : false

        Rectangle{
            id:topImage
            width: idimageRect.width
            height: idimageRect.height - bottomImage.height
            color:"transparent"
            clip: true
            Image{
                id:_close
                width: idimageRect.width
                height: idimageRect.height
                fillMode: Image.PreserveAspectCrop
                anchors.top: parent.top
                source: imagesource
                opacity:0.5

                // ColorOverlay{
                //     anchors.fill: _close
                //     source: _close
                //     color: "lightGrey"
                // }
            }
        }

        Rectangle{
            id:bottomImage
            width: idimageRect.width
            height: rate.toFixed(2) < 0.05 ? 0 : (rate.toFixed(2) > 0.98 ? idimageRect.height : 60 + (idimageRect.height - 150) * rate)
            color:"transparent"
            clip: true
            anchors.top: topImage.bottom
            anchors.horizontalCenter: topImage.horizontalCenter
            Image{
                id:_close1
                width: idimageRect.width
                height: idimageRect.height
                fillMode: Image.PreserveAspectCrop
                anchors.bottom: parent.bottom
                source: imagesource

                // ColorOverlay{
                //     anchors.fill: _close1
                //     source: _close1
                //     color: "#1C8FD1"
                // }
            }
        }


    }

    CusSkinButton_Image{
        id: idImageBtn
        width: 23
        height: 23
        z:1
        anchors.top : idVideoItem.top
        anchors.topMargin : 15
        anchors.left : idVideoItem.left
        anchors.leftMargin : 15
        visible: isShowSwitch
        btnImgUrl: selectMode == 0 ? "qrc:/UI/photo/videoImage_d.png" : (hovered ? "qrc:/UI/photo/videoImage_d.png" : "qrc:/UI/photo/videoImage.png")

        onClicked:
        {
            selectMode = 0
            idMaxBtn.visible = false
        }
    }

    CusSkinButton_Image{
        id: idVideoBtn
        width: 23
        height: 23
        z:1
        anchors.top : idVideoItem.top
        anchors.topMargin : 15
        anchors.left : idImageBtn.right
        anchors.leftMargin : 15
        visible: isShowSwitch
        btnImgUrl: selectMode == 1 ? "qrc:/UI/photo/videoShow_d.png" : (hovered ? "qrc:/UI/photo/videoShow_d.png" : "qrc:/UI/photo/videoShow.png")

        onClicked:
        {
            selectMode = 1
            idMaxBtn.visible = true
        }
    }

    CusSkinButton_Image{
        id: idMaxBtn
        width: 23
        height: 23
        z:1
        anchors.top : idVideoItem.top
        anchors.topMargin : 15
        anchors.right : idVideoItem.right
        anchors.rightMargin : 15
        visible: isShowSwitch
        btnImgUrl: isMaxShow ? (hovered ? "qrc:/UI/photo/videoMin_d.png" : "qrc:/UI/photo/videoMin.png") : (hovered ? "qrc:/UI/photo/videoMax_d.png" : "qrc:/UI/photo/videoMax.png")

        onClicked:
        {
            if(isMaxShow === false)
            {
                maxBtnClicked()
                isMaxShow = true
            }
            else{
                minBtnClicked()
                isMaxShow = false
            }
        }
    }

    Timer{
        id:showTimer;
        interval: 10000;
        repeat: true;
        triggeredOnStart: true;
        onTriggered: {
            updatePreview(deviceID)
        }
    }

    Timer{
        id:restartTimer;
        interval: 10000;
        repeat: false;
        triggeredOnStart: false;
        onTriggered: {
            console.log("restart beg:"+ playUrl)
            loadVideoUrl(playUrl)
        }
    }

    Timer{
        id:videoLinkTimer
        interval: 1000;
        repeat: true;
        triggeredOnStart: false;
        onTriggered: {
            if(linkTimes < 10)
            {
                //console.log("idvideoItem.getLinkState():" + idvideoItem.getLinkState())
                if(idvideoItem.getLinkState())
                {
                    hasVidew = true
                    linkTimes = 0
                    playUserVideo()
                    //videoLinkTimer.stop()
                }
            }
            else
            {
                hasVidew = false
                videoLinkTimer.stop()
                stopVideo()
                idMessage.text = qsTr("No camera detected, unable to use this function")
                idaddButton.visible = true
            }
            linkTimes++
        }
    }

    BasicDialog
    {
        id: idAddVideoDlg
        width: 400
        height: 170
        titleHeight : 30
        title: qsTr("Add camera")
        visible: false
        StyledLabel {
            id: idLabel
            text: qsTr("IP address:")
            width:40
            height: 28
            font.family: Constants.labelFontFamily
            font.pixelSize: 18
            anchors.top : parent.top
            anchors.topMargin: 70
            anchors.left:parent.left
            anchors.leftMargin: (parent.width - idLabel.width - idText.width - 10) / 2
            verticalAlignment: Qt.AlignVCenter
            horizontalAlignment: Qt.AlignRight
        }
        BasicDialogTextField {
            id: idText
            width: 200
            height : 28
            text: ""
            baseValidator:RegExpValidator { regExp: /((25[0-5]|2[0-4]\d|((1\d{2})|([1-9]?\d)))\.){3}(25[0-5]|2[0-4]\d|((1\d{2})|([1-9]?\d)))/ }
            anchors.top : idLabel.top
            anchors.left:idLabel.right
            anchors.leftMargin: 10
        }
        BasicButton{
            id: addbtn
            width: 125
            height: 28
            text : qsTr("Add")
            btnRadius:3
            btnBorderW:0
            defaultBtnBgColor: Constants.profileBtnColor
            hoveredBtnBgColor: Constants.profileBtnHoverColor
            anchors.top : idLabel.bottom
            anchors.topMargin : 10
            anchors.left:parent.left
            anchors.leftMargin: (parent.width - addbtn.width - cancelbtn.width - 10) / 2
            onSigButtonClicked:
            {
                var ipurl = "rtsp://" + idText.text + "/ch0_0"
                loadVideoUrl(ipurl)
                idAddVideoDlg.visible = false
                //hasVidew = true
                idMessage.text = qsTr("video loading ...")
                idaddButton.visible = false
                linkTimes = 0
                videoLinkTimer.start()
            }
        }

        BasicButton{
            id: cancelbtn
            width: 125
            height: 28
            text : qsTr("Cancel")
            btnRadius:3
            btnBorderW:0
            defaultBtnBgColor: Constants.profileBtnColor
            hoveredBtnBgColor: Constants.profileBtnHoverColor
            anchors.top : addbtn.top
            anchors.left:addbtn.right
            anchors.leftMargin: 10
            onSigButtonClicked:
            {
                idAddVideoDlg.visible = false
            }
        }
    }
}
