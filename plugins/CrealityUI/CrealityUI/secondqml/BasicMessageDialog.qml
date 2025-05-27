import QtQuick 2.0
import QtQuick.Controls 2.0
//import CrealityUI 1.0
//import "qrc:/CrealityUI"
import ".."
import "../qml"
BasicDialog {
    id: informationDlg
    width: 315
    height: 210/*190*/
    title: qsTr("Message")
    property alias messageText: idPanelname.text
    property alias width_x: informationDlg.width
    property alias width_y: informationDlg.height
	
	property alias yesBtnText: add_support.text
	property alias noBtnText: cancel_support.text
	property alias ignoreBtnText: ignore_support.text
	property alias checkedFlag: idPopup.checked

    //radius : 5
   // color: Constants.itemBackgroundColor
    signal accept()
    signal cancel()
	signal ignore()
	signal closeDlg()
	property var isInfo : true
	property var btnCount : 1
    property var showChecked : false

    function showSingleBtn()
    {
        cancel_support.visible = false
		ignore_support.visible = false
		btnCount= 1
		informationDlg.width = 315
    }
	function showDoubleBtn()
    {
        cancel_support.visible = true
		ignore_support.visible = false
		btnCount= 2
		informationDlg.width = 480
    }
	function showTripleBtn()
    {
        cancel_support.visible = true
		ignore_support.visible = true
		btnCount= 3
		informationDlg.width = 480
    }

    function showCheckBox(isShow)
    {
        showChecked = isShow
        checkedFlag = false
    }
	
	onDialogClosed:
	{
		closeDlg()
	}

    Image {
        id: imageRepair
        x : parent.width /2 - width/2
        // width: 52/*60*/
        // height: 44/*50*/
        height:sourceSize.height
        width: sourceSize.width
        source: isInfo ? "qrc:/UI/images/information.png" : "qrc:/UI/images/warning.png"
        fillMode: Image.PreserveAspectFit       //拉伸是缩放 总是显示完整图片
		anchors.horizontalCenter: parent.horizontalCenter
		anchors.horizontalCenterOffset: idPanelname.contentWidth * -0.5
		anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -30//
    }
    Text {
       id: idPanelname
       x: 65
       /*y: 100*/
       anchors.left: imageRepair.right
       anchors.leftMargin: 10
	   anchors.verticalCenter: imageRepair.verticalCenter
	   
       text: qsTr("Save and exit ?")

       color: Constants.textColor
       font.pixelSize:Constants.labelFontPixelSize  // panelFontSize
       wrapMode: Text.WordWrap
    }
	
	Item
    {
        id:idSeparator
		anchors.left: parent.left
		anchors.leftMargin: 20/*10*/
		anchors.top: imageRepair.bottom
		anchors.topMargin: 20
		
        width : parent.width - 20
        height : 2
        BasicSeparator
        {
            width : parent.width - 20
            height : 2
        }
    }

    StyleCheckBox {
        id: idPopup
        anchors.left: idSeparator.left       
        anchors.top: idSeparator.top
        anchors.topMargin: 10
        height: 17
        width : 17
        checked : false
        visible: showChecked
        // onCheckedChanged:{}
        // onClicked: {}
    }

    StyledLabel {
        id: label1
        anchors.top: idPopup.top
        anchors.topMargin: 2
		anchors.left: idPopup.right
        anchors.leftMargin: 5
        height: 50
        width : 300
        text: qsTr("Next time it will not pop up, remember that?")
        wrapMode: Text.WordWrap
        font.pixelSize: 12
        visible: showChecked
    }

	
	
    //by TCJ
    BasicDialogButton
    {
        id: add_support
        text: qsTr("yes")
//        btnTextColor : "#E3EBEE"
//        hoveredBtnBgColor:  Constants.hoveredColor
//        defaultBtnBgColor : Constants.buttonColor
        width: parent.width  > 250 ? 110 : (parent.width -30 )  /2
        height: 30
        btnRadius:3
        btnBorderW:0
        defaultBtnBgColor: Constants.profileBtnColor
        hoveredBtnBgColor: Constants.profileBtnHoverColor
        //x: parent.width/2-width
        //
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 18/*10*/
		anchors.left: parent.left
        anchors.leftMargin: informationDlg.width/2 - (add_support.width * btnCount + 25 * (btnCount -1))/2   
        onSigButtonClicked:
        {
            close();
            accept();
        }
    }

    BasicDialogButton
    {
        id: cancel_support
        text: qsTr("no")
        width: add_support.width
        height: 30
        anchors.left: add_support.right
        anchors.top: add_support.top
        anchors.leftMargin: 15
        btnRadius:3
        btnBorderW:0
        defaultBtnBgColor: Constants.profileBtnColor
        hoveredBtnBgColor: Constants.profileBtnHoverColor
        onSigButtonClicked:
        {
            close();
            cancel();
        }
    }
	
	BasicDialogButton
    {
        id: ignore_support
        text: qsTr("ignore")
        width: add_support.width
        height: 30
        anchors.left: cancel_support.right
        anchors.top: cancel_support.top
        anchors.leftMargin: 15
        btnRadius:3
        btnBorderW:0
        defaultBtnBgColor: Constants.profileBtnColor
        hoveredBtnBgColor: Constants.profileBtnHoverColor
        onSigButtonClicked:
        {
            close();
            ignore();
        }
    }
}
