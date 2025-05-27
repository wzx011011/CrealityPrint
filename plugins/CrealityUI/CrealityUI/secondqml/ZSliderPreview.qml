import QtQuick 2.0
import QtQuick.Controls 2.0
import CrealityUI 1.0
import "qrc:/CrealityUI"
Rectangle {
    id:control
    width:50
    height:400
    color: "transparent"
    property var speedData: 1
    property alias myRealTo: layeSlider.to
    property alias myRealFrom: layeSlider.from
    property alias myFirst: layeSlider.first
    property alias mySecond: layeSlider.second
    signal sliderSecondValueChanged(var value)
    signal sliderFirstValueChanged(var value)
    signal sliderMaxChanged()

    ZSliderRange
    {
        id : layeSlider
        x:3
        y : 5
        width: 20
        height : control.height
        from : 0
        to:100
        stepSize: 1
        second.value:to
        implicitWidth : 6
        implicitHeight :control.height
        orientation : Qt.Vertical
        secondLabelText : "Z: " + second.value.toFixed(1)
        firstLabelText : "Z: " + first.value.toFixed(1)
        second.onMoved:
        {
            sliderSecondValueChanged(second.value)
        }

        first.onMoved:
        {
            sliderFirstValueChanged(first.value)
        }
        onToChanged:
        {
            sliderMaxChanged()
        }

    }
}
