import QtQuick 2.0

Item {
    id:idLaserScene
    property double factor : 1.05
    property var control
    property var ismove: false
    property var isMoving:false
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
        idContent.x= -(idLaserScene.width*100)/2+ winWidth/2 
        idContent.y= -(idLaserScene.height*100)/2+ winHeight/2
    }

    onWinHeightChanged:
    {
        idContent.x= -(idLaserScene.width*100)/2+ winWidth/2 
        idContent.y= -(idLaserScene.height*100)/2+ winHeight/2
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
    // Flickable{
    //     anchors.fill: parent
    //     contentWidth: idLaserScene.width*tform.xScale
    //     contentHeight: idLaserScene.height*tform.yScale
    //     contentX: (idLaserScene.width*tform.xScale-idLaserScene.width)/2
    //     contentY: (idLaserScene.height*tform.yScale-idLaserScene.height)/2
    Item{
        id:idContent
        //color: "blue"
        //x:(idLaserScene.width*tform.xScale-idLaserScene.width)/2
        //y:(idLaserScene.height*tform.yScale-idLaserScene.height)/2
        x: -(idLaserScene.width*100)/2+ winWidth/2 //960
        y: -(idLaserScene.height*100)/2+ winHeight/2//512
        //anchors.fill: parent
        width: idLaserScene.width*100//idLaserScene.width
        height: idLaserScene.height*100//idLaserScene.height
        
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
                if(idContent.width*tform.xScale*zoomFactor<idLaserScene.width*100)
                {
                    return;
                }
                if(idContent.width*tform.xScale*zoomFactor>idLaserScene.width*10*100)
                {
                    return;
                }
                console.log(idContent.x)

                               tform.xScale *=zoomFactor
                               tform.yScale *=zoomFactor
            }
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onClicked: {//console.log("lserScene width:",idLaserScene.width,"lserScene height:",idLaserScene.height);console.log("lserScene winWidth:",idLaserScene.winWidth,"lserScene winHeight:",idLaserScene.winHeight)
                // if (mouse.button === Qt.RightButton) {
                    
                //     if(idDrawPanel.ctrlSelArray.length>0 && !isMoving){
                //         rightPopUpMenu.popup()
                //         rightPopUpMenu.actionEnabled= true
                //     }else{
                //         rightPopUpMenu.actionEnabled=false
                //     }
                // }
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

                if (mouse.button === Qt.RightButton) {
                    
                    if(idDrawPanel.ctrlSelArray.length>0 && !isMoving){
                        rightPopUpMenu.popup()
                        rightPopUpMenu.actionEnabled= true
                    }else{
                        rightPopUpMenu.actionEnabled=false
                    }
                }
                isMoving = false
            }
            onPositionChanged:
            {   
                releaseX = mouse.x
                releaseY = mouse.y
                if(ismove)
                {
                    idContent.x += (releaseX-pressX)
                    idContent.y += (releaseY-pressY)
                    isMoving = true
                    console.log("onPositionChanged~~~~~~~~~~~~~~~~~")
                }
                
            }
        }
        transform : Scale{
             id : tform
             origin.x: idContent.width/2//idContent.width/2-450//
             origin.y: idContent.height/2//idContent.height/2+450//
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
                idLaserScene.forceActiveFocus();
                idLaserScene.Keys.enabled = true;
            }
            onObjectChanged: {
                control.objectChanged(obj, oldX, oldY, oldWidth, oldHeight, oldRotation, newX, newY, newWidth, newHeight, newRotation)
            }
        }
    }
    // }

    Column {
        width: 58 *  screenScaleFactor
//        height: 272 *  screenScaleFactor
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.top: parent.top
        anchors.topMargin: 20
        spacing: 10 * screenScaleFactor

        LaserToolBar {
            id: idLaserToolBar
            width: 58 *  screenScaleFactor
//            height: 337 *  screenScaleFactor

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
