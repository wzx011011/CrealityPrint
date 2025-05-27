import QtQuick 2.0
import CrealityUI 1.0
import "qrc:/CrealityUI"
//Rectangle {
Item{
    property var camera
    property var plugin
    //color:"red"
    width: 200 * screenScaleFactor
    height: 200 * screenScaleFactor
    anchors.top: parent.top
    anchors.right: parent.right
    anchors.topMargin: -10 * screenScaleFactor
    anchors.rightMargin: Constants.rightPanelWidth * screenScaleFactor

    function connectFunc()
    {
        camera.signalCameraChaged.disconnect(onSignalCameraChaged)
        camera.signalCameraChaged.connect(onSignalCameraChaged)

        plugin.sigLanguageChanged.disconnect(onLanguageChanged)
        plugin.sigLanguageChanged.connect(onLanguageChanged)

        plugin.sigThemeChanged.disconnect(onThemeChanged)
        plugin.sigThemeChanged.connect(onThemeChanged)
    }

    function onSignalCameraChaged(position, upVector)
    {
        cameraPosButtons.cameraChaged(position, upVector)
    }

    function onLanguageChanged(langType)
    {
        console.log("onLanguageChanged :" + langType)
        cameraPosButtons.setLanguage(langType)
        Constants.languageType = langType
    }

    function onThemeChanged(themeType)
    {
        console.log("onThemeChanged :" + themeType)
        cameraPosButtons.setTheme(themeType)
    }

    CusSkinButton_Image {
        id:idRestBtn

		anchors.right: parent.right
		anchors.top: parent.top
        anchors.topMargin: 35 * screenScaleFactor
        anchors.rightMargin: 75 * screenScaleFactor
        btnImgUrl: hovered ? Constants.homeImg_Hovered : Constants.homeImg
        objectName: "restBtn"
        width: 20 * screenScaleFactor
        height: 20 * screenScaleFactor
        z: cameraPosButtons.z + 1
        onClicked:
        {
            // var posion = Qt.vector3d(0, 0, 30)
            // var upvector = Qt.vector3d(0, 1, 0)
            // camera.viewFromUserSetting(Qt.vector3d(posion.x, posion.z*-1, posion.y),Qt.vector3d(0.0, 0.0, 0.0),Qt.vector3d(upvector.x, upvector.z*-1, upvector.y))
            cameraPosButtons.resetViewer3D()
        }
    }

    CameraPosViewBtn
    {
        id: cameraPosButtons
        //anchors.fill:parent
        width: 150 * screenScaleFactor
        height: 150 * screenScaleFactor
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 25 * screenScaleFactor
        anchors.right: parent.right
        onSetViewShow:
        {
            //console.log("onSetViewShow")
            /* Todo
            camera.viewFromUserSetting(posion(mainCamera.position.x, mainCamera.position.z*-1, mainCamera.position.y), viewCenter(0,0,0), upvector(mainCamera.upvector.x, mainCamera.upvector.z*-1, mainCamera.upvector.y)
            */
            camera.viewFromUserSetting(Qt.vector3d(posion.x, posion.z*-1, posion.y),Qt.vector3d(0.0, 0.0, 0.0),Qt.vector3d(upvector.x, upvector.z*-1, upvector.y))
        }
    }
}
