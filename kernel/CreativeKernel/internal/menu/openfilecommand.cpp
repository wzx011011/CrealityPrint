#include "openfilecommand.h"
#include "submenurecentfiles.h"
#include "interface/loadinterface.h"
#include "kernel/kernelui.h"
#include "utils/meshloaderwrapper.h"
#include "kernel/translator.h"
#include "inout/cxopenandsavefilemanager.h"

using namespace creative_kernel;
OpenFileCommand::OpenFileCommand()
{
    m_shortcut = "Ctrl+I";
    m_actionname = tr("Open File");
    m_actionNameWithout = "Open File";
    m_icon = "qrc:/kernel/images/open.png";
    m_eParentMenu = eMenuType_File;

    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));

    MeshLoaderWrapper* loader = getKernelUI()->getMeshLoaderWrapper();
    connect(loader, SIGNAL(sigFinished(QString)), this, SLOT(slotFinished(QString)));
}

OpenFileCommand::~OpenFileCommand()
{
}

void OpenFileCommand:: execute()
{
   requestLoad();
}

void OpenFileCommand::slotFinished(QString strFilePath)
{
    SubMenuRecentFiles::getInstance()->setMostRecentFile(strFilePath);
}

void OpenFileCommand::slotLanguageChanged()
{
    m_actionname = tr("Open File") + "        " + m_shortcut;
}
