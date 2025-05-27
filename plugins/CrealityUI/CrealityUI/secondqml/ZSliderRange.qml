/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the Qt Quick Controls 2 module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.impl 2.12
import QtQuick.Templates 2.12 as T
 import QtGraphicalEffects 1.12
import "../qml"
T.RangeSlider {
    id: control
    property bool acceptWheel: true
    property color handleBorderColor: "#7DEEFF"
    property color handleNormalColor: "#1E9BE2"//"#42BDD8"
    property color handleHoverColor: "#00DEFF"
    property color handlePressColor: "#00DEFF"
    property color completeColor: "#00A3FF"//"cadetblue"
    property color incompleteColor: "#4A4A4D"//"#909090"
    property alias secondLabelText: idLabel.text
    property alias firstLabelText: idLabel2.text
    property var mysource : Constants.sliderBtnimg//"qrc:/UI/photo/rangSlider.png"
    property var mysource_d : "qrc:/UI/photo/rangSlider_d.png"
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            first.implicitHandleWidth + leftPadding + rightPadding,
                            second.implicitHandleWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             first.implicitHandleHeight + topPadding + bottomPadding,
                             second.implicitHandleHeight + topPadding + bottomPadding)

    //padding: 6
//    opacity: enabled ? 1:0.8
    first.handle: Rectangle {
        id: idHandle_first
        x: control.leftPadding + (control.horizontal ? control.first.visualPosition * (control.availableWidth - width) : (control.availableWidth - width) / 2)
        y: control.topPadding + (control.horizontal ? (control.availableHeight - height) / 2 : control.first.visualPosition * (control.availableHeight - height))
        color: control.first.pressed ||control.first.hovered ? handleHoverColor :handleNormalColor
        border.color: handleBorderColor
        border.width: 2
        width: 18 //control.horizontal?15:18
        height: 18 //control.horizontal?18:15
        radius: 9
//        Image {
//            width: sourceSize.width
//            height: sourceSize.height
//            source:  control.first.pressed ||control.first.hovered ? mysource_d : mysource
//        }

    }

    second.handle: Rectangle {
        id:idHandle_second
        x: control.leftPadding + (control.horizontal ? control.second.visualPosition * (control.availableWidth - width) : (control.availableWidth - width) / 2) - 0.5
        y: control.topPadding + (control.horizontal ? (control.availableHeight - height) / 2 : control.second.visualPosition * (control.availableHeight - height))
        // width: control.horizontal?15:18
        // height: control.horizontal?18:15
        color: control.second.pressed ||control.second.hovered ? handleHoverColor :handleNormalColor
        border.color: handleBorderColor
        border.width: 2
        width: 18 //control.horizontal?15:18
        height: 18 //control.horizontal?18:15
        radius: 9
//        Image {
//            width: sourceSize.width
//            height: sourceSize.height
//            source: control.second.pressed ||control.second.hovered ? mysource_d : mysource
//        }

    }

    background: Rectangle {
        id: idbackgroud
        x: control.leftPadding + (control.horizontal ? 0 : (control.availableWidth - width) / 2)
        y: control.topPadding + (control.horizontal ? (control.availableHeight - height) / 2 : 0)
        implicitWidth: control.horizontal ? 200 : 4/*4*/
        implicitHeight: control.horizontal ? 4 : 200
        width: control.horizontal ? control.availableWidth : implicitWidth
        height: control.horizontal ? implicitHeight : control.availableHeight
        radius: 2
        color:  control.completeColor
        opacity : control.opacity
//        LinearGradient {
//            anchors.fill: parent
//            start: Qt.point(0, 0)
//            end: Qt.point(idbackgroud.width, 0)
//            gradient:Gradient {
//                GradientStop {position: 0.0; color: Constants.sliderTopColor1}
//                GradientStop {position: 1.0; color: Constants.sliderTopColor2}
//            }
//            source: idbackgroud
//        }
        scale: control.horizontal && control.mirrored ? -1 : 1

        //first
        Rectangle {
            id : idBack_first
            y: control.horizontal ? 0 : control.first.visualPosition * parent.height
            width: control.horizontal ? control.position * parent.width : parent.width
            height: control.horizontal ? parent.height : control.first.position * parent.height//control.position * parent.height
            radius: 2
            color: control.incompleteColor
            opacity : control.opacity
            //control.palette.dark
//            LinearGradient {            ///--[Mark]
//                        anchors.fill: parent
//                        start: Qt.point(0, 0)
//                        end: Qt.point(idBack_first.width, 0)      ///1.横向渐变
//            //            end: Qt.point(0, height)     ///2.竖向渐变
//    //                    end: Qt.point(width, height) ///3.斜向渐变
//                        gradient: Gradient {
//                            GradientStop { position: 1; color: Constants.sliderBottomColor}//"white" }
//                            GradientStop { position: 0; color: Constants.sliderBottomColor/* "white"*/ }
//                        }
//                        source: idBack_first
//                    }

        }

        //second
        Rectangle {
            id : idBack_second

            y: control.horizontal ? 0 : 0
            width: control.horizontal ? control.position * parent.width : parent.width
            height: control.horizontal ? parent.height : control.second.visualPosition * parent.height + 3//control.position * parent.height
            radius: 3
            color: control.incompleteColor//control.palette.dark
            opacity : control.opacity
//            LinearGradient {            ///--[Mark]
//                        anchors.fill: parent
//                        start: Qt.point(0, 0)
//                        end: Qt.point(idBack_second.width, 0)      ///1.横向渐变
//            //            end: Qt.point(0, height)     ///2.竖向渐变
//    //                    end: Qt.point(width, height) ///3.斜向渐变
//                        gradient: Gradient {
//                            GradientStop { position: 1; color: Constants.sliderBottomColor}//"white" }
//                            GradientStop { position: 0; color: Constants.sliderBottomColor/* "white"*/ }
//                        }
//                        source: idBack_second
//                    }

        }
    }

    //second
    Rectangle
    {
        height: 40
        width: 100
        color: "#424B51"
        border.color: "#1E9BE2"
        visible:   control.second.pressed
        y: (control.height * second.visualPosition - 20 * second.visualPosition  - 15 )
        radius: 5
        anchors.right: parent.right
        anchors.rightMargin: 40
        StyledLabel
        {
            id: idLabel
            color:"white"
            anchors.centerIn: parent
        }
        Rectangle
        {
            color: "#1E9BE2"
            rotation: 45
            width: 10
            height: 10
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin:  -5
            z : parent.z - 1
        }
    }
    Rectangle
    {
        height: 40
        width: 100
        color: "#424B51"
        border.color: "#1E9BE2"
        visible:   control.first.pressed
        y: (control.implicitHeight * first.visualPosition - 20*first.visualPosition - 15)
        anchors.right: parent.right
        anchors.rightMargin: 40 * screenScaleFactor
        StyledLabel
        {
            id: idLabel2
            color:"white"
            anchors.centerIn: parent
        }

        Rectangle
        {
            color: "#1E9BE2"
            rotation: 45
            width: 10
            height: 10
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin:  -5
            z : parent.z - 1
        }
    }
//    Label
//    {
//        id: idLabel
//        width: 60
//        height : 30
//        text : "text:" + control.second.value.toFixed(1)
//        visible:   control.second.pressed
//        y: control.implicitHeight * second.visualPosition - 15 *second.visualPosition
//        x : -65
//        horizontalAlignment: Text.AlignRight
//        color:"white"
//    }

//    Label
//    {
//        id: idLabel2
//        width: 60
//        height : 30
//        text : "text:" + control.first.value.toFixed(1)
//        visible:   control.first.pressed
//        y: control.implicitHeight * first.visualPosition - 15*first.visualPosition
//        x : -65
//        horizontalAlignment: Text.AlignRight
//        color:"white"
//    }
}
