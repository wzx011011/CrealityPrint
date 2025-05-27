import QtQuick 2.10
import QtQuick.Controls 2.12
import QtQuick.Controls 2.0 as QQC2
import CrealityUI 1.0
import "qrc:/CrealityUI"
BasicDialog
{
    id: idDialog
    width: 600* screenScaleFactor
    height: 443* screenScaleFactor
    titleHeight : 30* screenScaleFactor
    title: qsTr("Login")
    property var loginKind: 2
    property var selectType: 1  //1:mail 2:phone 3:qrcode
    property var showPassWord: false
    property var basicWebUrl: ""
    property var signupWebsitebyPhone: basicWebUrl + "?signup=1&channel=creality-print"
    property var signupWebsitebyMail: basicWebUrl + "?signup=0&channel=creality-print"
    property var resetPassWordWebsitebyMail: basicWebUrl + "?resetpassword=0&channel=creality-print"
    property var resetPassWordWebsitebyPhone: basicWebUrl + "?resetpassword=1&channel=creality-print"
    property var downLoadAPPURL: basicWebUrl + "/software-firmware"
    property var isPasswordLogin: true
    //property alias imageurl: idQrCodeImage.source
    property var imageurl:""
    property var timeCnt: 60
    property var serverTypeSelect: 0

    signal loginClicked(var accont,var password, var logintype, var autoLogin,var phoneAreaCode)
    signal quickLoginClicked(var phoneNumber,var phoneAreaCode, var verifyCode, var autoLogin)
    signal qrcodeLogin()
    signal stopQrCodeUpdate()
    signal getCode(var phoneNum)
    signal serverTypeChanged(int type)

    onImageurlChanged:{
        console.log("-----imageurl:" + imageurl)
        idQrCodeImage.source = ""
        idQrCodeImage.source = "image://qrcodeImgProvider"

    }

    onDialogClosed:
    {
        resetLogindlg()
    }
    
    onVisibleChanged:{
        if(this.visible)
        {
            if(serverTypeSelect == 0)
            {
                idServer.displayText = qsTr("China")
            }
            else
            {
                idServer.displayText = qsTr("International")
            }
        }
	}

    function setAccont(mailaddr,phoneNum,areacode)
    {
        idMailInput.text = mailaddr

        if(areacode != "")
        {
            idPhoneNumber.text = phoneNum
            for(var i = 0; i < areaCodeList.count; i++)
            {
                if(areaCodeList.get(i).key == areacode)
                {
                    idAreaCodeCombox.currentIndex = i
                    break
                }
            }
        }
        
    }

    function resetLogindlg()
    {
        idErrorMsg.text = ""
        idMailLoginBtn.sigButtonClicked()
    }

    function showErrorMsg(errormsg)
    {
        idErrorMsg.text = qsTr(errormsg)
    }

    function setStringW(str, len)
    {
        if(str.length >= len)
        {
            return str
        }
        else
        {
            var cnt = len - str.length
            for(var i = 0; i <= cnt; i++)
            {
                str += " "
            }
            return str
        }
    }

    function setServerType(idx)
    {
        idServer.currentIndex = idx
    }

    function setWebUrl(webUrl)
    {
        basicWebUrl = webUrl
    }

    Timer{
        id:countDown;
        interval: 1000;
        repeat: true;
        triggeredOnStart: true;

        onTriggered: {
            getCodeBotton.text = "("+ timeCnt + "s)";
            getCodeBotton.enabled = false
            timeCnt -= 1;
            if(timeCnt < 0 )
            {
                getCodeBotton.text = qsTr("Get Code")
                getCodeBotton.enabled = true
                countDown.stop();
            }
        }
    }

    // StyledLabel
    // {
    //     id:idServerLable
    //     height: 20
    //     width: idServerLable.contentWidth
    //     y:80
    //     x:430
    //     text: qsTr("Server")
    //     verticalAlignment: Qt.AlignVCenter
    //     horizontalAlignment: Qt.AlignRight
    // }

    // BasicCombobox 
    // {
    //     id: idServer
    //     height: 20
    //     width : 105
    //     font.pixelSize: 14
    //     currentIndex : 0
    //     showCount:2
    //     y:80
    //     x:475
    //     model: ListModel {
    //         id: model
    //         ListElement { text: qsTr("China");}
    //         ListElement { text: qsTr("International");}
    //     }
    //     onCurrentIndexChanged: {
    //         serverTypeChanged(currentIndex)
    //         serverTypeSelect = currentIndex
    //         if(serverTypeSelect == 0)
    //         {
    //             idServer.displayText = qsTr("China")
    //         }
    //         else
    //         {
    //             idServer.displayText = qsTr("International")
    //         }

    //         if(selectType == 3)
    //         {
    //             qrcodeLogin()
    //         }
    //     }
    // }

    
    Item
    {
        //x:25
        y:50
        //spacing:10

        Rectangle {
            id:logoRect
            x: (idDialog.width - logoRect.width)/2
            y: 10* screenScaleFactor
            width: 440* screenScaleFactor
            height: 50* screenScaleFactor
            color: "transparent"
            Row
            {
                // width: logoImage.width + idText.contentWidth
                // anchors{
                //     horizontalCenter: parent.horizontalCenter
                //     verticalCenter: parent.verticalCenter
                // }
                width:parent.width
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
                    width: idText.contentWidth
                    text: qsTr("Creality Cloud")
                    font.pointSize:Constants.labelFontPointSize
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignLeft
                }

                Rectangle
                {
                    color: "transparent"
                    width: logoRect.width - (logoImage.width + idText.contentWidth + idServerLable.contentWidth + idServer.width) - 50
                    height: idText.height
                }

                StyledLabel
                {
                    id:idServerLable
                    height: 20* screenScaleFactor
                    width: idServerLable.contentWidth
                    y: (idText.height - idServerLable.height)/2
                    text: qsTr("Server")
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignRight
                }

                BasicCombobox 
                {
                    id: idServer
                    height: 20* screenScaleFactor
                    width : 105* screenScaleFactor
                    y: (idText.height - idServerLable.height)/2
                    font.pointSize:Constants.labelFontPointSize
                    currentIndex : 0
                    showCount:2
                    model: ListModel {
                        id: model
                        ListElement { text: qsTr("China");}
                        ListElement { text: qsTr("International");}
                    }
                    onCurrentIndexChanged: {
                        serverTypeChanged(currentIndex)
                        serverTypeSelect = currentIndex
                        if(serverTypeSelect == 0)
                        {
                            idServer.displayText = qsTr("China")
                        }
                        else
                        {
                            idServer.displayText = qsTr("International")
                        }

                        if(selectType == 3)
                        {
                            qrcodeLogin()
                        }
                    }
                }

                
            }
        }

        // Item {
        //     id: idSeparator
        //     width : idDialog.width - 20
        //     x: 10
        //     height: 2
        //     anchors.top: logoRect.bottom
        //     anchors.topMargin: 10
        //     BasicSeparator
        //     {
        //         anchors.fill: parent
        //     }
        // }

        Item {
            id: idSeparator
            width: idDialog.width - 12* screenScaleFactor
            height : 2
            anchors.top: logoRect.bottom
            //anchors.topMargin: 5
            Rectangle {
                x: 6* screenScaleFactor
                width:parent.width > parent.height ?  parent.width : 2
                height: parent.width > parent.height ?  2 : parent.height
                color: Constants.splitLineColor
                Rectangle {
                    width: parent.width > parent.height ? parent.width : 1
                    height: parent.width > parent.height ? 1 : parent.height
                    color: Constants.splitLineColorDark
                }
            }
        }

        Rectangle {
            id:loginTypeRect
            width: 440* screenScaleFactor
            height: 50* screenScaleFactor
            color: "transparent"
            anchors.top: idSeparator.bottom
            anchors.topMargin: 15* screenScaleFactor
            x: (idDialog.width - loginTypeRect.width)/2 + 4
            Row
            {
                spacing:10
                y: (loginTypeRect.height - idMailLoginBtn.height)/2
                BasicButton
                {
                    id: idMailLoginBtn
                    width: 120* screenScaleFactor
                    height: 24* screenScaleFactor
                    text: qsTr("Email Login")
                    pointSize:Constants.languageType == 0 ? 15 : 17
                    defaultBtnBgColor: "transparent"
                    hoveredBtnBgColor: "transparent"
                    fontFamily: "Microsoft YaHei UI"//selectType == 1 ? "Microsoft YaHei UI" : Constants.labelFontFamily
                    textBold: true
                    btnTextColor: {
                        if(selectType == 1)
                        {
                            return Constants.menuTextColor
                        }
                        else
                        {
                            if(hovered)
                            {
                                return Constants.menuTextColor
                            }
                            else
                            {
                                return "#777777"
                            }
                        }
                    }
                    btnRadius: 0
                    btnBorderW:0
                    onSigButtonClicked:
                    {
                        if(selectType == 3)
                        {
                            //stop qrcode auto update
                            stopQrCodeUpdate()
                        }
                        idforgetMsg.visible = true
                        loginKind = 2
                        selectType = 1
                        mailRect.visible = true
                        phoneRect.visible = false
                        errorRect.visible = true
                        loginBotton.visible = true
                        signupRow.visible = true
                        qrcodeRect.visible = false
                        idErrorMsg.text = ""
                    }
                }

                Rectangle
                {
                    y:3* screenScaleFactor
                    color: "#666666"
                    width: 1* screenScaleFactor
                    height: 17* screenScaleFactor
                }

                BasicButton
                {
                    id: idMobilLoginBtn
                    width: 125* screenScaleFactor
                    height: 24* screenScaleFactor
                    text: qsTr("Mobile Login")
                    pointSize:Constants.languageType == 0 ? 15 : 17
                    defaultBtnBgColor: "transparent"
                    hoveredBtnBgColor: "transparent"
                    //fontWeight:selectType == 2 ? Font.ExtraBold : Font.Normal
                    fontFamily: "Microsoft YaHei UI"//selectType == 2 ? "Microsoft YaHei UI" : Constants.labelFontFamily
                    textBold: true
                    btnTextColor: {
                        if(selectType == 2)
                        {
                            return Constants.menuTextColor
                        }
                        else
                        {
                            if(hovered)
                            {
                                return Constants.menuTextColor
                            }
                            else
                            {
                                return "#777777"
                            }
                        }
                    }
                    btnRadius: 0
                    btnBorderW:0
                    onSigButtonClicked:
                    {
                        if(selectType == 3)
                        {
                            //stop qrcode auto update
                            stopQrCodeUpdate()
                        }
                        if(!isPasswordLogin)
                        {
                            idforgetMsg.visible = false
                        }
                        else
                        {
                            idforgetMsg.visible = true
                        }
                        loginKind = 1
                        selectType = 2
                        mailRect.visible = false
                        phoneRect.visible = true
                        errorRect.visible = true
                        loginBotton.visible = true
                        signupRow.visible = true
                        qrcodeRect.visible = false
                        idErrorMsg.text = ""
                    }
                }

                Rectangle
                {
                    y:3* screenScaleFactor
                    color: "#666666"
                    width: 1* screenScaleFactor
                    height: 17* screenScaleFactor
                }

                BasicButton
                {
                    id: idMainLoginBtn
                    width: 150* screenScaleFactor
                    height: 24* screenScaleFactor
                    text: qsTr("Scan QR Code")
                    pointSize:Constants.languageType == 0 ? 15 : 17
                    defaultBtnBgColor: "transparent"
                    hoveredBtnBgColor: "transparent"
                    //fontWeight:selectType == 3 ? Font.ExtraBold : Font.Normal
                    fontFamily: "Microsoft YaHei UI"//selectType == 3 ? "Microsoft YaHei UI" : Constants.labelFontFamily
                    textBold: true
                    btnTextColor: {
                        if(selectType == 3)
                        {
                            return Constants.menuTextColor
                        }
                        else
                        {
                            if(hovered)
                            {
                                return Constants.menuTextColor
                            }
                            else
                            {
                                return "#777777"
                            }
                        }
                    }
                    btnRadius: 0
                    btnBorderW:0
                    onSigButtonClicked:
                    {
                        if(selectType == 3)
                        {
                            return
                        }
                        selectType = 3
                        mailRect.visible = false
                        phoneRect.visible = false
                        errorRect.visible = false
                        loginBotton.visible = false
                        signupRow.visible = false
                        qrcodeRect.visible = true
                        qrcodeLogin()
                    }
                }

            }
        }

        Rectangle {
            id:qrcodeRect
            color: "transparent"
            visible: false
            x: (idDialog.width - mailRect.width)/2 + 4
            width: 440* screenScaleFactor
            height: 230* screenScaleFactor
            anchors.top: loginTypeRect.bottom
            anchors.topMargin: 15
            Row
            {
                y:10* screenScaleFactor
                spacing:10
                Rectangle{
                    width: 200* screenScaleFactor
                    height: 200* screenScaleFactor
                    Item{
                        width: parent.width
                        height: parent.height
                        anchors.top: parent.top
                        anchors.topMargin: 10
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        Image {
                            id:idQrCodeImage
                            width: 180* screenScaleFactor
                            height: 180* screenScaleFactor
                            source: imageurl
                            cache: false
                        }
                    }
                }
                
                // Connections{
                //     target: CodeImage
                //     onCallQmlRefeshImg:{
                //         idQrCodeImage.source = "image://CodeImg/"+ Math.random()
                //         idQrCodeImage.source = "image://CodeImg"
                //     }
                // }

                Column
                {
                    x:5
                    StyledLabel
                    {
                        id:idInfoMsg
                        height: 20* screenScaleFactor
                        width: 225 * screenScaleFactor
                        text: qsTr("Me section in app > Scan icon on top") 
                        font.pointSize:Constants.labelFontPointSize
                        verticalAlignment: Qt.AlignVCenter
                        horizontalAlignment: Qt.AlignRight
                    }

                    StyledLabel
                    {
                        id:idSignUp_qrmode
                        height: 20* screenScaleFactor
                        width: 225 * screenScaleFactor
                        text: qsTr("Don't have an account?") + " " + "<font color='#1CA4F1'>" + qsTr("Sign Up") +  "</font>"
                        font.pointSize:Constants.labelFontPointSize
                        verticalAlignment: Qt.AlignVCenter
                        horizontalAlignment: Qt.AlignRight
                        MouseArea
                        {
                            id:idMouse_qrmode
                            anchors.fill: parent
                            anchors.leftMargin: idSignUp_qrmode.width - 60
                            onClicked:
                            {
                                Qt.openUrlExternally(signupWebsitebyPhone)
                            }
                        }
                    }

                    Rectangle
                    {
                        color: "transparent"
                        width: 240* screenScaleFactor
                        height: 125* screenScaleFactor
                    }

                   StyledLabel
                    {
                        id:idDownload
                        height: 20* screenScaleFactor
                        width: 225 * screenScaleFactor
                        text: "<font color='#1CA4F1'>" + qsTr("Visit website and download APP") +  "</font>"
                        font.pointSize:Constants.labelFontPointSize
                        verticalAlignment: Qt.AlignVCenter
                        horizontalAlignment: Qt.AlignRight
                        MouseArea
                        {
                            id:idMouse_Download
                            anchors.fill: parent
                            onClicked:
                            {
                                Qt.openUrlExternally(downLoadAPPURL)
                            }
                        }
                    }
                }
            }
            
        }


        Rectangle {
            id: mailRect
            color: "transparent"
            visible: true
            width: 440* screenScaleFactor
            height: __errorEmail.visible ? 115* screenScaleFactor :85* screenScaleFactor
            anchors.top: loginTypeRect.bottom
            anchors.topMargin: 15* screenScaleFactor
            x: (idDialog.width - mailRect.width)/2 + 4
            Column
            {
                spacing:10
                BasicLoginTextEdit
                {
                    id: idMailInput
                    placeholderText: qsTr("Please enter your email address")
                    baseValidator:RegExpValidator { regExp: /^([a-zA-Z0-9_\-\\.]+)@([a-zA-Z0-9_\\-\\.]+)\\.([a-zA-Z]{2,30})$/ }
                    height : 36* screenScaleFactor
                    width : 430* screenScaleFactor
                    font.pointSize:Constants.labelFontPointSize
                    headImageSrc:"qrc:/UI/photo/userImg_d.png"
                    text: ""
                    onTextChanged:
                    {
                        idErrorMsg.text = ""
                    }
                }
                StyledLabel
                {
                    id: __errorEmail
                    width : 430* screenScaleFactor
                    height: 20* screenScaleFactor
                    text: qsTr("Please input the correct email address")
                    color: "red"
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignLeft
                    visible: !idMailInput.text.match("^([a-zA-Z0-9_\\-\\.]+)@([a-zA-Z0-9_\\-\\.]+)\\.([a-zA-Z]{2,30})$") && idMailInput.text !== ""
                }
                BasicLoginTextEdit
                {
                    id: idMailPassWord
                    placeholderText: qsTr("Please enter password")
                    //baseValidator:RegExpValidator { regExp: /^\S{100}$/ }
                    height : 36* screenScaleFactor
                    width : 430* screenScaleFactor
                    font.pointSize:Constants.labelFontPointSize
                    headImageSrc: "qrc:/UI/photo/passwordImg.png"
                    tailImageSrc: showPassWord ? "qrc:/UI/photo/showPW.png" : "qrc:/UI/photo/hidePW.png"
                    hoveredTailImageSrc:showPassWord ? Constants.showPWHoveredImg : Constants.hidePWHoveredImg
                    text: ""
                    echoMode: showPassWord ? TextInput.Normal : TextInput.Password
                    onTailBtnClicked:
                    {
                        showPassWord = !showPassWord;
                    }
                    onTextChanged:
                    {
                        idErrorMsg.text = ""
                    }
                    onAccepted:
                    {
                        if(loginBotton.enabled)
                        {
                            loginBotton.sigButtonClicked()
                        }
                    }
                }
            }
        }

        Rectangle {
            id: phoneRect
            color: "transparent"
            visible: false
            width: 440* screenScaleFactor
            height: 108* screenScaleFactor
            anchors.top: loginTypeRect.bottom
            anchors.topMargin: 15* screenScaleFactor
            x: (idDialog.width - phoneRect.width)/2 + 4
            Column
            {
                spacing:10
                Rectangle
                {
                    id: idPhoneNumRect
                    height : 36* screenScaleFactor
                    width : 430* screenScaleFactor
                    color: Constants.dialogItemRectBgColor
                    border.width:1
                    border.color:Constants.dialogItemRectBgBorderColor
                    radius:0
                    BasicAreaCodeCombobox 
                    {
                        id:idAreaCodeCombox
                        height:34* screenScaleFactor
                        width: 118* screenScaleFactor
                        font.pointSize:Constants.labelFontPointSize
                        currentIndex: 36
                        model: areaCodeList
                        popRectWidth: 220* screenScaleFactor
                        cmbRadius:0
                        borderWidth:0
                        headImageSrc:"qrc:/UI/photo/phoneImg.png"
                        contentFontPointSize:Constants.labelFontPointSize
                        anchors.left: idPhoneNumRect.left
                        anchors.leftMargin: 2
                        anchors.top: idPhoneNumRect.top
                        anchors.topMargin: 1
                        onCurrentIndexChanged: {												
							idAreaCodeCombox.displayText = "+"+areaCodeList.get(currentIndex).key
                        }
                        Component.onCompleted:{
                            idAreaCodeCombox.displayText = "+"+areaCodeList.get(currentIndex).key
                        }
					}

                    Rectangle
                    {
                        height : 34* screenScaleFactor
                        width : 1
                        color:Constants.dialogItemRectBgBorderColor
                        anchors.left: idAreaCodeCombox.right
                        anchors.top: idAreaCodeCombox.top
                    }
                        
                    BasicDialogTextField
                    {
                        id: idPhoneNumber
                        anchors.left: idAreaCodeCombox.right
                        anchors.leftMargin: 1
                        anchors.top: idAreaCodeCombox.top
                        radius:0
                        borderW:0
                        placeholderText: qsTr("Please enter your phone number")
                        baseValidator:RegExpValidator { regExp: /^\d+$/ }
                        height : 34* screenScaleFactor
                        width : 308* screenScaleFactor
                        text: ""
                        onTextChanged:
                        {
                            idErrorMsg.text = ""
                        }
                    }
                }

                Row{
                    id: idInstantRow
                    anchors{
                        right: idPhoneNumRect.right
                        top: idPhoneNumRect.bottom
                        topMargin:2
                    }

                    BasicButton
                    {
                        id: idInstantLoginBtn
                        width: idInstantLoginBtn.btnText.contentWidth+10
                        height: 20* screenScaleFactor
                        text: isPasswordLogin ? qsTr("Instant Login"):qsTr("Password Login") 
                        pointSize:Constants.labelFontPointSize
                        defaultBtnBgColor: "transparent"
                        hoveredBtnBgColor: "transparent"
                        btnTextColor: "#1CA4F1"
                        btnRadius: 0
                        btnBorderW:0
                        onSigButtonClicked:
                        {
                            isPasswordLogin = !isPasswordLogin
                            idErrorMsg.text = ""
                            if(!isPasswordLogin)
                            {
                                idforgetMsg.visible = false
                            }
                            else
                            {
                                idforgetMsg.visible = true
                            }
                        }
                    }
                }
                

                BasicLoginTextEdit
                {
                    id: idPhonePassWord
                    anchors{
                        top: idInstantRow.bottom
                        topMargin:10* screenScaleFactor
                    }
                    visible:isPasswordLogin
                    placeholderText: qsTr("Please enter password")
                    //baseValidator:RegExpValidator { regExp: /^\S{100}$/ }
                    height : 36* screenScaleFactor
                    width : 430* screenScaleFactor
                    font.pointSize:Constants.labelFontPointSize
                    headImageSrc: "qrc:/UI/photo/passwordImg.png"
                    tailImageSrc: showPassWord ? "qrc:/UI/photo/showPW.png" : "qrc:/UI/photo/hidePW.png"
                    hoveredTailImageSrc:showPassWord ? Constants.showPWHoveredImg : Constants.hidePWHoveredImg
                    text: ""
                    echoMode: showPassWord ? TextInput.Normal : TextInput.Password
                    onTailBtnClicked:
                    {
                        showPassWord = !showPassWord;
                    }
                    onTextChanged:
                    {
                        idErrorMsg.text = ""
                    }
                    onAccepted:
                    {
                        if(loginBotton.enabled)
                        {
                            loginBotton.sigButtonClicked()
                        }
                    }
                }

                Row
                {
                    id: idVerificationCodeRow
                    spacing:10
                    visible:!isPasswordLogin
                    anchors{
                        top: idInstantRow.bottom
                        topMargin:10* screenScaleFactor
                    }
                    BasicLoginTextEdit
                    {
                        id: idVerificationCode
                        placeholderText: qsTr("Please enter verification code")
                        //baseValidator:RegExpValidator { regExp: /^\S{100}$/ }
                        height : 36* screenScaleFactor
                        width : 300* screenScaleFactor
                        font.pointSize:Constants.labelFontPointSize
                        headImageSrc: "qrc:/UI/photo/passwordImg.png"
                        tailImageSrc: showPassWord ? "qrc:/UI/photo/showPW.png" : "qrc:/UI/photo/hidePW.png"
                        hoveredTailImageSrc:showPassWord ? Constants.showPWHoveredImg : Constants.hidePWHoveredImg
                        text: ""
                        echoMode: showPassWord ? TextInput.Normal : TextInput.Password
                        onTailBtnClicked:
                        {
                            showPassWord = !showPassWord;
                        }
                        onTextChanged:
                        {
                            idErrorMsg.text = ""
                        }
                        onAccepted:
                        {
                            if(loginBotton.enabled)
                            {
                                loginBotton.sigButtonClicked()
                            }
                        }
                    }

                    BasicButton
                    {
                        id: getCodeBotton
                        width: 120* screenScaleFactor
                        height: 36* screenScaleFactor
                        text: qsTr("Get Code")
                        defaultBtnBgColor : enabled ?  "#1E9BE2" : Constants.profileBtnColor// : "#535353"
			            hoveredBtnBgColor: "#1EB6E2"
                        btnRadius: 3
                        btnBorderW:0
                        enabled: idPhoneNumber.text == "" ? false : true
                        onSigButtonClicked:
                        {
                            var accont = idAreaCodeCombox.displayText.substring(1) + idPhoneNumber.text
                            timeCnt = 60;
                            countDown.start();
                            getCode(accont)
                        }
                        
                    }
                }
            }
        }

        Rectangle {
            id: errorRect
            color: "transparent"
            visible: true
            x: (idDialog.width - mailRect.width)/2 + 4
            width: 440* screenScaleFactor
            height: 20* screenScaleFactor
            anchors{
                top: mailRect.visible ? mailRect.bottom : phoneRect.bottom
                topMargin:2* screenScaleFactor
            }

            Row
            {
                spacing: 5
                QQC2.TextArea{
                    id: idErrorMsg
                    text: ""
                    width: 300* screenScaleFactor
                    height : 20* screenScaleFactor
                    wrapMode: TextEdit.WordWrap
                    color: "red"
                    font.family: Constants.labelFontFamily
                    font.weight: Constants.labelFontWeight
                    font.pointSize:Constants.labelFontPointSize
                    readOnly: true
                    padding:0
                }
                StyledLabel
                {
                    id:idforgetMsg
                    height: 20* screenScaleFactor
                    width: 120* screenScaleFactor
                    text: qsTr("Forgot Password?")
                    font.pointSize:Constants.labelFontPointSize
                    color: "#1CA4F1"
                    //verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignRight
                    MouseArea
                    {
                        id:idMouse_forgetmode
                        anchors.fill: parent
                        onClicked:
                        {
                            if(selectType == 1)
                            {
                                Qt.openUrlExternally(resetPassWordWebsitebyMail)
                            }
                            else
                            {
                                Qt.openUrlExternally(resetPassWordWebsitebyPhone)
                            }
                            
                        }
                    }
                }
            }
        }

        BasicButton
        {
            id: loginBotton
            width: 430* screenScaleFactor
            height: 48* screenScaleFactor
            x: (idDialog.width - mailRect.width)/2 + 4
            anchors.top: errorRect.bottom
            anchors.topMargin: 10* screenScaleFactor
            text: qsTr("Log In")
            defaultBtnBgColor : enabled ?  "#1E9BE2" : "#DBDBDE"// : "#535353"
			hoveredBtnBgColor: "#1EB6E2"
            btnRadius: 3
            pixSize: 14
            btnBorderW:0
            enabled:{
                if(selectType == 1)
                {
                    //mail login
                    if(idMailInput.text == "" || idMailPassWord.text == ""
                    || __errorEmail.visible)
                    {
                        return false
                    }
                    else
                    {
                        return true
                    }
                }
                else if(selectType == 2)
                {
                    //phoneNum login
                    var accont = idPhoneNumber.text
                    var password = ""
                    if(isPasswordLogin)
                    {
                        password = idPhonePassWord.text
                    }
                    else
                    {
                        password = idVerificationCode.text
                    }

                    if(accont == "" || password == "")
                    {
                        return false
                    }
                    else
                    {
                        return true
                    }
                }
            }
            onSigButtonClicked:
            {
                console.log("onSigButtonClicked loginClicked")
                var accont = ""
                var password = ""
                var areaCode = ""
                if(selectType == 1)
                {
                    //mail login
                    accont = idMailInput.text
                    password = idMailPassWord.text
                    loginClicked(accont,password, loginKind, idCheckBox.checked, "")
                    return
                }
                else if(selectType == 2)
                {
                    //phoneNum login
                    if(isPasswordLogin)
                    {
                        accont = idPhoneNumber.text//idAreaCodeCombox.displayText.substring(1) + idPhoneNumber.text
                        password = idPhonePassWord.text
                        loginClicked(accont, password, loginKind, idCheckBox.checked, idAreaCodeCombox.displayText.substring(1))
                        return
                    }
                    else
                    {
                        accont =  idPhoneNumber.text//idAreaCodeCombox.displayText.substring(1) + idPhoneNumber.text
                        areaCode = idAreaCodeCombox.displayText.substring(1)
                        password = idVerificationCode.text
                        quickLoginClicked(accont,areaCode,password, idCheckBox.checked)
                        return
                    }
                }
            }
            
        }

        Row{
            id: signupRow
            anchors{
                left: loginBotton.left
                top: loginBotton.bottom
                topMargin: 5* screenScaleFactor
            }  

            StyleCheckBox
            {
                id :idCheckBox
                width: 120* screenScaleFactor
                height: 20* screenScaleFactor
                fontSize: 12
                text: qsTr("Auto login")
                checked:true
            }   

            Rectangle
            {
                color:"transparent"
                height:20* screenScaleFactor
                width:loginBotton.width - idCheckBox.width - signupRowLabel.width
            }     

            StyledLabel
            {
                //id:idSignUp
                id: signupRowLabel
                height: 20* screenScaleFactor
                width: 215* screenScaleFactor
                text: qsTr("Don't have an account?") + " " + "<font color='#1CA4F1'>" + qsTr("Sign Up") +  "</font>"
                font.pointSize:Constants.labelFontPointSize
                verticalAlignment: Qt.AlignVCenter
                horizontalAlignment: Qt.AlignRight
                MouseArea
                {
                    id:idMouse
                    anchors.fill: parent
                    anchors.leftMargin: signupRowLabel.width - 60
                    onClicked:
                    {
                        if(selectType == 1)
                        {
                            Qt.openUrlExternally(signupWebsitebyMail)
                        }
                        else
                        {
                            Qt.openUrlExternally(signupWebsitebyPhone)
                        }
                        
                    }
                }
            }
        }

        
        ListModel
        {
            id:areaCodeList
            ListElement{key:"93";modelData : qsTr("Afghanistan");}
            ListElement{key:"335";modelData : qsTr("Albania");}
            ListElement{key:"213";modelData : qsTr("Algeria");}
            ListElement{key:"376";modelData : qsTr("Andorra");}
            ListElement{key:"0244";modelData : qsTr("Angola");}
            ListElement{key:"1254";modelData : qsTr("Anguilla");}
            ListElement{key:"1268";modelData : qsTr("Antigua and Barbuda");}
            ListElement{key:"54";modelData : qsTr("Argentina");}
            ListElement{key:"374";modelData : qsTr("Armenia");}
            ListElement{key:"247";modelData : qsTr("Ascension");}
            ListElement{key:"61";modelData : qsTr("Australia");}
            ListElement{key:"43";modelData : qsTr("Austria");}
            ListElement{key:"994";modelData : qsTr("Azerbaijan");}
            ListElement{key:"1242";modelData : qsTr("Bahamas");}
            ListElement{key:"973";modelData : qsTr("Bahrain");}
            ListElement{key:"880";modelData : qsTr("Bangladesh");}
            ListElement{key:"1246";modelData : qsTr("Barbados");}
            ListElement{key:"375";modelData : qsTr("Belarus");}
            ListElement{key:"32";modelData : qsTr("Belgium");}
            ListElement{key:"501";modelData : qsTr("Belize");}
            ListElement{key:"229";modelData : qsTr("Benin");}
            ListElement{key:"1441";modelData : qsTr("Bermuda Is");}
            ListElement{key:"591";modelData : qsTr("Bolivia");}
            ListElement{key:"267";modelData : qsTr("Botswana");}
            ListElement{key:"55";modelData : qsTr("Brazil");}
            ListElement{key:"673";modelData : qsTr("Brunei");}
            ListElement{key:"359";modelData : qsTr("Bulgaria");}
            ListElement{key:"226";modelData : qsTr("Burkina Faso");}
            ListElement{key:"95";modelData : qsTr("Burma");}
            ListElement{key:"257";modelData : qsTr("Burundi");}
            ListElement{key:"237";modelData : qsTr("Cameroon");}
            ListElement{key:"1";modelData : qsTr("Canada");}
            ListElement{key:"1345";modelData : qsTr("Cayman Is");}
            ListElement{key:"236";modelData : qsTr("Central African Republic");}
            ListElement{key:"235";modelData : qsTr("Chad");}
            ListElement{key:"56";modelData : qsTr("Chile");}
            ListElement{key:"86";modelData : qsTr("China");}
            ListElement{key:"57";modelData : qsTr("Colombia");}
            ListElement{key:"242";modelData : qsTr("Congo");}
            ListElement{key:"682";modelData : qsTr("Cook Is");}
            ListElement{key:"506";modelData : qsTr("Costa Rica");}
            ListElement{key:"53";modelData : qsTr("Cuba");}
            ListElement{key:"357";modelData : qsTr("Cyprus");}
            ListElement{key:"420";modelData : qsTr("Czech Republic");}
            ListElement{key:"45";modelData : qsTr("Denmark");}
            ListElement{key:"253";modelData : qsTr("Djibouti");}
            ListElement{key:"1890";modelData : qsTr("Dominica Rep");}
            ListElement{key:"503";modelData : qsTr("EI Salvador");}
            ListElement{key:"593";modelData : qsTr("Ecuador");}
            ListElement{key:"20";modelData : qsTr("Egypt");}
            ListElement{key:"372";modelData : qsTr("Estonia");}
            ListElement{key:"251";modelData : qsTr("Ethiopia");}
            ListElement{key:"679";modelData : qsTr("Fiji");}
            ListElement{key:"358";modelData : qsTr("Finland");}
            ListElement{key:"33";modelData : qsTr("France");}
            ListElement{key:"594";modelData : qsTr("French Guiana");}
            ListElement{key:"689";modelData : qsTr("French Polynesia");}
            ListElement{key:"241";modelData : qsTr("Gabon");}
            ListElement{key:"220";modelData : qsTr("Gambia");}
            ListElement{key:"995";modelData : qsTr("Georgia");}
            ListElement{key:"49";modelData : qsTr("Germany");}
            ListElement{key:"233";modelData : qsTr("Ghana");}
            ListElement{key:"350";modelData : qsTr("Gibraltar");}
            ListElement{key:"30";modelData : qsTr("Greece");}
            ListElement{key:"1809";modelData : qsTr("Grenada");}
            ListElement{key:"1671";modelData : qsTr("Guam");}
            ListElement{key:"502";modelData : qsTr("Guatemala");}
            ListElement{key:"224";modelData : qsTr("Guinea");}
            ListElement{key:"592";modelData : qsTr("Guyana");}
            ListElement{key:"509";modelData : qsTr("Haiti");}
            ListElement{key:"504";modelData : qsTr("Honduras");}
            ListElement{key:"852";modelData : qsTr("Hong Kong");}
            ListElement{key:"36";modelData : qsTr("Hungary");}
            ListElement{key:"354";modelData : qsTr("Iceland");}
            ListElement{key:"91";modelData : qsTr("India");}
            ListElement{key:"62";modelData : qsTr("Indonesia");}
            ListElement{key:"98";modelData : qsTr("Iran");}
            ListElement{key:"964";modelData : qsTr("Iraq");}
            ListElement{key:"353";modelData : qsTr("Ireland");}
            ListElement{key:"972";modelData : qsTr("Israel");}
            ListElement{key:"39";modelData : qsTr("Italy");}
            ListElement{key:"225";modelData : qsTr("Ivory Coast");}
            ListElement{key:"1876";modelData : qsTr("Jamaica");}
            ListElement{key:"81";modelData : qsTr("Japan");}
            ListElement{key:"962";modelData : qsTr("Jordan");}
            ListElement{key:"855";modelData : qsTr("Kampuchea (Cambodia )");}
            ListElement{key:"327";modelData : qsTr("Kazakstan");}
            ListElement{key:"254";modelData : qsTr("Kenya");}
            ListElement{key:"82";modelData : qsTr("Korea");}
            ListElement{key:"965";modelData : qsTr("Kuwait");}
            ListElement{key:"331";modelData : qsTr("Kyrgyzstan");}
            ListElement{key:"856";modelData : qsTr("Laos");}
            ListElement{key:"371";modelData : qsTr("Latvia");}
            ListElement{key:"961";modelData : qsTr("Lebanon");}
            ListElement{key:"266";modelData : qsTr("Lesotho");}
            ListElement{key:"231";modelData : qsTr("Liberia");}
            ListElement{key:"218";modelData : qsTr("Libya");}
            ListElement{key:"4175";modelData : qsTr("Liechtenstein");}
            ListElement{key:"370";modelData : qsTr("Lithuania");}
            ListElement{key:"352";modelData : qsTr("Luxembourg");}
            ListElement{key:"853";modelData : qsTr("Macao");}
            ListElement{key:"261";modelData : qsTr("Madagascar");}
            ListElement{key:"265";modelData : qsTr("Malawi");}
            ListElement{key:"60";modelData : qsTr("Malaysia");}
            ListElement{key:"960";modelData : qsTr("Maldives");}
            ListElement{key:"223";modelData : qsTr("Mali");}
            ListElement{key:"356";modelData : qsTr("Malta");}
            ListElement{key:"1670";modelData : qsTr("Mariana Is");}
            ListElement{key:"596";modelData : qsTr("Martinique");}
            ListElement{key:"230";modelData : qsTr("Mauritius");}
            ListElement{key:"52";modelData : qsTr("Mexico");}
            ListElement{key:"373";modelData : qsTr("Moldova");}
            ListElement{key:"377";modelData : qsTr("Monaco");}
            ListElement{key:"976";modelData : qsTr("Mongolia");}
            ListElement{key:"1664";modelData : qsTr("Montserrat Is");}
            ListElement{key:"212";modelData : qsTr("Morocco");}
            ListElement{key:"258";modelData : qsTr("Mozambique");}
            ListElement{key:"264";modelData : qsTr("Namibia");}
            ListElement{key:"674";modelData : qsTr("Nauru");}
            ListElement{key:"977";modelData : qsTr("Nepal");}
            ListElement{key:"599";modelData : qsTr("Netheriands Antilles");}
            ListElement{key:"31";modelData : qsTr("Netherlands");}
            ListElement{key:"64";modelData : qsTr("New Zealand");}
            ListElement{key:"505";modelData : qsTr("Nicaragua");}
            ListElement{key:"227";modelData : qsTr("Niger");}
            ListElement{key:"234";modelData : qsTr("Nigeria");}
            ListElement{key:"850";modelData : qsTr("North Korea");}
            ListElement{key:"47";modelData : qsTr("Norway");}
            ListElement{key:"968";modelData : qsTr("Oman");}
            ListElement{key:"92";modelData : qsTr("Pakistan");}
            ListElement{key:"507";modelData : qsTr("Panama");}
            ListElement{key:"675";modelData : qsTr("Papua New Cuinea");}
            ListElement{key:"595";modelData : qsTr("Paraguay");}
            ListElement{key:"51";modelData : qsTr("Peru");}
            ListElement{key:"63";modelData : qsTr("Philippines");}
            ListElement{key:"48";modelData : qsTr("Poland");}
            ListElement{key:"351";modelData : qsTr("Portugal");}
            ListElement{key:"1787";modelData : qsTr("Puerto Rico");}
            ListElement{key:"974";modelData : qsTr("Qatar");}
            ListElement{key:"262";modelData : qsTr("Reunion");}
            ListElement{key:"40";modelData : qsTr("Romania");}
            ListElement{key:"7";modelData : qsTr("Russia");}
            ListElement{key:"1758";modelData : qsTr("Saint Lueia");}
            ListElement{key:"1784";modelData : qsTr("Saint Vincent");}
            ListElement{key:"684";modelData : qsTr("Samoa Eastern");}
            ListElement{key:"685";modelData : qsTr("Samoa Western");}
            ListElement{key:"378";modelData : qsTr("San Marino");}
            ListElement{key:"239";modelData : qsTr("Sao Tome and Principe");}
            ListElement{key:"966";modelData : qsTr("Saudi Arabia");}
            ListElement{key:"221";modelData : qsTr("Senegal");}
            ListElement{key:"248";modelData : qsTr("Seychelles");}
            ListElement{key:"232";modelData : qsTr("Sierra Leone");}
            ListElement{key:"65";modelData : qsTr("Singapore");}
            ListElement{key:"421";modelData : qsTr("Slovakia");}
            ListElement{key:"386";modelData : qsTr("Slovenia");}
            ListElement{key:"677";modelData : qsTr("Solomon Is");}
            ListElement{key:"252";modelData : qsTr("Somali");}
            ListElement{key:"27";modelData : qsTr("South Africa");}
            ListElement{key:"34";modelData : qsTr("Spain");}
            ListElement{key:"94";modelData : qsTr("SriLanka");}
            ListElement{key:"1758";modelData : qsTr("St.Lucia");}
            ListElement{key:"1784";modelData : qsTr("St.Vincent");}
            ListElement{key:"249";modelData : qsTr("Sudan");}
            ListElement{key:"597";modelData : qsTr("Suriname");}
            ListElement{key:"268";modelData : qsTr("Swaziland");}
            ListElement{key:"46";modelData : qsTr("Sweden");}
            ListElement{key:"41";modelData : qsTr("Switzerland");}
            ListElement{key:"963";modelData : qsTr("Syria");}
            ListElement{key:"886";modelData : qsTr("Taiwan");}
            ListElement{key:"992";modelData : qsTr("Tajikstan");}
            ListElement{key:"255";modelData : qsTr("Tanzania");}
            ListElement{key:"66";modelData : qsTr("Thailand");}
            ListElement{key:"228";modelData : qsTr("Togo");}
            ListElement{key:"676";modelData : qsTr("Tonga");}
            ListElement{key:"1809";modelData : qsTr("Trinidad and Tobago");}
            ListElement{key:"216";modelData : qsTr("Tunisia");}
            ListElement{key:"90";modelData : qsTr("Turkey");}
            ListElement{key:"993";modelData : qsTr("Turkmenistan");}
            ListElement{key:"256";modelData : qsTr("Uganda");}
            ListElement{key:"380";modelData : qsTr("Ukraine");}
            ListElement{key:"971";modelData : qsTr("United Arab Emirates");}
            ListElement{key:"44";modelData : qsTr("United Kingdom");}
            ListElement{key:"1";modelData : qsTr("United States of America");}
            ListElement{key:"598";modelData : qsTr("Uruguay");}
            ListElement{key:"233";modelData : qsTr("Uzbekistan");}
            ListElement{key:"58";modelData : qsTr("Venezuela");}
            ListElement{key:"84";modelData : qsTr("Vietnam");}
            ListElement{key:"967";modelData : qsTr("Yemen");}
            ListElement{key:"381";modelData : qsTr("Yugoslavia");}
            ListElement{key:"243";modelData : qsTr("Zaire");}
            ListElement{key:"260";modelData : qsTr("Zambia");}
            ListElement{key:"263";modelData : qsTr("Zimbabwe");}
        }

        ListModel
        {
            id:serverList
        }
        
    }
    
}
