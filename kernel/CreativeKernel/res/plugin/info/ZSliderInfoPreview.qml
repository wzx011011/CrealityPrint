import QtQuick 2.0
import CrealityUI 1.0
import "qrc:/CrealityUI"
Item {
    y:150  * screenScaleFactor
    anchors.right: parent.right
    anchors.rightMargin: (65 +  Constants.rightPanelWidth) * screenScaleFactor
    width : 20 * screenScaleFactor
    height :400 * screenScaleFactor
    property var sliderObj
    property var flag: false
    visible:  standaloneWindow.curState === 0 && standaloneWindow.functionType === 0
    function updateSliderValue()
    {

        idSlider.mySecond.value = sliderObj.layers()
        idSlider.myFirst.value = 0
        sliderObj.setTopCurrentLayer(sliderObj.layers())
        sliderObj.setBottomCurrentLayer(0)
    }
    function mySliderMaxChanged()
    {
        //console.log("sliderMaxChanged test ===" + sliderObj.layers())
        idSlider.myRealTo = sliderObj.layers()
        idSlider.mySecond.value = sliderObj.layers()
        idSlider.myFirst.value = 0
    }

    ZSliderPreview
    {
        id : idSlider
        anchors.fill: parent
        enabled: flag
//        visible: false
        myRealTo : sliderObj ? sliderObj.layers() : 0
      //  myRealValue : myRealTo
        mySecond.value : sliderObj ? sliderObj.layers() : 0
        onSliderSecondValueChanged:
        {
            //console.log("onSliderSecondValueChanged !!!")
            sliderObj.setTopCurrentLayer(value)
        }
        onSliderFirstValueChanged:
        {
            //console.log("onSliderFirstValueChanged value =" + value)
            sliderObj.setBottomCurrentLayer(value)
        }
        onSliderMaxChanged:
        {
            //console.log("onSliderMaxChanged !!!")
            if(sliderObj)
            {
              sliderObj.maxZHeightChanged.disconnect(mySliderMaxChanged)
              sliderObj.maxZHeightChanged.connect(mySliderMaxChanged)
            }
            
        }

    }
    
}
