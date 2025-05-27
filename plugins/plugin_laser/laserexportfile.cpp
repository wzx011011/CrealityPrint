#include "laserexportfile.h"

#include "interface/visualsceneinterface.h"
#include "interface/selectorinterface.h"
#include <QDebug>
#include "us/slicermanager.h"
#include "kernel/abstractkernel.h"
#include <interface/spaceinterface.h>
//#include "dlptype/sliceconfig.h"
//
//#include "interface/printermnginterface.h"
#include "kernel/addtabledata.h"
//#include "interface/settinginterface.h"
#include "inout/cxopenandsavefilemanager.h"
#include "interface/modelinterface.h"
#include <QFileInfo>
using namespace creative_kernel;

LaserExportFile::LaserExportFile(QObject *parent)
    :QObject(parent)
{
    m_footStatus = AbstractKernelUI::getSelf()->getUI("footer");

    m_addTable = creative_kernel::AddTableData::instance();
}

LaserExportFile::~LaserExportFile()
{
}
 QObject* LaserExportFile::getLaserScene()
{
    return m_laserScene;
}
  void LaserExportFile::localFileSaveSuccess(QObject* object)
  {
      AbstractKernelUI::getSelf()->executeCommand("requestmenudialog",this, "laserSaveSuccessDlg");
  }
  void LaserExportFile::sliceUnPreview()
  {
      QMetaObject::invokeMethod(m_footStatus, "showSlciePreviewStatus",  Q_ARG(QVariant, QVariant::fromValue(false)));
  }

  int LaserExportFile::onExportSuccess(QObject* object)
  {
      //if (m_slicePlugin)
      //{
      //    m_slicePlugin->sliceMainOp("unpreview");
      //}
      return 0;
  }

  Q_INVOKABLE QObject* LaserExportFile::getTableModel()
  {
      qDebug() << "AbstractKernelUI::getSelf()->tableModel() =" << AbstractKernelUI::getSelf()->tableModel();
      return AbstractKernelUI::getSelf()->tableModel();
  }

  Q_INVOKABLE void LaserExportFile::startSearchWifi()
  {
//      m_addTable->searchPrinter();
      QString fileName = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/" + "1.gcode";
           QString fileRealPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/"
               + CXFILE.currentMachineName() + "_" + CXFILE.currOpenFile() + ".gcode";
           if (QFile::exists(fileName))
           {
              bool bRet = copyFile(fileName, fileRealPath, true);

              QObject* pmainObj = AbstractKernelUI::getSelf()->getUI("uiappwindow");
              if (pmainObj)
              {
                  QObject* wifiObj = pmainObj->findChild<QObject*>("localWifiObj");

                  if (wifiObj)
                  {
                      QFileInfo info(fileRealPath);
                      QString realName = info.fileName();
                      QMetaObject::invokeMethod(wifiObj, "showTransControlDlg", Q_ARG(QVariant, QVariant::fromValue(fileRealPath)), Q_ARG(QVariant, QVariant::fromValue(realName)));
                  }
              }
           }
  }
  void LaserExportFile::sendSliceFile()
  {
      //lisugui 2021-3-4
      QString filePath = m_laserScene->getTmpFilePeth();
      if (filePath.length() > 0)
      {
          onSaveSliceSuccess(filePath);
      }
  }
  void LaserExportFile::onSaveSliceSuccess(QString filePath)
  {
      m_addTable->sendLocalSliceFile(filePath);
  }
  bool LaserExportFile::copyFile(QString srcPath, QString dstPath, bool coverFileIfExist)
   {
       srcPath.replace("\\", "/");
       dstPath.replace("\\", "/");
       if (srcPath == dstPath) {
           return true;
       }

       if (!QFile::exists(srcPath)) {  //源文件不存在
           return false;
       }

       if (QFile::exists(dstPath)) {
           if (coverFileIfExist) {
               QFile::remove(dstPath);
           }
       }

       if (!QFile::copy(srcPath, dstPath)) {
           return false;
       }
       return true;
   }
