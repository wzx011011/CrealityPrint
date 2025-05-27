import QtQuick 2.5
import QtQuick.Layouts 1.13
import QtQuick.Controls 2.12

import ".."
import "../qml"

ListView {
  id: control

  property var mode;
  property var currentLaserPlotterIndex : -1
  property int mulBegin: 0
  property var indexArray:[]

  signal checkOne(int idx)
  signal checkMul(int idx)
  signal checkMulHandle(var array)
  signal checkIndex(int selectIndex)
  signal checkMulIndex(var idxArray)
  signal checkAllIndex()
  signal clearAllIndex()

  function pushDeDuplication(xxx) {
    indexArray.push(xxx)

    var tmp = [];
    var len = indexArray.length;
    for (var i = 0; i < len; i++) {
      for (var j = i + 1; j < len; j++) {
        if(indexArray[i] === indexArray[j]) {
          i++;
          j = i;
        }
      }
      tmp.push(indexArray[i]);
    }

    indexArray = [];
    for (var k = 0; k < tmp.length; k++) {
      indexArray.push(tmp[k]);
    }
  }

  function hasIndex(array,idx) {
    for(var i = 0; i < array.length; i++) {
      if(array[i] === idx) {
        return true
      }
    }
    return false
  }

  onCurrentIndexChanged: {
    indexArray = [];
    control.checkOne(currentIndex);
    control.mulBegin = currentIndex;
    pushDeDuplication(currentIndex);
  }

  Connections {
    target: control
    onCheckOne: control.mulBegin = idx;
  }

  spacing: 1
  clip: true
  boundsBehavior: Flickable.StopAtBounds

  model: mode

  delegate: Rectangle {
    id: item_delegate
    width: parent.width
    height: 20 * screenScaleFactor
    radius: 2

    Row {
      anchors.verticalCenter: parent.verticalCenter
      height: parent.height

      Rectangle {
        width: parent.height / 2
        height: parent.height
        color: "transparent"
      }

      Text {
        anchors.left: parent.anchors.left
        height: parent.height

        text: name
        color: Constants.right_panel_item_text_default_color
        font.family: Constants.labelFontFamily
        font.pixelSize: Constants.labelFontPixelSize
      }
    }

    property bool checked: false
    color: item_delegate.checked ? Constants.right_panel_item_checked_color
                                 : item_delegate.hovered ? Constants.right_panel_item_hovered_color
                                                         : Constants.right_panel_item_default_color

    Connections {
      target: control

      onCheckOne: {
        item_delegate.checked = (idx === index);
        checkIndex(idx);
        pushDeDuplication(idx);
      }

      onCheckMul: {
        if (idx > control.mulBegin) {
          item_delegate.checked = (index >= control.mulBegin && index <= idx);
        } else {
          item_delegate.checked = (index <= control.mulBegin && index >= idx);
        }
      }

      onCheckMulHandle: {
        item_delegate.checked= hasIndex(array,index)
      }

      onCheckAllIndex: {
        item_delegate.checked = (index >= 0 && index <= control.count)
        indexArray = [];

        for(var i = 0; i < control.count; i++){
          pushDeDuplication(i);
        }

        checkMulIndex(indexArray)
      }

      onClearAllIndex: {
        item_delegate.checked = false
      }
    }

    MouseArea {
      id: item_mousearea
      anchors.fill: parent

      onClicked: {
        switch(mouse.modifiers) {
          case Qt.ControlModifier:
            item_delegate.checked =! item_delegate.checked;
            if (item_delegate.checked) {
              pushDeDuplication(index);
            } else {
              indexArray.pop(index);
            }
            checkMulIndex(indexArray)
            break;

          case Qt.ShiftModifier:
            //control.checkMul(index);
            break;

          default:
            indexArray = [];
            control.checkOne(index);
            control.mulBegin=index;
            pushDeDuplication(index)
            break;
        }
      }
    }
  }

  ScrollBar.vertical: ScrollBar {}
}
