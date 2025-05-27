import QtQuick 2.0

Item {
    property double factor : 1.05
    property var control
    id:idPlotterScene
    property var ismove: false
    property  var pressX:0
    property  var pressY:0
    property  var releaseX
    property  var releaseY
    property var winWidth: 1920
    property var winHeight: 1080
    clip: true
	signal upperObj()
	signal bottomObj()
    

    onWinWidthChanged:
    { 
        idContent.x= -(idPlotterScene.width*100)/2+ winWidth/2 
        idContent.y= -(idPlotterScene.height*100)/2+ winHeight/2
    }

    onWinHeightChanged:
    {
        idContent.x= -(idPlotterScene.width*100)/2+ winWidth/2 
        idContent.y= -(idPlotterScene.height*100)/2+ winHeight/2
    }
    
     function deleteUndoObject(obj)
    {
        control.deleteObject(obj)
        obj.visible = false
    }

    function deleteSelectObject()
    {
        if(idDrawPanel.ctrlSelArray.length>0){
            control.deleteMulObject(idDrawPanel.ctrlSelArray);
            for(var i = 0; i < idDrawPanel.ctrlSelArray.length; i++)
            {
                if(idDrawPanel.ctrlSelArray[i])
                {
                    if(idDrawPanel.ctrlSelArray[i].selected)
                    {
                        //idDrawPanel.ctrlSelArray[i].destroy()
                        idDrawPanel.ctrlSelArray[i].visible = false
                    }
                }
            }
            idDrawPanel.ctrlSelArray = [];
        }
        else
        {
            if(idDrawPanel.currentSel)
            {
                if(idDrawPanel.currentSel.selected)
                {
                    control.deleteObject(idDrawPanel.currentSel)
                    idDrawPanel.currentSel.destroy()
                    idDrawPanel.currentSel=null;
                }
            }
        }
    }
    onHeightChanged: {
        // console.log("x====="+tform.xScale)
        // if(height<900)
        // {
        //     var zoomFactor = height/1.0/900
        //     scaleCanvas(zoomFactor)
        //     idLaserGrid.x = (width-140-900)/2
        //     idLaserGrid.y = (height-idLaserGrid.height)/2
        // }else{
        //      tform.xScale =1;
        //      tform.yScale=1;
        //     idLaserGrid.x = (width-140-900)/2
        //     idLaserGrid.y = (height-idLaserGrid.height)/2
        // }


    }


    Item{
        id:idContent
        //color: "blue"
        //x:(idPlotterScene.width*tform.xScale-idPlotterScene.width)/2
        //y:(idPlotterScene.height*tform.yScale-idPlotterScene.height)/2
        x: -(idPlotterScene.width*100)/2
        y: -(idPlotterScene.height*100)/2
        //anchors.fill: parent
        width: idPlotterScene.width*100//idPlotterScene.width
        height: idPlotterScene.height*100//idPlotterScene.height
        LaserGrid {
            id:idLaserGrid
            width: 900
            height: 900
            anchors.centerIn: parent
            //x:(parent.width-130-900)/2
            //y:parent.height/2
        }
        MouseArea {
            anchors.fill: parent
            propagateComposedEvents: true

            onWheel: {
                if(wheel.angleDelta.y > 0)  // zoom in
                                   var zoomFactor = factor
                               else                        // zoom out
                                   zoomFactor = 1/factor
                if((idContent.width)*tform.xScale*zoomFactor<idPlotterScene.width*100)
                {
                    return;
                }
                if((idContent.width)*tform.xScale*zoomFactor>idPlotterScene.width*10*100)
                {
                    return;
                }
                console.log(idContent.x)

                               tform.xScale *=zoomFactor
                               tform.yScale *=zoomFactor
            }
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onClicked: {
                if (mouse.button === Qt.RightButton) {
                    rightPopUpMenu.popup()
                     if(idDrawPanel.ctrlSelArray.length>0){
                        rightPopUpMenu.actionEnabled= true
                     }else{
                         rightPopUpMenu.actionEnabled=false
                     }
                }
            }
            onPressed: {
                if (mouse.button == Qt.RightButton)
                {
                    pressX = mouse.x;
                    pressY = mouse.y;
                    ismove = true
                }
                    
            }
            onReleased:
            {
                ismove = false
            }
            onPositionChanged:

            {   
                releaseX = mouse.x
                releaseY = mouse.y
                if(ismove)
                {
                    idContent.x += (releaseX-pressX)
                    idContent.y += (releaseY-pressY)
                }
                
            }
        }
        transform : Scale{
             id : tform
             origin.x: idContent.width/2
             origin.y: idContent.height/2
        }
        DrawPanel {
            id:idDrawPanel
            anchors.fill: parent
            onAddRect: function(obj,sharpType){
                control.addDragRectObject(obj,sharpType)
            }
            onSelectChanged: function(obj){
                control.setSelectObject(obj)
            }
            onSelectedMul: function(objArray){
                control.setSelectMulObject(objArray)
            }
            onClearSelectedMul: function(){
                control.setClearSelectAllObject()
            }
            onVerifyFocus:{
                idDrawPanel.forceActiveFocus();
                idDrawPanel.Keys.enabled = true;
            }
	    onObjectChanged: {
                control.objectChanged(obj, oldX, oldY, oldWidth, oldHeight, oldRotation, newX, newY, newWidth, newHeight, newRotation)
            }
        }
    }

    Column {
        width: 58 *screenScaleFactor
//        height: 272*screenScaleFactor
        anchors.left: parent.left
        anchors.leftMargin: 20*screenScaleFactor
        anchors.top: parent.top
        anchors.topMargin: 20*screenScaleFactor
        spacing: 10

        PlotterToolBar {
            id: idLaserToolBar
//            anchors.fill: parent
            width: 58 *  screenScaleFactor
            visible: true
            onEllipseMode: {
                idDrawPanel.currentSharp = DrawPanel.SharpType.ELIPSE
            }
            onRectMode: {
                idDrawPanel.currentSharp = DrawPanel.SharpType.RECT
            }
            onPickMode: {
                idDrawPanel.currentSharp = DrawPanel.SharpType.NONE
            }
            onImageMode: {
                control.open()
            }
            onTextMode: {
                idDrawPanel.currentSharp = DrawPanel.SharpType.TEXT
            }
            onLineMode: {
                idDrawPanel.currentSharp = DrawPanel.SharpType.PATH
            }

        }
    }
    LaserRClickMenu {
        id: rightPopUpMenu
        onRClickDeleteSelect: {
            deleteSelectObject()
        }
		onSigSetTop:
		{
			upperObj()
		}
		onSigSetBottom:
		{
			bottomObj()
		}
    }
    onVisibleChanged: {
        if(visible)
        {
            forceActiveFocus();
            Keys.enabled = true;    

            idLaserGrid.width = control.width
            idLaserGrid.height = control.height
            idLaserGrid.repaint()
            //control.origin = Qt.point(idLaserGrid.x+idLaserGrid.width/2,idLaserGrid.y+idLaserGrid.height/2)
            control.origin = Qt.point(idLaserGrid.x,idLaserGrid.y+idLaserGrid.height)
        }else{
            idLaserGrid.clear()
        }

    }
    function canvasZoomOut(){
              scaleCanvas(1/factor);

          }

    function canvasZoomIn(){
              scaleCanvas(factor);

          }

    function scaleCanvas(scaleFactor){
              tform.xScale *=scaleFactor;
              tform.yScale *=scaleFactor;
              //canvas.height *= scaleFactor;
              //canvas.width *= scaleFactor;
          }
    function createImageSharp()
    {
        var obj = idDrawPanel.createSharp(0,0,100,100,DrawPanel.SharpType.IMAGE)
        obj.opacity = 1
        return obj;
    }
    function addRectObject(sharpName, obj)
    {
        obj.visible = true
        control.addRedoRectObject(obj, sharpName)
    }
}
