
#include <QUrl>
#include <QFileInfo>
#include "cx3drecentprojectcommand.h"
#include "kernel/translator.h"
#include "interface/jobsinterface.h"
#include "cx3dexportjob.h"
#include "inout/cxopenandsavefilemanager.h"
#include "cx3dsubmenurecentproject.h"
#include "cx3dautosaveproject.h"
#include "kernel/projectinfoui.h"
#include "kernel/abstractkernel.h"
using namespace creative_kernel;
Cx3dRecentProjectCommand::Cx3dRecentProjectCommand()
{
    m_actionname = "";
    m_eParentMenu = eMenuType_File;
    m_projectUI = new ProjectInfoUI();
    disconnect(m_projectUI, SIGNAL(acceptDialog()), this, SLOT(slotSaveLastPro()));
    connect(m_projectUI, SIGNAL(acceptDialog()), this, SLOT(slotSaveLastPro()));
    disconnect(m_projectUI,SIGNAL(cancelDialog()),this,SLOT(slotCancelSaveLastPro()));
    connect(m_projectUI,SIGNAL(cancelDialog()),this,SLOT(slotCancelSaveLastPro()));
}
Cx3dRecentProjectCommand::~Cx3dRecentProjectCommand()
{
    m_projectUI->deleteLater();
    m_projectUI = nullptr;
}
void Cx3dRecentProjectCommand:: execute()
{
    m_actionname = m_actionname.replace("file:///", "");//去掉前缀

    QFileInfo fileInfo(m_actionname);
    if (!fileInfo.exists())
    {
        AbstractKernelUI::getSelf()->executeCommand("requestMenuDialog", this, "openRecentlyFileDlg");
        return;
    }

    
    QString strOldPath = ProjectInfoUI::instance()->getProjectPath();//Cx3dAutoSaveProject::instance()->getProjectPath();
    if (!strOldPath.isEmpty() && strOldPath != m_actionname)
    {
        m_projectUI->requestMenuDialog();
    }
    else
    {
        m_projectUI->clearSecen();
        JobPtr job(new Cx3dExportJob(m_actionname, Cx3dExportJob::JobType::READJOB));
        executeJob(job);
        Cx3dAutoSaveProject::instance()->createOpenProject(m_actionname, false);
    }
}

void Cx3dRecentProjectCommand::slotSaveLastPro()
{
//    QString strOldPath = Cx3dAutoSaveProject::instance()->getProjectPath();
//    Cx3dWriter* writer = new Cx3dWriter(strOldPath, nullptr);
//    writer->deleteLater();
//    m_projectUI->clearSecen();
//    JobPtr job3(new Cx3dExportJob(m_actionname, Cx3dExportJob::JobType::READJOB));
//    executeJob(job3);
//    Cx3dAutoSaveProject::instance()->createOpenProject(m_actionname, false);
//    QSettings settings;
//    Cx3dSubmenuRecentProject::getInstance()->setMostRecentFile(m_actionname);
    qDebug()<<"m_strNewPath =" <<m_actionname;
    QString strOldPath = Cx3dAutoSaveProject::instance()->getProjectPath();

    Cx3dWriter *writer = new Cx3dWriter(strOldPath,nullptr);
    writer->deleteLater();

    ProjectInfoUI::instance()->clearSecen();
    JobPtr job3(new Cx3dExportJob(m_actionname, Cx3dExportJob::JobType::READJOB));
    executeJob(job3);
    Cx3dAutoSaveProject::instance()->createOpenProject(m_actionname,false);
    QSettings settings;
    Cx3dSubmenuRecentProject::getInstance()->setMostRecentFile(m_actionname);
}

void Cx3dRecentProjectCommand::slotCancelSaveLastPro()
{
//    m_projectUI->clearSecen();
//    JobPtr job3(new Cx3dExportJob(m_actionname, Cx3dExportJob::JobType::READJOB));
//    executeJob(job3);
//    Cx3dAutoSaveProject::instance()->createOpenProject(m_actionname, false);
//    QSettings settings;
//    Cx3dSubmenuRecentProject::getInstance()->setMostRecentFile(m_actionname);

    qDebug()<<"m_strNewPath =" <<m_actionname;
    ProjectInfoUI::instance()->clearSecen();
    JobPtr job3(new Cx3dExportJob(m_actionname, Cx3dExportJob::JobType::READJOB));
    executeJob(job3);
    Cx3dAutoSaveProject::instance()->createOpenProject(m_actionname,false);
    QSettings settings;
    Cx3dSubmenuRecentProject::getInstance()->setMostRecentFile(m_actionname);
}
