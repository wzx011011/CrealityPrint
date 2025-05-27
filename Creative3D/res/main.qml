import QtQuick 2.0
import CrealityUI 1.0
import "qrc:/CrealityUI"
QtObject {
      id: root
      property var crealityVersion: ""
      property QtObject $splashScreen:Splash {
//              timeoutInterval:1000
//              onTimeout:{
//              }
          //versionText : crealityVersion
          }
      property var loader: Loader{
          id:idMainWindow
          asynchronous: true
          source: "qrc:/scence3d/res/MainWindow.qml"
          active: false
          onLoaded: {
              console.log("scence3d/res/MainWindow.qml!!!!!")
              $splashScreen.visible = false
              idMainWindow.item.showWizardDlg()
          }
      }
      Component.onCompleted:{
          loader.active = true;
      }
  }
