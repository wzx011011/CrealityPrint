/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt3D Editor of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/
import QtQuick 2.5
import QtQuick.Controls 1.4 as QQC1
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 2.0 as QQC2
import "../secondqml"

QQC2.Label {
	id: control
    property alias tooltip: button.tooltip
	property var strToolTip: ""
    property var borderColor: "transparent"
    property var bgColor: "transparent"
    //property alias font: idTextArea.font
	
    color: enabled ? Constants.textColor : Constants.disabledTextColor
    font.family: Constants.labelFontFamily
    font.weight: Constants.labelFontWeight
    font.pointSize:9

    QQC1.Button {
        id: button
        anchors.fill: parent
        style: ButtonStyle {
            background: Rectangle {
                border.width: 1
                border.color: borderColor
                color: "transparent"
            }
        }
    }

	QQC2.ToolTip {
        id: tipCtrl
        visible: strToolTip ? ma.containsMouse : false
        //timeout: 2000
		delay: 100
		width: 400*screenScaleFactor
        implicitHeight: idTextArea.contentHeight + 40*screenScaleFactor
		font.pointSize: Constants.labelFontPointSize
		background: Rectangle {
            anchors.fill: parent
            color: "transparent"
        }
		
		contentItem: QQC2.TextArea{
            id: idTextArea
			text: strToolTip
			//Layout.fillWidth: true
            //Layout.alignment: Qt.AlignTop
			//width: parent.width
            //height: idTextArea.contentHeight + 40
			wrapMode: TextEdit.WordWrap
			color: Constants.textColor
            font.family: Constants.labelFontFamily
			font.weight: Constants.labelFontWeight
			font.pointSize:Constants.labelLargeFontPointSize
			readOnly: true
            verticalAlignment: Qt.AlignVCenter
			background: Rectangle
			{
				anchors.fill : parent
				color: Constants.tooltipBgColor
				border.width:1
				//border.color:hovered ? "lightblue" : "#bdbebf"
			}
        }
    }

    background: Rectangle
    {
        anchors.fill : parent
        color: bgColor
        border.width:0
    }
	
    MouseArea {
        id: ma
        anchors.fill: parent
        hoverEnabled: true
    }
	
}
