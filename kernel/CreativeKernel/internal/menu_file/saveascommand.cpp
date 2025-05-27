#include "saveascommand.h"

#include "interface/commandoperation.h"
#include "data/modeln.h"

#include "kernel/translator.h"
#include "inout/cxopenandsavefilemanager.h"

#include "trimesh2/TriMesh.h"

#include "utils/matrixutil.h"
#include "mmesh/trimesh/trimeshutil.h"
#include "kernel/projectinfoui.h"
#include "kernel/abstractkernel.h"
#include "interface/selectorinterface.h"
#include "interface/modelinterface.h"
#include "interface/gadatainterface.h"
using namespace creative_kernel;
SaveAsCommand::SaveAsCommand(QObject* parent)
    :ActionCommand(parent)
{
    m_shortcut = "Ctrl+S";
    m_actionname = tr("Save STL");
    m_actionNameWithout = "Save STL";
    m_insertKey = "saveSTL";
    m_eParentMenu = eMenuType_File;
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
}

SaveAsCommand::~SaveAsCommand()
{
}

void SaveAsCommand::execute()
{
    creative_kernel::sendDataToGA("File Menu", "Save as STL");
    QList<ModelN*> selections = selectionms();
    if (selections.size() == 0)
    {
        AbstractKernelUI::getSelf()->executeCommand("requestMenuDialog", this, "messageNoModelDlg");
        return;
    }

    QStringList filters;
    filters.push_back("stl");

    CXFILE.save(this, filters);
}

void SaveAsCommand::handle(const QString& fileName)
{
    creative_kernel::cmdSaveAs(fileName);
    //ProjectInfoUI::instance()->updateFileStateUI();
    m_strMessageText = tr("Save Finished, Open Local Folder");
    AbstractKernelUI::getSelf()->executeCommand("requestMenuDialog", this, "messageDlg");

    QString path = fileName;
    CXFILE.setLastSaveFileName(path.replace("/", "\\"));
}

void SaveAsCommand::slotLanguageChanged()
{
    m_actionname = tr("Save STL") + "    " + m_shortcut;
}

QString SaveAsCommand::getMessageText()
{
    return m_strMessageText;
}

void SaveAsCommand::accept()
{
    CXFILE.openLastSaveFolder();
}
