#include "cx3dimportercommand.h"
#include <QFileDialog>
#include "cx3dexportjob.h"
#include "cx3dexporter.h"

#include "interface/jobsinterface.h"
#include "kernel/translator.h"
#include "cx3dautosaveproject.h"
#include "kernel/projectinfoui.h"
#include "cx3dsubmenurecentproject.h"
#include "kernel/abstractkernel.h"
#include "interface/gadatainterface.h"

using namespace creative_kernel;
CX3DImporterCommand::CX3DImporterCommand(QObject* parent)
    :ActionCommand(parent)
{
    m_shortcut = "Ctrl+O";
    m_actionname = tr("Open Project");//tr("ImporterCX3D");
    m_actionNameWithout = "Open Project";
    m_icon = "qrc:/kernel/images/open.png";
    m_eParentMenu = eMenuType_File;
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    disconnect(ProjectInfoUI::instance(),SIGNAL(acceptDialog()),this,SLOT(slotSaveLastPro()));
    connect(ProjectInfoUI::instance(),SIGNAL(acceptDialog()),this,SLOT(slotSaveLastPro()));

    disconnect(ProjectInfoUI::instance(),SIGNAL(cancelDialog()),this,SLOT(slotCancelSaveLastPro()));
    connect(ProjectInfoUI::instance(),SIGNAL(cancelDialog()),this,SLOT(slotCancelSaveLastPro()));
    CXFILE.registerOpenHandler(this);
}

void CX3DImporterCommand::execute()
{
    creative_kernel::sendDataToGA("File Menu", "Open Project");
    Cx3dAutoSaveProject::instance()->stopTimer();

    QStringList filters;
    filters.push_back("cxprj");
    //filterlist.push_back("3mf");
    // filters.push_back("zip");
    CXFILE.open(this, filters);

    Cx3dAutoSaveProject::instance()->startTimer();
}

QStringList CX3DImporterCommand::nameFilters()
{
    QStringList exts;
    exts.append("3mf");
    return exts;
}

QStringList CX3DImporterCommand::supportFilters()
{
    QStringList list;
    list << "cxprj";
    return list;
}

QStringList CX3DImporterCommand::enableFilters()
{
    QStringList filters;
    filters << "cxprj";
    return filters;
}

void CX3DImporterCommand::handle(const QString& fileName)
{
    setNewPath(fileName);

    Cx3dSubmenuRecentProject::getInstance()->setMostRecentFile(fileName);
    QString strOldPath = ProjectInfoUI::instance()->getProjectPath();//Cx3dAutoSaveProject::instance()->getProjectPath();
    QString strPath = ProjectInfoUI::instance()->getProjectPath();
    if (!strOldPath.isEmpty() && strOldPath != fileName)
    {
//        if(!strPath.isEmpty())
//        {
            ProjectInfoUI::instance()->requestMenuDialog();
//        }
    }
    else
    {
        ProjectInfoUI::instance()->clearSecen();
        JobPtr job(new Cx3dExportJob(fileName, Cx3dExportJob::JobType::READJOB));
        executeJob(job);
        Cx3dAutoSaveProject::instance()->createOpenProject(fileName, false);
    }
    AbstractKernelUI::getSelf()->switchPickMode();
}

void CX3DImporterCommand::setNewPath(QString pathName)
{
    m_strNewPath = pathName;
}

void CX3DImporterCommand::slotLanguageChanged()
{
    m_actionname = tr("Open Project") + "        " + m_shortcut;
}

void CX3DImporterCommand::slotSaveLastPro()
{
    qDebug()<<"m_strNewPath =" <<m_strNewPath;
    QString strOldPath = Cx3dAutoSaveProject::instance()->getProjectPath();

    Cx3dWriter *writer = new Cx3dWriter(strOldPath,nullptr);
    writer->deleteLater();

    ProjectInfoUI::instance()->clearSecen();
    JobPtr job3(new Cx3dExportJob(m_strNewPath, Cx3dExportJob::JobType::READJOB));
    executeJob(job3);
    Cx3dAutoSaveProject::instance()->createOpenProject(m_strNewPath,false);
}

void CX3DImporterCommand::slotCancelSaveLastPro()
{
    qDebug()<<"m_strNewPath =" <<m_strNewPath;
    ProjectInfoUI::instance()->clearSecen();
    JobPtr job3(new Cx3dExportJob(m_strNewPath, Cx3dExportJob::JobType::READJOB));
    executeJob(job3);
    Cx3dAutoSaveProject::instance()->createOpenProject(m_strNewPath,false);
}
