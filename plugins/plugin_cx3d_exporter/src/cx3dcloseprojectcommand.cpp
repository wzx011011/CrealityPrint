#include "cx3dcloseprojectcommand.h"
#include <QQmlComponent>
#include <QQmlEngine>
#include <QDebug>
#include <QStandardPaths>
#include <QDir>
#include "kernel/translator.h"
#include "kernel/abstractkernel.h"
#include "kernel/projectinfoui.h"
#include "cx3dsubmenurecentproject.h"
#include "interface/jobsinterface.h"
#include "cx3dexportjob.h"
#include "cx3dexporter.h"
#include "kernel/kernel.h"
using namespace creative_kernel;
Cx3dCloseProjectCommand::Cx3dCloseProjectCommand()
{
    m_shortcut = "Ctrl+Q";      //不能有空格
    m_actionname = tr("Close");//
    m_actionNameWithout = "Close";
    m_icon = "qrc:/kernel/images/new.png";
    m_eParentMenu = eMenuType_File;
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));

    disconnect(AbstractKernelUI::getSelf(),SIGNAL(closeWindow()),this,SLOT(slotCloseWindow()));
    connect(AbstractKernelUI::getSelf(),SIGNAL(closeWindow()),this,SLOT(slotCloseWindow()));
}
Cx3dCloseProjectCommand::~Cx3dCloseProjectCommand()
{}
void Cx3dCloseProjectCommand::execute()
{
    requestMenuDialog();
}
void Cx3dCloseProjectCommand::requestMenuDialog()
{
    AbstractKernelUI::getSelf()->executeCommand("requestMenuDialog", this, "closeProfile");
     //getKernelUI()->requestMenuDialog(this,"closeProfile");
}

void Cx3dCloseProjectCommand::slotLanguageChanged()
{
    m_actionname = tr("Close") + "        " + m_shortcut;
}

void Cx3dCloseProjectCommand::saveFile()
{
    saveProPath();
   /* QString strProPath = ProjectInfoUI::instance()->getProjectPath();

    if(strProPath.isEmpty())
    {
        saveProPath();
    }
    else
    {
        handle(strProPath);
    }*/
//    getKernelUI()->requestCloseAction();
    //save fuction

}
//lisugui 2020-10-29
void Cx3dCloseProjectCommand::noSaveFile()
{
    AbstractKernelUI::getSelf()->executeCommand("requestCloseAction", nullptr, "");
    //getKernelUI()->requestCloseAction();
}

void Cx3dCloseProjectCommand::saveProPath()
{
    QStringList filters;
    filters.push_back("cxprj");
    // filters.push_back("zip");
    CXFILE.save(this, filters);

}
void Cx3dCloseProjectCommand::handle(const QString& fileName)
{
    JobPtr job(new Cx3dExportJob(fileName, Cx3dExportJob::JobType::WRITEJOB));
    executeJob(job);
    QSettings settings;
    Cx3dSubmenuRecentProject::getInstance()->setMostRecentFile(fileName);
    disconnect(getKernel()->jobExecutor(), SIGNAL(jobsEnd()), this, SLOT(finishJob()));
    connect(getKernel()->jobExecutor(), SIGNAL(jobsEnd()), this, SLOT(finishJob()));

    delDefaultProject();
}
QUrl Cx3dCloseProjectCommand::historyPath()
{
    CX3DExporter* exporter = qobject_cast<CX3DExporter*>(parent());
    return exporter->openedPath();
}


void Cx3dCloseProjectCommand::delDefaultProject()
{
    QString pathDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/tmpProject";
    QDir tempDir(pathDir);
    tempDir.removeRecursively();
}

void Cx3dCloseProjectCommand::finishJob()
{
    AbstractKernelUI::getSelf()->executeCommand("requestCloseAction", nullptr, "");
    //getKernelUI()->requestCloseAction();
}

void Cx3dCloseProjectCommand::slotCloseWindow()
{
    requestMenuDialog();
}
