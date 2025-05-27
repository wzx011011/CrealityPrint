#include "slicecommand.h"

#include "interface/visualsceneinterface.h"
#include "interface/selectorinterface.h"
#include <QDebug>
#include "sliceplugin.h"
#include "us/slicermanager.h"
#include "kernel/abstractkernel.h"
#include <interface/spaceinterface.h>
#include "inout/cxopenandsavefilemanager.h"
#include "interface/modelinterface.h"
#include <QFileInfo>

using namespace creative_kernel;

SliceCommand::SliceCommand(QObject *parent)
    :QObject(parent)
{
    m_footStatus = AbstractKernelUI::getSelf()->getUI("footer");
}

SliceCommand::~SliceCommand()
{
}
QObject* SliceCommand::getSlicePlugin()
{
    return m_slicePlugin;
}
void SliceCommand::sliceSuccess()
{
    AbstractKernelUI::getSelf()->executeCommand("requestmenudialog", this, "sliceSuccessDlg");
}
void SliceCommand::sliceUnPreview()
{
    QMetaObject::invokeMethod(m_footStatus, "showSlciePreviewStatus",  Q_ARG(QVariant, QVariant::fromValue(false)));
}

int SliceCommand::onExportSuccess(QObject* object)
{
    if (m_slicePlugin)
    {
        m_slicePlugin->sliceMainOp("unpreview");
    }
    return 0;
}

Q_INVOKABLE QObject* SliceCommand::getTableModel()
{
    qDebug() << "AbstractKernelUI::getSelf()->tableModel() =" << AbstractKernelUI::getSelf()->tableModel();
    return AbstractKernelUI::getSelf()->tableModel();
}

Q_INVOKABLE void SliceCommand::startSearchWifi()
{
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
void SliceCommand::sendSliceFile()
{

}
void SliceCommand::onSaveSliceSuccess(QString filePath)
{
    
}

bool SliceCommand::copyFile(QString srcPath, QString dstPath, bool coverFileIfExist)
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


