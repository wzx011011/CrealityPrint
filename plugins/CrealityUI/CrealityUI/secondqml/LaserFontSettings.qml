import QtQuick 2.0
import QtQuick.Layouts 1.13
import QtQuick.Controls 2.5
import "../qml"
Column {
    signal familyChanged(var text)
    //signal fontStyleChanged(var text)
    //signal fontTextChanged(var text)
    property alias fontText: idTextArea.text
    property alias fontFamily: idFontFamily.currentText
    property alias fontSize: idFontSize.text
    property var fontStyle //:idFontStyles.currentText
    width: 300
    height: 130
	spacing:5
    Rectangle{
        width: parent.width
        height: 5
        color: "transparent"
    }
    Row{
        spacing:25
        StyledLabel {
            text: qsTr("Text")
            font.pixelSize: 12
			width: 90
            height: 28
			verticalAlignment: Qt.AlignVCenter
			horizontalAlignment: Qt.AlignLeft
        }
        BasicDialogTextField {
            id:idTextArea
            width: 120
            height: 28
            baseValidator:RegExpValidator { regExp: /^\S{100}$/ }
            placeholderText: qsTr("Please input text")
            font.pointSize: 12
            text: "Creality"
            onTextChanged: {
                fontTextChanged(text)
            }
        }
    }
    Row{
		spacing:25
        StyledLabel {
            text: qsTr("Font Family")
            font.pixelSize: 12
			width: 90
            height: 28
			verticalAlignment: Qt.AlignVCenter
			horizontalAlignment: Qt.AlignLeft
        }
        BasicCombobox {
            id:idFontFamily
            width: 120
            height: 28
            model: control.getFontFamilys()
            onCurrentTextChanged:  {
                //idFontStyles.model = control.getFontFamilysStyles(currentText);
                //familyChanged(text)
            }
        }
    }
	
    Row{
        spacing:25
        StyledLabel {
            text: qsTr("Font Size")
            font.pixelSize: 12
			width: 90
            height: 28
			verticalAlignment: Qt.AlignVCenter
			horizontalAlignment: Qt.AlignLeft
        }
        BasicDialogTextField {
            id:idFontSize
            width: 120
            height: 28
            unitChar:"pt"
            text: "30"
        }
    }

    // Row{
    //     spacing:25
    //     StyledLabel {
    //         text: qsTr("Font Style")
    //         font.pixelSize: 14
	// 		width: 90
    //         height: 28
	// 		verticalAlignment: Qt.AlignVCenter
	// 		horizontalAlignment: Qt.AlignLeft
    //     }
    //     BasicCombobox {
    //         id:idFontStyles
    //         width: 120
    //         height: 28
    //         onCurrentTextChanged: {
    //             //fontStyleChanged(currentText)
    //         }
    //     }
    // }
}
