#include "cx3dexportercommand.h"
#include <QtWidgets/QFileDialog>
#include "cx3dexportjob.h"
#include "cx3dexporter.h"

#include "interface/jobsinterface.h"
#include "kernel/translator.h"
#include "cx3dsubmenurecentproject.h"
#include "kernel/projectinfoui.h"
#include "kernel/abstractkernel.h"
#include "interface/gadatainterface.h"

using namespace creative_kernel;
CX3DExporterCommand::CX3DExporterCommand(QObject* parent)
	:ActionCommand(parent)
{
    m_shortcut = "Ctrl+Shift+S";
    m_actionname = tr("Save As Project");//"ExporterCX3D";
    m_actionNameWithout = "Save As Project";
    m_eParentMenu = eMenuType_File;
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));

    disconnect(AbstractKernelUI::getSelf()->getUI("footer"), SIGNAL(acceptButton()), this, SLOT(slotOpenLocalFolder()));
    connect(AbstractKernelUI::getSelf()->getUI("footer"), SIGNAL(acceptButton()), this, SLOT(slotOpenLocalFolder()));
}

void CX3DExporterCommand::execute()
{
    creative_kernel::sendDataToGA("File Menu", "Save Project as");
    QStringList filters;
    filters.push_back("cxprj");
    //filters.push_back("zip");
    CXFILE.save(this, filters);
}

QUrl CX3DExporterCommand::historyPath()
{
    CX3DExporter* exporter = qobject_cast<CX3DExporter*>(parent());
    return exporter->openedPath();
}

void CX3DExporterCommand::handle(const QString& fileName)
{
    JobPtr job(new Cx3dExportJob(fileName, Cx3dExportJob::JobType::WRITEJOB));
    executeJob(job);
    QSettings settings;
    Cx3dSubmenuRecentProject::getInstance()->setMostRecentFile(fileName);
    ProjectInfoUI::instance()->setProjectPath(fileName);
    ProjectInfoUI::instance()->setProjectName(ProjectInfoUI::instance()->getNameFromPath(fileName));
    ProjectInfoUI::instance()->updateProjectNameUI();
    //ProjectInfoUI::instance()->requestMenuDialog();
    m_strMessageText = tr("Save Finished, Open Local Folder");
    AbstractKernelUI::getSelf()->executeCommand("requestMenuDialog", this, "messageDlg");

    QString path = fileName;
    CXFILE.setLastSaveFileName(path.replace("/", "\\"));
}

void CX3DExporterCommand::slotLanguageChanged()
{
    m_actionname = tr("Save As Project") + "    " + m_shortcut;
}

void CX3DExporterCommand::slotOpenLocalFolder()
{
    CXFILE.openLastSaveFolder();
}

QString CX3DExporterCommand::getMessageText()
{
    return m_strMessageText;
}

void CX3DExporterCommand::accept()
{
    CXFILE.openLastSaveFolder();
}
