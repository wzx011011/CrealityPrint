#include "cx3dautosaveproject.h"
#include <QSettings>
#include <QStandardPaths>
#include <QDebug>
#include <QDir>
//#include "interface/jobsinterface.h"
#include "cx3dexportjob.h"
#include "cx3dexporter.h"
#include "interface/jobsinterface.h"
#include "qtuserqml/property/qmlpropertysetter.h"
#include "kernel/projectinfoui.h"
#include "cx3dsubmenurecentproject.h"
#include "kernel/abstractkernel.h"
#include "us/slicermanager.h"
using namespace creative_kernel;
using namespace qtuser_3d;
using namespace qtuser_qml;
Cx3dAutoSaveProject::Cx3dAutoSaveProject(QObject* parent)
    :QObject(parent)
{
    m_timer = nullptr;
    QString tmpProject = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/tmpProject";
    QDir tempDir;
    if (!tempDir.exists(tmpProject))
    {
        tempDir.mkpath(tmpProject);
    }
    m_strFilePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/tmpProject" + "/default.cxprj";
    setSettingsData(m_strFilePath);
    QSettings setting;
    setting.beginGroup("AutoSave_Path");
    QString strFileName = setting.value("filePath","default").toString();
    setting.endGroup();
    if(!strFileName.contains("default"))
    {
        m_strFilePath = strFileName;
    }
    m_fMinutes = ProjectInfoUI::instance()->getMinutes();
    connect(ProjectInfoUI::instance(),SIGNAL(minutesChanged(float)),this,SLOT(slotMinutesChanged(float)));
}

Cx3dAutoSaveProject::~Cx3dAutoSaveProject()
{
    stopTimer();
    m_timer->deleteLater();
    if (tmp_timer)
    {
        tmp_timer->deleteLater();
    }
    
}
Cx3dAutoSaveProject *Cx3dAutoSaveProject::instance()
{
    static Cx3dAutoSaveProject m_saveManger;
    return  &m_saveManger;
}

void Cx3dAutoSaveProject::stopTimer()
{
    if(m_timer)
    {
        m_timer->stop();
    }
}
void Cx3dAutoSaveProject::startTimer()
{
    if(m_timer)
    {
        m_timer->start();
    }
}
void Cx3dAutoSaveProject::reject()
{
    QFile file(m_strFilePath);
    if (file.exists())
    {
        file.remove();
    }
}
void Cx3dAutoSaveProject::accept()
{
    us::GlobalSetting* globalsetting = SlicerManager::instance().globalsettings();
    QString machine_support_slicer_type = globalsetting->settings().value("machine_support_slicer_type")->value().toString();
    if (machine_support_slicer_type != "FDM")
    {
        QObject* switchModel = AbstractKernelUI::getSelf()->getUI("glmainview")->parent()->findChild<QObject*>("switchModelLayout")->findChild<QObject*>("switchModel");
        QMetaObject::invokeMethod(switchModel, "changeToFdm");
    }

    JobPtr job(new Cx3dExportJob(m_strFilePath, Cx3dExportJob::JobType::READJOB));
    executeJob(job);
}
void Cx3dAutoSaveProject::createOpenProject(QString strFilePath,bool bOpen)
{
    if(!m_timer)
    {
       m_timer = new QTimer(this);
       m_timer->start(60000 * m_fMinutes);
       connect(m_timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    }
    /*if(!strFilePath.isEmpty())
    {
        m_strFilePath = strFilePath;
    }*/
    //QSettings setting;
    //setting.beginGroup("AutoSave_Path");
    //setting.setValue("filePath", m_strFilePath);
    //setting.endGroup();
    //setSettingsData(m_strFilePath);
    if(bOpen)
    {
        if (QFile::exists(m_strFilePath))
        {
            if (!tmp_timer)
            {
                tmp_timer = new QTimer(this);
                tmp_timer->start(500);
                connect(tmp_timer, SIGNAL(timeout()), this, SLOT(updateTmpTime()));
            }
            //AbstractKernelUI::getSelf()->executeCommand("requestMenuDialog", this, "openDefaultCx3d");
        }

        //JobPtr job(new Cx3dExportJob(m_strFilePath, Cx3dExportJob::JobType::READJOB));
        //executeJob(job);
    }
    Cx3dSubmenuRecentProject::getInstance()->setMostRecentFile(m_strFilePath);
    ProjectInfoUI::instance()->setProjectPath(m_strFilePath);
    ProjectInfoUI::instance()->setProjectName(ProjectInfoUI::instance()->getNameFromPath(m_strFilePath));
    ProjectInfoUI::instance()->updateProjectNameUI();
}

void Cx3dAutoSaveProject::updateTime()
{
    qDebug()<<"update!!!!" << m_strFilePath;
    saveProject();
}
void Cx3dAutoSaveProject::updateTmpTime()
{
    AbstractKernelUI::getSelf()->executeCommand("requestMenuDialog", this, "openDefaultCx3d");
    if (tmp_timer)
    {
        tmp_timer->stop();
    }
}
void Cx3dAutoSaveProject::slotMinutesChanged(float nMinute)
{
    m_fMinutes = nMinute;
    qDebug()<<"Cx3dAutoSaveProject::slotMinutesChanged nMinute =" <<m_fMinutes;
    if(nMinute == 0.0)return;
    m_timer->start(60000 * m_fMinutes);
}

void Cx3dAutoSaveProject::saveProject()
{
    m_strFilePath = m_strFilePath.replace("file:///", "");
    if (m_strFilePath.isEmpty())
    {
        return;
    }
    JobPtr job(new Cx3dExportJob(m_strFilePath, Cx3dExportJob::JobType::WRITEJOB));
    executeJob(job);

    Cx3dSubmenuRecentProject::getInstance()->setMostRecentFile(m_strFilePath);
    ProjectInfoUI::instance()->setProjectPath(m_strFilePath);
    ProjectInfoUI::instance()->setProjectName(ProjectInfoUI::instance()->getNameFromPath(m_strFilePath));
    ProjectInfoUI::instance()->updateProjectNameUI();
}
void Cx3dAutoSaveProject::setSettingsData(QString file)
{
    qDebug()<<"setSettingsData =" <<file;
    file = file.replace("file:///", "");
    QSettings setting;
    setting.beginGroup("AutoSave_Path");
    setting.setValue("filePath", file);
    setting.endGroup();
}
QString Cx3dAutoSaveProject::getProjectPath()
{
    return m_strFilePath;
}
